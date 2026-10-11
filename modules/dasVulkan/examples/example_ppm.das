options gen2
options _comment_hygiene = true
options indenting = 4

// The PPM writer the offscreen examples share: a binary P6 of an RGBA8 readback, alpha dropped.

module example_ppm shared public

require daslib/fio

def public write_ppm(path : string; pixels : array<uint8>; w, h : int) {
    fopen(path, "wb") $(f) {
        fprint(f, "P6\n{w} {h}\n255\n")
        var rgb : array<uint8>
        rgb |> resize(w * h * 3)
        for (i in range(w * h)) {
            rgb[i * 3] = pixels[i * 4]
            rgb[i * 3 + 1] = pixels[i * 4 + 1]
            rgb[i * 3 + 2] = pixels[i * 4 + 2]
        }
        fwrite(f, rgb)
        delete rgb
    }
}
