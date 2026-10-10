## Introduction

das-fmt ships in the SDK as `bin/das-fmt.exe`, a copy of daslang that runs `dasfmt.das` by its
name - the `DAS_UTILS_SHIPPED_EXES` entry in `utils/CMakeLists.txt` (repo root); `utils/REVIEW.das`
reads this line as the record of that decision.

dasfmt is a tool that automatically formats daScript source code.

## Usage

- `dascript dasfmt.das -- --path folderWithScriptsOrSingleScript [--path anotherPath]`

### dastest.das arguments
- `--path`: Path to the folder with scripts or single script name
- `--exclude-mask <mask>`: A path with given masks as part of the path will be ignored
- `--verify`: (dry run) Doesn't change files, just make sure that all files are already formatted
- `--t`: Max number of used threads
- `--max-line-length <N>`: Also break code lines longer than N columns, after `=>` and inside brackets; a line that cannot fit stays. Off (0) by default
- `--verbose`: Print more information about the formatting process
- `--no-color`, `--color`: Disable or enable color output
