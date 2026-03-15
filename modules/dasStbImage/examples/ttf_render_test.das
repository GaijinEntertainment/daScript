options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Visual verification: renders text into a PNG image using software rasterization.
// Run: bin/Release/daslang.exe modules/dasStbImage/examples/ttf_render_test.das
// Output: modules/dasStbImage/examples/ttf_render_test.png

require stbimage/stbimage_ttf
require stbimage/stbimage_boost
require fio

def get_script_dir() : string {
    return "{get_das_root()}/modules/dasStbImage/examples"
}

[export]
def main() {
    let font_file = "{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf"
    var font <- load_ttf(font_file, [pixel_height = 32.0f])
    if (!font.bitmap.valid()) {
        print("ERROR: failed to load font\n")
        return
    }
    let m = font_metrics(font)
    print("Font loaded: atlas {font.bitmap.width}x{font.bitmap.height}, ascent={m.ascent}, descent={m.descent}, line_height={m.line_height}\n")
    // create output image: dark background, RGBA
    let img_w = 640
    let img_h = 200
    var img = make_image(img_w, img_h, 4)
    // fill with dark gray background (RGBA packed as uint)
    let bg = uint(32u) | (uint(32u) << 8u) | (uint(40u) << 16u) | (uint(255u) << 24u)
    img.fill_rect(0, 0, img_w, img_h, bg)
    // render several lines of text
    let texts = fixed_array("Hello, daslang! 0123456789",
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                            "abcdefghijklmnopqrstuvwxyz",
                            "!@#$%^&*()_+-=[]|;':,./<>?")
    for (line_idx in range(4)) {
        let baseline_y = int(m.ascent) + line_idx * int(m.line_height) + 10
        render_text(img, font, texts[line_idx], 10, baseline_y, 255, 255, 255)
    }
    // save
    let out_path = "{get_script_dir()}/ttf_render_test.png"
    let (ok, err) = img.save(out_path)
    if (ok) {
        print("Saved: {out_path}\n")
    } else {
        print("ERROR saving: {err}\n")
    }
}
