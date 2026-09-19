options gen2
options stack = 524288   // a ModelSlot local carries a whole Model and Scheduler: three of them outgrow the 16K default
options persistent_heap

require dastest/testing_boost public
require main                                            // the program root: vulkan_serving_want (sibling by bare name)
require openai_server                                   // ModelSlot + the served strings (sibling by bare name)
require strings

// Model-free unit coverage for how a slot says it is served: every arm of `served` and `served_note`,
// the device-mode predicate's five terms, the context clamp to a K/V region, and the want the
// vulkan backend asks for. Hand-built slots - no GGUF, no GPU, no JIT.

def private gpu_slot(effective : string) : ModelSlot {
    var s <- ModelSlot(name = "m")
    s.opts.backend = "gpu"
    s.backend_effective = effective
    s.sched.max_streams = 4l
    s.gpu_regions = 4l
    return <- s
}

def private drop(var s : ModelSlot) {
    unsafe {
        delete s
    }
}

[test]
def test_slot_served(t : T?) {
    t |> run("served: one sentence per way a slot is served") @(t : T?) {
        var cpu <- gpu_slot("cpu")
        t |> equal(slot_served(cpu, false, false), "CPU")
        drop(cpu)
        var whole <- gpu_slot("gpu:resident")
        let on_card = slot_served(whole, true, true)
        let on_host = slot_served(whole, false, true)
        t |> success(find(on_card, "every stream's cache on the card") >= 0, "the whole model with the caches on the card: {on_card}")
        t |> success(find(on_host, "caches on the host") >= 0, "the whole model with the caches on the host: {on_host}")
        let waiting = slot_served(whole, true, false)
        t |> success(find(waiting, "another model holds it") >= 0, "a GPU slot that does not hold the card says so: {waiting}")
        t |> equal(slot_served_note(whole, true, false), "", "and carries no note: nothing of its own holds it back")
        drop(whole)
        var part <- gpu_slot("gpu:rails")
        let in_part = slot_served(part, false, true)
        t |> success(find(in_part, "GPU in part") >= 0 && find(in_part, "caches on the host") >= 0, "the per-layer offload says where the caches sit: {in_part}")
        drop(part)
    }
    t |> run("served_note: what holds the slot back, each reason its own sentence") @(t : T?) {
        var asked_cpu <- gpu_slot("cpu")
        asked_cpu.opts.backend = "cpu"
        asked_cpu.resident_decline = "anything"
        t |> equal(slot_served_note(asked_cpu, false, false), "", "a slot that asked for the CPU has nothing holding it back")
        drop(asked_cpu)
        var declined <- gpu_slot("gpu:rails")
        t |> equal(slot_served_note(declined, false, true), "", "no decline recorded: no note")
        declined.resident_decline = "THE-ENGINE-TEXT"
        let note = slot_served_note(declined, false, true)
        t |> success(find(note, "THE-ENGINE-TEXT") > 0 && find(note, "The engine's reason") > 0, "the engine's decline rides after a plain lead: {note}")
        t |> success(find(note, "only part of the work") >= 0, "a slot served in part says so: {note}")
        declined.backend_effective = "cpu"
        t |> success(find(slot_served_note(declined, false, false), "serves nothing") >= 0, "a slot the GPU serves nothing of says that instead")
        drop(declined)
        var whole <- gpu_slot("gpu:resident")
        t |> equal(slot_served_note(whole, true, true), "", "caches on the card: nothing to say")
        whole.audio_armed = true
        t |> success(find(slot_served_note(whole, false, true), "images or audio") >= 0, "a media tower keeps the caches on the host")
        whole.audio_armed = false
        whole.sched.mtp = true
        t |> success(find(slot_served_note(whole, false, true), "self-speculation") >= 0, "so does self-speculation")
        whole.sched.mtp = false
        whole.gpu_regions = 2l
        let short = slot_served_note(whole, false, true)
        t |> success(find(short, "room for 2 stream caches") >= 0 && find(short, "serves 4 streams") >= 0, "fewer cache slots than streams names both counts: {short}")
        drop(whole)
    }
}

[test]
def test_slot_wants_device_kv(t : T?) {
    t |> run("the streams' caches go to the card only when every term holds") @(t : T?) {
        var s <- gpu_slot("gpu:resident")
        t |> success(slot_wants_device_kv(s), "the whole model on the card, a cache slot per stream, text only")
        s.backend_effective = "gpu:rails"
        t |> success(!slot_wants_device_kv(s), "not under the per-layer offload")
        s.backend_effective = "gpu:resident"
        s.audio_armed = true
        t |> success(!slot_wants_device_kv(s), "not with an audio tower")
        s.audio_armed = false
        s.sched.mtp = true
        t |> success(!slot_wants_device_kv(s), "not under self-speculation")
        s.sched.mtp = false
        s.gpu_regions = 3l
        t |> success(!slot_wants_device_kv(s), "not with fewer cache slots than streams")
        s.gpu_regions = 4l
        t |> success(slot_wants_device_kv(s), "and again once every term holds")
        drop(s)
    }
    t |> run("a stream's context is its cache slot's") @(t : T?) {
        t |> equal(region_clamped_ctx(131072l, 4l, 49926l), 49926l, "a model context past the slot is clamped to it")
        t |> equal(region_clamped_ctx(2048l, 4l, 49926l), 2048l, "a shorter one is kept")
        t |> equal(region_clamped_ctx(131072l, 0l, 0l), 131072l, "no slots armed: the model's own context stands")
    }
}

[test]
def test_vulkan_serving_want(t : T?) {
    t |> run("the vulkan backend asks for the whole-model driver on every GPU slot") @(t : T?) {
        let cfg = ServerArgs()
        let want = vulkan_serving_want(cfg)
        t |> success(want.resident, "the want names the whole-model driver outright")
        t |> success(!want.auto_tier, "and rides no auto pick: each rail is spelled out")
        t |> success(want.shexp, "the shared expert rides with the expert stacks")
    }
}
