options gen2
include _fixture_lint_skip_included_marked.das
include _fixture_lint_skip_included_plain.das

[export]
def main {
    print("{marked_bump(1)} {plain_bump(2)}\n")
}
