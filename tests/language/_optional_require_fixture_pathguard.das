options gen2

module _optional_require_fixture_pathguard shared public

// Same-directory fixture for optional_require.das: the target of a
// PATH-guarded require (`require ?daslib/fio
// _optional_require_fixture_pathguard`) — loads because the guard's
// own file resolves. `shared` so `typeinfo builtin_module_exists`
// sees it.

def public pathguard_marker : int {
    return 43

}
