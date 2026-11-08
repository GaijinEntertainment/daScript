options gen2

module _require_as_fixture shared public

// Same-directory fixture for modules.das: the target of a bare
// same-dir `require _require_as_fixture as fx` (importName was never
// set on this rail).

def public require_as_marker : int => 45
