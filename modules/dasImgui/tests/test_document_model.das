options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_document_model public

[test]
def test_document_source_roundtrip_preserves_bom_and_line_endings(t : T?) {
    let bom = "\xEF\xBB\xBF"
    let bytes = bom + "first\r\nsecond\nthird\r\n"
    var document = TextDocumentSource()
    t |> equal(text_document_install_bytes(document, bytes, "mixed.md", 7),
               TextDocumentStatus.ok, "valid UTF-8 bytes install")
    t |> success(document.has_utf8_bom, "BOM is retained as source metadata")
    t |> equal(document.text, "first\r\nsecond\nthird\r\n",
               "parser-facing source excludes only the BOM")
    t |> equal(text_document_bytes(document), bytes,
               "no-op load/save roundtrip is byte exact")
    t |> success(!text_document_dirty(document), "freshly loaded source is clean")
}

[test]
def test_document_edit_is_revision_checked_and_exactly_invertible(t : T?) {
    var document = TextDocumentSource()
    let _ = text_document_install_text(document, "- [X] done\r\n", "task.md", 3)
    let edit = TextEdit(
        source = TextSourceRange(start_byte = 3, end_byte = 4),
        replacement = " ", expected = "X", verify_expected = true,
        base_revision = 3)
    let changed <- text_document_apply_edit(document, edit)
    t |> success(changed.applied, "matching edit applies")
    t |> equal(document.text, "- [ ] done\r\n",
               "only the checkbox marker byte changes")
    t |> success(text_document_dirty(document), "applied edit makes source dirty")

    let stale <- text_document_apply_edit(document, edit)
    t |> equal(stale.status, TextDocumentStatus.stale_revision,
               "an obsolete UI event cannot edit a newer document")
    t |> equal(document.text, "- [ ] done\r\n",
               "rejected stale edit leaves bytes untouched")

    let undone <- text_document_apply_edit(document, changed.inverse)
    t |> success(undone.applied, "recorded inverse applies")
    t |> equal(document.text, "- [X] done\r\n",
               "inverse restores original spelling and line endings exactly")
}

[test]
def test_document_edit_maps_source_anchors(t : T?) {
    let edit = TextEdit(
        source = TextSourceRange(start_byte = 4, end_byte = 7),
        replacement = "replacement")
    t |> equal(text_position_after_edit(2, edit), 2,
               "position before edit is unchanged")
    t |> equal(text_position_after_edit(10, edit), 18,
               "position after edit follows its byte delta")
    let mapped = text_range_after_edit(
        TextSourceRange(start_byte = 0, end_byte = 10), edit)
    t |> equal(mapped.start_byte, 0, "range start before edit stays anchored")
    t |> equal(mapped.end_byte, 18, "range end after edit follows byte delta")
}
