options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

// Pins the borrowed-plane vocabulary (dasllama_plane): a plane binds to a byte range, reads
// back the bytes that are there, reports its own length, and owns nothing. Feed the function,
// check the bytes — no model, no image, no file. Out-of-range indexing panics by design; its cell reads the panic's text
// (`panic_text`).

require dastest/testing_boost public
require dasllama/dasllama_plane
require strings   // find: a refusal's text

// a stand-in image: 32 floats at byte 0, then 16 int8 at byte 128
def private fake_image(var b : array<uint8>) {
    b |> resize(144)
    unsafe {
        var f = addr<float?>(b[0])
        for (i in range(32)) {
            f[i] = float(i) * 0.25
        }
        var q = addr<int8?>(b[128])
        for (i in range(16)) {
            q[i] = int8(i - 8)
        }
    }
}

def private image_base(b : array<uint8>) : uint8 const? => unsafe(addr<uint8 const?>(b[0]))

[test]
def test_plane_float_binds_and_reads(t : T?) {
    t |> run("PlaneF binds a byte range and reads the bytes that are there") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var pl = PlaneF()
        t |> success(plane_bind(pl, image_base(b), 0l, 128l), "a 128-byte section binds")
        t |> equal(length(pl), 32l)
        t |> success(!empty(pl), "a bound plane is not empty")
        t |> equal(pl[0], 0.0)
        t |> equal(pl[7], 1.75)          // int index
        t |> equal(pl[31l], 7.75)        // int64 index
    }
}

[test]
def test_plane_float_offset_section(t : T?) {
    t |> run("a plane bound past the start reads from there, not from the base") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var pl = PlaneF()
        t |> success(plane_bind(pl, image_base(b), 64l, 64l), "the tail half binds")
        t |> equal(length(pl), 16l)
        t |> equal(pl[0], 4.0)           // element 16 of the whole run
        t |> equal(pl[15], 7.75)
    }
}

[test]
def test_plane_float_rejects_ragged_section(t : T?) {
    t |> run("a section that does not divide into whole floats is declined, not rounded") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var pl = PlaneF()
        t |> success(!plane_bind(pl, image_base(b), 0l, 126l), "126 bytes is not 31.5 floats")
        t |> equal(length(pl), 0l)
        t |> success(empty(pl), "a declined bind leaves the plane empty")
    }
}

[test]
def test_plane_int8_binds_and_reads(t : T?) {
    t |> run("PlaneI8 counts bytes as elements and reads them signed") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var pl = PlaneI8()
        t |> success(plane_bind(pl, image_base(b), 128l, 16l), "the quant section binds")
        t |> equal(length(pl), 16l)
        t |> equal(int(pl[0]), -8)
        t |> equal(int(pl[8l]), 0)
        t |> equal(int(pl[15]), 7)
    }
}

[test]
def test_plane_at_refuses_past_the_end(t : T?) {
    t |> run("plane_at one element past a plane's end panics naming the offset and the plane's length - a float plane and a byte plane") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var pf = PlaneF()
        var pu = PlaneU8()
        t |> success(plane_bind(pf, image_base(b), 0l, 128l) && plane_bind(pu, image_base(b), 128l, 16l), "bound")
        let past_f = panic_text() $ {
            let p = plane_at(pf, 33l)
            t |> success(p != null, "unreached: the offset is past the end")
        }
        t |> success(find(past_f, "offset 33 outside [0,32]") >= 0, "the float plane's refusal: '{past_f}'")
        let past_u = panic_text() $ {
            let p = plane_at(pu, 17l)
            t |> success(p != null, "unreached: the offset is past the end")
        }
        t |> success(find(past_u, "offset 17 outside [0,16]") >= 0, "the byte plane's refusal: '{past_u}'")
        t |> equal(panic_text() $ { let p = plane_at(pu, 16l); t |> success(p != null, "the end pointer") }, "", "the end itself is legal (the control)")
    }
}

[test]
def test_plane_at_is_the_hot_path_form(t : T?) {
    t |> run("plane_at hands out a raw pointer at an element offset; end is legal") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var pl = PlaneF()
        t |> success(plane_bind(pl, image_base(b), 0l, 128l), "bound")
        let mid = plane_at(pl, 8l)
        t |> equal(unsafe(mid[0]), 2.0)
        t |> equal(unsafe(mid[3]), 2.75)
        t |> success(plane_at(pl, 32l) != null, "the end pointer is a legal offset")
        t |> equal(unsafe(plane_at(pl, 0l)[31]), 7.75)
    }
}

[test]
def test_plane_release_owns_nothing(t : T?) {
    t |> run("release drops the view and leaves the bytes alone") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var pl = PlaneF()
        t |> success(plane_bind(pl, image_base(b), 0l, 128l), "bound")
        plane_release(pl)
        t |> equal(length(pl), 0l)
        t |> success(empty(pl), "a released plane is empty")
        t |> success(pl.p == null, "a released plane points nowhere")
        // the backing is untouched — a plane never owned it
        var again = PlaneF()
        t |> success(plane_bind(again, image_base(b), 0l, 128l), "rebinds")
        t |> equal(again[31], 7.75)
    }
}

[test]
def test_plane_two_views_one_backing(t : T?) {
    t |> run("two planes may view the same bytes - neither owns them") @(t : T?) {
        var inscope b : array<uint8>
        fake_image(b)
        var a = PlaneF()
        var c = PlaneF()
        t |> success(plane_bind(a, image_base(b), 0l, 128l), "first view")
        t |> success(plane_bind(c, image_base(b), 64l, 64l), "second view, overlapping")
        t |> equal(a[16], c[0])
        t |> equal(a[31], c[15])
    }
}
