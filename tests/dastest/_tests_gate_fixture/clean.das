options gen2

require daslib/fio
require dastest/testing_boost

def copied_into_temp(src : string) : string {
    let dst = path_join(test_temp_dir(), base_name(src))
    var err : string
    copy_file(src, dst, true, err)
    return dst
}

def clean() {
    fopen("{get_das_root()}/tests/x.das", "rb") $(f) {
        fread(f)
    }
    fwrite(path_join(test_temp_dir(), "ok.tmp"), "a")
    var tab <- { "a" => 1 }
    tab |> erase("a")
    remove(copied_into_temp("{get_das_root()}/tests/z.tmp"))
    let p = copied_into_temp("{get_das_root()}/tests/w.tmp")
    remove(p)
    print("fopen(")
}
