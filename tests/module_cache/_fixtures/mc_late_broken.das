options gen2
options indenting = 4

module mc_late_broken shared private

def public broken_answer() : int {
    return "not an int"
}
