options gen2
options gc
require imgui/imgui_harness
require opengl/opengl_boost
require daslib/safe_addr
var actual_samples = -1
[export]
def init(){harness_init("Framebuffer samples", 320, 180, 0); if (!harness_is_headless()){glGetIntegerv(GL_SAMPLES, safe_addr(actual_samples))}}
[export]
def update(){if (!harness_begin_frame()){return }; harness_new_frame(); window(SAMPLES_WINDOW, (text = "Samples", closable = false, flags = ImGuiWindowFlags.None)){text(SAMPLES_VALUE, (text = "{actual_samples}"))}; harness_end_frame()}
[export]
def shutdown(){harness_shutdown()}
[export]
def main(){init(); while (!exit_requested()){update(); harness_maybe_collect_gc()}; shutdown()}
