options gen2

require dastest/testing_boost
require daslib/fio

[export]
def main {
    let dir = test_temp_dir()
    fopen(path_join(dir, "probe.txt"), "wb") $(f) {
        fwrite(f, "probe")
    }
    print("TEMP_DIR={dir}\n")
}
