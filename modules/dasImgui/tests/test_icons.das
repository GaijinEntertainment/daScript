options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_icons public

//! Catalog integrity for imgui_icons. The single ``CATALOG`` list drives
//! draw_icon dispatch, the doc generator, and make_icon_doc, so a botched edit
//! (typo'd name, duplicate, missing category) must fail here. Glyph rendering
//! is verified visually by utils/make_icon_doc.das output.

[test]
def test_icon_text_markup_set(t : T?) {
    // the markdown toolbar glyphs (play round 3) - a rename here breaks the
    // viewer's MD_TOOLBAR_ICONS mapping silently (draw_icon no-ops on misses)
    var names : table<string>
    for (i in range(icon_count())) {
        names |> insert(icon_name(i))
    }
    var missing = 0
    for (nm in ["heading", "bold", "italic", "code-inline", "image",
                "list-bullets", "list-ordered", "checkbox", "quote",
                "code-block", "table"]) {
        if (!key_exists(names, nm)) {
            missing++
        }
    }
    t |> equal(missing, 0, "every text-markup glyph is registered")
}

[test]
def test_icon_catalog(t : T?) {
    t |> success(icon_count() > 0, "icon set is non-empty")
    var seen : table<string>
    var bad = 0
    for (i in range(icon_count())) {
        let nm = icon_name(i)
        if (empty(nm) || empty(icon_category(i)) || key_exists(seen, nm)) {
            bad++
        }
        seen |> insert(nm)
    }
    t |> equal(bad, 0, "every icon has a unique, non-empty name and a category")
}
