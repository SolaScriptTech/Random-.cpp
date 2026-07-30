# C++ Learning Archive

A curated collection of small C++ exercises and coursework projects by Matthew Johns.

This repository began as a catch-all folder named `Random-.cpp`. It has since been cleaned up so source projects are easy to find, generated binaries are excluded, and larger standalone projects live in their own repositories.

## Standalone projects

These projects outgrew the archive:

- [MicMattMoe](https://github.com/SolaScriptTech/MicMattMoe) — a command-line Tic-Tac-Toe game with an intentionally unpredictable computer opponent.
- [The Sea Siren Text Adventure Game](https://github.com/SolaScriptTech/The-Sea-Siren-Text-Adventure-Game) — a narrative C++ adventure game.
- [Windows User Migration Tool](https://github.com/SolaScriptTech/windows-user-migration-tool) — a sanitized and configurable PowerShell workstation-migration utility.

## Repository map

### Exercises

- [Al Gore's Revenge — revised](exercises/al-gores-revenge-revised.cpp)
- [Dry Run](exercises/dry-run.cpp)
- [Numeric Pattern](exercises/pattern.cpp)

### Projects

- [Aircraft Decision Tree](projects/aircraft-decision-tree/aircraft-decision-tree.cpp)
- [Baseball Score](projects/baseball-score/baseball-score.cpp)
- [Drake Equation](projects/drake-equation/)
- [Simple Calculator](projects/simple-calculator/)
- [Temperature Calculator](projects/temperature-calculator/)

Several folders contain original and enhanced coursework versions. They are retained to show how the programs evolved.

## Building a program

Each source file is an independent console program. Compile the file you want to run:

```sh
g++ -std=c++17 -Wall -Wextra -pedantic path/to/program.cpp -o program
```

For example:

```sh
g++ -std=c++17 -Wall -Wextra -pedantic projects/baseball-score/baseball-score.cpp -o baseball-score
```

## Repository policy

- Source code and project documentation belong in this repository.
- Compiled executables, object files, IDE output, and build directories do not.
- A project should move to its own repository when it develops independent documentation, releases, assets, or a continuing roadmap.

See [ABOUT.md](ABOUT.md) for the archive's purpose and extraction criteria.

## License

[MIT](LICENSE)
