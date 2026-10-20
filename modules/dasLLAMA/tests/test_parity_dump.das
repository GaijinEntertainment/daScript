options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_moe
require daslib/fio
require strings   // find: the refusals' names
require _model_tier
require _resident_feed
require ../harness/_parity_dump.das

// The parity dump round trip on Qwen3.5-0.8B-MTP: the device arm's forced-feed run to a file, the CPU arm's with the device
// arm's prompt, feed and routing tape replayed off that file, and the two files compared under the regions files' bar - the
// in-process parity the stocked suite runs, through the files a model past the box has to use. A poisoned row reds the compare,
// a reference with another feed is refused. Skips: no Qwen3.5-0.8B-MTP file under the models dir, the GPU tier not armed
// (DASLLAMA_GPU=1 on a device that serves the whole-model driver), not `-jit`.

let private MTP_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"
let private BAR_REL = 0.20   //! the regions files' row bar

//! a file at `name` under the test's temp dir holding the two header words `magic` and `version` and nothing after
def private header_file(name : string; magic, version : int64) : string {
    let path = path_join(test_temp_dir(), name)
    fopen(path, "wb") $(f) {
        fwrite(f, magic)
        fwrite(f, version)
    }
    return path
}

[test]
def test_parity_dump_refuses_foreign_files(t0 : T?) {
    t0 |> run("a file with another magic word is no parity dump, one with another version word is another version's, and this build's header words are read: each refused by name, model-free") @(t : T?) {
        let foreign = header_file("foreign.pd", 1313821514l, PARITY_DUMP_VERSION)   // "JSON" as a little-endian word
        let old = header_file("old.pd", PARITY_DUMP_MAGIC, PARITY_DUMP_VERSION + 1l)
        let ours = header_file("ours.pd", PARITY_DUMP_MAGIC, PARITY_DUMP_VERSION)
        let why_foreign = parity_dump_refusal(foreign)
        let why_old = parity_dump_refusal(old)
        t |> success(find(why_foreign, "is not a parity dump") >= 0, "another magic word is refused by name ({why_foreign})")
        t |> success(find(why_old, "is another version's") >= 0, "another version word is refused by name ({why_old})")
        t |> equal(parity_dump_refusal(ours), "", "this build's header words are read")
        t |> success(find(parity_dump_refusal(path_join(test_temp_dir(), "missing.pd")), "cannot read") >= 0, "a missing file is refused by name")
    }
}

[test]
def test_parity_dump_round_trip(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the device arm dumped, the CPU arm dumped on the device arm's tape, the files within the bar; a poisoned row reds, another feed is refused") @(t : T?) {
        let path = path_join(models_dir(), MTP_FILE)
        if (!resident_ready(t, path)) return
        let dev_path = path_join(test_temp_dir(), "parity_dev.pd")
        let cpu_path = path_join(test_temp_dir(), "parity_cpu.pd")
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the carrier")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(40l)
            let none : array<int64>
            var dev <- parity_dump_run(tr, MTP_FILE, mirror_kv_dtype(), prompt, none, 6l, none)
            t |> equal(dev.arm, "device", "the first arm is the device's")
            t |> equal(length(dev.rows), 7, "the prefill's row and six fed steps")
            t |> equal(length(dev.feed), 6, "six fed tokens, the run's own greedy continuation")
            t |> equal(int(moe_pick_tape_mode().mode), int(MoePickTape.off), "the tape is off after the run")
            parity_dump_write(dev_path, dev)
            var back <- parity_dump_read(dev_path)
            t |> success(back.arm == dev.arm && back.vocab == dev.vocab && back.picks_per_select == dev.picks_per_select && ids_equal(back.prompt, dev.prompt) && ids_equal(back.feed, dev.feed) && ids_equal(back.tape, dev.tape), "the dump reads back as written")
            t |> equal(logits_maxdiff(back.rows[3], dev.rows[3]), 0.0, "... its rows bit for bit")
            moe_gpu_drop_model()
            var cpu <- parity_dump_run(tr, MTP_FILE, mirror_kv_dtype(), back.prompt, back.feed, 0l, back.tape)
            t |> equal(cpu.arm, "cpu", "the second arm is the CPU chain's")
            // the carrier is dense: its tape is empty and the replay sees no select; a routed carrier's replay counts every select
            t |> success(dev.picks_per_select == 0l ? cpu.tape_seen == 0l && empty(back.tape) : cpu.tape_seen > 0l, "the CPU arm replayed the device arm's routing ({cpu.tape_seen} selects, {cpu.tape_miss} own picks off the tape, {dev.picks_per_select} picks a select)")
            parity_dump_write(cpu_path, cpu)
            var cpu_back <- parity_dump_read(cpu_path)
            var r <- parity_dump_compare(back, cpu_back, BAR_REL)
            for (row in r.rows) {
                to_log(LOG_INFO, "parity dump row {row.row}: maxdiff {row.diff} (bar {row.bar}) one-step-off {row.ctrl}, argmax device {row.argmax_a} '{decode_(tr, [row.argmax_a])}' against cpu {row.argmax_b} '{decode_(tr, [row.argmax_b])}'\n")
            }
            t |> success(r.ok && empty(r.why), "the device dump is within the bar of the CPU dump on every row, every one-step-off control past it ({r.why})")
            back.rows[2][17] = back.rows[2][17] + 3.0 * r.rows[2].bar
            var poisoned <- parity_dump_compare(back, cpu_back, BAR_REL)
            t |> success(!poisoned.ok && !poisoned.rows[2].ok && poisoned.rows[3].ok, "a poisoned row (one logit three bars off) reds its compare alone")
            cpu_back.feed[0] = cpu_back.feed[0] + 1l
            var refused <- parity_dump_compare(back, cpu_back, BAR_REL)
            t |> success(!refused.ok && !empty(refused.why), "a reference with another feed is refused ({refused.why})")
            delete refused
            delete poisoned
            delete r
            delete cpu_back
            delete cpu
            delete back
            delete dev
            delete prompt
        }
        delete tr
    }
}
