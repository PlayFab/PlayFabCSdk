"""Count API methods per category from GDK headers listed in headers.md.

Parses headers.md for categories and header file lists, scans each header
for public API function declarations, cross-references with commands in
DeviceCommandHandlers.cpp, and rewrites headers.md with a summary table,
per-category stats, and missing-command lists.

Usage:
    python count_api_methods.py
"""

import os
import re
from collections import OrderedDict, Counter

HEADER_DIR = r"C:\Program Files (x86)\Microsoft GDK\251002\windows\include"
HEADERS_MD = os.path.join(os.path.dirname(__file__), "gdk-api-coverage.md")
COMMAND_HANDLERS_DIR = r"C:\git\PlayFab.C\Test\GameTestAppShared"
SCENARIOS_DIR = r"C:\git\PlayFab.C\Test\GameTestController\Scenarios"

# Map scenario file prefixes to category names in headers.md
SCENARIO_PREFIX_MAP = {
    "lhc": "libHttpClient",
    "gamesave": "PFGameSaveFiles",
    "grts": "xgameruntime",
    "pfcore": "PFCore",
    "pfservices": "PFServices",
    "xsapi": "XSAPI",
    "controller": None,  # infra tests, not mapped to a specific API category
}

# Patterns for extracting function names from different header styles
PATTERNS = [
    re.compile(r"STDAPI(?:_\([^)]*\))?\s+(\w+)\s*\(", re.MULTILINE),
    re.compile(r"PF_API\s+(\w+)\s*\(", re.MULTILINE),
    re.compile(r"PFMULTIPLAYER_API\s+(\w+)\s*\(", re.MULTILINE),
    re.compile(r"PARTY_API\s+(\w+)\s*\(", re.MULTILINE),
    re.compile(r"GC_API\s+(\w+)\s*\(", re.MULTILINE),
]

# Generated metadata lines we strip on re-parse
_META_PREFIXES = ("API methods", "Prefix", "Commands:", "Test Commands:", "Missing (", "Included (", "- ", "Scenarios:", "Test Scenarios:", "Passing:")
_SUMMARY_START = "<!-- summary-start -->"
_SUMMARY_END = "<!-- summary-end -->"


def parse_headers_md():
    """Parse headers.md returning OrderedDict of category -> list of header paths.

    Skips generated metadata lines and the summary table so the file is
    safe to re-parse after a previous run.
    """
    categories = OrderedDict()
    current = None
    in_summary = False
    with open(HEADERS_MD, "r", encoding="utf-8") as f:
        for line in f:
            stripped = line.strip()
            if stripped == _SUMMARY_START:
                in_summary = True
                continue
            if stripped == _SUMMARY_END:
                in_summary = False
                continue
            if in_summary:
                continue
            if stripped.startswith("# "):
                current = stripped[2:].strip()
                categories[current] = []
            elif stripped and current is not None and not stripped.startswith(_META_PREFIXES):
                categories[current].append(stripped)
    return categories


def extract_commands():
    """Extract command names from CommandRegistrar blocks across all handler .cpp files."""
    import glob as _glob
    commands = []
    for cpp in _glob.glob(os.path.join(COMMAND_HANDLERS_DIR, "**", "*.cpp"), recursive=True):
        try:
            with open(cpp, "r", encoding="utf-8") as f:
                content = f.read()
        except (OSError, UnicodeDecodeError):
            continue
        if "CommandRegistrar" not in content:
            continue
        commands.extend(re.findall(r'{\s*"(\w+)"\s*,\s*Handle\w+\s*}', content))
    return commands


def count_scenarios():
    """Count scenario .yml files per category using SCENARIO_PREFIX_MAP.

    Returns (total_counts, passing_counts) — both Counter objects keyed by category.
    A scenario counts as passing if its YAML tags list contains 'passing'.
    """
    import glob
    import yaml
    counts = Counter()
    passing = Counter()
    for path in glob.glob(os.path.join(SCENARIOS_DIR, "*.yml")):
        name = os.path.basename(path)
        prefix = name.split("-")[0]
        category = SCENARIO_PREFIX_MAP.get(prefix)
        if category:
            counts[category] += 1
            try:
                with open(path, "r", encoding="utf-8") as f:
                    doc = yaml.safe_load(f)
                if isinstance(doc, dict) and "passing" in (doc.get("tags") or []):
                    passing[category] += 1
            except Exception:
                pass
    return counts, passing


def extract_methods(filepath):
    """Extract API method names from a single header file."""
    methods = []
    try:
        with open(filepath, "r", encoding="utf-8") as f:
            content = f.read()
    except FileNotFoundError:
        print(f"  WARNING: not found: {filepath}")
        return methods
    for pattern in PATTERNS:
        for match in pattern.finditer(content):
            name = match.group(1)
            if name not in methods:
                methods.append(name)
    return methods


def extract_prefix(name):
    """Extract the API prefix from a function name."""
    parts = re.findall(r"[A-Z]+(?=[A-Z][a-z])|[A-Z][a-z]+", name)
    if not parts:
        return name
    for known in ["PartyXbl", "GameInput", "PFGameSave", "PFMultiplayer", "PFMatchmaking",
                   "PFLobby", "PostDecodeAudio", "PreEncodeAudio", "GameChat"]:
        if name.startswith(known):
            return known
    if parts[0].isupper() and len(parts[0]) <= 3:
        return parts[0]
    return parts[0]


def find_prefix(methods):
    """Find the most common prefix(es) for a list of method names."""
    if not methods:
        return ""
    prefixes = Counter(extract_prefix(m) for m in methods)
    top = prefixes.most_common()
    if len(top) == 1:
        return top[0][0]
    total = sum(c for _, c in top)
    if top[0][1] / total > 0.8:
        return top[0][0]
    return ", ".join(p for p, c in top if c > 1)


def build_summary_table(results):
    """Build a markdown summary table from results."""
    lines = [
        _SUMMARY_START,
        "",
        "| Category | API Methods | Test Commands | Test Scenarios | Passing | % Passing |",
        "|---|---|---|---|---|---|",
    ]
    total_methods = 0
    total_cmds = 0
    total_scenarios = 0
    total_passing = 0
    for cat, r in results.items():
        pct = (r["passing"] / r["scenarios"] * 100) if r["scenarios"] else 0
        lines.append(f"| {cat} | {r['count']} | {r['commands']} | {r['scenarios']} | {r['passing']} | {pct:.0f}% |")
        total_methods += r["count"]
        total_cmds += r["commands"]
        total_scenarios += r["scenarios"]
        total_passing += r["passing"]
    total_pct = (total_passing / total_scenarios * 100) if total_scenarios else 0
    lines.append(f"| **Total** | **{total_methods}** | **{total_cmds}** | **{total_scenarios}** | **{total_passing}** | **{total_pct:.0f}%** |")
    lines.append("")
    lines.append(_SUMMARY_END)
    return lines


def build_category_block(cat_name, r, commands_set):
    """Build the metadata lines for a single category section."""
    lines = [
        f"API methods: {r['count']}",
        f"Prefix: `{r['prefix']}`",
        f"Test Commands: {r['commands']}",
        f"Test Scenarios: {r['scenarios']}",
        f"Passing: {r['passing']}",
    ]
    included = [m for m in r["methods"] if m in commands_set]
    missing = [m for m in r["methods"] if m not in commands_set]
    if included:
        lines.append(f"Included ({len(included)}):")
        for m in included:
            lines.append(f"- {m}")
    if missing:
        lines.append(f"Missing ({len(missing)}):")
        for m in missing:
            lines.append(f"- {m}")
    return lines


def main():
    categories = parse_headers_md()
    commands_set = set(extract_commands())
    scenario_counts, passing_counts = count_scenarios()
    results = OrderedDict()

    for category, headers in categories.items():
        all_methods = []
        for header in headers:
            filepath = os.path.join(HEADER_DIR, header)
            all_methods.extend(extract_methods(filepath))

        prefix = find_prefix(all_methods)
        methods_with_cmd = [m for m in all_methods if m in commands_set]
        results[category] = {
            "count": len(all_methods),
            "prefix": prefix,
            "methods": all_methods,
            "commands": len(methods_with_cmd),
            "scenarios": scenario_counts.get(category, 0),
            "passing": passing_counts.get(category, 0),
        }
        print(f"{category}: {len(all_methods)} methods, {len(methods_with_cmd)} commands, {scenario_counts.get(category, 0)} scenarios, {passing_counts.get(category, 0)} passing, prefix={prefix!r}")

    # Rebuild headers.md
    with open(HEADERS_MD, "r", encoding="utf-8") as f:
        original = f.read()

    # Strip old summary block and metadata, then rebuild
    old_lines = original.split("\n")
    new_lines = []
    in_summary = False
    summary_inserted = False
    i = 0
    while i < len(old_lines):
        line = old_lines[i]
        stripped = line.strip()

        # Skip old summary block
        if stripped == _SUMMARY_START:
            in_summary = True
            i += 1
            continue
        if stripped == _SUMMARY_END:
            in_summary = False
            i += 1
            continue
        if in_summary:
            i += 1
            continue

        # Skip old metadata/missing lines
        if stripped.startswith(_META_PREFIXES):
            i += 1
            if i < len(old_lines) and old_lines[i].strip() == "":
                i += 1
            continue

        # Insert summary table before first category heading
        if stripped.startswith("# ") and not summary_inserted:
            new_lines.extend(build_summary_table(results))
            new_lines.append("")
            summary_inserted = True

        new_lines.append(line)

        # Insert category metadata after heading
        if stripped.startswith("# "):
            cat_name = stripped[2:].strip()
            if cat_name in results:
                if i + 1 < len(old_lines) and old_lines[i + 1].strip() == "":
                    new_lines.append(old_lines[i + 1])
                    i += 1
                new_lines.extend(build_category_block(cat_name, results[cat_name], commands_set))
                new_lines.append("")
        i += 1

    with open(HEADERS_MD, "w", encoding="utf-8") as f:
        f.write("\n".join(new_lines))

    total = sum(r["count"] for r in results.values())
    total_cmds = sum(r["commands"] for r in results.values())
    print(f"\nUpdated {HEADERS_MD}")
    print(f"Total API methods: {total}, Commands: {total_cmds}, Missing: {total - total_cmds}")


if __name__ == "__main__":
    main()
