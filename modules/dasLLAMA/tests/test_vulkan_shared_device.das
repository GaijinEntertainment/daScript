options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap

require dastest/testing_boost public
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - sleep: the wait for a finished context's teardown, inside the Vulkan cells
require ?vulkan vulkan   // vk_shared_device*: the process's device holder
require ?vulkan vulkan/vulkan_boost   // set_device_queue_ask / device_queue_count: the queues a created device asks
require ?vulkan dasllama/dasllama_vulkan_common   // vk_moe_init / g_gpu / make_device_buf: a context's share of the device
require _child_run   // child_cmd_at / run_child: the shared-queue cell rerun under the validation layer
require strings
require daslib/strings_boost   // count: the child's threading errors
require math

// One Vulkan device a process (ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-shared-device): every context - a worker thread's
// included - borrows the device the first context made, takes a queue of its own round robin and shares one past the queue
// count, and gives its reference back at its shutdown; a destroyed buffer's bytes leave every other context's count and its
// resident cap; the device asks its queues under the clamped ask; the last context out destroys the device and the next
// makes a fresh one; the shared-queue and lifecycle cells rerun in a child under the Khronos validation layer report no
// threading or destroy-order error. Model-free; skips without dasVulkan or a Vulkan device, and the validation cells also where the layer is not
// installed or the child finds no device under it.

let HELD_BYTES = 32l << 20l   //! the device buffer this context holds while the worker plans

//! what a worker context saw of the process's device
struct WorkerSaw {
    device : uint64
    refs : int
    queue : uint64
    xqueue : uint64   //! its transfer queue, 0 where the device has no transfer family
    bad : int   //! the round trips whose bytes came back wrong
    others : int64   //! the device bytes it saw every other context hold
    carved : int64   //! its resident plan's cap (`carved_budget`)
    budget : int64   //! its uncarved weight budget
}

//! `rounds` upload-and-read-back round trips of `bytes` on this context's own buffers: how many came back wrong
def private round_trips(rounds, bytes : int; seed : int) : int {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let n = bytes / 4
        let dev = make_device_buf(int64(bytes))
        let hb = make_host_buf(int64(bytes), false, [cached = true])
        var src, got : array<float>
        src |> reserve(n)
        src |> resize(n)
        got |> reserve(n)
        got |> resize(n)
        var bad = 0
        for (r in range(rounds)) {
            for (i in range(n)) {
                src[i] = float(seed * 100003 + r * 7919 + i)
            }
            unsafe {
                upload_region_at(dev, 0l, addr<uint8 const?>(src[0]), int64(bytes))
                readback_range(dev, 0u, 0l, hb, addr(got[0]), int64(bytes))
            }
            for (a, b in src, got) {
                if (a != b) {
                    bad++
                    break
                }
            }
        }
        destroy_device_buf(dev)
        destroy_host_buf(hb)
        delete src
        delete got
        return bad
    } else {
        return rounds
    }
}

//! the device ready in this context, or a skip naming why not
def private device_or_skip(t : T?) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!vk_moe_init() || g_gpu == null || g_gpu.dry) {
            t |> skip("no Vulkan device")
            return false
        }
        return true
    } else {
        t |> skip("dasVulkan not built")
        return false
    }
}

//! the holder's reference count once it reads `want`, or what it reads after five seconds - a finished thread's context
//! tears down after the job status it notified
def private refs_settle(want : int) : int {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        for (_ in range(500)) {
            return want if (vk_shared_device_refs() == want)
            sleep(10u)
        }
        return vk_shared_device_refs()
    } else {
        return want
    }
}

//! the queues family `family` of this context's physical device carries, 0 past its families
def private family_queues(family : uint) : uint {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var count = 0u
        vkGetPhysicalDeviceQueueFamilyProperties(g_gpu.phys, count, null)
        var props : array<VkQueueFamilyProperties>
        props |> resize(int(count))
        vkGetPhysicalDeviceQueueFamilyProperties(g_gpu.phys, count, unsafe(addr(props[0])))
        let have = family < count ? props[int(family)].queueCount : 0u
        delete props
        return have
    } else {
        return 0u
    }
}

//! what one worker thread's context saw of the device after `rounds` round trips of `bytes`; a zero device = its tier did not arm
def private worker_saw(rounds, bytes, seed : int) : WorkerSaw {
    var saw = WorkerSaw()
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_channel(1) $(ch) {
            new_thread() <| @capture(= ch) {
                let ok = vk_moe_init()
                let bad = ok ? round_trips(rounds, bytes, seed) : -1
                ch |> push_clone(WorkerSaw(device = ok ? g_gpu.device._vk : 0ul, refs = vk_shared_device_refs(),
                    queue = ok ? intptr(g_gpu.queue) : 0ul, bad = bad, others = ok ? vk_others_dev_bytes() : 0l,
                    carved = ok ? carved_budget() : 0l, budget = ok ? g_gpu.weight_budget : 0l))
                ch |> notify_and_release
            }
            ch |> for_each_clone() $(w : WorkerSaw#) {
                saw = WorkerSaw(device = w.device, refs = w.refs, queue = w.queue, bad = w.bad, others = w.others,
                    carved = w.carved, budget = w.budget)
            }
        }
    }
    return saw
}

//! how many distinct non-zero values `vals` holds
def private distinct_count(vals : array<uint64>) : int {
    var seen : table<uint64>
    for (v in vals) {
        if (v != 0ul) {
            seen |> insert(v)
        }
    }
    let n = length(seen)
    delete seen
    return n
}

[test]
def test_vk_one_device_a_process(t : T?) {
    t |> run("a worker context borrows the process's device, holds a reference while it lives and gives it back at its shutdown") @(t : T?) {
        return if (!device_or_skip(t))
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let dev0 = g_gpu.device._vk
            let refs0 = vk_shared_device_refs()
            let held = make_device_buf(HELD_BYTES)   //! this context's bytes on the device: the worker's plan must give them up
            t |> success(refs0 >= 1 && vk_shared_device() == dev0, "this context holds the process's device ({refs0} references)")
            let held_bytes = g_gpu.dev_bytes_of?[held] ?? 0l
            let saw = worker_saw(4, 4096, 1)
            t |> equal(saw.device, dev0, "the worker's context runs on the device this context made, not a second one")
            t |> equal(saw.refs, refs0 + 1, "while the worker lives the device carries its reference too")
            t |> equal(saw.bad, 0, "the worker's round trips land on its own buffers")
            t |> success(saw.others >= HELD_BYTES, "the worker's room gives up the {HELD_BYTES} bytes this context holds ({saw.others} seen)")
            t |> success(saw.carved <= saw.budget - HELD_BYTES, "the worker's resident cap gives them up too ({saw.carved} of {saw.budget})")
            t |> equal(refs_settle(refs0), refs0, "the worker's shutdown gave its reference back")
            t |> equal(vk_shared_device(), dev0, "and the device stays for the contexts still holding it")
            destroy_device_buf(held)
            let after = worker_saw(1, 4096, 2)
            t |> equal(after.device, dev0, "a second worker borrows the same device")
            t |> equal(saw.others - after.others, held_bytes, "the destroyed buffer's {held_bytes} bytes left every other context's count")
            t |> equal(after.carved - saw.carved, held_bytes, "... and came back to the next worker's resident cap")
            t |> equal(refs_settle(refs0), refs0, "the second worker gave its reference back")
        }
    }
}

[test]
def test_vk_shared_queue_submits(t : T?) {
    t |> run("more worker contexts than the device has queues: the takes go round robin, two share a queue, every submit under its lock, every round trip lands") @(t : T?) {
        return if (!device_or_skip(t))
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let nq = int(vk_shared_queue_count(false))
            let nxq = int(vk_shared_queue_count(true))
            t |> equal(uint(nq), min(family_queues(g_gpu.fam), VK_DEVICE_QUEUES), "the device asked VK_DEVICE_QUEUES compute queues, at most what its family carries")
            if (nxq > 0) {
                t |> equal(uint(nxq), min(family_queues(vk_shared_queue_family(true)), VK_DEVICE_QUEUES), "... and transfer queues the same way")
            }
            let workers = nq + 1   //! with this context's queue in play, at least two contexts share one
            let refs0 = vk_shared_device_refs()
            var queues, worker_queues, worker_xqueues : array<uint64>
            queues |> push(intptr(g_gpu.queue))
            var bad = 0
            with_channel(workers) $(ch) {
                for (k in range(workers)) {
                    new_thread() <| @capture(= ch, = k) {
                        let ok = vk_moe_init()
                        let b = ok ? round_trips(40, 65536, k + 2) : 40
                        ch |> push_clone(WorkerSaw(device = ok ? g_gpu.device._vk : 0ul, queue = ok ? intptr(g_gpu.queue) : 0ul,
                            xqueue = ok && g_gpu.xfer_fam >= 0 ? intptr(g_gpu.xfer_queue) : 0ul, bad = b))
                        ch |> notify_and_release
                    }
                }
                ch |> for_each_clone() $(w : WorkerSaw#) {
                    queues |> push(w.queue)
                    worker_queues |> push(w.queue)
                    worker_xqueues |> push(w.xqueue)
                    bad += w.bad
                }
            }
            t |> equal(length(queues), workers + 1, "every worker answered")
            t |> equal(distinct_count(worker_queues), nq, "{workers} workers' consecutive takes cover all {nq} compute queues: round robin, not one queue for all")
            if (nxq > 0) {
                t |> equal(distinct_count(worker_xqueues), min(nxq, workers), "... and their transfer takes all {nxq} transfer queues")
            }
            delete worker_queues
            delete worker_xqueues
            var paired = false
            for (i in range(length(queues))) {
                for (j in range(i + 1, length(queues))) {
                    paired = paired || queues[i] == queues[j]
                }
            }
            t |> success(paired, "{workers + 1} contexts over {nq} queues: two of them submit to one queue")
            t |> equal(bad, 0, "every context's round trips landed, the shared queue's included")
            t |> equal(refs_settle(refs0), refs0, "every worker gave its reference back")
            delete queues
        }
    }
}

[test]
def test_vk_device_queue_ask(t : T?) {
    t |> run("set_device_queue_ask clamps an ask to at least one queue, and device_queue_count answers the ask, at most what the family carries") @(t : T?) {
        return if (!device_or_skip(t))
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let fam = g_gpu.fam
            let have = family_queues(fam)
            set_device_queue_ask(0u)
            t |> equal(device_queue_count(g_gpu.phys, fam), 1u, "an ask of zero clamps to one queue")
            set_device_queue_ask(2u)
            t |> equal(device_queue_count(g_gpu.phys, fam), min(have, 2u), "an ask of two gets two where the family carries them ({have})")
            set_device_queue_ask(1000u)
            t |> equal(device_queue_count(g_gpu.phys, fam), have, "an ask past the family's count gets the family's {have}")
            t |> equal(device_queue_count(g_gpu.phys, 1000u), 1u, "a family past the device's answers one queue")
            set_device_queue_ask(1u)
        }
    }
}

//! this file's cell `name` rerun in a child, under the Khronos validation layer when `layered`; the child's exit code, or -1
//! after a skip naming why it proves nothing (the layer not installed, no device in the child)
def private child_cell(t : T?; name : string; layered : bool; var output : string&) : int {
    var envs : array<tuple<name : string; value : string>>
    if (layered) {
        envs |> push((name = "VK_INSTANCE_LAYERS", value = "VK_LAYER_KHRONOS_validation"))
        envs |> push((name = "VK_LOADER_DEBUG", value = "layer"))
    }
    let cmd = child_cmd_at("dastest/dastest.das", envs, "--test \"{get_das_root()}/modules/dasLLAMA/tests/test_vulkan_shared_device.das\" --test-names {name}", true)
    delete envs
    let rc = run_child(cmd, output)
    if (layered && (find(output, "Insert instance layer") < 0 || find(output, "VK_LAYER_KHRONOS_validation") < 0)) {
        t |> skip("the Khronos validation layer is not installed on this box: no loader line inserts it")
        return -1
    }
    if (find(output, "1 skipped") >= 0) {
        t |> skip("the child found no Vulkan device")
        return -1
    }
    return rc
}

//! the last 2000 bytes of `output`, for a failed child's message
def private output_tail(output : string) : string => slice(output, max(0, length(output) - 2000))

//! the validation layer's id for two threads in one externally synchronized call on the same object - here a queue
let THREADING_ERROR = "UNASSIGNED-Threading"

//! the validation layer's ids for an object destroyed while its children live - a device past its instance, a buffer past
//! its device; the layer's other errors (a packaged layer older than the driver's pNext structs) say nothing of the lifecycle
let DESTROY_ERROR = "VUID-vkDestroy"

//! the loader's layer log (`VK_LOADER_DEBUG=layer`): a line for each instance the layer goes into, and one for each it leaves
//! at `vkDestroyInstance`
let LAYER_INSERTED = "Insert instance layer"
let LAYER_UNLOADED = "Unloading layer library"

[test]
def test_vk_shared_queue_under_validation(t : T?) {
    t |> run("the shared-queue cell rerun in a child under the Khronos validation layer reports no threading error: every submit and wait on a shared queue holds its lock") @(t : T?) {
        return if (!device_or_skip(t))
        var output = ""
        let rc = child_cell(t, "test_vk_shared_queue_submits", true, output)
        return if (rc < 0)
        t |> equal(rc, 0, "the child's shared-queue cell passed under the layer\n{output_tail(output)}")
        t |> equal(count(output, THREADING_ERROR), 0, "no {THREADING_ERROR} error: a shared queue's submits and waits each held its lock")
    }
}

[test]
def test_vk_last_context_out(t : T?) {
    t |> run("the last context out destroys the device and clears the holder; the next context makes a fresh device and serves on it, a finalized loader included") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (vk_shared_device_refs() != 0) {   //! another cell's context holds the device: the cell runs in a child that holds none
                var output = ""
                let rc = child_cell(t, "test_vk_last_context_out", false, output)
                return if (rc < 0)
                t |> equal(rc, 0, "the child's lifecycle cell passed\n{output_tail(output)}")
                return
            }
            let first = worker_saw(2, 4096, 3)
            if (first.device == 0ul) {
                t |> skip("no Vulkan device")
                return
            }
            t |> equal(first.refs, 1, "the worker made the device: its reference the only one")
            t |> equal(first.bad, 0, "its round trips land")
            t |> equal(refs_settle(0), 0, "its shutdown released the last reference")
            t |> equal(vk_shared_device(), 0ul, "the last context out destroyed the device and cleared the holder")
            t |> equal(vk_shared_device_bytes(0l), 0l, "no device bytes stay counted")
            let second = worker_saw(2, 4096, 4)
            t |> equal(second.refs, 1, "the next context made a fresh device, not a borrow of the destroyed one")
            t |> equal(second.bad, 0, "its round trips land: the loader table serves the fresh device")
            t |> equal(refs_settle(0), 0, "its shutdown released the fresh device's last reference")
            t |> equal(vk_shared_device(), 0ul, "and destroyed it")
            volkFinalize()   //! no device lives: the loader goes, and the next context's volkInitialize must load it again
            let third = worker_saw(2, 4096, 5)
            t |> equal(third.refs, 1, "after volkFinalize the next context reloads the loader and makes a device")
            t |> equal(third.bad, 0, "... and serves on it")
            t |> equal(refs_settle(0), 0, "its shutdown released that device's last reference")
        } else {
            t |> skip("dasVulkan not built")
        }
    }
}

[test]
def test_vk_validation_last_context_out(t : T?) {   //! not a `test_vk_last_context_out` prefix: the child's name filter would rerun this cell too
    t |> run("the lifecycle cell rerun in a child under the Khronos validation layer reports no destroy-order error: the last context out destroys every object it made, the device before the instance") @(t : T?) {
        return if (!device_or_skip(t))
        var output = ""
        let rc = child_cell(t, "test_vk_last_context_out", true, output)
        return if (rc < 0)
        t |> equal(rc, 0, "the child's lifecycle cell passed under the layer\n{output_tail(output)}")
        t |> equal(count(output, DESTROY_ERROR), 0, "no {DESTROY_ERROR} error: every object went before its parent\n{output_tail(output)}")
        let made = count(output, LAYER_INSERTED)
        t |> success(made >= 3, "the loader inserted the layer into each of the cell's three instances ({made})")
        t |> equal(count(output, LAYER_UNLOADED), made, "and unloaded it as many times: every instance the cell made was destroyed")
    }
}
