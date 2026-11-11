options gen2
options indenting = 4

module mc_late_ignoring shared private

require daslib/strings_boost

def public ignoring_value() : int => length(join(["a", "b"], ","))
