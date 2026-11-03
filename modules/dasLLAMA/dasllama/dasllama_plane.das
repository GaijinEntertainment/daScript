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

//! A borrowed byte plane — a K-quant quant or scale plane in the engine's own layout (the GigaAM kq lane). Same contract as PlaneF.
struct PlaneU8 {
    borrowed_plane : bool = true
    p : uint8 const? = null
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

//! Every borrowed plane type: one generic body each below serves them all.
typedef BorrowedPlane = PlaneF | PlaneI8 | PlaneU16 | PlaneU8 | PlaneI32 | PlaneI64

// One unsigned compare catches negative and overrun both. Measured free under -jit against an
// unchecked pointer read, and faster than the array<T> indexing it replaces.
def operator [] (pl : BorrowedPlane; i : int | int64) {
    if (uint64(i) >= uint64(pl.n)) {
        panic("dasLLAMA plane: index {i} outside [0,{pl.n})")
    }
    return unsafe(pl.p[i])
}

def length(pl : BorrowedPlane) : int64 => pl.n
def empty(pl : BorrowedPlane) : bool => pl.n == 0l

//! The hot-path form: the plane's base advanced by `off` elements, checked once here so the
//! loop that follows can index the raw pointer. `off == n` is legal and yields the end pointer.
def plane_at(pl : BorrowedPlane; off : int64) {
    if (uint64(off) > uint64(pl.n)) {
        panic("dasLLAMA plane: offset {off} outside [0,{pl.n}]")
    }
    return unsafe(pl.p + off)
}

//! Point a plane at `nbytes` of a mapped image starting at `base + off`. false = the section
//! does not divide into whole elements, which is a malformed image and never a recoverable one.
def plane_bind(var pl : BorrowedPlane; base : uint8 const?; off, nbytes : int64) : bool {
    let es = int64(typeinfo sizeof(*pl.p))
    return false if (nbytes % es != 0l)
    unsafe {
        pl.p = reinterpret<typedecl(pl.p)>(base + off)
    }
    pl.n = nbytes / es
    return true
}

//! Drop a plane's view. A plane never owns bytes, so this frees nothing — it exists so a
//! carrier's teardown reads as an intent rather than as two field writes.
def plane_release(var pl : BorrowedPlane) {
    pl.p = null
    pl.n = 0l
}

//! one concrete finalize a plane type (`delete` calls no generic finalize): it nulls the borrowed pointer so a torn-down carrier cannot dangle; a struct holding a plane still takes an unsafe delete
def finalize(var pl : PlaneF) {
    plane_release(pl)
}

def finalize(var pl : PlaneI8) {
    plane_release(pl)
}

def finalize(var pl : PlaneU16) {
    plane_release(pl)
}

def finalize(var pl : PlaneU8) {
    plane_release(pl)
}

def finalize(var pl : PlaneI32) {
    plane_release(pl)
}

def finalize(var pl : PlaneI64) {
    plane_release(pl)
}

//! The plane's elements copied into `dst` - a staging takes a borrowed plane's bytes as its own
def plane_copy(pl; var dst : array<auto(ET)>) {
    dst |> reserve(pl.n)
    dst |> resize(pl.n)
    if (pl.n > 0l) {
        unsafe {
            memcpy(addr(dst[0]), pl.p, pl.n * int64(typeinfo sizeof(dst[0])))
        }
    }
}
