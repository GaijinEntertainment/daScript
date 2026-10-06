options gen2
//! The kernels both GPU homes run, written once: a `class template` here carries a kernel's buffers, its
//! argument struct and its body, and each home stamps it under its own dispatch name.
module dasllama_gpu_kernels_common shared public
require dasllama/dasllama_lint public
require daslib/typemacro_boost

struct GkAxpyArgs {
    nelem : uint
    scale : float
    zero : uint    //! 1 = o's old contents ignored
}

//! o = (zero ? 0 : o) + scale * y over nelem elements, one element an invocation
[ |> template_struct_instance]
class template GkAxpy {
    @ssbo @binding = 0 @role = "readwrite" o : array<float>
    @ssbo @binding = 1 @role = "read" y : array<float>
    @kargs ka : GkAxpyArgs

    def axpy_body {
        let i = gl_GlobalInvocationID.x
        if (i < ka.nelem) {
            let base = ka.zero != 0u ? 0.0 : o[i]
            o[i] = base + ka.scale * y[i]
        }
    }
}

struct GkRopeTabArgs {
    rows : uint   //! the rows roped, row r at the tables' row r past their element bases
    qs : uint     //! x's row stride
    col : uint    //! the roped span's first column (a fused [q | k | v] row's q or k slot)
    d : uint      //! the span's width, its heads side by side
    dh : uint     //! the head size
    coff : uint   //! the cos table [pos][dh / 2]'s element base in its binding
    soff : uint   //! the sin table's element base in its binding, laid out as the cos table
}

//! the rotary embedding over a span of rows in place from per-position tables: row r turns each pair of each head by
//! the tables' angle j at row r - NEOX the pairs (j, j + dh / 2), the CPU `rope_neox_tab_rows` (the vision mrope tables),
//! else the adjacent pairs (2j, 2j + 1), the CPU `rope_rows`; one pair an invocation over rows * d / 2
[ |> template_struct_instance]
class template GkRopeTab {
    @template_constant NEOX : bool = false
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "read" cos_t : array<float>
    @ssbo @binding = 2 @role = "read" sin_t : array<float>
    @kargs pa : GkRopeTabArgs

    def pair_first(hb, j : uint) : uint {
        static_if (NEOX) {
            return hb + j
        } else {
            return hb + 2u * j
        }
    }

    def pair_partner_dist(hh : uint) : uint {
        static_if (NEOX) {
            return hh
        } else {
            return 1u
        }
    }

    def rope_tab_body {
        let gid = gl_GlobalInvocationID.x
        let half = pa.d / 2u
        if (gid < pa.rows * half) {
            let r = gid / max(half, 1u)
            let pi = gid % max(half, 1u)
            let hh = max(pa.dh / 2u, 1u)
            let j = pi % hh
            let i0 = pair_first(r * pa.qs + pa.col + (pi / hh) * pa.dh, j)
            let i1 = i0 + pair_partner_dist(hh)
            let tb = r * hh + j
            let cv = cos_t[pa.coff + tb]
            let sv = sin_t[pa.soff + tb]
            let v0 = x[i0]
            let v1 = x[i1]
            x[i0] = v0 * cv - v1 * sv
            x[i1] = v0 * sv + v1 * cv
        }
    }
}

struct GkConcatArgs {
    c : uint          //! x's row width
    rc : uint         //! the residual columns, read only under `with_res`
    rs : uint         //! the residual rows' stride: 0 broadcasts one row to every position
    f0s : uint        //! the F0 column's row stride
    ns : uint         //! the noise column's row stride
    with_res : uint   //! 1 = the residual columns follow x
    width : uint      //! the output row width, the columns past the last input zero
    rows : uint
}

//! o [rows][width] = cat(x [rows][c], res at stride rs under `with_res`, f0 at stride f0s, nd at stride ns), zero-padded to
//! width - the CPU `concat_rows`; one element an invocation
[ |> template_struct_instance]
class template GkConcat {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "read" res : array<float>
    @ssbo @binding = 2 @role = "read" f0 : array<float>
    @ssbo @binding = 3 @role = "read" nd : array<float>
    @ssbo @binding = 4 @role = "write" o : array<float>
    @kargs ka : GkConcatArgs

    def concat_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < ka.rows * ka.width) {
            let w = max(ka.width, 1u)
            let r = gid / w
            let j = gid % w
            let rc = ka.with_res != 0u ? ka.rc : 0u
            var v = 0.0
            if (j < ka.c) {
                v = x[r * ka.c + j]
            } elif (j < ka.c + rc) {
                v = res[r * ka.rs + (j - ka.c)]
            } elif (j == ka.c + rc) {
                v = f0[r * ka.f0s]
            } elif (j == ka.c + rc + 1u) {
                v = nd[r * ka.ns]
            }
            o[gid] = v
        }
    }
}
