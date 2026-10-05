options gen2
options _allow_imgui_legacy = true
require imgui/imgui_harness

// Run interpreted and with -jit, always with -- --headless.
let OPERATIONS = fixed_array("AddImage", "AddImageQuad", "AddImageRounded", "PushTexture", "_SetTexture", "AddImage default colour", "AddImageQuad default colour", "AddImageRounded default flags")

[export]
def main(){
    harness_init("Texture reference ABI", 320, 180, 0)
    for (kind in range(length(OPERATIONS))){
        verify(harness_begin_frame())
        harness_new_frame()
        unsafe { // nolint:STYLE025 -- eight branches dereference dl; a let-ref cannot be wrapped as one expression.
            var dl = GetForegroundDrawList()
            let id = 0x12345678abcdef01ul + uint64(kind)
            let ref = ImTextureRef(id)
            if (kind == 0){*dl |> AddImage(ref, float2(0), float2(100), float2(0), float2(1), 0xffffffffu)}
            elif (kind == 1){*dl |> AddImageQuad(ref, float2(0), float2(100, 0), float2(100), float2(0, 100), float2(0), float2(1, 0), float2(1), float2(0, 1), 0xffffffffu)}
            elif (kind == 2){*dl |> AddImageRounded(ref, float2(0), float2(100), float2(0), float2(1), 0xffffffffu, 5.0, ImDrawFlags.None)}
            elif (kind == 3){*dl |> PushTexture(ref)}
            elif (kind == 4){*dl |> _SetTexture(ref)}
            elif (kind == 5){*dl |> AddImage(ref, float2(0), float2(100), float2(0), float2(1))}
            elif (kind == 6){*dl |> AddImageQuad(ref, float2(0), float2(100, 0), float2(100), float2(0, 100), float2(0), float2(1, 0), float2(1), float2(0, 1))}
            else{*dl |> AddImageRounded(ref, float2(0), float2(100), float2(0), float2(1), 0xffffffffu, 5.0)}
            let actual = dl.CmdBuffer[0].TexRef |> GetTexID()
            print("{OPERATIONS[kind]}: expected={id}, actual={actual}\n")
            if (actual != id){panic("{OPERATIONS[kind]} lost the texture handle: expected {id}, got {actual}")}
            if (kind == 3){*dl |> PopTexture()}
        }
        harness_end_frame()
    }
    harness_shutdown()
}
