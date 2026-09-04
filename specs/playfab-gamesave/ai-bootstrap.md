## Workflow
1. **Start with context-gathering** -- do not generate any files yet, first read the context source files below to understand the project.
2. Ask clarifying questions about the project as needed.
3. After carefully reading the context source files below, wait for next prompt.
4. **When writing new markdown files:**
   - Use clear section headings (`##`, `###`, etc.).
   - Include front-matter or metadata only if appropriate.
   - Maintain consistent formatting, indentation, and tone across all files.
   - Use technical accuracy and PlayFab-specific conventions when relevant.
5. **Never assume unknown details.**  
   Always pause to ask follow-up questions or confirm interpretations before proceeding.
6. Use an objective, developer-facing tone consistent with other PlayFab SDK documentation.

## PFGameSaves Context Sources
To understand the PFGameSaves project, read in this order:

### Start here (fast onboarding)
- **AI summary:** `specs\playfab-gamesave\ai-summary.md` -- high-level concepts, API snapshot, patterns
- **Architecture & code flow:** `specs\playfab-gamesave\ai-architecture.md` -- layer diagram, how API calls flow through the code, where to make changes
- **Source code map:** `specs\playfab-gamesave\ai-source-map.md` -- every file with class names and one-line descriptions

### Reference material
- **Public headers:** `Source\PlayFabGameSave\Include\playfab\gamesave` -- the API surface
- **Dev spec:** `specs\playfab-gamesave\design\client-dev-spec.md` -- detailed design decisions
- **Public docs:** `specs\playfab-gamesave\docs` -- user-facing documentation
- **Design specs:** `specs\playfab-gamesave\design\` -- state machines, conflict resolution, rollback, telemetry, quota

### Testing
- **Test loop guide:** `specs\playfab-gamesave\ai-test-loop-guide.md` -- build/test/fix workflow with scripts
- **Test strategy:** `specs\playfab-gamesave\testing\testing-strategy-summary.md`
- **Test automation specs:** `specs\playfab-gamesave\testing\test-automation`
- **C# test controller:** `Test\GameTestController`
- **C++ test device:** `Test\GameTestAppWindows`

### Deep dives (when needed)
- **Sample code:** `Samples\PlayFabGameSaveSample-Windows`
- **Source code:** `Source\PlayFabGameSave\Source`
- **Code review (25-step):** `specs\playfab-gamesave\ai-code-review\` -- detailed per-component analysis
- **Bug write-ups:** `specs\playfab-gamesave\bug-*.md` -- past bug investigations with root cause and fix

Just read these for now.  
I will tell you what to do in next prompt.
Don't bother summarizing.
But do read the docs!

