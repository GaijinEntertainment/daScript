options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/fio   // the temp dump directory the read test writes
require ../harness/vk_spv_diff.das

// The SPIR-V dump differ (harness/vk_spv_diff.das) - the identity gate every kernel fold's
// "byte-identical" claim rests on: its four bins over two in-memory dumps, and the verdict's refusal
// of a side with nothing in it.

def private dump_of(stamps : table<string; array<uint8>>) : table<string; array<uint8>> {
    return <- clone_to_move(stamps)
}

[test]
def test_vk_spv_diff_bins(t : T?) {
    t |> run("the differ bins every stamp: identical, moved, appeared, vanished - sorted by name") @(t : T?) {
        var base <- dump_of({ "b_cls_spv" => [uint8(1), uint8(2), uint8(3)], "a_cls_spv" => [uint8(9)], "gone_cls_spv" => [uint8(4), uint8(4)], "len_cls_spv" => [uint8(7), uint8(7)] })
        var fresh <- dump_of({ "b_cls_spv" => [uint8(1), uint8(2), uint8(3)], "a_cls_spv" => [uint8(8)], "new_cls_spv" => [uint8(5)], "len_cls_spv" => [uint8(7), uint8(7), uint8(7)] })
        let d <- diff_dumps(base, fresh)
        t |> equal(d.same, ["b_cls_spv"], "the byte-identical stamp is the one whose every byte matches")
        t |> equal(d.moved, ["a_cls_spv", "len_cls_spv"], "a changed byte and a changed length both read as moved, in name order")
        t |> equal(d.appeared, ["new_cls_spv"], "a stamp only the fresh dump holds appeared")
        t |> equal(d.vanished, ["gone_cls_spv"], "a stamp only the base dump holds vanished")
        t |> success(!dumps_identical(d, length(base), length(fresh)), "the verdict fails on any moved, appeared or vanished stamp")
    }
    t |> run("the verdict passes only two non-empty dumps whose every stamp matches") @(t : T?) {
        var base <- dump_of({ "a_cls_spv" => [uint8(1)], "b_cls_spv" => [uint8(2), uint8(3)] })
        var fresh <- dump_of({ "b_cls_spv" => [uint8(2), uint8(3)], "a_cls_spv" => [uint8(1)] })
        let d <- diff_dumps(base, fresh)
        t |> equal(d.same, ["a_cls_spv", "b_cls_spv"], "both stamps match")
        t |> success(dumps_identical(d, length(base), length(fresh)), "two matching dumps pass")
        var none : table<string; array<uint8>>
        let e <- diff_dumps(none, fresh)
        t |> equal(e.appeared, ["a_cls_spv", "b_cls_spv"], "against an empty base every fresh stamp appeared")
        let both <- diff_dumps(none, none)
        t |> success(empty(both.same) && empty(both.moved) && empty(both.appeared) && empty(both.vanished), "two empty dumps bin nothing")
        t |> success(!dumps_identical(both, 0, 0), "two empty dumps do not pass - nothing was compared")
    }
}

[test]
def test_vk_spv_diff_reads_a_directory(t : T?) {
    //! read_dump takes the .spv files of a directory by stem and ignores the rest
    let dir_r = create_temp_directory_result("vk_spv_diff")
    t |> success(dir_r is value, "temp dump dir created")
    if (!(dir_r is value)) return
    let dirp = unsafe(dir_r.value)
    t |> success(fwrite(path_join(dirp, "one_cls_spv.spv"), "abc"), "a stamp written")
    t |> success(fwrite(path_join(dirp, "notes.txt"), "x"), "a non-stamp written")
    var d <- read_dump(dirp)
    t |> equal(length(d), 1, "one stamp read")
    t |> success(key_exists(d, "one_cls_spv"), "the stamp is keyed by its stem")
    t |> equal(length(d["one_cls_spv"]), 3, "the stamp's bytes are the file's")
    t |> success(rmdir_rec_result(dirp) is value, "temp dump dir removed")
}
