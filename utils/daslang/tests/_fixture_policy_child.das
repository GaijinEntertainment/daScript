options gen2

//! a child for test_policy.das: prints whether its compile ran under
//! force_inscope_pod
[export]
def main() {
    print(typeinfo needs_container_finalize(type<array<int>>) ? "pod on\n" : "pod off\n")
}
