options gen2

module rules shared private

require rule_private_docs public


def public apply_all_rules(src : string) : string {
    return trim_private_docstrings(src)
}
