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
