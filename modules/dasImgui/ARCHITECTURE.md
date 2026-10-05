# dasImgui architecture

## 1. Browser callback routing {#browser-callback-routing}

On Emscripten, ImGui ignores the embedded GLFW scroll callback. Initialization
installs ImGui's browser callbacks against the persistent `#canvas` selector.
The wheel listener bypasses GLFW, so `das_imgui_set_real_input_callbacks` installs
and removes it alongside the GLFW callbacks to keep the real-input gate coherent.

The binding removes its resize and fullscreen listeners before deleting the backend.
Those listeners retain backend data; upstream shutdown only removes the wheel listener.
Ownership follows the context that installed the embedded-GLFW browser callbacks.
The contrib GLFW port retains its own callback lifecycle.

Embedded GLFW updates its polled cursor coordinates on touch-start without sending
its cursor-position callback. The browser mouse-button bridge queues that current
position before forwarding the button event to ImGui. This preserves a quick tap's
location even when press/release arrive before the next frame. It does not synthesize
additional GLFW cursor callbacks. Installation follows backend initialization and
real-input re-enabling; backend callback restoration removes the bridge again.

## 2. Touch {#touch}

On a touch device (`glfw_is_touch_device`, or `--imgui-touch` after the daslang `--`) the live
init bakes the font at 17 px instead of 14, sets `IsTouchScreen` (which the snapshot reports),
pads every hit box through `TouchExtraPadding`, widens the scrollbar and grab, and moves windows
only by their title bar, so a finger dragging over a window body never moves it. One finger stays
the mouse the backend already makes of it. Two fingers scroll: the harness feeds the frame's
finger list to `imgui_touch`, which on the frame the second finger lands releases the mouse
button (clearing that frame's whole event queue, so a press that landed with the second finger
never becomes a click) and hovers their centroid once, so the wheel lands on the window under
them; a hover every frame would hold each wheel a frame behind it under ImGui's trickle rule. It
then turns the centroid's motion into wheel events at the distance ImGui scrolls per unit - five
font heights vertically, two horizontally, at the in-frame font size - so content follows the
fingers one to one. A finger landing or lifting while two stay down moves the centroid but not
the content. When the fingers lift, the last delta decays over a few frames as a flick. The machine is a pure
function of the finger list, and `imgui_touch_inject` feeds a test's fingers into the same path;
injected fingers persist until the next injection, because a headless host runs thousands of
frames between two commands.

## 3. Modified Enter shortcuts

The pinned ImGui 1.92.6 input editor uses exact modifier shortcuts for Enter.
`patches/input_routing.cmake` adds Shift+Enter and Shift+KeypadEnter to that routing;
the existing multiline newline/validation policy still decides what they do. The
configure step is idempotent and refuses an unrecognized upstream implementation,
so an ImGui update requires reviewing the patch rather than silently dropping it.
The headless `test_io_synth_text.das` test drives both modified keys into a real
multiline widget and checks its resulting buffer. The same patch set preserves queued
text-before-mouse ordering so a mouse click cannot redirect an earlier text commit
into the newly focused editor.

## 4. Browser clipboard

The GLFW browser platform layer installs its own clipboard callbacks. The native
clipboard installer leaves them intact on Emscripten, where the desktop clipboard
module reports unsupported. Browser paste events supply bounded plain-text bytes;
`TextEncoder` converts DOM UTF-16 to well-formed UTF-8. ImGui consumes those bytes
through its normal paste operation, preserving selection replacement and undo.
Ordinary HTML inputs are excluded from the canvas clipboard listener.

The current backend owns the listener and buffer, gates them with real input and
text focus, and clears them on shutdown or owner replacement. Clipboard writes use
the asynchronous browser API without substituting an internal fake clipboard on
failure. Initial browser platform setup detects macOS so its Command shortcuts use
ImGui's logical Control mapping. Browser keyboard/paste coverage lives in
`tests/browser_input.cjs`; native key synthesis alone does not exercise this path.

## Browser composition ordering {#browser-composition-ordering}

The browser input bridge commits pending composition text in a capture-phase
`pointerdown` listener before GLFW queues the following `mousedown`. The committed
text therefore precedes the click that can focus another editor in ImGui's input
queue.

## 6. A texture reference crosses the native JIT by address {#texture-ref-abi}

The native JIT passes a managed struct argument by address. `ImTextureRef` is a managed
struct, so an ImGui entry point that takes it by value receives a pointer where it expects
the struct, and the texture handle does not survive the call. `ImDrawList::AddImage`,
`AddImageQuad`, `AddImageRounded`, `PushTexture` and `_SetTexture` are therefore excluded
from the generator (`imgui_skip_func` in `bind/bind_imgui.das`) and hand-bound in
`src/dasIMGUI.main.cpp` through wrappers taking `const ImTextureRef &`, which the
interpreter and the JIT both call with a pointer. `tests/texture_ref_abi.das` runs
interpreted and under `-jit` (CTest `imgui_texture_ref_abi_interpreted` /
`imgui_texture_ref_abi_jit`) and asserts the handle survives each of the five calls.
