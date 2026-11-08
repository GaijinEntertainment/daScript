options gen2

module _optional_require_fixture_pctguard shared public

// Same-directory fixture for optional_require.das: the target of a
// %/-rooted PATH-guarded require (`require
// ?%/tests/language/_optional_require_fixture.das …`) — loads because
// the guard's own file resolves. `shared` so `typeinfo
// builtin_module_exists` sees it.

def public pctguard_marker : int {
    return 44

}
