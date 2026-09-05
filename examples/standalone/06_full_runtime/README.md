# 06_full_runtime - a standalone context that links C++ modules

The same `-ctx` generation as the nano examples beside it, linked against the full
`libDaScript` and the static archive of every C++ module the script reaches, instead
of nano. This one reaches two: dasHV (an HTTP client) and fio (a child process).

What the binary still does without: the compiler, the interpreter, and every shared
module. It parses nothing at run time and holds no lock on any file a deploy
replaces - the shape a supervisor needs.

What it costs over nano is the module registry. Handled types resolve their
annotations through it, so the first context constructed registers the C++ modules
it links (the builtin set in the C++ registrar's order, then the rest
dependencies-first), and the last context destroyed shuts them down with
`Module::ShutdownStandalone` - no interpreter is linked to own a fusion reset. That
happens inside `main`, where it belongs: keep contexts there. `main.cpp` constructs
the context and nothing else. A host that registered the
builtin module owns the registry instead: it registers every module the context
links before constructing it, and shuts down itself; a module it missed stops the
program at construction, by name.

The generated code names only what the program reaches: a module the script used at
compile time alone is neither included nor registered, and the function table has a
slot per function the context still calls - nothing for what the interpreter would
have removed.

## Running it

```
standalone_06_full_runtime [url]
```

Probes `url` (default: a port nobody listens on, so the status is -1), then runs a
copy of itself with `--child` and prints what the child wrote. It builds in-tree with
dasHV enabled and runs as a small-lane test, exit 0 when the child round trip held.
