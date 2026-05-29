options gen2
require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require dastest/testing_boost public

// XML LINQ fold (pass 2a): `_fold` over a `from_xml_node(...)` chain emits an inlined DOM child-walk
// via XmlAdapter. Each lane below must match the unfused reference (a plain `for` over from_xml_node).
// The source is `[unsafe_outside_of_for]`, so a fold chain wraps it in `unsafe(...)`; the node is passed
// by value (`var root`) because `_fold`'s macro-argument inference does not insert the const&->value copy.

struct XCar {
    id    : int
    price : float
    qty   : int
}

// 4 car elements (prices 10/20/30/40, qty 2/4/6/8) with interleaved PCDATA noise the element walk skips.
let TEST_XML = ("<cars>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\"/>noise" +
    "<car id=\"2\" price=\"20.0\" qty=\"4\"/>more" +
    "<car id=\"3\" price=\"30.0\" qty=\"6\"/>" +
    "<car id=\"4\" price=\"40.0\" qty=\"8\"/>" +
    "</cars>")

// cars + a non-car element, to exercise the named (tag-filtered) overload.
let NAMED_XML = ("<root>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\"/>" +
    "<other id=\"99\" price=\"99.0\" qty=\"0\"/>" +
    "<car id=\"2\" price=\"20.0\" qty=\"4\"/>" +
    "</root>")

[test]
def test_xml_fold_count(t : T?) {
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        // count where price > 15 -> cars 2,3,4
        let c = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price > 15.0).count())
        t |> equal(c, 3)
        // bare count (no predicate) -> all 4 elements
        let total = _fold(unsafe(from_xml_node(root, type<XCar>)).count())
        t |> equal(total, 4)
    }
}

[test]
def test_xml_fold_select_where_sum(t : T?) {
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        // project price, filter > 15, sum -> 20 + 30 + 40
        let s = _fold(unsafe(from_xml_node(root, type<XCar>))._select(_.price)._where(_ > 15.0).sum())
        t |> equal(s, 90.0)
    }
}

[test]
def test_xml_fold_aggregates(t : T?) {
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        let mn = _fold(unsafe(from_xml_node(root, type<XCar>))._select(_.price).min())
        let mx = _fold(unsafe(from_xml_node(root, type<XCar>))._select(_.price).max())
        let av = _fold(unsafe(from_xml_node(root, type<XCar>))._select(_.price).average())
        t |> equal(mn, 10.0)
        t |> equal(mx, 40.0)
        t |> equal(av, 25.0lf)
    }
}

[test]
def test_xml_fold_early_exit(t : T?) {
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        // first matching element
        let row = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.id == 3).first())
        t |> equal(row.id, 3)
        t |> equal(row.price, 30.0)
        // any
        t |> success(_fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price > 35.0).any()))
        t |> success(!_fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price > 100.0).any()))
    }
}

[test]
def test_xml_fold_take(t : T?) {
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        // cap at 2 of the 3 matches
        let c = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price > 15.0).take(2).count())
        t |> equal(c, 2)
    }
}

[test]
def test_xml_fold_named_overload(t : T?) {
    parse_xml(NAMED_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        // named overload skips <other>; only the 2 <car> elements count
        let c = _fold(unsafe(from_xml_node(root, "car", type<XCar>)).count())
        t |> equal(c, 2)
        let s = _fold(unsafe(from_xml_node(root, "car", type<XCar>))._select(_.price).sum())
        t |> equal(s, 30.0)
    }
}

[test]
def test_xml_fold_matches_unfused_reference(t : T?) {
    // The fold must be observationally equal to the plain (unfused) generator walk.
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        var refCount = 0
        var refSum = 0.0
        for (car in from_xml_node(root, type<XCar>)) {
            if (car.qty > 3) {
                refCount ++
                refSum += car.price
            }
        }
        let foldCount = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.qty > 3).count())
        let foldSum = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.qty > 3)._select(_.price).sum())
        t |> equal(foldCount, refCount)
        t |> equal(foldSum, refSum)
        t |> equal(foldCount, 3)
        t |> equal(foldSum, 90.0)
    }
}

// ===== Field-pruning (pass 2b) =====
// A struct with a string field (whose clone is pruned away when unread) and a non-zero declared
// default (qty), to exercise the pruned reads, default preservation, and whole-row escape.
struct XCarN {
    id    : int
    price : float
    qty   : int = 1
    name  : string
}

// car 2 has no `qty` attribute -> reads must fall back to the declared default (1). All rows carry a
// `name` attr so a whole-row escape has a string to clone (and a pruned chain has one to skip).
let PRUNE_XML = ("<cars>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\" name=\"alpha\"/>" +
    "<car id=\"2\" price=\"20.0\" name=\"beta\"/>" +
    "<car id=\"3\" price=\"30.0\" qty=\"6\" name=\"gamma\"/>" +
    "</cars>")

[test]
def test_xml_fold_prunes_unread_string_field(t : T?) {
    // Chain reads only `price` — the `name` string clone is never emitted. Result must still be exact.
    parse_xml(PRUNE_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        let s = _fold(unsafe(from_xml_node(root, type<XCarN>))._where(_.price > 15.0)._select(_.price).sum())
        t |> equal(s, 50.0)   // 20 + 30
        // cross-check against the unfused generator walk
        var refSum = 0.0
        for (car in from_xml_node(root, type<XCarN>)) {
            if (car.price > 15.0) {
                refSum += car.price
            }
        }
        t |> equal(s, refSum)
    }
}

[test]
def test_xml_fold_preserves_declared_default(t : T?) {
    // car 2 omits `qty`; the pruned read must fall back to the declared default (1) — matching build_xml_row.
    parse_xml(PRUNE_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        let q = _fold(unsafe(from_xml_node(root, type<XCarN>))._select(_.qty).sum())
        t |> equal(q, 9)   // 2 + 1 (defaulted) + 6
        var refQ = 0
        for (car in from_xml_node(root, type<XCarN>)) {
            refQ += car.qty
        }
        t |> equal(q, refQ)
    }
}

// 3 cars; price 20.0 appears TWICE (ids 2,3) and 10.0 once (id 1) — to verify single()'s exactly-one
// contract over XML actually scans past the first match instead of early-returning it. Uses the
// no-string XCar (single()/first() declare an uninitialized `var of the element type, which is unsafe
// for a string field outside `options persistent_heap` — the bench uses that; tests should not need it).
let DUP_XML = ("<cars>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\"/>" +
    "<car id=\"2\" price=\"20.0\" qty=\"4\"/>" +
    "<car id=\"3\" price=\"20.0\" qty=\"6\"/>" +
    "</cars>")

[test]
def test_xml_fold_single_unique(t : T?) {
    // single() over XML must return the one survivor when exactly one matches.
    parse_xml(DUP_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        let row = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 10.0).single())
        t |> equal(row.id, 1)
        t |> equal(row.qty, 2)
    }
}

[test]
def test_xml_fold_single_or_default_enforces_uniqueness(t : T?) {
    // The exactly-one contract: single_or_default must SEE the second match (it does not early-exit on
    // the first) and fall back to the default. This is the proof that single-family scans, not first-match.
    parse_xml(DUP_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        let SENTINEL = XCar(id = -1)
        // exactly one match -> the survivor
        let one = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 10.0).single_or_default(SENTINEL))
        t |> equal(one.id, 1)
        // two matches -> default (proves the second match was observed, i.e. a full scan, not find-first)
        let dup = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 20.0).single_or_default(SENTINEL))
        t |> equal(dup.id, -1)
        // zero matches -> default
        let none = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 99.0).single_or_default(SENTINEL))
        t |> equal(none.id, -1)
    }
}

[test]
def test_xml_fold_whole_row_escape(t : T?) {
    // `.to_array()` pushes the whole row -> the scanner sees a whole-`it` ref -> full build_xml_row, so
    // every field (including the pruned-elsewhere `name` string) materializes.
    parse_xml(PRUNE_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        var rows <- _fold(unsafe(from_xml_node(root, type<XCarN>))._where(_.price > 15.0).to_array())
        t |> equal(length(rows), 2)
        t |> equal(rows[0].id, 2)
        t |> equal(rows[0].price, 20.0)
        t |> equal(rows[0].qty, 1)        // defaulted (car 2 has no qty attr)
        t |> equal(rows[0].name, "beta")  // string field cloned in the whole-row path
        t |> equal(rows[1].name, "gamma")
        delete rows
    }
}
