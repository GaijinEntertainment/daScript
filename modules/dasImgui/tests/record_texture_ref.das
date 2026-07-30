options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record texture_ref.apng against examples/tutorial/texture_ref.das.
//! imgui 1.92 ImTextureRef: a PNG decoded (dasStbImage) -> GL texture ->
//! ImTextureRef(_TexID) -> image(). Display-only tutorial, so the verified
//! "effect" is that the picture widget renders (wait_for_render on TR_PIC);
//! the rest is narration dwelling on the loaded image.

[export]
def main {
    with_recording_app("examples/tutorial/texture_ref.das",
                       "texture_ref.apng", 25) $(app) {
        let T_PIC = "TR_WIN/TR_PIC"

        var snap = wait_for_render(app, T_PIC, 10.0f)
        if (snap == null) {
            panic("{T_PIC} never rendered - texture failed to load/upload?")
        }

        say(app, "A real PNG shown via ImTextureRef", [T_PIC],
            [voice = "This is a real image - a P N G decoded, uploaded to a GL texture, and drawn through an Im Texture Ref whose Tex I D is the GL texture name. In imgui one nine two that ref is how you hand your own texture to image."])
    }
}
