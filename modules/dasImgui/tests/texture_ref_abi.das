options gen2
options _allow_imgui_legacy = true
require imgui/imgui_harness

// Run interpreted, with -jit, and as a standalone -exe, always with -- --headless.
[export]
def main(){
    harness_init("Texture reference ABI", 320, 180, 0)
    for (kind in range(5)){
        verify(harness_begin_frame())
        harness_new_frame()
        unsafe { // nolint:STYLE025 -- ImTextureRef requires an unsafe local lifetime.
            var dl = GetForegroundDrawList()
            let id = 0x12345678abcdef01ul + uint64(kind)
            let ref = ImTextureRef(id)
            if (kind == 0){*dl |> AddImage(ref, float2(0), float2(100), float2(0), float2(1), 0xffffffffu)}
            elif (kind == 1){*dl |> AddImageQuad(ref, float2(0), float2(100, 0), float2(100), float2(0, 100), float2(0), float2(1, 0), float2(1), float2(0, 1), 0xffffffffu)}
            elif (kind == 2){*dl |> AddImageRounded(ref, float2(0), float2(100), float2(0), float2(1), 0xffffffffu, 5.0, ImDrawFlags.None)}
            elif (kind == 3){*dl |> PushTexture(ref)}
            else{*dl |> _SetTexture(ref)}
            let actual = dl.CmdBuffer[0].TexRef |> GetTexID()
            print("texture operation {kind}: expected={id}, actual={actual}\n")
            assert(actual == id, "Texture handle must survive the native binding")
            if (kind == 3){*dl |> PopTexture()}
        }
        harness_end_frame()
    }
    harness_shutdown()
}
