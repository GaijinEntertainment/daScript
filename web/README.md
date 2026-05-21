# daslang — WebAssembly

Daslang supports WASM interpreter and direct compilation to WASM via LLVM JIT.

Builds daslang (compiler + runtime) as WASM via Emscripten.
Outputs land in `output/`: `daslang.wasm`, `daslang_static.wasm`, `das-fmt.wasm`, plus JS loaders.

## Prerequisites

- **CMake ≥ 3.16**, **Ninja**, **Python 3**
- **Emscripten** — `emcc` / `em++` / `emcmake` on `PATH`. Install via your package manager (easiest) or pin a specific version via `emsdk` (CI does this).

## Install Emscripten

**Ubuntu / Debian**
```bash
sudo apt install emscripten
```

**macOS (Homebrew)**
```bash
brew install emscripten
```

**Windows (Chocolatey)**
```powershell
choco install emscripten
```

After install, `emcc --version` should work in any new shell.

## Configure and build

From `web/`, with emsdk activated:

**Release** (recommended, smaller `.wasm`):
```bash
emcmake cmake -S . -B cmake_temp -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build cmake_temp
```

**Debug** (larger, with DWARF symbols):
```bash
emcmake cmake -S . -B cmake_temp -G Ninja -DCMAKE_BUILD_TYPE=Debug && cmake --build cmake_temp
```

## Running locally

WASM files require a local HTTP server (browsers block `file://` for WASM).

```bash
python3 -m http.server 8080 -d output/
```

Open http://localhost:8080 — the in-browser daslang IDE loads.

Or serve with any static server (Node `serve`, nginx, etc.).

## Outputs

| File | Description |
|---|---|
| `output/daslang_static.js` + `.wasm` | interpreter (no dynamic modules) |
| `output/das-fmt.js` + `.wasm` | Source formatter |
| `output/index.html` | Web IDE entry point |
| `output/samples/examples/<name>.wasm` | JIT-compiled examples (optional, see below) |

## JIT-compiled examples (optional)

The playground's "JIT (wasm)" radio runs precompiled `.wasm` artifacts cross-
compiled from each sample `.das` via dasLLVM's `--jit-target=wasm32-unknown-emscripten`.
Without these, the JIT radio stays disabled and only the interpreter runs.

Build steps (assumes host daslang already built with dasLLVM):

```bash
# 1. Build runtime archive (the wasm build documented above):
emcmake cmake -S web -B web/cmake_temp -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build web/cmake_temp --target libDaScript_runtime

# 2. Build the example + util .wasm files (from repo root):
cmake -B build -DDAS_LLVM_DISABLED=OFF -DDAS_BUILD_WASM=ON && cmake --build build --target all_wasm
```

Outputs land in `web/output/samples/examples/`. CI builds these in the
`wasm_cross` job (see `.github/workflows/wasm_build.yml`).

## Troubleshooting

**`emcc: not found`** — package-manager install: open a new shell or re-run your shell init. emsdk install: `source ./emsdk/emsdk_env.sh` in the current shell.

**CMake can't find Emscripten** — use `emcmake cmake ...` (not bare `cmake`), or verify `EMSDK`/`EMSCRIPTEN` env vars are set: `echo $EMSDK $EMSCRIPTEN`.

**`emcc: unrecognized option '-sWASM_LEGACY_EXCEPTIONS=0'` or similar** — distro emscripten is too old. Switch to emsdk install (Option B) and pin a current version.

**Build fails with SIGSEGV / stack overflow** — use Release build or increase stack size; Debug stack is much larger.

**Browser shows blank page** — must serve via HTTP, not `file://`.
