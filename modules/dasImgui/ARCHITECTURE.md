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
