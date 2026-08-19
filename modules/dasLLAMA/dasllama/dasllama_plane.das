options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_plane shared public

// The borrowed-plane vocabulary. A plane is a pointer into a prepared image plus its element
// count: the image owns the bytes and a weight carrier owns nothing but its backing. This
// module requires no engine file — the image rail binds planes, every carrier holds them.

require dasllama/dasllama_lint public

//! A borrowed float plane: `p` points into a prepared image, `n` counts elements.
//! Read it with `pl[i]` (bounds-checked) or hand `pl.p` to a pointer kernel.
struct PlaneF {
    borrowed_plane : bool = true   // the marker the image walks static_if on — see dasllama_image
    p : float const? = null
    n : int64 = 0l
}

//! A borrowed int8 plane — the Q8 quant planes. Same contract as PlaneF.
struct PlaneI8 {
    borrowed_plane : bool = true
    p : int8 const? = null
    n : int64 = 0l
}

//! A borrowed halfword plane — native BF16 weights (the gemma4uv GEMMs). Same contract as
//! PlaneF; the elements are raw bf16 bit patterns, widened by the kernel that reads them.
struct PlaneU16 {
    borrowed_plane : bool = true
    p : uint16 const? = null
    n : int64 = 0l
}

//! A borrowed int plane — small baked tables (parakeet's TDT durations).
struct PlaneI32 {
    borrowed_plane : bool = true
    p : int const? = null
    n : int64 = 0l
}

//! A borrowed int64 plane — baked offset lists (parakeet's per-layer LSTM offsets).
struct PlaneI64 {
    borrowed_plane : bool = true
    p : int64 const? = null
    n : int64 = 0l
}

// One unsigned compare catches negative and overrun both. Measured free under -jit against an
// unchecked pointer read, and faster than the array<T> indexing it replaces.
def private plane_guard(i, n : int64) {
    if (uint64(i) >= uint64(n)) {
        panic("dasLLAMA plane: index {i} outside [0,{n})")
    }
}

def operator [] (pl : PlaneF; i : int64) : float {
    plane_guard(i, pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneF; i : int) : float {
    plane_guard(int64(i), pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneI8; i : int64) : int8 {
    plane_guard(i, pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneI8; i : int) : int8 {
    plane_guard(int64(i), pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneU16; i : int64) : uint16 {
    plane_guard(i, pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneU16; i : int) : uint16 {
    plane_guard(int64(i), pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneI32; i : int64) : int {
    plane_guard(i, pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneI32; i : int) : int {
    plane_guard(int64(i), pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneI64; i : int64) : int64 {
    plane_guard(i, pl.n)
    return unsafe(pl.p[i])
}

def operator [] (pl : PlaneI64; i : int) : int64 {
    plane_guard(int64(i), pl.n)
    return unsafe(pl.p[i])
}

def length(pl : PlaneF) : int64 => pl.n
def length(pl : PlaneI8) : int64 => pl.n
def length(pl : PlaneU16) : int64 => pl.n
def length(pl : PlaneI32) : int64 => pl.n
def length(pl : PlaneI64) : int64 => pl.n
def empty(pl : PlaneF) : bool => pl.n == 0l
def empty(pl : PlaneI8) : bool => pl.n == 0l
def empty(pl : PlaneU16) : bool => pl.n == 0l
def empty(pl : PlaneI32) : bool => pl.n == 0l
def empty(pl : PlaneI64) : bool => pl.n == 0l

//! The hot-path form: the plane's base advanced by `off` elements, checked once here so the
//! loop that follows can index the raw pointer. `off == n` is legal and yields the end pointer.
def plane_at(pl : PlaneF; off : int64) : float const? {
    if (uint64(off) > uint64(pl.n)) {
        panic("dasLLAMA plane: offset {off} outside [0,{pl.n}]")
    }
    return unsafe(pl.p + off)
}

def plane_at(pl : PlaneI8; off : int64) : int8 const? {
    if (uint64(off) > uint64(pl.n)) {
        panic("dasLLAMA plane: offset {off} outside [0,{pl.n}]")
    }
    return unsafe(pl.p + off)
}

def plane_at(pl : PlaneU16; off : int64) : uint16 const? {
    if (uint64(off) > uint64(pl.n)) {
        panic("dasLLAMA plane: offset {off} outside [0,{pl.n}]")
    }
    return unsafe(pl.p + off)
}

//! Point a plane at `nbytes` of a mapped image starting at `base + off`. false = the section
//! does not divide into whole elements, which is a malformed image and never a recoverable one.
def plane_bind(var pl : PlaneF; base : uint8 const?; off, nbytes : int64) : bool {
    if (nbytes % 4l != 0l) {
        return false
    }
    unsafe {
        pl.p = reinterpret<float const?>(base + off)
    }
    pl.n = nbytes / 4l
    return true
}

def plane_bind(var pl : PlaneI8; base : uint8 const?; off, nbytes : int64) : bool {
    unsafe {
        pl.p = reinterpret<int8 const?>(base + off)
    }
    pl.n = nbytes
    return true
}

def plane_bind(var pl : PlaneU16; base : uint8 const?; off, nbytes : int64) : bool {
    if (nbytes % 2l != 0l) {
        return false
    }
    unsafe {
        pl.p = reinterpret<uint16 const?>(base + off)
    }
    pl.n = nbytes / 2l
    return true
}

def plane_bind(var pl : PlaneI32; base : uint8 const?; off, nbytes : int64) : bool {
    if (nbytes % 4l != 0l) {
        return false
    }
    unsafe {
        pl.p = reinterpret<int const?>(base + off)
    }
    pl.n = nbytes / 4l
    return true
}

def plane_bind(var pl : PlaneI64; base : uint8 const?; off, nbytes : int64) : bool {
    if (nbytes % 8l != 0l) {
        return false
    }
    unsafe {
        pl.p = reinterpret<int64 const?>(base + off)
    }
    pl.n = nbytes / 8l
    return true
}

//! Drop a plane's view. A plane never owns bytes, so this frees nothing — it exists so a
//! carrier's teardown reads as an intent rather than as two field writes.
def plane_release(var pl : PlaneF) {
    pl.p = null
    pl.n = 0l
}

def plane_release(var pl : PlaneI8) {
    pl.p = null
    pl.n = 0l
}

def plane_release(var pl : PlaneU16) {
    pl.p = null
    pl.n = 0l
}

def plane_release(var pl : PlaneI32) {
    pl.p = null
    pl.n = 0l
}

def plane_release(var pl : PlaneI64) {
    pl.p = null
    pl.n = 0l
}

// Declaring the teardown says what it is — dropping a view — and nulls the borrowed pointer, so
// a torn-down carrier cannot dangle. (It does NOT make containing structs safe to delete: a
// struct holding any plane still takes an unsafe delete, finalize or no finalize.)
def finalize(var pl : PlaneF) {
    plane_release(pl)
}

def finalize(var pl : PlaneI8) {
    plane_release(pl)
}

def finalize(var pl : PlaneU16) {
    plane_release(pl)
}

def finalize(var pl : PlaneI32) {
    plane_release(pl)
}

def finalize(var pl : PlaneI64) {
    plane_release(pl)
}
