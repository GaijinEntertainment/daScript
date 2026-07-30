options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_markdown_view public

[test]
def test_markdown_host_image_binding_lifecycle(t : T?) {
    var state = MarkdownViewState()
    let invalid = markdown_view_bind_image(state, MarkdownImageBinding(
        document_revision = 4, node_index = 7, texture_id = 0uL,
        display_size = float2(32.0f, 24.0f)))
    t |> success(!invalid, "zero texture handles are rejected")

    var binding = MarkdownImageBinding(
        document_revision = 4,
        node_index = 7,
        resource_revision = 11,
        texture_id = 123uL,
        display_size = float2(32.0f, 24.0f))
    t |> success(markdown_view_bind_image(state, binding),
                 "first resolved host image changes binding state")
    t |> success(markdown_view_image_bound(state, 4, 7),
                 "binding is keyed by document revision and image node")
    t |> success(!markdown_view_image_bound(state, 5, 7),
                 "another document revision cannot observe a stale texture")
    let first_revision = state.image_bindings_revision

    t |> success(!markdown_view_bind_image(state, binding),
                 "identical resource revision is an idempotent no-op")
    t |> equal(state.image_bindings_revision, first_revision,
               "idempotent binding preserves cached layout revision")

    binding.resource_revision++
    binding.display_size = float2(48.0f, 36.0f)
    t |> success(markdown_view_bind_image(state, binding),
                 "new host resource revision replaces the binding")
    t |> success(state.image_bindings_revision > first_revision,
                 "resource replacement invalidates image layout")

    t |> success(markdown_view_unbind_image(state, 4, 7),
                 "host can explicitly return an image to fallback state")
    t |> success(!markdown_view_image_bound(state, 4, 7),
                 "unbound image no longer resolves")
    t |> success(!markdown_view_unbind_image(state, 4, 7),
                 "repeated unbind is idempotent")
    markdown_view_dispose(state)
}
