options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The GPU home as a contract, model-free and device-free (a switch installs seats; nothing arms): the home is the
// environment's spelling, the arm registrant follows it, a name no entry registered is refused, and the switch to the
// other home this build carries lands before the tier arms and is refused after.

require dastest/testing_boost public
require dasllama/dasllama_core   // the facade's home calls (gpu_home, gpu_homes, set_gpu_home); both discrete homes' entries register at [init]
require dasllama/dasllama_gpu_tier   // the declared-flag readers; DnSlotOwner, dn_slot_bind / dn_slot_advance / dn_slot_drop: the recurrent slot's owner record
require dasllama/dasllama_gpu_stream_cache   // set_stream_pinned_fns, stream_pinned_release, PinnedBuf, STREAM_HOME_*: the expert cache's pinned seat
require dasllama/dasllama_env   // g_env_gpu: the environment's spelling of the home

//! the flags a home's installer declares by value, read back: the CUDA home feeds every site in its own form, the Vulkan
//! home does not (the host twin is no witness: the Vulkan entry installs it, the CUDA home's arm does)
def private home_flags_follow(tc : T?; home : string) {
    tc |> equal(gpu_home(), home, "the home reads {home}")
    tc |> equal(moe_gpu_backend_name(), home, "the arm registrant is the {home} home's")
    tc |> equal(gpu_want_cuda(), home == "cuda", "gpu_want_cuda follows the home")
    tc |> equal(rdec_can_mix_site_forms(), home == "cuda", "the site-feeds flag is the {home} home's declaration")
}

//! the environment's spelling of the home: the CUDA ask in a build that carries the home, else vulkan
def private env_home : string => g_env_gpu.cuda && has_value(gpu_homes(), "cuda") ? "cuda" : "vulkan"

var private g_shared_installs = 0

def private count_shared_install() {
    g_shared_installs++
}

[test(description = "The GPU home is the environment's spelling with the arm registrant and site-feed flags following it, unknown names are refused, and switching to the build's other home lands with one shared install before the tier arms and is refused after")]
def test_gpu_home(t : T?) {
    if (!has_value(gpu_homes(), env_home())) {
        t |> skip("this build does not carry the {env_home()} home the environment spells (it has {gpu_homes()})")
        return
    }
    t |> run("the home is the environment's spelling, among the homes this build carries") @(tc : T?) {
        let home = gpu_home()
        tc |> success(home == "vulkan" || home == "cuda", "the home is a discrete home's name ({home})")
        tc |> success(has_value(gpu_homes(), home), "the home is a registered one ({gpu_homes()})")
        tc |> equal(home, env_home(), "DASLLAMA_CUDA spells the home")
        home_flags_follow(tc, home)
    }
    t |> run("a name no entry registered is refused; the current home is a no-op") @(tc : T?) {
        let home = gpu_home()
        tc |> success(!set_gpu_home("metal"), "metal is no discrete home")
        tc |> success(!set_gpu_home(""), "the empty name is refused")
        tc |> equal(gpu_home(), home, "a refused switch leaves the home")
        tc |> success(set_gpu_home(home), "the current home answers true and changes nothing")
        home_flags_follow(tc, home)
    }
    t |> run("the switch to the other home lands before the tier arms, is refused after, and comes back") @(tc : T?) {
        let home = gpu_home()
        var other = ""
        for (h in gpu_homes()) {
            if (h != home) {
                other = h
            }
        }
        if (empty(other)) {
            tc |> skip("this build carries one discrete home ({gpu_homes()})")
            return
        }
        register_gpu_home_shared(@@count_shared_install)
        let installs = g_shared_installs
        tc |> success(moe_gpu_loader_seats_installed(), "the whole-model driver's provider seats hold its providers at [init]")
        if (moe_gpu_tier_installed()) {
            tc |> success(!set_gpu_home(other), "the tier armed on {home} at [init] (DASLLAMA_GPU=1): the switch to {other} is refused")
            home_flags_follow(tc, home)
            tc |> equal(g_shared_installs, installs, "the refused switch (DASLLAMA_GPU=1) ran no shared installer")
            tc |> success(moe_gpu_loader_seats_installed(), "and left the provider seats filled")
            unregister_gpu_home_shared(@@count_shared_install)
            return
        }
        tc |> success(set_gpu_home(other), "the switch to {other} lands")
        home_flags_follow(tc, other)
        tc |> equal(g_shared_installs, installs + 1, "the switch (DASLLAMA_GPU unset) ran the shared installer once")
        tc |> success(moe_gpu_loader_seats_installed(), "the provider seats hold the driver's providers again under {other}: the shared installers ran after the reset")
        tc |> success(set_gpu_home(home), "and back to {home}")
        home_flags_follow(tc, home)
        tc |> equal(g_shared_installs, installs + 2, "the switch back (DASLLAMA_GPU unset) ran it once more")
        tc |> success(moe_gpu_loader_seats_installed(), "and the provider seats hold the driver's providers under {home}")
        unregister_gpu_home_shared(@@count_shared_install)
    }
}

// Model-free units over the host code both discrete homes share: the expert cache's pinned-memory seat frees a buffer
// through the home that made it, whichever home serves by then, and the recurrent slot's owner record keeps the ring image
// a dropped slot last advanced onto - a parked claim's checkpoint copies that image after the owner let go. No device and
// no model run here; the fake allocators below replace, for both homes, whatever pinned seats the homes' entries
// registered at [init], so they are the only allocators the cells call.

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
[test(description = "With fake allocators registered for both homes, the expert cache frees a pinned buffer through the free of the home that made it, never the other, and a refused buffer with null mapping through neither")]
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
[test(description = "A recurrent slot's owner record keeps the ring image it last advanced onto when dropped, forgetting its owner and addresses, so a parked claim's checkpoint copies the right parity; a new bind resets it")]
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
