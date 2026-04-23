// main.das — example of file-path requires.
//
// Run with the default file access (no .das_project necessary):
//   bin/Release/daslang.exe examples/file_path_require/main.das
//
// Demonstrates:
//   ./shared_helpers.das — relative path to a sibling helper
//   %/daslib/random.das  — dasroot-rooted path (resolves to the daslib module)

options gen2

require ./shared_helpers.das
require %/daslib/random.das as rng

[export]
def main {
    print("{shared_helpers::shared_greeting()}\n")
    var seed = int4(1, 2, 3, 4)
    print("random_int4 = {rng::random_int4(seed)}\n")
}
