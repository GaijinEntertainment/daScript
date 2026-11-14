options gen2
options _dasllama_internal = true

module _image_memory shared public

require dasllama/dasllama_image   // IMAGE_PAGE: the boundary a prepared image starts on
require daslib/fio

//! The file at `path` copied into `raw`, `skew` bytes past an `IMAGE_PAGE` boundary - a host's own copy of a prepared image
//! (skew 0), or of a file that keeps no alignment. Answers the copy's base and writes its `size`; null where the file does not
//! open. `raw` owns the bytes: it outlives every model that borrows them.
def read_page_aligned(path : string; var raw : array<uint8>; var size : uint64&; skew : int64 = 0l) : void? {
    size = stat(path).size
    raw |> reserve(int64(size) + IMAGE_PAGE + skew)
    raw |> resize(int64(size) + IMAGE_PAGE + skew)
    var base : void?
    unsafe {
        let first = reinterpret<uint64>(addr(raw[0]))
        let page = uint64(IMAGE_PAGE)
        base = reinterpret<void?>((first + page - 1ul) / page * page + uint64(skew))
    }
    let f = fopen(path, "rb")
    return null if (f == null)
    let bytes = size
    fmap(f) $(var mapped : array<uint8>#) {
        unsafe(memcpy(base, addr(mapped[0]), bytes))
    }
    fclose(f)
    return base
}

//! The middle three quarters of the image copy at `base` zeroed - a control's poison. An image keeps its header first and
//! its meta last, so the load still parses, and what it serves reads from zeroed planes.
def zero_planes(base : void?; size : uint64) {
    zero_bytes(base, size / 8ul, size / 8ul * 6ul)
}

//! `count` bytes of the copy at `base` zeroed from byte `from` on.
def zero_bytes(base : void?; from, count : uint64) {
    unsafe {
        memset8(reinterpret<void?>(intptr(base) + from), 0u8, int(count))
    }
}
