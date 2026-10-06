options gen2
options remove_unused_symbols = false

require UnitTest

let OTHER_UNIT_TEST_CONSTANT = UNIT_TEST_CONSTANT + 1;

def start_effect(name; tm) => start_effect(name, tm, 1.0)
