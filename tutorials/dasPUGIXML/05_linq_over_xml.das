// Tutorial PUGIXML-05: LINQ over XML
//
// This tutorial covers:
//   - from_xml_node(root, type<Row>) — a typed, lazy iterator over child elements
//   - Mapping struct fields to same-named XML attributes (the v1 convention)
//   - Missing attributes falling back to the field's declared default
//   - Driving comprehensions and linq_boost queries straight off the iterator
//   - Why materialized rows stay valid past the document's RAII block
//   - The tag-filtered overload from_xml_node(root, name, type<Row>)
//
// requires: daslib/PUGIXML_boost (re-exports pugixml), daslib/linq_boost
// Run: daslang.exe tutorials/dasPUGIXML/05_linq_over_xml.das

options gen2
options persistent_heap

require pugixml/PUGIXML_boost
require daslib/linq_boost
require daslib/linq_fold

// A row type. Each field maps to a same-named XML *attribute* on the child
// element. `year` carries a default — rows that omit the attribute keep it.
struct Car {
    id : int
    make : string
    price : float
    year : int = 2000
    in_stock : bool
}

// Attribute-style data: one <car .../> child per row. (from_xml_node v1 reads
// attributes; child-element / text mapping is a planned growth path.)
let CATALOG = ("<cars>" +
    "<car id=\"1\" make=\"Audi\"   price=\"45000.0\" year=\"2021\" in_stock=\"true\"/>" +
    "<car id=\"2\" make=\"Toyota\" price=\"28000.0\" year=\"2020\" in_stock=\"true\"/>" +
    "<car id=\"3\" make=\"Ford\"   price=\"31000.0\" in_stock=\"false\"/>" +   // no year → default 2000
    "<car id=\"4\" make=\"Kia\"    price=\"22000.0\" year=\"2022\" in_stock=\"true\"/>" +
    "</cars>")

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Basics: typed rows straight from attributes
// ──────────────────────────────────────────────────────────────────────────
//
// from_xml_node walks each child of `root` and fills a Car from its attributes.
// `car` is a fully materialized value — no XML node escapes into the loop body.

def basics() {
    print("=== Section 1: from_xml_node basics ===\n")
    parse_xml(CATALOG) $(doc, ok) {
        if (!ok) {
            print("parse failed\n")
            return
        }
        let root = doc.document_element
        for (car in from_xml_node(root, type<Car>)) {
            print("  #{car.id} {car.make} ${car.price} ({car.year}) in_stock={car.in_stock}\n")
        }
    }
    // Note row #3 (Ford) prints year=2000 — the attribute was absent, so the
    // struct's declared default stands in.
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — LINQ comprehensions over the iterator
// ──────────────────────────────────────────────────────────────────────────
//
// The iterator is just an iterable, so comprehensions filter + project directly.

def comprehensions() {
    print("=== Section 2: comprehension filter + project ===\n")
    parse_xml(CATALOG) $(doc, ok) {
        if (!ok) return
        let root = doc.document_element
        // makes of in-stock cars under $40k, cheapest data first
        let affordable <- [for (car in from_xml_node(root, type<Car>));
            car.make;
            where car.in_stock && car.price < 40000.0]
        print("  affordable in-stock: {affordable}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — linq_boost aggregates
// ──────────────────────────────────────────────────────────────────────────
//
// linq_boost operators compose with from_xml_node the same way they do with any
// source — here, the average price across the catalog.

def aggregates() {
    print("=== Section 3: linq_boost aggregate ===\n")
    parse_xml(CATALOG) $(doc, ok) {
        if (!ok) return
        let root = doc.document_element
        let prices <- [for (car in from_xml_node(root, type<Car>)); car.price]
        var total = 0.0
        for (p in prices) {
            total += p
        }
        print("  {length(prices)} cars, average price ${total / float(length(prices))}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3.5 — Reverse iteration
// ──────────────────────────────────────────────────────────────────────────
//
// each_child_reverse walks children in reverse document order via last_child /
// previous_sibling (both O(1) in pugixml). The fused _fold lane uses the same
// backward walk for reverse|>take(N) and a bare last(): it visits only the kept
// tail (the last N elements), not the whole child list.

def reverse_iteration() {
    print("=== Section 3.5: reverse iteration ===\n")
    parse_xml(CATALOG) $(doc, ok) {
        if (!ok) return
        let root = doc.document_element
        // Raw reverse walk over <car> children — last car first.
        print("  ids backward:")
        for (ch in each_child_reverse(root, "car")) {
            print(" {ch["id"] as int}")
        }
        print("\n")
        // Fused _fold reverse: the last two cars, reversed, as typed rows — the
        // macro walks backward and stops after 2, never touching cars #1/#2.
        unsafe {
            let last_two <- _fold(from_xml_node(doc.document_element, type<Car>).reverse().take(2).to_array())
            print("  last 2 (reversed): {[for (c in last_two); c.make]}\n")
            // reverse |> take(1) is the last car in a single backward step.
            let newest <- _fold(from_xml_node(doc.document_element, type<Car>).reverse().take(1).to_array())
            print("  last car: #{newest[0].id} {newest[0].make}\n")
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Tag-filtered overload
// ──────────────────────────────────────────────────────────────────────────
//
// from_xml_node(root, "tag", type<Row>) walks only children with that tag —
// handy when a document interleaves several element kinds under one parent.

def tag_filter() {
    print("=== Section 4: tag-filtered walk ===\n")
    let mixed = ("<fleet>" +
        "<car id=\"10\" make=\"Mazda\" price=\"26000.0\"/>" +
        "<note>service due</note>" +
        "<car id=\"11\" make=\"Honda\" price=\"24000.0\"/>" +
        "</fleet>")
    parse_xml(mixed) $(doc, ok) {
        if (!ok) return
        let root = doc.document_element
        let ids <- [for (car in from_xml_node(root, "car", type<Car>)); car.id]
        print("  car ids (note skipped): {ids}\n")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — Lifetime: results outlive the document
// ──────────────────────────────────────────────────────────────────────────
//
// Rows are owned values (strings cloned out of the document), so collecting them
// and using them AFTER the RAII block closes is safe. A for-loop is the safe way
// to consume the iterator (to_array would need an `unsafe` block — the
// [unsafe_outside_of_for] rule — since it consumes the iterator outside a for).

def lifetime() {
    print("=== Section 5: results outlive the document ===\n")
    var inventory : array<Car>
    parse_xml(CATALOG) $(doc, ok) {
        if (!ok) return
        let root = doc.document_element
        for (car in from_xml_node(root, type<Car>)) {
            inventory |> push(car)
        }
    }
    // doc is freed here — inventory and its strings remain valid
    print("  captured {length(inventory)} cars; first make = {inventory[0].make}\n")
}

[export]
def main() {
    basics()
    comprehensions()
    aggregates()
    reverse_iteration()
    tag_filter()
    lifetime()
}
