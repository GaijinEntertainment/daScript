# dasVulkan module instructions

dasVulkan is the daslang binding + ergonomic boost layer for [Vulkan](https://www.vulkan.org/),
in-tree at `modules/dasVulkan/`. **How it is built and why - the two layers, the generator, the
ownership model, the boost conventions, the macOS mechanics - is `ARCHITECTURE.md` beside this
file. Read the section you are about to work in before writing code here.** The rules binding a
diff are `REVIEW.md`; postponed work is `ROADMAP.md`.

Follow the daslang **gen2** conventions - the root `CLAUDE.md` rules apply to every `.das` file
here.

## Locations

- Module source: `modules/dasVulkan/` (`src/`, `daslib/`, `generator/`, `examples/`,
  `tutorials/`, `utils/`, `vendor/`)
- Tests: `modules/dasVulkan/tests/integration/`
- CI: `.github/workflows/vulkan_checks.yml` (per-PR), `.github/workflows/nightly_vulkan.yml`
  (render suite)
- Docs: stdlib section + generated pages in the main Sphinx tree; tutorials at
  `doc/source/reference/tutorials/vulkan/`
- Recordings: intermediates under `tutorials/**/recording/` (gitignored); MP4 deliverables on
  the rolling `docs-assets` GitHub release

## Skill files (REQUIRED)

| Skill file | Read BEFORE... |
|---|---|
| `skills/internal/vulkan_recording.md` | Writing or revising any `tutorials/*/recording/record_*.das` driver |
| `skills/perf_lint.md` / `skills/style_lint.md` | Suppressing any lint finding here - the module is lint-clean and gated |
| `skills/daslang_live.md` | Touching `daslib/vulkan_live.das` (the live-command mirror of `opengl_live`) |

## Build & run

The boost layer is pure daslang - **editing `daslib/*.das` needs no rebuild**. Only C++ or
generator changes need the native module rebuilt, and then it is the ordinary in-tree build:

```
cmake --build build --config Release
```

**Build the shared twin at `--parallel 2`, not unbounded** - unbounded exhausts a 7 GB runner
(`ARCHITECTURE.md` sec.1):

```
cmake --build ./build --parallel 2 --target dasModuleVulkan
```

Run anything from the repo root:

```
bin/Release/daslang -project_root . modules/dasVulkan/examples/offscreen_triangle_boost.das
```

## Regenerating

`daslang generator/generate.das` regenerates both layers. **`--no-cpp` regenerates only the
boost** - the fast iteration loop, no C++ rebuild. `--boost-out` defaults to `daslib`, and a
full run rewrites `generator/skip_report.txt`.

## Tests

Run from the repo root, so the cwd-relative shader paths resolve:

```
bin/Release/daslang -project_root . dastest/dastest.das -- \
  --test modules/dasVulkan/tests/integration --isolated-mode --isolated-mode-threads 4
```

The gates a change here answers to:

```
bin/Release/daslang utils/lint/main.das -- modules/dasVulkan -q -j 0
bin/Release/daslang modules/dasVulkan/REVIEW.das
```

## Workflow

Work here goes through the repo PR flow (`skills/internal/make_pr.md`) and its gates; there is
no module-local hook or lint recipe.
