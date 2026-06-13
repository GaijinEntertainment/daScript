// Tutorial STBIMAGE-07: HDR Images (float pixels, Radiance .hdr round-trip)
//
// This tutorial covers:
//   - Creating a float-pixel (bpc = 4) image for high dynamic range
//   - Writing pixel values above 1.0 via the float with_pixels overload
//   - Saving to Radiance .hdr and reloading with load_hdr
//   - Detecting HDR files (is_hdr) and verifying the round-trip
//
// Run: daslang.exe tutorials/dasStbImage/07_hdr.das

options gen2
options indenting = 4

require daslib/fio
require stbimage/stbimage_boost
require math

[export]
def main() {
    let W = 32
    let H = 32
    let path = "_tutorial_out.hdr"

    // ──────────────────────────────────────────────────────────────────────
    // Section 1 — Create a float HDR image
    // ──────────────────────────────────────────────────────────────────────
    //
    // HDR images store float pixels (bpc = 4), so values are not clamped to
    // [0, 1]: a channel can be brighter than full white. make_image(w, h, ch, 4)
    // allocates a float buffer; the float overload of with_pixels exposes it.

    print("=== Section 1: Create a float HDR image ===\n")
    var img = make_image(W, H, 3, 4)
    img |> with_pixels() $(var px : array<float>#) {
        for (i in range(W * H)) {
            px[i * 3 + 0] = 2.5            // red brighter than full white
            px[i * 3 + 1] = 1.0
            px[i * 3 + 2] = 0.5
        }
        px[0] = 10.0                       // one very bright pixel
        px[1] = 0.25
        px[2] = 0.0
    }
    print("  created {W}x{H} float image, bpc={img.bpc}, channels={img.channels}\n")

    // ──────────────────────────────────────────────────────────────────────
    // Section 2 — Save as .hdr and reload
    // ──────────────────────────────────────────────────────────────────────
    //
    // save() picks the encoder from the extension; .hdr writes Radiance RGBE.
    // load_hdr() reads it back as float and sets bpc = 4.

    print("\n=== Section 2: Save + reload ===\n")
    let (ok, err) = img.save(path)
    if (!ok) {
        print("  save error: {err}\n")
    }
    assert(ok, "save .hdr failed")
    let detected_hdr = is_hdr(path)
    assert(detected_hdr, "saved file not detected as HDR")
    var inscope reloaded : Image
    let (ok2, err2) = reloaded.load_hdr(path)
    if (!ok2) {
        print("  load error: {err2}\n")
    }
    assert(ok2, "load_hdr failed")
    assert(reloaded.bpc == 4 && reloaded.channels == 3, "reloaded image is not float RGB")
    assert(reloaded.width == W && reloaded.height == H, "round-trip changed dimensions")
    print("  reloaded {reloaded.width}x{reloaded.height}, bpc={reloaded.bpc}, channels={reloaded.channels}\n")

    // ──────────────────────────────────────────────────────────────────────
    // Section 3 — Verify the round-trip
    // ──────────────────────────────────────────────────────────────────────
    //
    // Radiance .hdr is RGBE: an 8-bit mantissa per channel plus one shared
    // exponent, so it is slightly lossy — precision is about max_channel / 256.
    // The reconstructed pixels match within that tolerance, and HDR magnitudes
    // (values > 1.0) survive.

    print("\n=== Section 3: Round-trip accuracy ===\n")
    var maxerr = 0.0
    img |> with_pixels() $(var a : array<float>#) {
        reloaded |> with_pixels() $(var b : array<float>#) {
            for (i in range(length(a))) {
                let e = abs(a[i] - b[i])
                if (e > maxerr) {
                    maxerr = e
                }
            }
        }
    }
    print("  max per-channel round-trip error: {maxerr}\n")
    assert(maxerr < 0.05, "round-trip error larger than RGBE tolerance")

    // confirm the bright pixel kept its high dynamic range
    var bright_r = 0.0
    reloaded |> with_pixels() $(var b : array<float>#) {
        bright_r = b[0]
    }
    print("  bright pixel red channel after round-trip: {bright_r}\n")
    assert(bright_r > 1.0, "HDR magnitude was clamped")

    remove(path)
    print("\nDone.\n")
}
