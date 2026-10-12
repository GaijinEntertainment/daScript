options gen2
require daslib/fio
[export]
def initialize(project_path : string) {
    register_dynamic_module("{project_path}/missing.shared_module", "Module_OptionalAbsent")
}
