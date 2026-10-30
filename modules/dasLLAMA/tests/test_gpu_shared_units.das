options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gpu_stream_cache   // set_stream_pinned_fns, stream_pinned_release, PinnedBuf, STREAM_HOME_*: the expert cache's pinned seat
require dasllama/dasllama_gpu_tier           // DnSlotOwner, dn_slot_bind / dn_slot_advance / dn_slot_drop: the recurrent slot's owner record

// Model-free units over the host code both discrete homes share, in a program that loads neither home: the expert cache's
// pinned-memory seat frees a buffer through the home that made it, whichever home serves by then, and the recurrent slot's
// owner record keeps the ring image a dropped slot last advanced onto - a parked claim's checkpoint copies that image after
// the owner let go. No device, no model, no `[init]` of a GPU home runs here, so the fake allocators below are the only
// registrations the process holds.

var private g_vk_frees : array<int>    //! the home each Vulkan-side free was handed (the buffer's `home`)
var private g_cud_frees : array<int>   //! ... and each CUDA-side free

[unused_argument(bytes, soft)]
def private fake_alloc(bytes : int64; soft : bool) : PinnedBuf => PinnedBuf()

def private vk_free(b : PinnedBuf) {
    g_vk_frees |> push(b.home)
}

def private cud_free(b : PinnedBuf) {
    g_cud_frees |> push(b.home)
}

//! a buffer carries its maker (`PinnedBuf.home`) and goes back through that home's free whichever home serves by then: a
//! Vulkan-made buffer through the Vulkan free, a CUDA-made one through the CUDA free, a refused one (`mapped` null) through
//! neither - a process that brings up both homes (the Vulkan tower arms, the CUDA driver serves) frees nothing across them
[test]
def test_stream_pinned_free_follows_the_maker(t : T?) {
    t |> run("the expert cache frees a pinned buffer through the home that made it, a refused buffer through neither") @(t : T?) {
        set_stream_pinned_fns(STREAM_HOME_VULKAN, @@fake_alloc, @@vk_free)
        set_stream_pinned_fns(STREAM_HOME_CUDA, @@fake_alloc, @@cud_free)
        var word = 0
        let p = unsafe(addr(word))
        stream_pinned_release(PinnedBuf(mapped = p, bytes = 4l, home = STREAM_HOME_CUDA))
        t |> equal(length(g_cud_frees), 1, "a CUDA-made buffer goes back through the CUDA free")
        t |> equal(length(g_vk_frees), 0, "... and never through the Vulkan one")
        stream_pinned_release(PinnedBuf(mapped = p, bytes = 4l, home = STREAM_HOME_VULKAN))
        t |> equal(length(g_vk_frees), 1, "a Vulkan-made buffer goes back through the Vulkan free")
        t |> equal(length(g_cud_frees), 1, "... and never through the CUDA one")
        stream_pinned_release(PinnedBuf(bytes = 4l, home = STREAM_HOME_CUDA))
        t |> equal(length(g_cud_frees) + length(g_vk_frees), 2, "a refused buffer (mapped null) is freed through neither home")
        t |> success(g_cud_frees[0] == STREAM_HOME_CUDA && g_vk_frees[0] == STREAM_HOME_VULKAN, "each free was handed the buffer it made")
    }
}

//! the recurrent slot's owner record: a bind names the ring image and the owner, an advance moves the image and marks the
//! host copy stale, and a drop forgets the owner but keeps the image - the parked claim's checkpoint reads the slot's image
//! after the owner let go, so a drop that zeroed it would copy the other image at odd parity
[test]
def test_dn_slot_drop_keeps_the_ring_image(t : T?) {
    t |> run("a dropped recurrent slot keeps the ring image it last advanced onto and forgets its owner") @(t : T?) {
        var state, hist : array<float>
        state |> resize(4)
        hist |> resize(4)
        var o = DnSlotOwner()
        let stp = unsafe(addr(state[0]))
        let histp = unsafe(addr(hist[0]))
        dn_slot_bind(o, stp, histp, 0, false)
        t |> success(o.valid && !o.dirty && o.parity == 0 && dn_slot_holds(o, stp), "the bind names the owner on ring image 0")
        dn_slot_advance(o, 1)
        t |> success(o.dirty && o.parity == 1, "the device's step moved the slot onto image 1 and the host copy is stale")
        dn_slot_drop(o)
        t |> equal(o.parity, 1, "the dropped slot keeps ring image 1 - the image a parked claim's checkpoint copies")
        t |> success(!o.valid && !o.dirty && o.state_addr == 0ul && o.hist_addr == 0ul, "... and forgets its owner: nothing to write back")
        t |> success(!dn_slot_holds(o, stp), "the dropped slot holds no session")
        dn_slot_bind(o, stp, histp, 0, true)
        t |> success(o.parity == 0 && o.dirty, "a new bind names its own image again")
    }
}
