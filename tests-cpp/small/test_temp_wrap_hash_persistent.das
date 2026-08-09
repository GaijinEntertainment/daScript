options gen2
options persistent_heap
require test_temp_wrap_fixture // nolint:STYLE030 — the require IS the test: the fixture must compile in this program for the hash probe

[export]
def main() {
    print("ok\n")
}
