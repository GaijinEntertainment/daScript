options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The GPU home as a contract, model-free and device-free (a switch installs seats; nothing arms): the home is the
// environment's spelling, the arm registrant follows it, a name no entry registered is refused, and the switch to the
// other home this build carries lands before the tier arms and is refused after.

require dastest/testing_boost public
require dasllama/dasllama_core   // the facade's home calls (gpu_home, gpu_homes, set_gpu_home); both discrete homes' entries register at [init]
require dasllama/dasllama_gpu_tier   // the declared-flag readers
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

[test]
def test_gpu_home(t : T?) {
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
