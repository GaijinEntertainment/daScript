options gen2
module _optional_require_fixture_private shared public

// private on purpose: the root sees this module, not that one
require _optional_require_fixture_hidden

def private_marker() : int => hidden_marker() + 1
