options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_vad
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require math
require dasllama/dasllama_audio_io
require ?das_metal dasllama/dasllama_metal_tower   // metal_tower_vad_stats: the device seat's served counters
require ?vulkan dasllama/dasllama_vulkan_tower     // vulkan_tower_vad_stats
require ?vulkan dasllama/dasllama_vulkan_common    // g_gpu: the Vulkan device and memory decline legs' levers
require daslib/fio
require _model_tier
require _compares   // check / check_exceeds: the twin's bar and its control
require _gpu_knobs   // with_gpu_knobs / set_serving_gpu_tower: the seat cells' tower pin
require ?das_metal dasllama/dasllama_metal_common   // metal_common_init: whether the box has a Metal device

// Silero-VAD tests. The forward gate is self-contained (the ~1.2 MB weights are checked in at
// modules/dasLLAMA/models/silero_vad.bin) and drives a deterministic integer-LCG signal — the
// expected per-frame probs are frozen from onnxruntime on the official silero_vad_16k_op15.onnx
// (harness/silero_vad_oracle.py). The segment case additionally needs the corpus's jfk.wav
// and matches silero get_speech_timestamps sample-for-sample. The GPU seat cells skip where no seat
// serves: a build with neither das_metal nor the Vulkan module, a Metal build on a box with no Metal
// device, a Vulkan build where the tier arms no device (`DASLLAMA_GPU=1` unset, or no device); the
// decline cells skip on the other home's build. The streaming leg skips without jfk.wav, and the
// batch segmenter runs a leg per clip present (jfk.wav, benchmarks/asr/corpus/ru_long.wav), skipping
// with neither.

def private vad_bin_path() : string => "{get_das_root()}/modules/dasLLAMA/models/silero_vad.bin"

// deterministic integer LCG (Numerical Recipes), fp32-mapped to [-0.15, 0.15) — identical to
// the python freeze in the oracle run, so the inputs are bit-for-bit equal on both sides.
def private lcg_signal(n : int) : array<float> {
    var out : array<float>
    out |> resize(n)
    var state = 12345u
    for (i in range(n)) {
        state = state * 1664525u + 1013904223u
        out[i] = (float(state >> 16u) / 32768.0 - 1.0) * 0.15
    }
    return <- out
}

[test]
def test_vad_forward_lcg(t : T?) {
    t |> run("per-frame prob matches onnxruntime op15 on a deterministic LCG signal") @(t : T?) {
        // frozen from harness/silero_vad_oracle.py on the same LCG (10 chunks)
        let expected = [0.01383990, 0.02051842, 0.01915139, 0.01694492, 0.01415426,
                        0.01128137, 0.01160875, 0.01086920, 0.00850144, 0.02220330]
        let sig <- lcg_signal(10 * 512)
        with_dasllama_jobque_() {
            var m <- load_vad_model(vad_bin_path())
            let probs <- vad_probs(m, sig)
            t |> success(length(probs) == 10, "10 chunks -> 10 probs")
            for (i in range(min(length(probs), length(expected)))) {
                t |> success(abs(probs[i] - expected[i]) < 1e-3, "frame {i} prob within 1e-3 of oracle")
            }
        }
    }
}

[test]
def test_vad_empty_clip(t : T?) {
    t |> run("an empty clip: no probability, no speech region, no streaming event, nothing asked of a seat") @(t : T?) {
        with_dasllama_jobque_() {
            var m <- load_vad_model(vad_bin_path())
            let none : array<float>
            let probs <- vad_probs(m, none)
            t |> equal(length(probs), 0, "vad_probs answers no probability")
            let cpu <- vad_probs_cpu(m, none)
            t |> equal(length(cpu), 0, "vad_probs_cpu answers no probability")
            let segs <- find_speech_timestamps(m, none, default_vad_opts())
            t |> equal(length(segs), 0, "find_speech_timestamps answers no region")
            let evs <- stream_events(m, none, 1300)
            t |> equal(length(evs), 0, "the streaming iterator emits no event")
            var it : VadIter
            vad_iter_reset(it, default_vad_opts())
            let was = set_vad_stream_gpu(true)
            var fed = 0
            vad_iter_feed(m, it, none) $(ev) {
                fed++
            }
            set_vad_stream_gpu(was)
            t |> equal(fed, 0, "an empty feed with the stream lever on emits no event")
        }
    }
}

[test]
def test_vad_segments_jfk(t : T?) {
    t |> run("find_speech_timestamps matches silero get_speech_timestamps on jfk.wav") @(t : T?) {
        let wav = asr_clip("jfk.wav")
        let samples <- load_audio_16k_mono(wav)
        if (empty(samples)) {
            t |> skip("jfk.wav not present in the ASR corpus")
            return
        }
        // silero get_speech_timestamps defaults, sample indices (harness/silero_segment_oracle.py)
        let exp_start = [5152l, 52256l, 86048l, 130592l]
        let exp_end   = [36320l, 71136l, 122848l, 169952l]
        with_dasllama_jobque_() {
            var m <- load_vad_model(vad_bin_path())
            let probs <- vad_probs(m, samples)
            // decoder-robust smoke checks (exact per-frame values are gated by the LCG test);
            // the WAV reader here decodes marginally differently from the oracle's soundfile.
            t |> success(probs[0] < 0.05, "jfk frame 0 (silence) low")
            t |> success(probs[11] > 0.5, "jfk frame 11 (speech onset) high")
            let segs <- find_speech_timestamps(m, samples, default_vad_opts())
            t |> success(length(segs) == 4, "4 speech segments")
            for (i in range(min(length(segs), 4))) {
                t |> success(segs[i].start == exp_start[i], "segment {i} start")
                t |> success(segs[i].end == exp_end[i], "segment {i} end")
            }
        }
    }
}

// ===== the GPU seat against the CPU chunk step (the specification) =====

let private VAD_BAR = Bar(abs_floor = 1e-5)   //! the probabilities' absolute bar: the device's dots sum in another order than the CPU's tuned ones

//! the chunks the build's GPU VAD seat has served so far (0 on a build with no driver)
def private vad_device_chunks() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_vad_stats().chunks
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_vad_stats().chunks
        } else {
            return 0l
        }
    }
}

//! the box carries a seat that must serve: a Metal build where the box has a Metal device; a Vulkan build where the tier arms it
//! and the device has a tile family the tower rides
def private vad_device_expected() : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_common_init()
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_arms() && vulkan_tower_tiles_on()
        } else {
            return false
        }
    }
}


def private same_segments(a, b : array<SpeechSegment>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x.start != y.start || x.end != y.end) {
            return false
        }
    }
    return true
}

//! the streaming iterator over `samples` fed `piece` samples a call, every event as (kind, sample) in order
[cold_path]
def private stream_events(m : VadModel; samples : array<float>; piece : int) : array<int64> {
    var it : VadIter
    vad_iter_reset(it, default_vad_opts())
    var evs : array<int64>
    var at = 0
    while (at < length(samples)) {
        let k = min(piece, length(samples) - at)
        var inscope part : array<float>
        part |> resize(k)
        for (i in range(k)) {
            part[i] = samples[at + i]
        }
        vad_iter_feed(m, it, part) $(ev) {
            evs |> push(int64(ev.kind))
            evs |> push(ev.sample)
        }
        at += k
    }
    return <- evs
}

//! the device probabilities against the CPU's over `sig`, the served chunk count asserted, `VAD_BAR`'s control the device run
//! over `sig` with its likeliest speech chunk (by the CPU's probabilities) silenced, outside the bar of the CPU's over `sig`;
//! answers false where no seat served
[cold_path]
def private twin_probs(t : T?; m : VadModel; tag : string; sig : array<float>) : bool {
    let c0 = vad_device_chunks()
    let dev <- vad_probs(m, sig)
    let served = vad_device_chunks() - c0
    let want_chunks = (long_length(sig) + 511l) / 512l
    if (served == 0l && !vad_device_expected()) {
        t |> skip("{tag}: no GPU VAD seat served on this box")
        return false
    }
    t |> equal(served, want_chunks, "{tag}: the device served every chunk in the call")
    let cpu <- vad_probs_cpu(m, sig)
    check(t, "{tag}: the device probabilities against the CPU's", dev, cpu, VAD_BAR, FLAT_SHAPE)
    var c = 0l   //! the chunk the CPU reads as most likely speech: a silent chunk silenced moves nothing
    for (p, i in cpu, count()) {
        c = p > cpu[c] ? int64(i) : c
    }
    var inscope hushed := sig
    for (i in range64(c * 512l, min((c + 1l) * 512l, long_length(sig)))) {
        hushed[i] = 0.0
    }
    let moved <- vad_probs(m, hushed)
    check_exceeds(t, "{tag}: the control - chunk {c} (speech probability {cpu[c]}) silenced on the device side", moved, cpu, VAD_BAR)
    return true
}

[test]
def test_vad_gpu_twin(t : T?) {
    t |> run("the GPU seat's probabilities against the CPU chunk step on the LCG signal, whole and ragged") @(t : T?) {
        with_dasllama_jobque_() {
            with_gpu_knobs() {
                set_serving_gpu_tower(true)
                var m <- load_vad_model(vad_bin_path())
                let sig <- lcg_signal(10 * 512)
                return if (!twin_probs(t, m, "lcg-10", sig))
                let ragged <- lcg_signal(37 * 512 + 100)
                twin_probs(t, m, "lcg-ragged", ragged)
            }
        }
    }
    t |> run("the streaming seat: the device feed's events are the CPU feed's, every whole chunk served") @(t : T?) {
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present in the ASR corpus")
            return
        }
        with_dasllama_jobque_() {
            let knobs = gpu_knobs()
            set_serving_gpu_tower(true)
            var m <- load_vad_model(vad_bin_path())
            let was = set_vad_stream_gpu(false)
            let off0 = vad_device_chunks()
            let cpu <- stream_events(m, samples, 1300)
            t |> equal(vad_device_chunks() - off0, 0l, "the lever off: the stream stepped no chunk on the device")
            set_vad_stream_gpu(true)
            let c0 = vad_device_chunks()
            let dev <- stream_events(m, samples, 1300)
            let served = vad_device_chunks() - c0
            set_vad_stream_gpu(was)
            set_gpu_knobs(knobs)
            if (served == 0l && !vad_device_expected()) {
                t |> skip("no GPU VAD seat served on this box")
                return
            }
            t |> equal(served, long_length(samples) / 512l, "the device stepped every whole chunk of the stream")
            t |> success(length(cpu) >= 4, "the CPU feed opens and closes speech ({length(cpu) / 2} events)")
            t |> success(length(dev) == length(cpu), "as many device events as CPU events")
            var first_off = -1
            for (i in range(min(length(dev), length(cpu)))) {
                if (dev[i] != cpu[i] && first_off < 0) {
                    first_off = i
                }
            }
            t |> success(first_off < 0, "every device event the CPU's (first mismatch at {first_off})")
        }
    }
    t |> run("the batch segmenter on the device: jfk's and ru_long's speech regions are the CPU's, the probabilities within the bar") @(t : T?) {
        with_dasllama_jobque_() {
            let knobs = gpu_knobs()
            set_serving_gpu_tower(true)
            var m <- load_vad_model(vad_bin_path())
            var ran = 0
            for (wav in [asr_clip("jfk.wav"), asr_clip("ru_long.wav")]) {
                var inscope samples <- load_audio_16k_mono(wav)
                if (empty(samples)) {
                    t->log("{wav} absent: no leg")
                    continue
                }
                let tag = base_name(wav)
                continue if (!twin_probs(t, m, tag, samples))
                let c0 = vad_device_chunks()
                var inscope dev <- find_speech_timestamps(m, samples, default_vad_opts())
                t |> success(vad_device_chunks() > c0, "{tag}: the segmenter's probabilities came off the device")
                var inscope cpu_probs <- vad_probs_cpu(m, samples)
                t |> success(!empty(dev), "{tag}: {length(dev)} speech regions")
                var inscope cpu <- speech_timestamps_of(cpu_probs, long_length(samples), default_vad_opts())
                t |> success(same_segments(dev, cpu), "{tag}: every device speech region is the CPU's ({length(dev)} against {length(cpu)})")
                ran++
            }
            set_gpu_knobs(knobs)
            if (ran == 0) {
                t |> skip("neither jfk.wav nor ru_long.wav is present")
            }
        }
    }
}

[test]
def test_vad_metal_declines(t : T?) {
    t |> run("the Metal VAD seat declines on its knob, a model with no slab, a recurrence pipeline under 1024 lanes and a scratch it cannot take") @(t : T?) {
        static_if (!typeinfo builtin_module_exists(das_metal)) {
            t |> skip("not a Metal build")
        } else {
            with_dasllama_jobque_() {
                let knobs = gpu_knobs()
                set_metal_tower(true)
                var m <- load_vad_model(vad_bin_path())
                let sig <- lcg_signal(10 * 512)
                twin_probs(t, m, "the seat serves", sig)
                let tower_was = metal_tower_knob()
                vad_decline_leg(t, m, sig, "knob") $(on) {
                    set_metal_tower(on ? false : tower_was)
                }
                var bare := m
                bare.slab |> clear()
                vad_decline_leg(t, bare, sig, "shape") $(on) {}
                var lanes_was = 0u
                vad_decline_leg(t, m, sig, "device") $(on) {
                    if (on) {
                        lanes_was = set_metal_vad_lanes_probe_(512u)
                    } else {
                        set_metal_vad_lanes_probe_(lanes_was)
                    }
                }
                var take_was = 0ul
                vad_decline_leg(t, m, sig, "memory") $(on) {
                    if (on) {
                        take_was = set_metal_take_max_probe_(512ul)
                    } else {
                        set_metal_take_max_probe_(take_was)
                    }
                }
                twin_probs(t, m, "the seat serves again with every pin put back", sig)
                set_gpu_knobs(knobs)
            }
        }
    }
}


//! one GPU VAD seat decline: `pin` sets the lever and puts it back; the call must decline as `reason`, step no chunk on the
//! device and answer the CPU chunk step's probabilities bit for bit
[cold_path]
def private vad_decline_leg(t : T?; m : VadModel; sig : array<float>; reason : string; pin : block<(on : bool) : void>) {
    let c0 = vad_device_chunks()
    let d0 = gpu_tower_declines_of(reason)
    invoke(pin, true)
    let probs <- vad_probs(m, sig)
    invoke(pin, false)
    t |> equal(vad_device_chunks() - c0, 0l, "{reason}: no chunk stepped on the device")
    t |> equal(gpu_tower_declines_of(reason) - d0, 1l, "{reason}: one decline counted")
    let cpu <- vad_probs_cpu(m, sig)
    check_exact(t, "{reason}: the CPU chunk step answered", probs, cpu)
}

[test]
def test_vad_vulkan_declines(t : T?) {
    t |> run("the Vulkan VAD seat declines on its knob, a model with no slab, a device with neither tile family, a slab past the storage range on a released resident and a scratch past it, then serves on a rebuilt scratch") @(t : T?) {
        static_if (!typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(das_metal)) {
            t |> skip("not a Vulkan build")
        } else {
            if (!vulkan_tower_arms()) {
                t |> skip("no Vulkan device (DASLLAMA_GPU=1 and a device the tier serves)")
                return
            }
            if (!vulkan_tower_tiles_on()) {
                t |> skip("the seat's stamps need cm2 or the KHR tile at subgroup 32 (subgroup {g_gpu.subgroup_size} here) - every call declines device")
                return
            }
            with_dasllama_jobque_() {
                let knobs = gpu_knobs()
                set_vulkan_tower(true)
                var m <- load_vad_model(vad_bin_path())
                let sig <- lcg_signal(10 * 512)
                twin_probs(t, m, "the seat serves", sig)
                let tower_was = vulkan_tower_on()
                vad_decline_leg(t, m, sig, "knob") $(on) {
                    set_vulkan_tower(on ? false : tower_was)
                }
                var bare := m
                bare.slab |> clear()
                vad_decline_leg(t, bare, sig, "shape") $(on) {}
                let km = g_gpu.has_coopmat
                let mode = g_gpu.coopmat_mode
                vad_decline_leg(t, m, sig, "device") $(on) {
                    g_gpu.has_coopmat = on ? false : km
                    g_gpu.coopmat_mode = on ? COOPMAT_MM : mode
                }
                // a released resident: the upload's range decline releases it again, and must not destroy the slab the
                // shutdown already destroyed
                vulkan_tower_shutdown()
                let msr = g_gpu.msr
                vad_decline_leg(t, m, sig, "shape") $(on) {
                    g_gpu.msr = on ? 4096l : msr
                }
                // the slab fits the range and a call's scratch runs past it
                let slab_bytes = long_length(m.slab) * 4l
                let long_sig <- lcg_signal(int(slab_bytes / (512l * 4l) + 2l) * 512)
                vad_decline_leg(t, m, long_sig, "shape") $(on) {
                    g_gpu.msr = on ? slab_bytes : msr
                }
                // the range decline must leave no capacity behind: the next call builds its scratch, never dispatching into
                // the buffers the declined build released
                twin_probs(t, m, "the seat serves on a scratch rebuilt after the range decline", sig)
                set_gpu_knobs(knobs)
            }
        }
    }
}
