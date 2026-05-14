---
slug: emsdk-step0-only-clones-need-install-activate-too
title: Why does my CI Emscripten WASM build fail with "Could not find toolchain file" when the emsdk repo was cloned successfully?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**`step0_emsdk_install.sh` (a one-liner: `git clone https://github.com/emscripten-core/emsdk.git`) only clones the emsdk *wrapper*. The actual Emscripten toolchain — `emsdk/upstream/emscripten/`, which is what `-DCMAKE_TOOLCHAIN_FILE=.../Emscripten.cmake` resolves against — only lands after you also run `./emsdk install <version>` and `./emsdk activate <version>`.**

Without the install+activate, `emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake` does not exist, cmake errors with:
```
CMake Error at .../CMakeDetermineSystem.cmake:159 (message):
  Could not find toolchain file:
   "../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
```

The trap: `step0_emsdk_install.sh` looks like it's doing the install. It's only doing the clone. The actual install is in the activate script (`step1_emsdk_activate_linux.sh`):

```bash
./emsdk/emsdk install latest
./emsdk/emsdk activate latest
source ./emsdk_env.sh
```

## CI recipe

```yaml
- name: "Build WASM (Emscripten)"
  run: |
    set -eux
    cd web
    bash step0_emsdk_install.sh         # clone wrapper
    ./emsdk/emsdk install latest        # download toolchain (~1 GB)
    ./emsdk/emsdk activate latest       # set version pointer
    bash -c "source emsdk/emsdk_env.sh && \
             cd build && cmake ... && ninja"
```

`source ./emsdk_env.sh` only sets env vars; without `install + activate` first, the env points at nothing useful. Pin to a specific version (`install 3.1.74`) for reproducible deploys instead of `latest`.

## Where this bit me

daslang.io Forge redesign (PR #2648): the new pages.yml shipped without the install+activate pair, so the WASM build stage failed every deploy. `continue-on-error: true` painted it green in the run summary, and the original staging guard `if [ -d web/output ]` was satisfied (cmake configure created the empty dir even on failure) — silent breakage for hours. The round-5 placeholder gate `if [ -f web/output/daslang_static.js ]` surfaced it loudly when the post-merge deploy finally served the "Runtime rebuild in progress" page. Fixed by adding the two missing commands in [pages.yml](.github/workflows/pages.yml) ahead of the cmake call.

## Questions
- Why does my CI Emscripten WASM build fail with "Could not find toolchain file" when the emsdk repo was cloned successfully?
