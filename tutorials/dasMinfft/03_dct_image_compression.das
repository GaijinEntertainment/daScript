options gen2

require minfft
require math

// Tutorial MINFFT-03: 2D DCT and JPEG block compression
//
// This tutorial covers:
//   - make_dct_plan_2d: a 2D plan for rows x cols blocks (8x8 here)
//   - the JPEG pipeline on a single 8x8 block: DCT -> quantize -> dequantize -> IDCT
//   - why quantization turns most coefficients into zeros (which an entropy
//     coder then stores cheaply)
//
// JPEG and MPEG intra frames split an image into 8x8 blocks and code each block
// exactly like this. For the full image-in / image-out demo with a PSNR sweep,
// see examples/minfft/dct_jpeg.das.
//
// Run: daslang.exe tutorials/dasMinfft/03_dct_image_compression.das

let BLOCK = 8

// Standard JPEG luminance quantization table (JPEG spec Annex K, Table K.1).
let JPEG_LUMA = [
    16f, 11f, 10f, 16f,  24f,  40f,  51f,  61f,
    12f, 12f, 14f, 19f,  26f,  58f,  60f,  55f,
    14f, 13f, 16f, 24f,  40f,  57f,  69f,  56f,
    14f, 17f, 22f, 29f,  51f,  87f,  80f,  62f,
    18f, 22f, 37f, 56f,  68f, 109f, 103f,  77f,
    24f, 35f, 55f, 64f,  81f, 104f, 113f,  92f,
    49f, 64f, 78f, 87f, 103f, 121f, 120f, 101f,
    72f, 92f, 95f, 98f, 112f, 100f, 103f,  99f ]

// Per-axis scale: minfft's unnormalized DCT-II -> the orthonormal DCT the JPEG
// table expects. s[0] = 1/(4*sqrt2), else 1/4. (idct(dct(x)) == 2N*x per axis,
// and 2N = 16 for N=8, so the inverse divides by 256 = 16*16.)
def ortho_scale() : float[BLOCK] {
    var s : float[BLOCK]
    s[0] = 1.0f / (4.0f * sqrt(2.0f))
    for (k in range(1, BLOCK)) {
        s[k] = 0.25f
    }
    return s
}

def print_block(title : string; b : array<float>) {
    print("  {title}\n")
    for (r in range(BLOCK)) {
        print("   ")
        for (c in range(BLOCK)) {
            print("{int(b[r * BLOCK + c])}\t")
        }
        print("\n")
    }
}

[export]
def main() {
    print("=== One 8x8 block through the JPEG pipeline ===\n")
    let s = ortho_scale()
    var plan = make_dct_plan_2d(BLOCK, BLOCK)

    // A gentle gradient block (pixel values 0..255), JPEG-level-shifted by -128.
    var pixels <- [for (i in range(BLOCK * BLOCK));
        float(40 + 3 * (i % BLOCK) + 12 * (i / BLOCK))]
    let blk <- [for (p in pixels); p - 128.0f]
    print_block("input pixels:", pixels)

    // Forward DCT, then convert to orthonormal coefficients.
    var coeff : array<float>
    dct(blk, coeff, plan)
    let ortho <- [for (i in range(BLOCK * BLOCK)); coeff[i] * s[i / BLOCK] * s[i % BLOCK]]
    print_block("DCT coefficients (orthonormal):", ortho)

    // Quantize at quality ~50 (scale = 1.0), then dequantize.
    var nonzero = 0
    var deq : array<float>
    deq |> resize(BLOCK * BLOCK)
    for (i in range(BLOCK * BLOCK)) {
        let level = round(ortho[i] / JPEG_LUMA[i])
        if (level != 0.0f) {
            nonzero++
        }
        // back to minfft scale for the inverse transform
        deq[i] = (level * JPEG_LUMA[i]) / (s[i / BLOCK] * s[i % BLOCK])
    }
    print("  -> {nonzero} of {BLOCK * BLOCK} coefficients survive quantization\n")

    // Inverse DCT, undo the 256 scale and the level shift.
    var back : array<float>
    idct(deq, back, plan)
    let inv = 1.0f / float((2 * BLOCK) * (2 * BLOCK))
    let recon <- [for (v in back); clamp(v * inv + 128.0f, 0.0f, 255.0f)]
    print_block("reconstructed pixels:", recon)

    var max_err = 0.0f
    for (i in range(BLOCK * BLOCK)) {
        max_err = max(max_err, abs(recon[i] - pixels[i]))
    }
    print("  max per-pixel error: {max_err}\n")
    print("\n  Most coefficients quantized to zero, yet the block reconstructs\n")
    print("  closely. That is lossy block-DCT coding — the basis of JPEG/MPEG.\n")

    unsafe { delete plan }
}
