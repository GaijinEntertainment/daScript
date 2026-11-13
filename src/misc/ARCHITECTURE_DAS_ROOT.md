# src/misc Architecture - the das root

The das root is the folder a daslang reads its tree from: `daslib/`, `modules/`, `utils/`, the
runtime libraries. `ARCHITECTURE.md` beside this file holds the folder's other mechanisms.

## The shipped daslang is a stub, and a sidecar can move its root {#root-sidecar}

`bin/daslang` is `utils/daslang/stub.cpp`: a stub that links no daslang library. It finds the
das root, loads the host library from `<root>/lib` (`<root>/bin` on Windows, where the runtime
DLLs sit beside the executables) and calls its one export - `daslang_main` for daslang and every
tool copy of it, `daslang_live_main` for `daslang-live`, chosen by the stub's own file name. The
host library is `utils/daslang/main.cpp` built with `DAS_MAIN_LIBRARY`, which renames `main` and
exports it; `daslang_static` builds the same source with `main` and no stub. A stub that finds no
library exits 1 naming every path it looked at and the sidecar that would have pointed it right.

The root comes from one of two places, in `getDasRoot` (`sysos.cpp`) and in the stub alike. A
file `daslang.root` beside the executable names it: one candidate per line, absolute or relative
to the executable's folder, surrounding whitespace and the line end dropped, and the first
candidate that is a folder wins (`include/daScript/misc/das_root.h`, the one reader
both share; several lines let one wheel serve pip layouts whose scripts folder sits at
different depths). Without a sidecar the root is the executable's folder with a trailing `bin`
- or a configuration folder under it - removed, which is every build tree and every bundle.
`-dasroot` overrides both for the runtime. The stub loads its library from the sidecar's root
when there is one - from nowhere else, so a sidecar can never pair one tree's compiler with
another tree's daslib - and otherwise from its own tree: `bin/..`'s `lib` (and `lib/<config>`
under a multi-config generator), its own folder, where a Windows build keeps the DLLs, then
`bin/`; a `-dasroot` tree (the last one named) comes last, only when its own has none, so a
`-dasroot` that names a tree of `.das` files alone starts on this binary's compiler. The stub
also sets the Windows DLL directory to the folder it loaded from, so a DLL a module imports by
bare name from the bundle's `bin/` still resolves when the stub sits elsewhere; the runtime's
loader asks for that directory with `LOAD_LIBRARY_SEARCH_USER_DIRS`.

The sidecar is what lets a package manager put the binary where it wants and the tree where it
wants. pip is one such installer: a wheel's scripts land in the interpreter's `bin/`
(`Scripts\` on Windows) and its data under the interpreter's prefix, so `daslang` sits at
`<prefix>/bin/daslang` while the tree sits at `<prefix>/daslang_sdk`, and the sidecar says
`../daslang_sdk`. The stub is what makes that binary runnable there at all: an executable
linked against the runtime libraries needs them on its rpath before `main` runs, and no rpath
reaches a folder whose location only the installer knows; a stub that resolves the root first
can load from wherever it points. A console-script shim in Python does the same job at the
cost of an interpreter start on every run - 15 ms on macOS, 100 ms on Windows, doubling the
cost of a small script. An environment variable would be global to the machine where the
sidecar is per install, and pip cannot set one.

The stub pays for linking nothing in three places. Its own file never changes when the
compiler does, so the build regenerates a stamp source from the library file and compiles it
into the stub - on every generator, where `LINK_DEPENDS` is Ninja and Makefile only - and a
rebuild relinks the stub, so every build step that depends on the `daslang` file follows the
library; the module cache keys on the host library's own size and time
(`getModuleFileNameOf`), not the stub's. The leak tracker's global
`operator new` has to be defined in the executable to win the symbol lookup on Linux and
macOS, so a `RelWithDebInfo` stub carries `alloc_tracker_overrides.cpp` with hooks that are
no-ops until the stub binds them to the runtime's through one C export
(`das_alloc_tracker_hooks`), after its own temporaries are freed and before the entry runs;
Windows resolves `operator new` per module and needs none of this. And the stub resolves
paths itself: `realpath` on macOS, `/proc/self/exe` on Linux and the final path of the file
handle on Windows, so a symlinked command (brew's `bin/` links into `libexec/`, winget's
`Links\`) finds the real tree. Every path the runtime holds is UTF-8, on Windows too: the
executable's path, the root, the sources the compiler opens, the module folders it lists, the
manifests and the module cache it writes, and the libraries it loads all go through the
UTF-8 file API in `sysos.h` (`das_fopen_utf8`, `das_file_stat_utf8`, `das_each_directory_entry`,
`das_utf8_to_wide`), so a root under any name - a user's own - works, and the stub converts
the same way. One form everywhere is what keeps the module-scan manifests replaying: a root
written in one encoding and checked in another reads as "artifact missing" on every start and
costs the whole descriptor compile each time.

## The das root is a place, not a spelling {#das-root-absolute}

`setDasRoot` stores the root as an absolute path with forward slashes, no `.` or `..` segment
and, below a filesystem root, no trailing slash, so `-dasroot .`, `-dasroot build/..` and the
full path name one root when they point at one folder. A relative root is joined to the working
directory the process has at that call. The form is lexical: a symlink in the path stays as
written. The root `getDasRoot` derives from the executable's path goes through the same call,
the `.` it answers for a binary outside a `bin` folder included. `get_das_root()` therefore
answers the same string from every such start, and so does everything built on it - the module
scan's descriptor folders and manifest key (`src/ast/ARCHITECTURE_MODULE_SCAN.md#module-scan-manifest`), the
paths a descriptor registers, the file names a compile reports.

The reason is the manifest key. A descriptor's rows carry the root as the descriptor read it, so
the key has to compare the same string the rows were built from; were the root kept as typed, two
starts that spell one tree differently would each find the other's manifest stale and rewrite
it, on every start, and a start reading mid-rewrite would compile cold beside replayed
descriptors.

Windows resolves the path with `normalizeFileName`; Linux, macOS and Haiku with
`std::filesystem`. The root stays as given on every other target (the web build, a console, a
`DAS_NO_FILEIO` build), and wherever the resolve fails - a path past `MAX_PATH`, a working
directory that cannot be read.

## One lock guards the root, and one thread derives it {#das-root-lock}

`getDasRoot` and `setDasRoot` take one mutex, and the stored root changes in a single assignment
under it. `setDasRoot` builds the absolute form in a local before it locks. The first
`getDasRoot` of a process that set no root derives one from the executable's path, in a local,
while it holds the lock: threads that ask at the same moment wait for that one derivation and
all get its answer. `getDasRoot` answers a copy, so no caller holds a reference into the stored
string. `setDasRoot("")` clears the stored root, and the next `getDasRoot` derives it again.

The reason is a host that compiles on several threads. Every compile asks for the root -
`daslib/` paths, the module scan, the anonymous-module hash - and nothing orders the first
compile of one thread against the first compile of another. A root stored in steps (the sidecar
text, then the absolute form, then the slashes) is a root a second thread can copy half-made or
free a second time; a half-made root that does not crash is a relative or back-slashed path, and
`#das-root-absolute` is what that breaks.

The lock makes a late `setDasRoot` safe, not coherent: a compile that already built paths from
the old root keeps them. Every host in the tree sets the root while it reads its arguments,
before it starts a thread that compiles.
