options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_vision
require dasllama/dasllama_vision_io
require daslib/math_bits
require daslib/fio
require stbimage
require math

// Tier-0 vision preprocessing gates — model-free, bit-exact against llama.cpp's mtmd
// (`llama-mtmd-debug -p preproc`, CPU). The fnv1a64 pins and probe values come from the dumps
// in <models>/gemma4-vision-oracle/ — mint.sh there regenerates them (vision_plan.md).

let ALIGN = 48
let MIN_TOKENS = 40
let MAX_TOKENS = 280

def make_fixture(kind : string; w, h : int) : VisionImage {
    var img = VisionImage(width = w, height = h)
    img.rgb |> resize(w * h * 3)
    if (kind == "black") {
        pass   // resize already zero-fills
    } elif (kind == "white" || kind == "gray") {
        let v = uint8(kind == "white" ? 255 : 128)
        for (b in img.rgb) {
            b = v
        }
    } elif (kind == "cb") {
        for (y in range(h)) {
            for (x in range(w)) {
                let v = uint8(((x + y) % 2) != 0 ? 0 : 255)
                let p = (y * w + x) * 3
                img.rgb[p] = v
                img.rgb[p + 1] = v
                img.rgb[p + 2] = v
            }
        }
    }
    return <- img
}

// a valid 54-byte BMP header (BITMAPINFOHEADER, 24bpp) declaring w x h, with NO pixel data —
// header probes answer without any pixels (stbi would zero-fill an uncapped decode)
def bmp_header_stub(w, h : int) : array<uint8> {
    var bytes : array<uint8>
    bytes |> resize(54)
    bytes[0] = uint8('B')
    bytes[1] = uint8('M')
    bytes[10] = uint8(54)         // pixel-data offset
    bytes[14] = uint8(40)         // BITMAPINFOHEADER size
    for (i in range(4)) {
        bytes[18 + i] = uint8((w >> (i * 8)) & 0xFF)
        bytes[22 + i] = uint8((h >> (i * 8)) & 0xFF)
    }
    bytes[26] = uint8(1)          // planes
    bytes[28] = uint8(24)         // bits per pixel
    return <- bytes
}

// the oracle's gate: FNV-1a 64 over the f32le bytes of the normalized planes
def fnv1a64_f32(planes : array<float>) : uint64 {
    var h = 0xcbf29ce484222325ul
    for (f in planes) {
        let bits = uint(float_bits_to_int(f))
        h = (h ^ uint64(bits & 0xFFu)) * 0x100000001b3ul
        h = (h ^ uint64((bits >> 8u) & 0xFFu)) * 0x100000001b3ul
        h = (h ^ uint64((bits >> 16u) & 0xFFu)) * 0x100000001b3ul
        h = (h ^ uint64((bits >> 24u) & 0xFFu)) * 0x100000001b3ul
    }
    return h
}

def preproc_hash(t : T?; kind : string; w, h : int; want_w, want_h : int; want_hash : uint64) {
    let src <- make_fixture(kind, w, h)
    let target = vision_target_size(w, h, ALIGN, MIN_TOKENS, MAX_TOKENS)
    t |> equal(target.x, want_w, "{kind} {w}x{h} target width")
    t |> equal(target.y, want_h, "{kind} {w}x{h} target height")
    let boxed <- vision_letterbox(src, target)
    let planes <- vision_normalize(boxed)
    t |> equal(length(planes), want_w * want_h * 3, "{kind} plane count")
    let hash = fnv1a64_f32(planes)
    if (hash != want_hash) {
        t |> failure("{kind} {w}x{h}: fnv1a64 {hash} != oracle {want_hash} - preproc diverged from mtmd; regenerate via gemma4-vision-oracle/mint.sh and diff probes")
    }
}

[test]
def test_vision_geometry(t : T?) {
    t |> run("dynamic-resolution geometry matches the oracle table") @(t : T?) {
        // (src w, h) → (target w, h): round-to-align, then β down/upscale — all six oracle cases
        t |> equal(vision_target_size(640, 640, ALIGN, MIN_TOKENS, MAX_TOKENS), int2(624, 624))
        t |> equal(vision_target_size(640, 480, ALIGN, MIN_TOKENS, MAX_TOKENS), int2(624, 480))
        t |> equal(vision_target_size(800, 480, ALIGN, MIN_TOKENS, MAX_TOKENS), int2(816, 480))
        t |> equal(vision_target_size(480, 800, ALIGN, MIN_TOKENS, MAX_TOKENS), int2(480, 816))
        t |> equal(vision_target_size(100, 100, ALIGN, MIN_TOKENS, MAX_TOKENS), int2(336, 336))
        t |> equal(vision_target_size(3000, 2000, ALIGN, MIN_TOKENS, MAX_TOKENS), int2(960, 624))
        // 120/48 = 2.5 with both candidate areas in [min,max] (96x960 = exactly min stays):
        // ties must round AWAY (144); a half-even port gives 96 - elsewhere the beta branches mask it
        t |> equal(vision_target_size(120, 960, ALIGN, MIN_TOKENS, MAX_TOKENS), int2(144, 960))
    }
    t |> run("token counts stay inside the [min, max] contract") @(t : T?) {
        t |> equal(vision_token_count(int2(624, 624), ALIGN), 169)
        t |> equal(vision_token_count(int2(624, 480), ALIGN), 130)
        t |> equal(vision_token_count(int2(816, 480), ALIGN), 170)
        t |> equal(vision_token_count(int2(336, 336), ALIGN), 49)
        t |> equal(vision_token_count(int2(960, 624), ALIGN), 260)
    }
    t |> run("an extreme aspect ratio still holds the token budget") @(t : T?) {
        // the per-side align floor defeats the area constraint without the clamp: 300000x48
        // wants 1322 columns against a 1120-entry position table
        for (wh in [int2(300000, 48), int2(1000000, 48), int2(48, 1000000), int2(16777216, 1)]) {
            let tgt = vision_target_size(wh.x, wh.y, ALIGN, MIN_TOKENS, MAX_TOKENS)
            t |> success(vision_token_count(tgt, ALIGN) <= MAX_TOKENS, "{wh.x}x{wh.y} -> {tgt} within {MAX_TOKENS} tokens")
        }
        // int64 pixel math: max_tokens in the int32-wrapping band [932067, 1864135] must not
        // wrap max_pixels negative and collapse the canvas to one patch
        let big = vision_target_size(4000, 3000, ALIGN, MIN_TOKENS, 1000000)
        t |> success(vision_token_count(big, ALIGN) > 1000, "no int32 collapse at max_tokens=1e6 ({big})")
        // and the SOURCE product: w*h past 2^31 must not wrap before the beta ratio
        let vast = vision_target_size(16777216, 16777216, ALIGN, MIN_TOKENS, MAX_TOKENS)
        t |> success(vast.x > 0 && vast.y > 0 && vision_token_count(vast, ALIGN) <= MAX_TOKENS,
            "a 2^48-pixel source downscales sanely ({vast})")
    }
}

[test]
def test_vision_preproc_bit_exact(t : T?) {
    t |> run("square fixtures - pure downscale, no pad") @(t : T?) {
        t |> preproc_hash("gray", 640, 640, 624, 624, 0x08e308d22870c325ul)
        t |> preproc_hash("white", 640, 640, 624, 624, 0xafad97ec4b4a1b25ul)
        t |> preproc_hash("black", 640, 640, 624, 624, 0x9a81aa0f0ca61325ul)
        t |> preproc_hash("cb", 640, 640, 624, 624, 0xa71ad648937f4b85ul)
    }
    t |> run("non-square - the letterbox pads, content survives exactly") @(t : T?) {
        t |> preproc_hash("cb", 800, 480, 816, 480, 0x1920852466156325ul)
        t |> preproc_hash("gray", 480, 800, 480, 816, 0x543b7496fda3c325ul)
    }
    t |> run("beta upscale and downscale") @(t : T?) {
        t |> preproc_hash("cb", 100, 100, 336, 336, 0xa2dfcfbc07a1190dul)
        t |> preproc_hash("white", 3000, 2000, 960, 624, 0xf805eca281c1ef25ul)
    }
    t |> run("half-away rounding survives to the output") @(t : T?) {
        t |> preproc_hash("gray", 120, 960, 144, 960, 0x4b7eec756723c325ul)
    }
}

def count_byte_diffs(a, b : array<uint8>) : int {
    if (length(a) != length(b)) {
        return max(length(a), length(b))
    }
    var n = 0
    for (x, y in a, b) {
        if (x != y) {
            n++
        }
    }
    return n
}

// per-process temp dir for the file-form round-trip — writing fixtures into the worktree
// would leave untracked state, and a shared path races parallel dastest runs
var private _tmp_dir : string
var private _tmp_dir_inited = false

def private tmp_path(name : string) : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("dasllama_vision_io")
        if (!(r is value)) {
            panic("could not create temp dir for the vision io test: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    return path_join(_tmp_dir, name)
}

[finalize]
def cleanup_temp_dir {
    if (_tmp_dir_inited && !empty(_tmp_dir)) {
        rmdir_rec(_tmp_dir)
    }
}

[test]
def test_vision_io_roundtrip(t : T?) {
    t |> run("PNG bytes decode back to the exact RGB the writer saw") @(t : T?) {
        let src <- make_fixture("cb", 21, 13)   // odd non-square keeps stride and orientation honest
        var png : array<uint8>
        stbi_write_png_to_memory(21, 13, 3, unsafe(addr(src.rgb[0])), 21 * 3) $(buf : array<uint8> const#) {
            png |> reserve(length(buf))
            for (b in buf) {   // push_from refuses a temp-array source; the view dies with the block
                png |> push(b)
            }
        }
        t |> success(!empty(png), "png encoded")
        let back <- load_image_rgb(png)
        t |> equal(back.width, 21)
        t |> equal(back.height, 13)
        t |> equal(count_byte_diffs(back.rgb, src.rgb), 0, "lossless byte round-trip")
    }
    t |> run("dump_image_png writes a file load_image_rgb reads back") @(t : T?) {
        let src <- make_fixture("gray", 32, 20)
        let path = tmp_path("roundtrip.png")
        t |> success(dump_image_png(src, path), "png written")
        let back <- load_image_rgb(path)
        t |> equal(back.width, 32)
        t |> equal(back.height, 20)
        t |> equal(count_byte_diffs(back.rgb, src.rgb), 0, "lossless file round-trip")
    }
    t |> run("write_image_ppm round-trips header and bytes") @(t : T?) {
        let src <- make_fixture("cb", 9, 5)
        let path = tmp_path("probe.ppm")
        t |> success(write_image_ppm(src, path), "ppm written")
        var inscope bytes : array<uint8>
        fopen(path, "rb") $(f) {
            if (f != null) {
                bytes |> resize(int(fstat(f).size))
                fread(f, bytes)
            }
        }
        let head = "P6\n9 5\n255\n"
        t |> equal(length(bytes), length(head) + 9 * 5 * 3, "header plus one byte per channel")
        for (i in range(9 * 5 * 3)) {
            if (bytes[length(head) + i] != src.rgb[i]) {
                t |> failure("pixel byte {i} differs")
                break
            }
        }
        let empty_img = VisionImage()
        t |> success(!write_image_ppm(empty_img, tmp_path("empty.ppm")), "a zero-size image refuses to write")
    }
}

[test]
def test_vision_io_guards(t : T?) {
    t |> run("decode failure yields a zero-size image") @(t : T?) {
        var none : array<uint8>
        let nothing <- load_image_rgb(none)   // the EMPTY guard arm: no addr on a zero array
        t |> equal(nothing.width, 0)
        var garbage : array<uint8>
        garbage |> resize(16)   // zeroed - no image container starts like this
        let bad <- load_image_rgb(garbage)
        t |> equal(bad.width, 0)
        t |> equal(bad.height, 0)
        let missing <- load_image_rgb(tmp_path("no_such_file.png"))
        t |> equal(missing.width, 0)
        t |> equal(missing.height, 0)
    }
    t |> run("probe_image_size reads the header alone") @(t : T?) {
        // 54 header bytes declaring 4096x4096: the probe answers from the header, and a capped
        // load refuses at the same header — before the 50 MB pixel allocation would happen
        let stub <- bmp_header_stub(4096, 4096)
        let dims = probe_image_size(stub)
        t |> equal(dims.x, 4096)
        t |> equal(dims.y, 4096)
        t |> equal(load_image_rgb(stub, 100l).width, 0, "a capped load refuses it at the header")
        var garbage : array<uint8>
        garbage |> resize(16)
        t |> equal(probe_image_size(garbage).x, 0)
        var none : array<uint8>
        t |> equal(probe_image_size(none).x, 0)
    }
    t |> run("max_pixels refuses an oversized image from the header alone") @(t : T?) {
        let src <- make_fixture("gray", 32, 20)
        let path = tmp_path("cap.png")
        t |> success(dump_image_png(src, path), "png written")
        let refused <- load_image_rgb(path, 100l)   // 640 px > 100: the info probe declines
        t |> equal(refused.width, 0)
        let served <- load_image_rgb(path, 640l)
        t |> equal(served.width, 32)
        var png : array<uint8>
        stbi_write_png_to_memory(32, 20, 3, unsafe(addr(src.rgb[0])), 32 * 3) $(buf : array<uint8> const#) {
            png |> reserve(length(buf))
            for (b in buf) {
                png |> push(b)
            }
        }
        let refused2 <- load_image_rgb(png, 100l)
        t |> equal(refused2.width, 0)
        let served2 <- load_image_rgb(png, 640l)
        t |> equal(served2.width, 32)
    }
}

[test]
def test_vision_letterbox_probes(t : T?) {
    t |> run("portrait gray letterboxes on black - oracle probe values") @(t : T?) {
        let src <- make_fixture("gray", 480, 800)
        let boxed <- vision_letterbox(src, int2(480, 816))
        // corners are PAD (black), center is content — a per-side stretch would fail all four
        t |> equal(int(boxed.rgb[(0 * 480 + 0) * 3]), 0, "top-left pad")
        t |> equal(int(boxed.rgb[(0 * 480 + 479) * 3]), 0, "top-right pad")
        t |> equal(int(boxed.rgb[(815 * 480 + 0) * 3]), 0, "bottom-left pad")
        t |> equal(int(boxed.rgb[(815 * 480 + 479) * 3]), 0, "bottom-right pad")
        t |> equal(int(boxed.rgb[(408 * 480 + 240) * 3]), 128, "center content")
    }
    t |> run("identity-size letterbox is a pixel-exact copy") @(t : T?) {
        let src <- make_fixture("cb", 96, 48)
        let same <- vision_letterbox(src, int2(96, 48))
        t |> equal(same.width, 96)
        t |> equal(same.height, 48)
        t |> equal(count_byte_diffs(same.rgb, src.rgb), 0, "bytes identical")
    }
    t |> run("upscaled checkerboard keeps corner-aligned orientation") @(t : T?) {
        let src <- make_fixture("cb", 100, 100)
        let boxed <- vision_letterbox(src, int2(336, 336))
        // corner-aligned bilinear maps dst corners to src corners: cb(0,0)=white, cb(99,0)=black
        t |> equal(int(boxed.rgb[0]), 255, "top-left = cb(0,0) white")
        t |> equal(int(boxed.rgb[(0 * 336 + 335) * 3]), 0, "top-right = cb(99,0) black")
        t |> equal(int(boxed.rgb[(335 * 336 + 0) * 3]), 0, "bottom-left = cb(0,99) black")
        t |> equal(int(boxed.rgb[(335 * 336 + 335) * 3]), 255, "bottom-right = cb(99,99) white")
        // oracle probe: interpolated center is 0.541176498 = 138/255
        t |> equal(int(boxed.rgb[(168 * 336 + 168) * 3]), 138, "center interpolated")
    }
}
