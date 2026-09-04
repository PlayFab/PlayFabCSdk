"""Extract public API method names from xsapi-c headers and write to a txt file."""

import os
import re
import glob

HEADER_DIR = os.path.join(os.path.dirname(__file__), "Include", "xsapi-c")
OUTPUT_FILE = os.path.join(os.path.dirname(__file__), "xsapi_c_api_methods.txt")

# Matches STDAPI or STDAPI_(type) followed by the function name and opening paren
PATTERN = re.compile(r"^STDAPI(?:_\([^)]*\))?\s+(\w+)\s*\(", re.MULTILINE)

def extract_api_methods():
    methods = []
    for header in sorted(glob.glob(os.path.join(HEADER_DIR, "*.h"))):
        with open(header, "r", encoding="utf-8") as f:
            content = f.read()
        for match in PATTERN.finditer(content):
            methods.append(match.group(1))
    return methods

if __name__ == "__main__":
    methods = extract_api_methods()
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        f.write("\n".join(methods) + "\n")
    print(f"Extracted {len(methods)} API methods to {OUTPUT_FILE}")
