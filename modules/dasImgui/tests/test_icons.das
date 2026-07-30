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
