//! with (module name) must require unsafe when the
//! with_module_is_unsafe policy is on (31037).
options gen2
options with_module_is_unsafe = true

expect 31037

require ../language/_with_module_helper.das

[export]
def main {
    // 31037: policy demands unsafe around user-written module-with
    with (module _with_module_helper) {
        let x = secret_scale
    }
}
