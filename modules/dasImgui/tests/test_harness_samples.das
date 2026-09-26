options gen2
require dastest/testing_boost
require imgui/imgui_playwright
require daslib/json
[test]
def zero_samples_creates_single_sample_framebuffer(t : T?){
 with_imgui_app(get_das_root() + "/modules/dasImgui/tests/harness_samples_app.das") $(d){
  var snap = wait_for_widget(d, "SAMPLES_WINDOW/SAMPLES_VALUE", 15.0)
  t |> success(snap != null, "framebuffer probe starts"); if (snap == null){return }
  let value = widget_payload_field(snap, "SAMPLES_WINDOW/SAMPLES_VALUE", "value") ?? ""
  if (value == "-1"){delete_json(snap); t |> skip("a headless harness has no OpenGL framebuffer"); return }
  t |> equal(value, "0", "explicit zero disables multisampling on a real framebuffer"); delete_json(snap)
 }
}
