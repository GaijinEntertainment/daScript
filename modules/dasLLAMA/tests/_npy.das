options gen2
options _dasllama_internal = true

module _npy shared public

require dasllama/dasllama_lint public
require daslib/fio
require strings
require daslib/strings_boost

//! A reference dump: `rows` x `cols` f32 in row-major order.
struct Npy {
    rows : int64
    cols : int64
    data : array<float>
}

//! A little-endian f32 .npy of one or two dims, read into row-major order (a Fortran-order dump transposes).
def read_npy(path : string) : Npy {
    var r = Npy()
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("_npy: cannot open '{path}'")
        }
        fmap(f) $(var bytes : array<uint8>#) {
            let hlen = int64(uint(bytes[8]) | (uint(bytes[9]) << 8u))
            let header = build_string() $(var w) {
                for (i in range64(10l, 10l + hlen)) {
                    w |> write_char(int(bytes[i]))
                }
            }
            if (find(header, "'<f4'") < 0) {
                panic("_npy: '{path}' is not a little-endian f32 array: {header}")
            }
            let fortran = find(header, "'fortran_order': True") >= 0
            let lp = find(header, "(")
            let rp = find(header, ")")
            var dims : array<int64>
            for (s in split(slice(header, lp + 1, rp), ",")) {
                let v = strip(s)
                if (!empty(v)) {
                    dims |> push(to_int64(v, false))
                }
            }
            r.rows = dims[0]
            r.cols = length(dims) > 1 ? dims[1] : 1l
            let n = r.rows * r.cols
            r.data |> reserve(n)
            r.data |> resize(n)
            unsafe {
                let src = addr<float const?>(bytes[10l + hlen])
                for (i in range64(r.rows)) {
                    for (j in range64(r.cols)) {
                        r.data[i * r.cols + j] = fortran ? src[j * r.rows + i] : src[i * r.cols + j]
                    }
                }
            }
        }
    }
    return <- r
}
