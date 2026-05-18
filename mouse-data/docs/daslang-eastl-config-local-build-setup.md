---
slug: daslang-eastl-config-local-build-setup
title: how do I locally build and verify the daslang EASTL CI configuration — clone setup, cmake invocation, what to test
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

CI workflow: `.github/workflows/build_eastl.yml`. Mirrors it for local repro:

```bash
# Clone EASTL and EABase separately — EASTL repo no longer has EABase as a submodule
# (the workflow's `--recurse-submodules` is now a no-op; EABase is a separate repo)
git clone --depth 1 https://github.com/electronicarts/EASTL.git eastl
git clone --depth 1 https://github.com/electronicarts/EABase.git eabase

cmake -B build_eastl -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DDAS_LLVM_DISABLED=ON -DDAS_GLFW_DISABLED=ON \
  -DDAS_TESTS_DISABLED=ON -DDAS_TUTORIAL_DISABLED=ON \
  -DDAS_AOT_EXAMPLES_DISABLED=ON \
  -DDAS_CONFIG_INCLUDE_DIR=cmake/das_config_eastl \
  -DCMAKE_CXX_FLAGS="-isystem $PWD/eastl/include -isystem $PWD/eabase/include/Common"

cmake --build build_eastl --target daslang --parallel
./bin/daslang --version    # smoke test — should print version like "0.6.2"
```

**The CI build only verifies the `daslang` target compiles** (`-DDAS_TESTS_DISABLED=ON`). Full test execution is not done; the goal is just compile-coverage to catch type-conversion bugs (e.g. eastl::string → std::filesystem::path).

**Gitignore**: add `eastl/`, `eabase/`, `build_eastl/` to `.gitignore` so the local checkouts don't show up as untracked. PR #2716 did this.

**Common errors and meanings:**
- `error: no member named 'X' in namespace 'das'` — your code uses `das::X` but X isn't visible via `using namespace eastl;` AND isn't explicitly `using std::X` in the EASTL config. Fix: either ensure it's in eastl, add an explicit `using std::X` to the EASTL config, or use a different primitive (e.g. `das_throw` instead of `throw das::exception`).
- Linker error on `das_throw` — declaration is gated on `DAS_ENABLE_EXCEPTIONS=0`; if the build sets `=1`, declaration disappears. As of PR #2716 the declaration is unconditional. Pre-PR was a known foot-gun.

**Note**: das_config_eastl includes `<EASTL/sort.h>`, `vector.h`, `string.h`, `type_traits.h`, etc. — but NOT `<EASTL/stdexcept.h>` (which doesn't exist anyway — EASTL throws `std::out_of_range` directly from `<stdexcept>` pulled transitively).

## Questions
- how do I locally build and verify the daslang EASTL CI configuration — clone setup, cmake invocation, what to test
