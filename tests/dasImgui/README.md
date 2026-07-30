# dasImgui integration tests

Two complementary test surfaces live in this directory.

## 1. dastest integration suite — daslang-native, full coverage

`test_*.das` files exercised by daslang's `dastest` runner. Each test spawns
`daslang-live` against a feature app under `modules/dasImgui/examples/features/`
(or `.../save_demo/`), drives it via `require imgui/imgui_playwright`,
and asserts behavior. This is the canonical regression suite.

The suite is **nightly-only in CI** (`nightly_imgui.yml`): the `tests/.das_test`
gate skips this directory in `--test tests/` full sweeps unless the runner
passes `--imgui`, so per-PR sweeps never pay for the 151 subprocess spawns.
Targeting the folder directly (as below and as the nightly does) bypasses the
gate — no flag needed.

Run from the daScript repo root:

```
daslang.exe dastest/dastest.das -- --test tests/dasImgui --headless
```

Expect ~151 tests (one daslang-live subprocess per test, ~4-5s each; add
`--isolated-mode --isolated-mode-threads 4` to parallelize).

## 2. Curl smoke — mechanism-level, tool-agnostic

`smoke_curl.ps1` (Windows) and `smoke_curl.sh` (POSIX) drive
`examples/save_demo/main.das` entirely via raw HTTP. NO playwright module,
NO dastest. They cover one happy path: spawn → ready → snapshot → click
SAVE_BTN → poll STATUS_TEXT.value == "saved" → shutdown.

```
powershell -ExecutionPolicy Bypass -File tests/dasImgui/smoke_curl.ps1
bash       tests/dasImgui/smoke_curl.sh
```

`smoke_curl.sh` requires `curl` and `jq` on PATH.

### Which one to run when

- **Most days**: the dastest suite. Full surface, fast feedback per test.
- **A dastest failure that doesn't make sense**: run the curl smoke. If it
  PASSes, the bug is somewhere in `widgets/imgui_playwright.das` or the
  test's wait/assertion logic. If the smoke FAILs too, the bug is deeper
  — daslang-live's dispatch, dasImgui's server-side `imgui_*` commands,
  or the playwright transport's HTTP layer.
- **Onboarding a non-daslang tool** (LLM-driven UI tester, headless probe,
  custom IDE integration, etc.): the curl smoke is the documented shape.
  Read it as the contract for "what a third-party client should do."

## 3. Recordings — MP4 artifact generation

`record_*.das` files produce one tutorial-companion video each. They
are NOT in CI; they're one-shell driver scripts that spawn their own
`daslang-live` host via `with_recording_app`, post a narrate/click/drag
timeline, and save an `.apng`. A single ffmpeg pass converts each
`.apng` to the shipped `.mp4` (~300× smaller). The `.mp4` files are NOT
committed — they ship as assets on the rolling `docs-assets` GitHub
release; docs builds stage them via `utils/docs_assets/fetch.ps1` before
sphinx runs.

### The script

| Script | Purpose | When to run |
|---|---|---|
| `rerecord_all.ps1` | Sequentially re-record every `record_*.das` driver. Writes intermediate APNGs to the gitignored `modules/dasImgui/doc/source/_static/tutorials/`. ~20 min. | After cross-cutting visual changes (theme, font, narrate placement) or after touching multiple drivers. |

After re-recording, convert APNGs to MP4 via ffmpeg (the deliverables).

### Full workflow

Step 1 — generate APNGs locally (PowerShell):

```powershell
# One-time setup — point at your daslang build (repo-root relative works)
$env:DASLANG_EXE = "bin/Release/daslang.exe"

# Whole sweep (all drivers found via glob, ~20 min):
pwsh tests/dasImgui/rerecord_all.ps1
# OR single driver (uses the env var set above; works whether or
# not daslang.exe is on PATH):
& $env:DASLANG_EXE -project_root . tests/dasImgui/record_X.das
```

Step 2 — eyeball-review the resulting `.apng` files in
`modules/dasImgui/doc/source/_static/tutorials/`. Extract individual frames
if needed (bash; on PowerShell call `bash -c '<the command>'` or use WSL):

```bash
ffmpeg -i modules/dasImgui/doc/source/_static/tutorials/X.apng \
    -vf "select=eq(n\,200)" -frames:v 1 -update 1 frame200.png -y
```

Step 3 — convert `.apng` → `.mp4` (the deliverable). Single recording
works in either shell:

```bash
cd modules/dasImgui/doc/source/_static/tutorials
for f in *.apng; do
    base="${f%.apng}"
    ffmpeg -y -loglevel error -i "$f" -c:v libx264 -crf 23 \
           -pix_fmt yuv420p -movflags +faststart "$base.mp4"
done
```

Bulk-convert — PowerShell equivalent:

```powershell
Set-Location modules/dasImgui/doc/source/_static/tutorials
Get-ChildItem *.apng | ForEach-Object {
    $base = $_.BaseName
    ffmpeg -y -loglevel error -i $_.Name -c:v libx264 -crf 23 `
           -pix_fmt yuv420p -movflags +faststart "$base.mp4"
}
```

Step 4 — upload to the rolling `docs-assets` release (MP4s are not in git):

```bash
cd modules/dasImgui/doc/source/_static/tutorials
gh release upload docs-assets *.mp4 --clobber
```

### CI integration

The main-repo docs builds (`doc.yml`, `pages.yml`) run
`utils/docs_assets/fetch.ps1` to stage the release MP4s before sphinx.
Sphinx runs with `-W` and the video role checks file existence, so an RST
citing a missing `.mp4` fails the build. Implication for a new tutorial PR:
upload the `.mp4` to `docs-assets` ahead of the RST cite to keep CI passing.

### Adding a new recording

For a new tutorial `foo`:

1. Write `modules/dasImgui/examples/tutorial/foo.das` and
   `tests/dasImgui/record_foo.das` (see `skills/imgui_recording.md` for the
   driver template).
2. `daslang.exe -project_root . tests/dasImgui/record_foo.das` —
   produces `modules/dasImgui/doc/source/_static/tutorials/foo.apng`
   (gitignored).
3. Eyeball-review. ffmpeg-extract frames if needed.
4. ffmpeg-convert `foo.apng` → `foo.mp4` (see step 3 of the workflow above).
5. `gh release upload docs-assets foo.mp4`.
6. Write `doc/source/reference/tutorials/imgui/foo.rst` + add to the
   tutorials toctree.
7. Stage + commit + push: `git add record_foo.das foo.rst`,
   `git commit -m "tutorial: foo"`, `git push -u origin <branch>`.

### Historical note: orphan `assets` branch

Before the MP4 migration (PR-MP4), the 37-tutorial APNG set totaled
~1.7 GB and lived on an orphan `assets` branch (force-amend-pushed each
sweep). The branch is now dead — at ~5 MB of MP4 for the full set, the
source repo absorbs the deliverables cleanly. The `origin/assets` ref
can be deleted; nothing references it.

Detailed driver-authoring guidance, pacing constants, menu-interaction
gotchas, and visual aid recipes live in
[`../../skills/recording.md`](../../skills/recording.md).

## Notes

- Tests are dastest-discovered by `test_*.das` filename. Files starting
  with `_` are skipped (see `feedback_dastest_underscore_skip`).
- Interactive tests must wait for observable state: a widget value, exact
  hover target, event revision, committed selection, scroll position, or
  another domain acknowledgement. Do not use sleeps, elapsed time, timeline
  duration, or frame counts as success criteria. A wall-clock limit around a
  semantic poll is permitted only as a deadlock/crash guard.
- The harness is `require imgui/imgui_playwright public`, declared in
  `widgets/imgui_playwright.das` and registered via `.das_module`.
- `harness_begin_frame()` pumps `advance_coroutines()` automatically — feature
  apps no longer need to call it. `click()`, `right_click()`, `type_text()`,
  `drag()` and other coroutine-based playwright commands work in every demo
  that uses the harness API. Hosts that bypass the harness (raw `live_*` +
  `begin_frame` + `Impl_*_NewFrame` + `NewFrame`) still need the explicit call
  between `Impl_Glfw_NewFrame` and `NewFrame`.
