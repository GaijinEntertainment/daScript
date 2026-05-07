# daslang — WebAssembly build

Builds daslang (compiler + runtime) as WASM via Emscripten.
Outputs land in `output/`: `daslang.wasm`, `daslang_static.wasm`, `das-fmt.wasm`, plus JS loaders.

## Prerequisites

- **CMake ≥ 3.16**, **Ninja**
- **Emscripten SDK** — installed by the step scripts below (≈ 1 GB, clones into `web/emsdk/`)

## Install emsdk (once)

```bash
cd web/
bash step0_emsdk_install.sh
```

Clones https://github.com/emscripten-core/emsdk into `web/emsdk/`.

## Activate emsdk (each shell session)

**Linux / macOS**
```bash
bash step1_emsdk_activate_linux.sh
```

**Windows**
```bat
step1_emsdk_activate_windows.sh
```

Installs the latest toolchain and puts `emcc` on `PATH`. Must be run in every new shell before building.

## Configure and build

**Release** (recommended, smaller `.wasm`):
```bash
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -DCMAKE_TOOLCHAIN_FILE=../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ../
ninja
```

**Debug** (larger, with DWARF symbols):
```bash
cp ../CMakeXxdImpl.txt .
rm -rf cmake_temp && mkdir cmake_temp && cd cmake_temp
cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
      ../
ninja
```

First build takes 5–15 min; incremental rebuilds are faster (just re-run `ninja` from `cmake_temp/`).

## Running locally

WASM files require a local HTTP server (browsers block `file://` for WASM).

```bash
cd output/
python3 -m http.server 8080
```

Open http://localhost:8080 — the in-browser daslang IDE loads.

Or serve with any static server (Node `serve`, nginx, etc.).

## Outputs

| File | Description |
|---|---|
| `output/daslang_static.js` + `.wasm` | interpreter (no dynamic modules) |
| `output/das-fmt.js` + `.wasm` | Source formatter |
| `output/index.html` | Web IDE entry point |

## Troubleshooting

**`emcc: not found`** — run Step 1 (`source emsdk/emsdk_env.sh`) in the current shell.

**CMake can't find Emscripten** — verify `EMSCRIPTEN` env var is set: `echo $EMSCRIPTEN`.

**Build fails with SIGSEGV / stack overflow** — use Release build or increase stack size; Debug stack is much larger.

**Browser shows blank page** — must serve via HTTP, not `file://`.
