options gen2

require dastest/testing_boost public
require daslib/linq_das
require pugixml/PUGIXML_boost

// linq_das over an XML source: `from c : Car in <node>` → `from_in(node, type<Car>)` →
// `unsafe(from_xml_node(node, type<Car>))`, which `_fold` fuses via the XmlAdapter. The node is passed
// inline (a `let`-bound node would be `const&` and rejected — see PUGIXML linq_fold tests).

struct Car {
    id    : int
    price : float
    brand : string
}

let XML = ("<cars>" +
    "<car id=\"1\" price=\"100.0\" brand=\"Ford\"/>noise" +
    "<car id=\"2\" price=\"300.0\" brand=\"Honda\"/>" +
    "<car id=\"3\" price=\"50.0\" brand=\"Ford\"/>" +
    "</cars>")

[test]
def test_xml_where_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let brands <- %linq! from c : Car in doc.document_element where c.price > 99.0 select c.brand %%
        t |> equal(length(brands), 2, "xml where/select")
        t |> equal(brands[0], "Ford")
        t |> equal(brands[1], "Honda")
    }
}

[test]
def test_xml_identity_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let rows <- %linq! from c : Car in doc.document_element where c.price >= 100.0 select c %%
        t |> equal(length(rows), 2, "xml identity select returns rows")
        t |> equal(rows[0].id, 1)
        t |> equal(rows[1].id, 2)
    }
}

[test]
def test_xml_iterator_output(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let brands <- [for (b in %linq! from c : Car in doc.document_element where c.price > 99.0 select c.brand iterator %%); b]
        t |> equal(length(brands), 2, "xml iterator output")
    }
}

[test]
def test_xml_orderby(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // ORDER BY price ASC → 50, 100, 300 → ids 3, 1, 2
        let ids <- %linq! from c : Car in doc.document_element orderby c.price select c.id %%
        t |> equal(length(ids), 3)
        t |> equal(ids[0], 3)
        t |> equal(ids[1], 1)
        t |> equal(ids[2], 2)
    }
}

[test]
def test_xml_orderby_descending(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let ids <- %linq! from c : Car in doc.document_element orderby c.price descending select c.id %%
        t |> equal(ids[0], 2)   // 300
        t |> equal(ids[2], 3)   // 50
    }
}

[test]
def test_xml_orderby_multi_key(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // brand asc, price desc → Ford(id1@100, id3@50) then Honda(id2@300) → ids 1, 3, 2
        let ids <- %linq! from c : Car in doc.document_element orderby c.brand, c.price descending select c.id %%
        t |> equal(length(ids), 3)
        t |> equal(ids[0], 1)   // Ford, 100
        t |> equal(ids[1], 3)   // Ford, 50
        t |> equal(ids[2], 2)   // Honda, 300
    }
}

[test]
def test_xml_group_by(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // Ford: car1+car3, Honda: car2 → 2 buckets
        let groups <- %linq! from c : Car in doc.document_element group c by c.brand %%
        t |> equal(length(groups), 2, "xml group buckets Ford/Honda")
        for (g in groups) {
            if (g._0 == "Ford")  { t |> equal(length(g._1), 2) }
            if (g._0 == "Honda") { t |> equal(length(g._1), 1) }
        }
    }
}

// ===== join: XML left source ⋈ in-memory array right source =====

struct BrandHQ {
    brand   : string
    country : string
}

def private mk_brands() : array<BrandHQ> {
    return <- [BrandHQ(brand = "Ford",  country = "USA"),
               BrandHQ(brand = "Honda", country = "Japan")]
}

[test]
def test_xml_join_select_terminal(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let rows <- %linq! from c : Car in doc.document_element join b in brands on c.brand equals b.brand select (Id = c.id, Country = b.country) %%
        t |> equal(length(rows), 3, "all 3 cars match a brand HQ")
        var usa = 0
        for (r in rows) {
            if (r.Country == "USA") { usa++ }
        }
        t |> equal(usa, 2, "two Fords → USA")
    }
}

[test]
def test_xml_join_pre_join_where(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // pre-join where filters the XML rows (price > 99 → car1 Ford, car2 Honda) before the join
        let rows <- %linq! from c : Car in doc.document_element where c.price > 99.0 join b in brands on c.brand equals b.brand select (Id = c.id, Country = b.country) %%
        t |> equal(length(rows), 2, "pre-join where prunes the DOM walk")
    }
}

[test]
def test_xml_join_orderby(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // post-join orderby → transparent-id carry; sort the joined pairs by price
        let ids <- %linq! from c : Car in doc.document_element join b in brands on c.brand equals b.brand orderby c.price select c.id %%
        t |> equal(length(ids), 3)
        t |> equal(ids[0], 3, "car3 price 50 first")
        t |> equal(ids[2], 2, "car2 price 300 last")
    }
}

[test]
def test_xml_join_group_after(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let groups <- %linq! from c : Car in doc.document_element join b in brands on c.brand equals b.brand group c by b.country %%
        t |> equal(length(groups), 2, "USA + Japan buckets")
        for (g in groups) {
            if (g._0 == "USA")   { t |> equal(length(g._1), 2, "two Fords") }
            if (g._0 == "Japan") { t |> equal(length(g._1), 1, "one Honda") }
        }
    }
}

// ===== multiple `from`: XML left source × in-memory array right source (uncorrelated cross product).
// The XML source is the unfused `_cross_join` chain head — `from_xml_node` is passed as a raw argument
// (not a fused for-loop source), so it relies on the unsafe wrap surviving from_in's `$c` expansion. =====

[test]
def test_xml_multifrom_tuple(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let rows <- %linq! from c : Car in doc.document_element from b in brands select (Id = c.id, Country = b.country) %%
        t |> equal(length(rows), 6, "3 XML cars × 2 brands cartesian product")
    }
}

[test]
def test_xml_multifrom_pre_where(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // pre-from where prunes the DOM walk (price > 99 → car1, car2) before the cross
        let rows <- %linq! from c : Car in doc.document_element where c.price > 99.0 from b in brands select (Id = c.id, Country = b.country) %%
        t |> equal(length(rows), 4, "2 filtered XML cars × 2 brands")
    }
}

// ===== let bindings over an XML source (inlined computed expression over the row attributes) =====

[test]
def test_xml_let(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // half = price/2 → 50, 150, 25 → orderby asc → ids 3, 1, 2; the `let` is inlined into both
        // the order key and the projection.
        let rows <- %linq! from c : Car in doc.document_element let half = c.price / 2.0
            orderby half select (Id = c.id, Half = half) %%
        t |> equal(length(rows), 3)
        t |> equal(rows[0].Id, 3)         // 25
        t |> equal(rows[0].Half, 25.0)
        t |> equal(rows[2].Id, 2)         // 150
        t |> equal(rows[2].Half, 150.0)
    }
}
