options gen2

// Shared GPU plumbing + eyeball-dump compares for the per-class Metal kernel-oracle suites
// (test_metal_*_kernels). Generic bodies instantiate only inside each suite's Apple static_if,
// so off-Apple this module parses and drops out whole.
module _metal_kernel_common shared public

require math
require ?das_metal metal/das_metal_boost

// ===== device buffers =====

def buf_upload(dev; arr : array<auto(TT)>) : auto {
    var buf = metal_new_buffer(dev, uint64(long_length(arr)) * uint64(typeinfo sizeof(arr[0])))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(length(arr))) {
        unsafe(p[k]) = arr[k]
    }
    return buf
}

def buf_store(buf; arr : array<auto(TT)>) {
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(length(arr))) {
        unsafe(p[k]) = arr[k]
    }
}

def buf_fill(dev; nn : int; value : auto(TT)) : auto {
    var buf = metal_new_buffer(dev, uint64(nn) * uint64(typeinfo sizeof(value)))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(nn)) {
        unsafe(p[k]) = value
    }
    return buf
}

def buf_u32(dev; v : uint) : auto {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def buf_f32(dev; v : float) : auto {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def buf_download(buf; nn : int; t : type<auto(TT)>) : array<TT -const> {
    var out : array<TT -const>
    out |> resize(nn)
    let p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(nn)) {
        out[k] = unsafe(p[k])
    }
    return <- out
}

// ===== compares =====
// Every mismatch dumps the first 6 diffs with gpu/cpu values — enough to tell tolerance drift
// from garbage at a glance, before any rerun (the vulkan kernel suite's eyeball rail).

let private DUMP_CAP = 6

// relative bar + an absolute floor (absf > 0 serves f16-rounded outputs, whose error scales
// with magnitude, not the possibly-cancelled-small result)
def buf_mismatch_bars(buf; want : array<float>; rel, absf : float; tag : string) : int {
    let p = unsafe(reinterpret<float const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = unsafe(p[k])
        let w = want[k]
        if (abs(g - w) > absf && abs(g - w) > rel * max(1.0, max(abs(g), abs(w)))) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

def buf_mismatch_approx(buf; want : array<float>; tag : string) : int {
    return buf_mismatch_bars(buf, want, 1e-4, 0.0, tag)
}

// f16-holding buffer vs an f32 expectation: dequant per element, compare under bars (the f16
// store rounds at 2^-11 relative — pass rel >= 1e-3 so the rounding never reads as a red)
def buf_mismatch_f16(buf; want : array<float>; rel, absf : float; tag : string) : int {
    let p = unsafe(reinterpret<float16 const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = float(unsafe(p[k]))
        let w = want[k]
        if (abs(g - w) > absf && abs(g - w) > rel * max(1.0, max(abs(g), abs(w)))) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

def buf_mismatch_exact(buf; want : array<auto(TT)>; tag : string) : int {
    let p = unsafe(reinterpret<TT -const const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = unsafe(p[k])
        if (g != want[k]) {
            if (bad < DUMP_CAP) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {want[k]}\n")
            }
            bad++
        }
    }
    return bad
}
