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
