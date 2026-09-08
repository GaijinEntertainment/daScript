options gen2
options indenting = 4
// lint-skip-file: a shared module that does not compile, the late require's error arm

module mc_late_broken shared private

def public broken_answer() : int {
    return "not an int"
}
