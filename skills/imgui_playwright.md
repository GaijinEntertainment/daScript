# Driving a headless ImGui app: playwright

`widgets/imgui_playwright.das` is the test/recording driver layer over daslang-live's HTTP
live-command API. A driver process (`daslang.exe`) spawns a `daslang-live` **host** running a
feature file, then drives it with verbs (`click`, `right_click`, `drag`, `type_text`,
`imgui_key_chord`, `force_set`) and reads it back with `snapshot` + the `wait_*` family. Every
`tests/integration/test_*.das` and `record_*.das` is a playwright driver. The node-editor layer
`imgui_editor_playwright.das` (in dasImguiNodeEditor) adds an `EditorSession` + `ne_*` helpers on top.

Read this before writing or revising any playwright test or driver. For the recording-specific layer
(voiceover/music/self-verify), read `skills/recording.md` — it sits on top of everything here.

## The one rule: playwright is ASYNCHRONOUS — gate on the effect, never on a guess (REQUIRED)

The driver and the host are **separate processes**. Every verb POSTs over HTTP and returns as soon
as the command is **queued into the host's synth-IO state** — NOT when its effect has happened. The
host runs its own frame loop; the effect (hover → active → `click_count++`, node selection, window
focus, popup open, value change) lands **one or more frames later**. `click(app, target)` is a fire;
it is not a wait.

So after any action that a later step depends on, you **must wait for the observable effect** before
issuing the dependent action or asserting. Counting frames or `sleep()`-ing a fixed window is a
**guess that fails silently**: a headless host advances frames at multi-kHz (no vsync), so a
frame/time budget that "worked" windowed can expire in milliseconds — long before the effect lands.

### Worked failure: the clipboard chord drop

`click()` on a node both **selects** it and **focuses** the canvas. `Ctrl+D` duplicates the
*selected* node. A driver that clicked and immediately fired the chord:

```daslang
click(d, "{canvas}/node_1/NODE_TITLE[1]")
wait_for_mouse_idle(d)                                  // input drained — necessary, NOT sufficient
post_command(d, "imgui_key_chord", JV((mods = ["Ctrl"], key = "D")))   // fires before selection applied
```

dropped the chord on fast POSIX-headless: selection hadn't applied when `Ctrl+D` fired, so it
duplicated nothing and the node count stayed 2 instead of 3. Windows passed only because its slower
frame cadence let the selection land first — a **coincidence, not correctness**. Two earlier
mitigations (the chord's own frame-pacing, and a `FOCUS_SETTLE_LEAD_FRAMES` lead on the chord) were
both ineffective because neither gates on the *effect*. The fix gates on it:

```daslang
click(d, "{canvas}/node_1/NODE_TITLE[1]")
wait_for_mouse_idle(d)
t |> success(ne_wait_selected(s, 1) != null, "node_1 selected before Ctrl+D")   // EFFECT confirmed
post_command(d, "imgui_key_chord", JV((mods = ["Ctrl"], key = "D")))
t |> success(wait_for_key_idle(d, 4.0f), "Ctrl+D chord completes")
```

Rule of thumb: **WRONG** = `click` → dependent action. **RIGHT** = `click` → wait-for-effect →
dependent action. The "effect" is whatever the next step needs to be true (selected, rendered,
focused, value-changed), read from the snapshot.

## The wait family — what each one actually guarantees

| Wait | Guarantees | Use for |
|---|---|---|
| `wait_for_widget(app, id)` | `globals[id]` present (registered at init **or** painted once) | subprocess-startup gating only |
| `wait_for_visible(app, id)` / `wait_for_render` | present **AND painted this frame** | a click target that can hide & re-show (popups, menus) — `wait_for_widget` stale-passes a re-shown widget |
| `wait_for_int_value` / `wait_for_bool_value` / `wait_for_string_value` / `wait_for_payload_value` | `globals[id].payload[field] == expected` (typed) | the value effect on a known widget+field |
| `wait_until_sec(app, secs) $(snap) { pred }` | your predicate over the snapshot returns true | anything the typed helpers don't cover — the general escape hatch |
| `wait_for_mouse_idle` / `wait_for_key_idle` | synth **input** timeline drained | bounding the input animation — pair with an effect gate; on its own it does NOT prove the effect |
| `expect_value` / `expect_render` | assertion forms (panic on timeout) | raw `[test]` functions that should hard-fail |
| `await_quiescent` | whole-server "settled" (coarse) | last resort — see caveat below |

- **`wait_until_sec` is the workhorse** for "wait until the editor state shows the effect." The
  node-editor `ne_wait_*` helpers are all thin wrappers over it.
- **`wait_for_mouse_idle`/`wait_for_key_idle` bound the *input*, not the *state* it produces.** They
  are necessary (don't fire the next input mid-animation) but not sufficient (the widget effect may
  still be a frame out). Always follow with an effect gate when a later step depends on the effect.
- **`await_quiescent` is coarse and risky.** It waits for the *whole app* to go idle, not for a
  specific effect — and a focus-setting click can leave input active so quiescence is never reached,
  which floods the host with `await` POSTs and stalls libhv on Windows (this regressed CI once).
  Prefer a targeted effect gate (`wait_for_*` / `ne_wait_*`) over `await_quiescent`.

### Don't let a wait silently pass (wait-gate holes)

- **Absent/misspelled path.** `widget_rendered` requires existence now (no false-pass), but any
  predicate you write yourself with `... ?? true` will **pass on an absent path**. Write predicates
  fail-closed: `?? false` for "is it there/true yet?" checks.
- **Re-showable widgets.** `wait_for_widget` returns on a stale registry entry from a prior
  appearance, so a click fired right after can dispatch before the widget re-renders and be dropped.
  Use `wait_for_visible` for any target that may have been shown before.

## Node-editor layer (`imgui_editor_playwright`, in dasImguiNodeEditor)

`EditorSession` bundles `{app, handle, canvas}`; `ne_open(app, canvas)` fills it. The `ne_*` verbs
inject the editor handle into the live commands. The effect gates:

| Gate | Effect it confirms |
|---|---|
| `ne_wait_widget(s, subpath)` | a node/pin (`"node_5"`, `"node_5/pin_8"`) is present |
| `ne_wait_selected(s, id)` | node `id` reports `selected` — **gate `Ctrl+D` / `Ctrl+C` on this** |
| `ne_wait_payload_str(s, field, expected)` | the canvas payload's string `field` reached `expected` |
| `ne_wait_shortcut(s, action)` | the editor served the injected/real chord (`last_shortcut_action`) |

When you add a new public `ne_*` (or any documented-module public function), **add it by name to the
matching `group_by_regex()` in `utils/node_editor2rst.das`** — the groups use explicit name lists, not
`ne_*` wildcards, so a new helper otherwise lands in an "Uncategorized" doc section and the Pages
**build job fails** at the uncategorized-gate.

## One live host per port (REQUIRED)

daslang-live binds `DEFAULT_LIVE_PORT = 9090`. **One host per port.** `dastest
--isolated-mode-threads N` spreads workers across `9090 + worker_index`; a plain sequential run always
reuses 9090.

**Manually-launched probe host? First disable user control.** A host you launch by hand (CLI or
`mcp__daslang__live_launch`) starts with user-control ON, so the real OS cursor races GLFW's poll and
clobbers `io.MousePos` — synth gestures miss and snapshots read the OS cursor. Post
`set_user_control` `{"enabled":false}` before any synth input. (`with_recording_app` / the dastest
harness do this for you; a bare probe host does not.)

**Never run two headless suites at once on the same port — and Windows + WSL are NOT separate here.**
WSL2's `localhostForwarding` (the `.wslconfig` default) mirrors a WSL listener onto Windows
`localhost`, and Win11 mirrored-networking shares the namespace outright, so `127.0.0.1:9090` is
**bridged across the two**. Two concurrent runs cross-talk and produce **mass ~120s
`daslang-live timed out` failures on whichever tests are mid-flight** — different tests each run, so it
reads like a regression but is pure port contention. A timed-out dastest also leaves its `daslang-live`
child holding 9090; sweep before re-running:

```powershell
Get-Process -Name daslang,daslang-live -ErrorAction SilentlyContinue | Stop-Process -Force
```
```bash
pkill -9 -f daslang          # WSL / POSIX
```

Run dual-platform verification **sequentially** — one platform fully finished before the other starts.

## Mental model recap

Driver process (`daslang.exe test_X.das`) ⇄ HTTP/9090 ⇄ host process (`daslang-live feature.das`,
its own frame loop). Verbs POST and return on *queue*; effects land *later*; the `wait_*` family is how
you observe an effect before depending on it. Recordings (`skills/recording.md`) are playwright drivers
that additionally self-verify each interaction and carry a voiceover/music soundtrack.
