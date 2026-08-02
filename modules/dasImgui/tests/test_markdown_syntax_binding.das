options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_markdown_view public

[test]
def test_markdown_syntax_binding_lifecycle(t : T?) {
    var state = MarkdownViewState()
    var tokens <- [
        MarkdownSyntaxToken(
            start_byte = 0, end_byte = 3, style = MarkdownSyntaxStyle.keyword),
        MarkdownSyntaxToken(
            start_byte = 4, end_byte = 9, style = MarkdownSyntaxStyle.function_)
    ]

    t |> success(!markdown_view_bind_syntax(state, -1, 7, tokens),
                 "negative document revisions are rejected")
    t |> success(!markdown_view_bind_syntax(state, 4, -1, tokens),
                 "negative node indices are rejected")
    t |> equal(state.syntax_bindings_revision, 0,
               "rejected bindings preserve the layout revision")

    t |> success(markdown_view_bind_syntax(state, 4, 7, tokens),
                 "first token snapshot changes binding state")
    t |> success(markdown_view_syntax_bound(state, 4, 7),
                 "binding is keyed by document revision and code node")
    t |> success(!markdown_view_syntax_bound(state, 5, 7),
                 "another document revision cannot observe stale tokens")
    t |> equal(markdown_view_syntax_token_count(state, 4, 7), 2,
               "the complete token snapshot is retained")
    let first_revision = state.syntax_bindings_revision

    t |> success(!markdown_view_bind_syntax(state, 4, 7, tokens),
                 "repeated bind is an idempotent no-op")
    t |> equal(state.syntax_bindings_revision, first_revision,
               "idempotent binding preserves the layout revision")

    t |> success(markdown_view_bind_syntax(state, 5, 7, tokens),
                 "a new document revision owns an independent snapshot")
    t |> equal(markdown_view_syntax_token_count(state, 5, 7), 2,
               "new revision token count is independently addressable")

    let before_unbind = state.syntax_bindings_revision
    t |> success(markdown_view_unbind_syntax(state, 4, 7),
                 "host can explicitly remove a token snapshot")
    t |> success(state.syntax_bindings_revision > before_unbind,
                 "unbind invalidates syntax layout")
    t |> success(!markdown_view_syntax_bound(state, 4, 7),
                 "unbound syntax no longer resolves")
    t |> success(markdown_view_syntax_bound(state, 5, 7),
                 "unbind preserves snapshots for other revisions")
    t |> success(!markdown_view_unbind_syntax(state, 4, 7),
                 "repeated unbind is idempotent")

    let before_clear = state.syntax_bindings_revision
    markdown_view_clear_syntax(state)
    t |> success(!markdown_view_syntax_bound(state, 5, 7),
                 "clear removes every remaining snapshot")
    t |> success(state.syntax_bindings_revision > before_clear,
                 "clear invalidates syntax layout")
    let after_clear = state.syntax_bindings_revision
    markdown_view_clear_syntax(state)
    t |> equal(state.syntax_bindings_revision, after_clear,
               "clearing an empty binding set is an idempotent no-op")

    delete tokens
    markdown_view_dispose(state)
}
