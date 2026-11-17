options gen2
options _dasllama_internal = true

module _image_memory shared public

require dasllama/dasllama_image   // IMAGE_PAGE: the boundary a prepared image starts on

//! The file at `path` copied into `raw`, `skew` bytes past an `IMAGE_PAGE` boundary - a host's own copy of a prepared image
//! (skew 0), or of a file that keeps no alignment. Answers the copy's base and writes its `size`; null where the file does not
//! open. `raw` owns the bytes: it outlives every model that borrows them.
def read_page_aligned(path : string; var raw : array<uint8>; var size : uint64&; skew : int64 = 0l) : void? {
    let read = read_prepared_image_(path, raw, skew)
    size = read.size
    return read.base
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
