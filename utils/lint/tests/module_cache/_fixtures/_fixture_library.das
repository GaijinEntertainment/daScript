options gen2

require hidden_mod
require dasbind

[extern(cdecl, name="missing_symbol", library="lint_project_missing_library")]
def native_value() : int => 0

[export]
def main() : int => hidden_value() + native_value()
