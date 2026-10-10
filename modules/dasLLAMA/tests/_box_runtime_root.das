options gen2
options _dasllama_internal = true

require dasllama/dasllama_fat_start
require daslib/fio

// the per-box runtime file fixture test_fat_start.das spawns: the file this process would read its runtime section from,
// raced and written first where none exists, or nothing where the hook stands down

[export]
def main() {
    let p = dasllama_box_runtime_file()
    print("BOX_FILE=[{p}] exists={stat(p).is_valid}\n")
}
