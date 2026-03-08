# daspkg — daslang Package Manager

## Goals

Package manager for the daslang ecosystem. Automates discovery, installation, dependency resolution, and building of daslang packages (both pure-das and C++ modules).

## Package Types

### Pure-das packages
- Just `.das` files + `.das_module` (provided by author), no build step
- Package manager downloads and places in `modules/`
- Works for everyone — no toolchain required

### C++ packages
- Contains C++ source + `CMakeLists.txt` using `find_package(DAS)`
- Package manager runs `cmake --configure` + `cmake --build` automatically
- Requires C++ toolchain (MSVC on Windows, GCC/Clang on Linux/Mac)
- Fails gracefully with "you need a C++ compiler" if toolchain is missing
- No prebuilt binary hosting — always build from source

## Version Model

Three version axes:
- **Package version** — semver of the package itself
- **daslang version** — which SDK release the package is compatible with
- **Dependencies** — other packages (with their own version constraints)

The manifest declares `daslang >= X.Y` compatibility. The package manager checks the installed SDK version and refuses if incompatible.

C++ packages are sensitive to the exact daslang version (ABI compatibility). Pure-das packages are more tolerant (only language-level compatibility).

## Key Design Decisions

### Registry: Git-based (like Go modules) + curated index
- Packages are git repositories (GitHub, GitLab, etc.)
- No centralized registry server to maintain
- Install by URL: `daspkg install github.com/user/repo`
- Version = git tag (e.g., `v1.0.0`)

### Package index
- Single file listing known packages — name, URL, short description
- Lives in a dedicated index repo (small, fast to clone)
- Authors add packages via PR to the index repo
- `daspkg search <query>` clones/fetches the index repo, matches against it. Can cache locally.
- Install by name (from index): `daspkg install imgui`
- Install by URL (direct): `daspkg install github.com/user/dasImgui`

### Manifest: `.das_package` (executable)

The manifest is a daslang script — same pattern as `.das_module`. daspkg fetches it first (before downloading the full package), runs it, and calls exported functions to get answers.

Each function answers a specific question. Only `package` and `resolve` are required; the rest are optional.

```das
options gen2

// Required: package metadata
[export]
def package() {
    package_name("mymodule")
    package_author("aleksisch")
    package_description("Cool module for daslang")
    package_source("https://github.com/aleksisch/mymodule")
}

// Required: what to download for a given version + SDK
[export]
def resolve(sdk_version : string; version : string) {
    if (version == "" || version == "latest")
        download_tag("v2.1.0")
    elif (version == "1.x")
        download_tag("v1.5.3")
    else
        download_tag("v{version}")
    // alternatives: download_branch("main"), download_zip("https://...")
}

// Optional: what other packages are needed
[export]
def dependencies(version : string) {
    require_package("github.com/user/some_lib", ">=1.0")
    require_package("github.com/user/other_lib", ">=0.3")
}

// Optional: how to build (if missing, assumes pure-das — no build step)
[export]
def build(sdk_path : string; version : string) {
    cmake_build()  // default: cmake configure + build with find_package(DAS)
    // alternatives: no_build(), custom_build("make all")
}
```

**Why executable, not JSON?**
- Authors can put version-conditional logic in `resolve` and `dependencies`
  (e.g., "for SDK >= 0.6 use this tag, for older use that one")
- Same pattern as `.das_module` — familiar to daslang authors
- daspkg provides the registration functions (`package_name`, `download_tag`, etc.) as a built-in API

Module registration (namespaces, DLLs, require paths) is handled entirely by `.das_module` — not duplicated in `.das_package`.

### Local layout

daspkg installs into `modules/` relative to the project root. The daslang runtime already scans `modules/` for `.das_module` files — first in the SDK, then in the app's directory. No new discovery mechanism needed.

```
my_project/
  main.das
  modules/                     # standard daslang modules directory
    <package_name>/
      .das_module              # provided by package author
      *.shared_module          # built C++ output (if applicable)
      *.das                    # source files
    ...
  daspkg.lock                  # JSON — installed packages, versions, sources, root vs dependency
```

This means packages are **per-project** (like `node_modules`), which is the right default for game projects that need reproducible builds. The SDK's own `modules/` has the built-in modules; the project's `modules/` has third-party packages.

### Package naming
- Package name = what `package_name()` declares in `.das_package`. Must be unique.
- Directory in `modules/` uses the package name (not the git repo name)
- The curated index enforces uniqueness — PR review catches collisions
- **Module namespace conflicts** (two packages registering the same `require` path) are a runtime issue, not a daspkg issue. Authors must pick unique namespaces. Not enforced in v1.

### Repos without `.das_package`
If a repo has no `.das_package`, daspkg does a "dumb clone" — clones the repo into `modules/<name>/` at the requested tag (or HEAD). No version resolution, no dependency tracking, no build. The repo must already have a `.das_module` and be self-contained. Recorded in the lock file with minimal info.

### `.das_module` handling
- `.das_module` is always provided by the package author — not generated by daspkg
- The author knows their DLL names, `require` namespaces, load dependencies
- This is already the established convention (see `dascript-demo`, all in-tree modules)
- daspkg just places the package in `modules/<name>/` and the existing `.das_module` works as-is

### Package sources

Packages can come from three sources:

1. **Git URL**: `daspkg install github.com/user/repo[@version]` — clones the repo
2. **Index name**: `daspkg install imgui` — looks up URL in the curated index
3. **Local path**: `daspkg install /path/to/my/module` — copies or symlinks from a local directory

Local paths are essential for development — you're working on a package and want to test it without pushing to git. daspkg can either:
- **Copy** the directory into `modules/` (safe, but stale)
- **Symlink** it (always up to date, better for dev workflow)

Default: symlink for local paths, copy for git. Could be overridden with `--copy` / `--link` flags.

## Commands

```
daspkg install <url|name|path>[@version]  # install from git, index, or local path (--force to reinstall)
daspkg remove <name>                       # uninstall a package
daspkg update [name]                       # update one or all packages
daspkg list                                # list installed packages
daspkg search <query>                      # search the package index
daspkg build                               # build C++ packages (after install or SDK upgrade)
daspkg check                               # verify all packages are compatible with current SDK
```

## Transport

Only external dependency: **`git` CLI**. No HTTP library, no curl, no dasHV.

### Git sparse checkout flow

Sparse checkout lets us fetch just `.das_package` before downloading the full repo. Provider-agnostic, works with any git host supporting protocol v2.

**Install:**
1. `git clone --depth 1 --filter=blob:none --sparse <url> modules/.daspkg_tmp/<name>`
2. `git -C <tmp> sparse-checkout set .das_package`
3. Run `.das_package` → call `resolve(sdk_version, version)` → get tag, call `dependencies()` → get deps
4. If incompatible with SDK → delete tmp, report error, done
5. `git -C <tmp> sparse-checkout disable` (fetch full tree)
6. `git -C <tmp> fetch --depth 1 origin tag <tag>` then `git -C <tmp> checkout <tag>`
7. Build if C++, move to final `modules/<name>/`

**Update check:** same sparse clone of default branch, run `resolve()`, compare tag with lock file. Only fetch full tree if version changed.

### Local paths
- Filesystem only (symlink or copy), no git involved
- `.das_package` read directly from the local directory

## Implementation

- Written in daslang (dogfooding)
- Uses `fio` for filesystem, `git` CLI for cloning
- Project root: explicit `--root <path>` flag, defaults to `.` (CWD). No magic directory walking.
- Runs as: `daslang utils/daspkg/main.das -- install <url>`
- Later: could be compiled to a standalone binary via LLVM

### `.das_package` API module

`daslib/daspkg.das` — shipped with the SDK, provides the registration functions that `.das_package` scripts call. The `.das_package` does `require daslib/daspkg` and it's available — no special setup.

The API functions accumulate state into module globals. daspkg compiles and runs the `.das_package`, then reads the accumulated state to get package info, resolved version, dependencies, and build instructions.

### Security

`.das_package` scripts are compiled with a restricted `.das_project` that only allows `require daslib/daspkg`. No `fio`, no `os`, no `unsafe` — the script can only call daspkg registration functions. If it tries to `require fio` → compilation error.

This is better than npm (arbitrary `postinstall` scripts) and similar to Go (declarative `go.mod`). daslang's existing `.das_project` sandbox mechanism makes this easy — no new security infrastructure needed.

Note: C++ packages can still do anything at build time (CMake scripts, compiler invocations). This is inherent to building C++ from source — same as cargo, Go cgo, etc. The trust boundary is "you chose to install a C++ package."

## Version Resolution

### v1: Simple, no constraint solver

**Install with version**: `daspkg install foo@1.2.0` → runs `resolve(sdk_version, "1.2.0")` → gets a git tag → clones it.

**Install without version**: `daspkg install foo` → runs `resolve(sdk_version, "")` → `.das_package` decides what "latest" means.

**Update one**: `daspkg update foo` → re-runs `resolve(sdk_version, "")` → if the resolved tag differs from installed, re-clone + rebuild.

**Update all**: `daspkg update` → does this for every installed package.

**Lock file** (`daspkg.lock`): single JSON file, machine-managed. Records all installed packages:

```json
{
  "sdk_version": "0.6.0",
  "packages": {
    "dasVulkan": {
      "source": "github.com/user/dasVulkan",
      "version": "1.2.0",
      "tag": "v1.2.0",
      "root": true
    },
    "dasUtils": {
      "source": "github.com/user/dasUtils",
      "version": "0.3.1",
      "tag": "v0.3.1",
      "root": false
    },
    "myLocalMod": {
      "source": "/home/me/dev/myLocalMod",
      "local": true,
      "root": true
    }
  }
}
```

- **`root: true`** — user explicitly installed this package
- **`root: false`** — pulled in as a transitive dependency
- On a fresh checkout, `daspkg install` (no args) reads the lock file and installs exact pinned versions. Reproducible builds.
- **Upgrade** only touches roots by default. Dependencies are re-resolved from the updated root's `.das_package`.

**Dependency version checks**: if package A declares `require_package("some_lib", ">=1.0")` and the installed `some_lib` is `0.9`, daspkg **warns** but doesn't auto-resolve. User manually runs `daspkg update some_lib`. No SAT solver, no automatic upgrade cascades.

### Diamond dependencies
Only one version of a package can be installed — daslang has a single `require` namespace, no way to load two versions simultaneously (unlike npm's nested `node_modules`).

**v1 rule:** first installed wins. If C@1.0 is already installed and package B wants C>=2.0, daspkg reports the conflict: "C@1.0 installed, B wants >=2.0. Run `daspkg update C` or `daspkg install C@2.0`." User decides.

Similar to Go modules — one version per module, manual upgrade. Works fine for small ecosystems.

### Future: constraint resolution
Could add a proper constraint solver later if the ecosystem grows large enough to need it. For now, the package count is small enough that manual resolution is fine.

## Remove and Orphan Cleanup

`daspkg remove foo`:
1. Remove `foo` from `modules/` and lock file
2. Check remaining packages' dependencies — any dep no longer needed by any remaining root?
3. For each orphan candidate, optionally run `sg` (ast-grep) to check if user code directly `require`s it
4. If unreferenced → ask user: "dasUtils is no longer needed. Remove? [y/n]"
5. If user code references it → warn but suggest promoting to root: "dasUtils is used by your code but no longer a dependency. Keep as root? [y/n]"

## C++ Build

Build directory: `modules/.daspkg_build/<name>/` — separate from the package source. Can be safely deleted and rebuilt with `daspkg build`.

```
cmake -B modules/.daspkg_build/<name> -S modules/<name> -DCMAKE_PREFIX_PATH=<sdk_path>
cmake --build modules/.daspkg_build/<name> --config Release
```

The package's `CMakeLists.txt` is responsible for placing output (`.shared_module`) into its own source directory (via `RUNTIME_OUTPUT_DIRECTORY` etc.) — same convention as `dascript-demo`. daspkg just runs cmake.

`modules/.daspkg_build/` should be in `.gitignore`.

## Error Handling and Rollback

### Install (new packages)
1. Resolve all deps first (run `.das_package` scripts — no downloads yet)
2. Clone all packages into `modules/.daspkg_tmp/<name>/`
3. Build C++ packages in dependency order — still in temp
4. **All succeed** → move each from temp to `modules/<name>/`, update `daspkg.lock`
5. **Any step fails** → delete `modules/.daspkg_tmp/`, lock file unchanged, `modules/` untouched

### Update (existing packages)
1. Move old `modules/<name>/` to `modules/.daspkg_backup/<name>/`
2. Install new version into temp, then move to `modules/<name>/`
3. **Success** → delete backup, update lock file
4. **Failure** → restore from backup, lock file unchanged

### Key principle
`modules/` and `daspkg.lock` are always in a consistent state. Either the operation fully completes or it fully rolls back. No half-installed packages.

## SDK Discovery and C++ ABI

### How it works
- daspkg finds the installed SDK and uses its CMake package (`find_package(DAS)`)
- The SDK's `DASConfig.cmake` already exports targets (`DAS::libDaScriptDyn`, `DAS::daslang`), compiler flags, include paths — everything CMake needs
- C++ ABI is implicit in the SDK install — same compiler, same flags, same platform. No need to track compiler details separately
- If you upgrade the SDK, rebuild C++ packages: `daspkg build`

### SDK version detection
- `daslang --version` → get current SDK version string
- Compared with `sdk_version` in `daspkg.lock` on every command
- If mismatch → warn: "SDK version changed (0.5 → 0.6). C++ packages need rebuilding. Run `daspkg build`."
- Since daspkg runs AS `daslang utils/daspkg/main.das`, the SDK version is the version of the binary running daspkg itself

### SDK location resolution (in order)
1. `--sdk <path>` command-line flag (explicit override)
2. `DASLANG_SDK` environment variable
3. Auto-detect from `daslang.exe` location (walk up to find `lib/cmake/DAS/`)

### Custom SDK builds
Users who build daslang from source set `DASLANG_SDK` to their build's install prefix. Everything else works the same — `find_package(DAS)` picks it up.

### Lock file
Tracks `package_version + sdk_version`. When the SDK version changes, daspkg warns that C++ packages need rebuilding.

## Open Questions

- [x] ~~Should packages live inside the SDK or per-project?~~ Per-project `modules/` — runtime already scans it
- [x] ~~Transitive C++ deps / build order~~ — topological sort on dependency graph, build in order. CMake handles the rest (authors set up their own `find_package` / include paths). daspkg just ensures deps are installed before dependents.
- [x] ~~Package tests~~ — not a daspkg concern. Authors put tests in `modules/<name>/tests/`. dastest can learn to glob `modules/*/tests/*.das`. Separate enhancement to dastest, not the package manager.
- [x] ~~Index/discovery~~ — single curated file in main repo, authors add via PR
