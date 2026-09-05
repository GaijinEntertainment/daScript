options gen2

// child for cross_target_folds.das: prints the compile-time target folds, and which static_if arm
// the cross-target test the tune framework's inert gate uses selects, so the parent can compare a
// native compile against --jit-target=<triple>

[export]
def main {
    print("TRIPLE=[{get_target_triple()}]\n")
    print("TARGET_ARCH=[{get_target_architecture_name()}]\n")
    print("HOST_ARCH=[{get_architecture_name()}]\n")
    print("CROSS_PLATFORM=[{get_cross_platform_name()}]\n")
    static_if (!empty(get_target_triple())) {
        print("CROSS=[yes]\n")
    } else {
        print("CROSS=[no]\n")
    }
}
