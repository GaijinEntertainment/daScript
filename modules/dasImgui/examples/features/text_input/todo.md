# CJK and emoji input acceptance

This example uses the same source on desktop and in the browser. It contains two
single-line fields, a multiline field, and the dasImgui/text editor. The first field
counts Enter submissions. Load samples to compare glyph rendering and editing;
Clear resets the fields and counter. Fonts are bundled, with original licences,
attribution, versions and SHA256 values in `fonts/manifest.json`.

Run from the SDK root:

```sh
bin/daslang modules/dasImgui/examples/features/text_input/main.das
bin/daslang modules/dasImgui/examples/features/text_input/main.das -- --headless --headless-frames=120
bin/daslang utils/daspkg/main.das -- release --root modules/dasImgui/examples/features/text_input --out build/text-input-native
bin/daslang utils/daspkg/main.das -- release wasm --root modules/dasImgui/examples/features/text_input --out build/text-input-web --wasm-lib-dir web/output64/lib
```

Activate emsdk before the web build. Serve the output with COOP/COEP headers for
its worker-enabled runtime. `modules/dasImgui/tests/text_input_browser.cjs` takes
the web release directory and uses an isolated local server/browser. A live native
host exposes `input_state` and the normal `imgui_snapshot` command. Browser
inspection is available at `/text-input-state.json` in the app's virtual filesystem.
Use an absolute script path when launching a live host with a different cwd.
The regular browser regression command is documented in [BROWSER_TESTS.md](../../../BROWSER_TESTS.md).

## Confirmed capabilities and gaps

- Native sample glyph checks cover Chinese, Japanese, Korean and supplementary-plane
  emoji. Noto Sans + Noto Sans CJK SC + Twemoji Mozilla load without OS-font discovery.
  Basic emoji such as the grinning face and rocket render in color.
- Complex emoji sequences currently render as separate components: the skin-tone
  modifier, regional-indicator flag pair and woman-technologist ZWJ sample do not
  become one shaped glyph. Grapheme-aware editing is separate from glyph shaping.
  Do not mark emoji sequences supported because their UTF-8 text round-trips.
- The CJK font uses Simplified Chinese glyph forms. Regional Japanese/Korean/Traditional
  Chinese forms need a separate font-selection/shaping check; glyph existence alone
  does not prove the appropriate regional form.
- Stock ImGui and the dasImgui/text editor have different cursor/deletion behavior.
  Test both, including combining marks, emoji modifiers, flags and ZWJ sequences.
- Browser composition previously committed correctly but its textarea was invisible.
  Visible preedit passes the Chromium regression; CDP supplies a reproducible
  regression, not proof of real OS candidate-window behavior.
- The custom text editor needs explicit platform text-input ownership and caret
  publication for browser IME. This is distinct from its existing grapheme model.
- macOS GLFW accepts committed/marked text, but its candidate rectangle is based on
  the view origin, not the ImGui caret. Marked text and Enter/command routing need
  native Cocoa integration and real input-method checks.
- Windows has the ImGui/GLFW IMM candidate-position path. Real Microsoft IME
  conversion, cancellation, Enter routing and supplementary characters remain
  unverified; existing hooks do not constitute acceptance.
- Linux X11 has XIM committed-text support. Preedit, candidate placement and key
  consumption need testing with IBus/Fcitx. The SDK configuration currently disables
  Wayland; do not report Wayland acceptance from an X11 test.

## Manual matrix

Record OS/build, browser/version, input method, field type, result and evidence.
An unchecked item is unverified, not a known failure.

| Platform | Input methods / actions | Status |
| -- | -- | -- |
| macOS native | Pinyin, Japanese conversion, Korean 2-set; emoji picker | Pending; caret/preedit gap above |
| Windows native | Microsoft Pinyin/Japanese/Korean; Win+. emoji picker | Pending |
| Linux X11 | IBus/Fcitx Pinyin, Japanese, Korean; emoji/paste | Pending |
| Chromium desktop | Real OS IME + synthetic CDP composition regression | Synthetic regression passes; real IME pending |
| Safari desktop | Real OS IME, emoji picker, focus changes | Pending |
| Firefox desktop | Real OS IME, emoji picker, focus changes | Pending |
| Mobile browsers | Software keyboard, conversion, viewport movement | Pending |

For every supported cell:

1. Compose several characters and move among candidates. Preedit stays visible,
   remains uncommitted and follows the caret without duplication.
2. Enter confirms conversion without incrementing the submission count. A separate
   Enter submits once. Escape cancels without leaking a key into another action.
3. Change focus during composition, including to an ordinary DOM input in web tests.
   No late commit enters the new field. Scroll/resize/zoom and check candidate placement.
4. Replace a selection, undo/redo, copy/cut/paste, then continue typing. Include
   decomposed accents, supplementary-plane characters and mixed CJK/Latin text.
5. Delete and navigate across emoji clusters. Compare stock fields and the custom
   editor; distinguish logical grapheme boundaries from shaped visual glyphs.
6. Inspect rendered widget buffers and actual pixels, not only model values. Check
   fonts/licences in a relocated native bundle and the actual web output.

## Remaining implementation / verification

- [x] Visible browser preedit, cancellation/focus isolation and masked-field checks (Chromium).
- [x] Platform input ownership/caret for the custom editor; neighboring controls retain focus behavior.
- [x] Run automated native input/rendered-buffer and browser composition/committed-input tests; real OS IME acceptance remains below.
- [x] Verify relocated native bundle and browser artefacts include all fonts/licences.
- [x] Run the module's required host preflight and applicable AOT checks.
- [ ] Complete native OS input-method sessions above; keep unresolved desktop work here.
- [ ] Add shaping for complex emoji only with observable rendering/selection regressions.
