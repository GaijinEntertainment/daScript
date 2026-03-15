options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module stbimage_boost shared public

//! High-level image I/O and format conversion API wrapping stb_image.
//!
//! Provides ``Image`` struct with methods for loading, saving, resizing,
//! flipping, and channel conversion. Pixel data is GC-managed (``array<uint8>``).
//! All unsafe pointer operations are encapsulated internally.

require stbimage public
require daslib/safe_addr
require daslib/class_boost
require fio
require strings
require math

// pixel-sized structs for typed dispatch (no native 3-byte or 6-byte types)
struct private rgb8 {
    r : uint8
    g : uint8
    b : uint8
}

struct private rgb16 {
    r : uint16
    g : uint16
    b : uint16
}

// generic per-pixel copy helpers — instantiated per pixel type via reinterpret dispatch

def private copy_reversed(var dst; src; w : int) {
    unsafe {
        for (x in range(w)) {
            dst[w - 1 - x] = src[x]
        }
    }
}

def private copy_rect(var dst; src; dst_w, src_w, sx0, sy0, dx0, dy0, w, h : int) {
    unsafe {
        for (row in range(h)) {
            for (col in range(w)) {
                dst[(dy0 + row) * dst_w + dx0 + col] = src[(sy0 + row) * src_w + sx0 + col]
            }
        }
    }
}

def private get_pixel_layout(ch : int) : stbir_pixel_layout {
    if (ch == 4) {
        return stbir_pixel_layout.STBIR_RGBA
    } elif (ch == 3) {
        return stbir_pixel_layout.STBIR_RGB
    } elif (ch == 2) {
        return stbir_pixel_layout.STBIR_2CHANNEL
    }
    return stbir_pixel_layout.STBIR_1CHANNEL
}

def private adopt_pixels(var self : Image; pixels : void?; w, h, ch, bytes_per_component : int) {
    self.bpc = bytes_per_component
    self.width = w
    self.height = h
    self.channels = ch
    let sz = w * h * ch * bytes_per_component
    self.bytes |> resize(sz)
    unsafe {
        memcpy(addr(self.bytes[0]), pixels, sz)
    }
    stbi_image_free(pixels)
}

def private load_from_memory_impl(var self : Image; data : void?; data_length, requested_channels : int) : tuple<bool; string> {
    var w, h, comp : int
    var pixels = stbi_load_from_memory(unsafe(reinterpret<uint8 const?>(data)), data_length, safe_addr(w), safe_addr(h), safe_addr(comp), requested_channels)
    if (pixels == null) {
        return (false, stbi_failure_reason())
    }
    let ch = requested_channels != 0 ? requested_channels : comp
    adopt_pixels(self, pixels, w, h, ch, 1)
    return (true, "")
}

def private load_hdr_from_memory_impl(var self : Image; data : void?; data_length, requested_channels : int) : tuple<bool; string> {
    var w, h, comp : int
    var pixels = stbi_loadf_from_memory(unsafe(reinterpret<uint8 const?>(data)), data_length, safe_addr(w), safe_addr(h), safe_addr(comp), requested_channels)
    if (pixels == null) {
        return (false, stbi_failure_reason())
    }
    let ch = requested_channels != 0 ? requested_channels : comp
    adopt_pixels(self, pixels, w, h, ch, 4)
    return (true, "")
}

def private load_16_from_memory_impl(var self : Image; data : void?; data_length, requested_channels : int) : tuple<bool; string> {
    var w, h, comp : int
    var pixels = stbi_load_16_from_memory(unsafe(reinterpret<uint8 const?>(data)), data_length, safe_addr(w), safe_addr(h), safe_addr(comp), requested_channels)
    if (pixels == null) {
        return (false, stbi_failure_reason())
    }
    let ch = requested_channels != 0 ? requested_channels : comp
    adopt_pixels(self, pixels, w, h, ch, 2)
    return (true, "")
}

struct public Image {
    //! Image container with raw pixel data in original format.
    //! ``bpc`` (bytes per component): 1 = uint8, 2 = uint16, 4 = float.
    //! ``channels``: 1 = grey, 2 = grey+alpha, 3 = RGB, 4 = RGBA.
    bytes    : array<uint8>
    width    : int
    height   : int
    channels : int
    bpc      : int

    // ---- queries ----

    [class_method]
    def static const valid() : bool {
        //! Returns true if the image has been loaded successfully.
        return width > 0 && height > 0 && length(bytes) > 0
    }

    [class_method]
    def static const stride() : int {
        //! Returns the number of bytes per row.
        return width * channels * bpc
    }

    [class_method]
    def static const has_alpha() : bool {
        //! Returns true if the image has an alpha channel.
        return channels == 2 || channels == 4
    }

    [class_method]
    def static const pixel_size() : int {
        //! Returns the number of bytes per pixel.
        return channels * bpc
    }

    [class_method]
    def static const is_hdr() : bool {
        //! Returns true if the image stores float (HDR) data.
        return bpc == 4
    }

    [class_method]
    def static const data_size() : int {
        //! Returns the total size of pixel data in bytes.
        return width * height * channels * bpc
    }

    // ---- load from file ----

    [class_method]
    def static load(path : string; requested_channels : int = 0) : tuple<bool; string> {
        //! Load an 8-bit image from file. Sets ``bpc = 1``.
        var w, h, comp : int
        var pixels = stbi_load(path, safe_addr(w), safe_addr(h), safe_addr(comp), requested_channels)
        if (pixels == null) {
            return (false, stbi_failure_reason())
        }
        let ch = requested_channels != 0 ? requested_channels : comp
        adopt_pixels(self, pixels, w, h, ch, 1)
        return (true, "")
    }

    [class_method]
    def static load_hdr(path : string; requested_channels : int = 0) : tuple<bool; string> {
        //! Load an image as float (HDR) data from file. Sets ``bpc = 4``.
        var w, h, comp : int
        var pixels = stbi_loadf(path, safe_addr(w), safe_addr(h), safe_addr(comp), requested_channels)
        if (pixels == null) {
            return (false, stbi_failure_reason())
        }
        let ch = requested_channels != 0 ? requested_channels : comp
        adopt_pixels(self, pixels, w, h, ch, 4)
        return (true, "")
    }

    [class_method]
    def static load_16(path : string; requested_channels : int = 0) : tuple<bool; string> {
        //! Load an image as 16-bit data from file. Sets ``bpc = 2``.
        var w, h, comp : int
        var pixels = stbi_load_16(path, safe_addr(w), safe_addr(h), safe_addr(comp), requested_channels)
        if (pixels == null) {
            return (false, stbi_failure_reason())
        }
        let ch = requested_channels != 0 ? requested_channels : comp
        adopt_pixels(self, pixels, w, h, ch, 2)
        return (true, "")
    }

    // ---- load from memory ----

    [class_method]
    def static load_from_memory(data : array<uint8> const; requested_channels : int = 0) : tuple<bool; string> {
        //! Load an 8-bit image from memory buffer. Sets ``bpc = 1``.
        return load_from_memory_impl(self, unsafe(addr(data[0])), length(data), requested_channels)
    }

    [class_method]
    def static load_from_memory(data : array<uint8> # const; requested_channels : int = 0) : tuple<bool; string> {
        //! Load an 8-bit image from a temporary memory buffer. Sets ``bpc = 1``.
        return load_from_memory_impl(self, unsafe(reinterpret<void?>(addr(data[0]))), length(data), requested_channels)
    }

    [class_method]
    def static load_hdr_from_memory(data : array<uint8> const; requested_channels : int = 0) : tuple<bool; string> {
        //! Load a float (HDR) image from memory buffer. Sets ``bpc = 4``.
        return load_hdr_from_memory_impl(self, unsafe(addr(data[0])), length(data), requested_channels)
    }

    [class_method]
    def static load_hdr_from_memory(data : array<uint8> # const; requested_channels : int = 0) : tuple<bool; string> {
        //! Load a float (HDR) image from a temporary memory buffer. Sets ``bpc = 4``.
        return load_hdr_from_memory_impl(self, unsafe(reinterpret<void?>(addr(data[0]))), length(data), requested_channels)
    }

    [class_method]
    def static load_16_from_memory(data : array<uint8> const; requested_channels : int = 0) : tuple<bool; string> {
        //! Load a 16-bit image from memory buffer. Sets ``bpc = 2``.
        return load_16_from_memory_impl(self, unsafe(addr(data[0])), length(data), requested_channels)
    }

    [class_method]
    def static load_16_from_memory(data : array<uint8> # const; requested_channels : int = 0) : tuple<bool; string> {
        //! Load a 16-bit image from a temporary memory buffer. Sets ``bpc = 2``.
        return load_16_from_memory_impl(self, unsafe(reinterpret<void?>(addr(data[0]))), length(data), requested_channels)
    }

    // ---- save to file ----

    [class_method]
    def static const save(path : string; quality : int = 0) : tuple<bool; string> {
        //! Save image to file. Format detected from extension (.png, .bmp, .tga, .jpg/.jpeg, .hdr).
        //! ``quality`` only affects JPEG (0 = default 90).
        if (!self |> valid()) {
            return (false, "image is not valid")
        }
        let ext = to_lower(extension(path))
        var ok = 0
        if (ext == ".png") {
            ok = stbi_write_png(path, width, height, channels, unsafe(addr(bytes[0])), self |> stride())
        } elif (ext == ".bmp") {
            ok = stbi_write_bmp(path, width, height, channels, unsafe(addr(bytes[0])))
        } elif (ext == ".tga") {
            ok = stbi_write_tga(path, width, height, channels, unsafe(addr(bytes[0])))
        } elif (ext == ".jpg" || ext == ".jpeg") {
            let q = quality != 0 ? quality : 90
            ok = stbi_write_jpg(path, width, height, channels, unsafe(addr(bytes[0])), q)
        } elif (ext == ".hdr") {
            let fp = unsafe(reinterpret<float?>(unsafe(addr(bytes[0]))))
            ok = stbi_write_hdr(path, width, height, channels, fp)
        } else {
            return (false, "unsupported format: {ext}")
        }
        if (ok == 0) {
            return (false, "write failed")
        }
        return (true, "")
    }

    // ---- encode to memory ----

    [class_method]
    def static const encode(format : string; var out_data : array<uint8>; quality : int = 0) : tuple<bool; string> {
        //! Encode image to memory in the specified format ("png", "bmp", "tga", "jpg").
        if (!self |> valid()) {
            return (false, "image is not valid")
        }
        let fmt = to_lower(format)
        var encoded = false
        if (fmt == "png") {
            stbi_write_png_to_memory(width, height, channels, unsafe(addr(bytes[0])), self |> stride()) <| $(data : array<uint8>#) {
                out_data := data
                encoded = true
            }
        } elif (fmt == "bmp") {
            stbi_write_bmp_to_memory(width, height, channels, unsafe(addr(bytes[0]))) <| $(data : array<uint8>#) {
                out_data := data
                encoded = true
            }
        } elif (fmt == "tga") {
            stbi_write_tga_to_memory(width, height, channels, unsafe(addr(bytes[0]))) <| $(data : array<uint8>#) {
                out_data := data
                encoded = true
            }
        } elif (fmt == "jpg" || fmt == "jpeg") {
            let q = quality != 0 ? quality : 90
            stbi_write_jpg_to_memory(width, height, channels, unsafe(addr(bytes[0])), q) <| $(data : array<uint8>#) {
                out_data := data
                encoded = true
            }
        } else {
            return (false, "unsupported format: {fmt}")
        }
        if (!encoded) {
            return (false, "encode failed")
        }
        return (true, "")
    }

    // ---- transforms ----

    [class_method]
    def static const resize(new_w, new_h : int) : Image {
        //! Resize image using default filter. Returns a new Image.
        return self |> resize(new_w, new_h, stbir_filter.STBIR_FILTER_DEFAULT)
    }

    [class_method]
    def static const resize(new_w, new_h : int; filter : stbir_filter) : Image {
        //! Resize image using the specified filter. Returns a new Image.
        var result = Image(width = new_w, height = new_h, channels = channels, bpc = bpc)
        result.bytes |> resize(new_w * new_h * channels * bpc)
        let layout = get_pixel_layout(channels)
        unsafe {
            let src = addr(bytes[0])
            let dst = addr(result.bytes[0])
            if (bpc == 1) {
                stbir_resize(src, width, height, 0, dst, new_w, new_h, 0, layout, stbir_datatype.STBIR_TYPE_UINT8_SRGB, stbir_edge.STBIR_EDGE_CLAMP, filter)
            } elif (bpc == 4) {
                stbir_resize(src, width, height, 0, dst, new_w, new_h, 0, layout, stbir_datatype.STBIR_TYPE_FLOAT, stbir_edge.STBIR_EDGE_CLAMP, filter)
            } elif (bpc == 2) {
                stbir_resize(src, width, height, 0, dst, new_w, new_h, 0, layout, stbir_datatype.STBIR_TYPE_UINT16, stbir_edge.STBIR_EDGE_CLAMP, filter)
            }
        }
        return <- result
    }

    [class_method]
    def static const flip_vertical() : Image {
        //! Flip image vertically. Returns a new Image.
        var result = Image(width = width, height = height, channels = channels, bpc = bpc)
        let row_bytes = width * channels * bpc
        result.bytes |> resize(height * row_bytes)
        unsafe {
            for (y in range(height)) {
                let src_row = (height - 1 - y) * row_bytes
                let dst_row = y * row_bytes
                memcpy(addr(result.bytes[dst_row]), addr(bytes[src_row]), row_bytes)
            }
        }
        return <- result
    }

    [class_method]
    def static const flip_horizontal() : Image {
        //! Flip image horizontally (mirror). Returns a new Image.
        var result = Image(width = width, height = height, channels = channels, bpc = bpc)
        result.bytes |> resize(width * height * channels * bpc)
        let w = int(width)
        let h = int(height)
        unsafe {
            let px = channels * bpc
            if (px == 1) {
                var dp = reinterpret<uint8?>(addr(result.bytes[0]))
                var sp = reinterpret<uint8?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            } elif (px == 2) {
                var dp = reinterpret<uint16?>(addr(result.bytes[0]))
                var sp = reinterpret<uint16?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            } elif (px == 3) {
                var dp = reinterpret<rgb8?>(addr(result.bytes[0]))
                var sp = reinterpret<rgb8?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            } elif (px == 4) {
                var dp = reinterpret<uint?>(addr(result.bytes[0]))
                var sp = reinterpret<uint?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            } elif (px == 6) {
                var dp = reinterpret<rgb16?>(addr(result.bytes[0]))
                var sp = reinterpret<rgb16?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            } elif (px == 8) {
                var dp = reinterpret<uint64?>(addr(result.bytes[0]))
                var sp = reinterpret<uint64?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            } elif (px == 12) {
                var dp = reinterpret<float3?>(addr(result.bytes[0]))
                var sp = reinterpret<float3?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            } elif (px == 16) {
                var dp = reinterpret<float4?>(addr(result.bytes[0]))
                var sp = reinterpret<float4?>(addr(bytes[0]))
                for (row in range(h)) {
                    copy_reversed(dp + row * w, sp + row * w, w)
                }
            }
        }
        return <- result
    }

    [class_method]
    def static const crop(cx, cy, cw, ch : int) : Image {
        //! Extract a rectangular sub-region. Returns a new Image.
        //! Coordinates are clamped to image bounds.
        let x0 = clamp(cx, 0, int(width))
        let y0 = clamp(cy, 0, int(height))
        let x1 = clamp(cx + cw, 0, int(width))
        let y1 = clamp(cy + ch, 0, int(height))
        let rw = x1 - x0
        let rh = y1 - y0
        if (rw <= 0 || rh <= 0) {
            return <- Image()
        }
        var result = Image(width = int(rw), height = int(rh), channels = channels, bpc = bpc)
        let px_bytes = channels * bpc
        let src_stride = width * px_bytes
        let dst_stride = rw * px_bytes
        result.bytes |> resize(rh * dst_stride)
        unsafe {
            for (row in range(rh)) {
                let src_off = (y0 + row) * src_stride + x0 * px_bytes
                let dst_off = row * dst_stride
                memcpy(addr(result.bytes[int(dst_off)]), addr(bytes[int(src_off)]), dst_stride)
            }
        }
        return <- result
    }

    [class_method]
    def static blit(src : Image; dst_x, dst_y : int) {
        //! Copy ``src`` image onto this image at (dst_x, dst_y).
        //! Both images must have the same ``channels`` and ``bpc``.
        //! Source is clipped to destination bounds.
        assert(src.channels == channels && src.bpc == bpc, "blit requires matching channels and bpc")
        let x0 = max(dst_x, 0)
        let y0 = max(dst_y, 0)
        let x1 = min(dst_x + src.width, int(width))
        let y1 = min(dst_y + src.height, int(height))
        if (x0 >= x1 || y0 >= y1) {
            return
        }
        let cw = x1 - x0
        let rh = y1 - y0
        let sx0 = x0 - dst_x
        let sy0 = y0 - dst_y
        let dw = int(width)
        let sw = int(src.width)
        unsafe {
            let px = channels * bpc
            if (px == 1) {
                copy_rect(reinterpret<uint8?>(addr(bytes[0])), reinterpret<uint8?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            } elif (px == 2) {
                copy_rect(reinterpret<uint16?>(addr(bytes[0])), reinterpret<uint16?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            } elif (px == 3) {
                copy_rect(reinterpret<rgb8?>(addr(bytes[0])), reinterpret<rgb8?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            } elif (px == 4) {
                copy_rect(reinterpret<uint?>(addr(bytes[0])), reinterpret<uint?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            } elif (px == 6) {
                copy_rect(reinterpret<rgb16?>(addr(bytes[0])), reinterpret<rgb16?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            } elif (px == 8) {
                copy_rect(reinterpret<uint64?>(addr(bytes[0])), reinterpret<uint64?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            } elif (px == 12) {
                copy_rect(reinterpret<float3?>(addr(bytes[0])), reinterpret<float3?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            } elif (px == 16) {
                copy_rect(reinterpret<float4?>(addr(bytes[0])), reinterpret<float4?>(addr(src.bytes[0])), dw, sw, sx0, sy0, x0, y0, cw, rh)
            }
        }
    }

    [class_method]
    def static const to_channels(target_channels : int) : Image {
        //! Convert image to a different number of channels. Returns a new Image.
        //! Uses encode-to-PNG then decode with requested channels.
        if (target_channels == channels) {
            var result = Image(width = width, height = height, channels = channels, bpc = bpc, bytes <- clone(bytes))
            return <- result
        }
        var result : Image
        stbi_write_png_to_memory(width, height, channels, unsafe(addr(bytes[0])), width * channels * bpc) <| $(data : array<uint8>#) {
            result.load_from_memory(data, target_channels)
        }
        return <- result
    }
}

// ---- pixel access: with_pixels (overloaded by block type) ----

def public with_pixels(var self : Image; blk : block<(var pixels : array<uint8>#) : void>) {
    //! Access pixel data as ``array<uint8>``. Image must have ``bpc == 1``.
    assert(self.bpc == 1, "with_pixels<uint8> requires bpc=1")
    unsafe {
        __builtin_array_lock(self.bytes)
        var view : array<uint8>#
        _builtin_make_temp_array(view, addr(self.bytes[0]), self.width * self.height * self.channels)
        invoke(blk, view)
        __builtin_array_unlock(self.bytes)
    }
}

def public with_pixels(var self : Image; blk : block<(var pixels : array<float>#) : void>) {
    //! Access pixel data as ``array<float>``. Image must have ``bpc == 4``.
    assert(self.bpc == 4, "with_pixels<float> requires bpc=4")
    unsafe {
        __builtin_array_lock(self.bytes)
        var view : array<float>#
        _builtin_make_temp_array(view, reinterpret<float?>(addr(self.bytes[0])), self.width * self.height * self.channels)
        invoke(blk, view)
        __builtin_array_unlock(self.bytes)
    }
}

def public with_pixels(var self : Image; blk : block<(var pixels : array<uint16>#) : void>) {
    //! Access pixel data as ``array<uint16>``. Image must have ``bpc == 2``.
    assert(self.bpc == 2, "with_pixels<uint16> requires bpc=2")
    unsafe {
        __builtin_array_lock(self.bytes)
        var view : array<uint16>#
        _builtin_make_temp_array(view, reinterpret<uint16?>(addr(self.bytes[0])), self.width * self.height * self.channels)
        invoke(blk, view)
        __builtin_array_unlock(self.bytes)
    }
}

def public with_pixels_f(var self : Image; blk : block<(var pixels : array<float>#) : void>) {
    //! Explicit-name alias for ``with_pixels`` with float block. For use in generics.
    self |> with_pixels(blk)
}

def public with_pixels_16(var self : Image; blk : block<(var pixels : array<uint16>#) : void>) {
    //! Explicit-name alias for ``with_pixels`` with uint16 block. For use in generics.
    self |> with_pixels(blk)
}

// ---- pixel access: with_row ----

def public with_row(var self : Image; y : int; blk : block<(var row : array<uint8>#) : void>) {
    //! Access a single row as ``array<uint8>``. Image must have ``bpc == 1``.
    assert(self.bpc == 1, "with_row<uint8> requires bpc=1")
    assert(y >= 0 && y < self.height, "row out of bounds")
    let row_elems = self.width * self.channels
    unsafe {
        __builtin_array_lock(self.bytes)
        var view : array<uint8>#
        _builtin_make_temp_array(view, addr(self.bytes[y * row_elems]), row_elems)
        invoke(blk, view)
        __builtin_array_unlock(self.bytes)
    }
}

def public with_row(var self : Image; y : int; blk : block<(var row : array<float>#) : void>) {
    //! Access a single row as ``array<float>``. Image must have ``bpc == 4``.
    assert(self.bpc == 4, "with_row<float> requires bpc=4")
    assert(y >= 0 && y < self.height, "row out of bounds")
    let row_elems = self.width * self.channels
    unsafe {
        __builtin_array_lock(self.bytes)
        var view : array<float>#
        _builtin_make_temp_array(view, reinterpret<float?>(addr(self.bytes[y * row_elems * 4])), row_elems)
        invoke(blk, view)
        __builtin_array_unlock(self.bytes)
    }
}

def public with_row_f(var self : Image; y : int; blk : block<(var row : array<float>#) : void>) {
    //! Explicit-name alias for ``with_row`` with float block.
    self |> with_row(y, blk)
}

// ---- standalone functions ----

def public make_image(w, h, channels : int; bpc : int = 1) : Image {
    //! Create a blank (zeroed) image with the given dimensions.
    var img = Image(width = w, height = h, channels = channels, bpc = bpc)
    img.bytes |> resize(w * h * channels * bpc)
    return <- img
}

def public load_image(path : string) : Image {
    //! Load an image from file. Panics on failure.
    var img : Image
    let (ok, error) = img.load(path)
    if (!ok) {
        panic("load_image failed: {error}")
    }
    return <- img
}

def public load_image(path : string; var error : string&) : Image {
    //! Load an image from file. Sets ``error`` on failure.
    var img : Image
    let (ok, err) = img.load(path)
    if (!ok) {
        error = err
    }
    return <- img
}

def public image_info(path : string) : tuple<w : int; h : int; comp : int> {
    //! Get image dimensions and channel count without loading pixel data.
    var w, h, comp : int
    stbi_info(path, safe_addr(w), safe_addr(h), safe_addr(comp))
    return (w = w, h = h, comp = comp)
}

def public is_hdr(path : string) : bool {
    //! Returns true if the file is in HDR format.
    return stbi_is_hdr(path) != 0
}

def public is_16_bit(path : string) : bool {
    //! Returns true if the file contains 16-bit data.
    return stbi_is_16_bit(path) != 0
}

def public is_hdr_from_memory(data : array<uint8> const) : bool {
    //! Returns true if the memory buffer contains HDR format data.
    return stbi_is_hdr_from_memory(unsafe(addr(data[0])), length(data)) != 0
}

def public is_16_bit_from_memory(data : array<uint8> const) : bool {
    //! Returns true if the memory buffer contains 16-bit data.
    return stbi_is_16_bit_from_memory(unsafe(addr(data[0])), length(data)) != 0
}
