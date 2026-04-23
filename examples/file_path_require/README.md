# File-path `require` example

Shows the three new file-path require forms:

| Form | Resolves to |
|---|---|
| `require ./shared_helpers.das` | sibling file relative to the current file |
| `require ../foo/bar.das` | relative with `..` segment |
| `require %/daslib/random.das` | rooted at `get_das_root()` |

Triggered when the require both starts with `./`, `../`, or `%/` **and** ends
in `.das` (or `.das_project`). Available in the default file access (no
`.das_project`) and during `.das_project` bootstrap (the project file itself
can use file-path requires).

## Run

```
bin/Release/daslang.exe examples/file_path_require/main.das
```

`project.das_project` shows the chicken-and-egg case: the project file
itself does `require ./shared_helpers.das` to pull in a helper before
its own `module_get` callback is wired up. This works because the
project file is loaded by the inner default `FsFileAccess`, which
handles file-path requires directly.

In project mode, regular `.das` source files have their requires routed
through `module_get` — so `main.das` here does **not** use file-path
requires when run with `-project`; you would need to implement
file-path resolution inside your own `module_get` to support that.
