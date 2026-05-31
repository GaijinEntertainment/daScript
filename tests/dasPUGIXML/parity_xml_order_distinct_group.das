options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require dastest/testing_boost public

// XML linq parity — the cascade families (order_by / distinct / distinct_by / group_by / reverse,
// plus distinct+take, group_by+reduction, order+take). These now FUSE over the XmlAdapter (the cascade
// arms ride wrap_source_loop / build_group_by_adapter / emit_distinct_take_loop). This file proves the
// fused XML result equals the array lanes — the correctness gate for the fusion.
//   m5f = _fold over from_xml_node(...)   m3f = _fold over each(arr)   m3 = plain array linq

struct Car {
    id    : int
    price : float
    qty   : int
    brand : string
}

let XML = ("<cars>" +
    "<car id=\"1\" price=\"100.0\" qty=\"2\" brand=\"Ford\"/>" +
    "<car id=\"2\" price=\"9999.0\" qty=\"4\" brand=\"Ferrari\"/>" +
    "<car id=\"3\" price=\"200.0\" qty=\"6\" brand=\"Honda\"/>" +
    "<car id=\"4\" price=\"50.0\" qty=\"8\" brand=\"Ford\"/>" +
    "<car id=\"5\" price=\"88888.0\" qty=\"1\" brand=\"Ferrari\"/>" +
    "<car id=\"6\" price=\"300.0\" qty=\"3\" brand=\"Honda\"/>" +
    "<car id=\"7\" price=\"150.0\" qty=\"5\" brand=\"Ford\"/>" +
    "<car id=\"8\" price=\"2000.0\" qty=\"7\" brand=\"Toyota\"/>" +
    "</cars>")

def fixture_array() : array<Car> {
    return <- [
        Car(id = 1, price = 100.0,   qty = 2, brand = "Ford"),
        Car(id = 2, price = 9999.0,  qty = 4, brand = "Ferrari"),
        Car(id = 3, price = 200.0,   qty = 6, brand = "Honda"),
        Car(id = 4, price = 50.0,    qty = 8, brand = "Ford"),
        Car(id = 5, price = 88888.0, qty = 1, brand = "Ferrari"),
        Car(id = 6, price = 300.0,   qty = 3, brand = "Honda"),
        Car(id = 7, price = 150.0,   qty = 5, brand = "Ford"),
        Car(id = 8, price = 2000.0,  qty = 7, brand = "Toyota")]
}

def cars_equal(a : array<Car>; b : array<Car>) : bool {
    if (length(a) != length(b)) { return false }
    for (i in range(length(a))) {
        if (a[i].id != b[i].id || a[i].price != b[i].price || a[i].qty != b[i].qty || a[i].brand != b[i].brand) {
            return false
        }
    }
    return true
}
def check3(t : T?; var m5f, m3f, m3 : array<Car>) {
    t |> success(cars_equal(m5f, m3f), "m5f == m3f")
    t |> success(cars_equal(m3f, m3), "m3f == m3")
    delete m5f; delete m3f; delete m3
}

def strings_equal(a : array<string>; b : array<string>) : bool {
    if (length(a) != length(b)) { return false }
    for (i in range(length(a))) {
        if (a[i] != b[i]) { return false }
    }
    return true
}

[test]
def parity_order_by(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by(_.price).to_array())
        var m3f <- _fold(each(arr)._order_by(_.price).to_array())
        var m3  <- (arr |> _order_by(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_order_by_descending(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by_descending(_.price).to_array())
        var m3f <- _fold(each(arr)._order_by_descending(_.price).to_array())
        var m3  <- (arr |> _order_by_descending(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_distinct(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand).distinct().to_array())
        var m3f <- _fold(each(arr)._select(_.brand).distinct().to_array())
        t |> success(strings_equal(m5f, m3f), "distinct brands m5f == m3f")
        delete m5f; delete m3f
    }
}

[test]
def parity_distinct_by(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand).to_array())
        var m3  <- (arr |> _distinct_by(_.brand))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_reverse(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 0.0).reverse().to_array())
        var m3f <- _fold(each(arr)._where(_.price > 0.0).reverse().to_array())
        var m3  <- (arr |> _where(_.price > 0.0) |> reverse())
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_reverse_distinct_by(t : T?) {
    // reverse + distinct_by: keep LAST per brand, output in reverse-discovery order. m5f fuses via the
    // FORWARD keep-last table-overwrite (R-2b, deferred handles); m3f rides the array backward-index walk.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse()._distinct_by(_.brand).to_array())
        var m3f <- _fold(each(arr).reverse()._distinct_by(_.brand).to_array())
        var m3  <- (arr |> reverse() |> _distinct_by(_.brand))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_reverse_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().take(3).to_array())
        var m3f <- _fold(each(arr).reverse().take(3).to_array())
        var m3  <- (arr |> reverse() |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_reverse_take_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().take(3)._select(_.brand).to_array())
        var m3f <- _fold(each(arr).reverse().take(3)._select(_.brand).to_array())
        t |> success(strings_equal(m5f, m3f), "reverse|take|select m5f == m3f")
        delete m5f; delete m3f
    }
}

[test]
def parity_distinct_by_take(t : T?) {
    // distinct+take fuses via XmlAdapter.emit_distinct_take_loop (early-exit after N distinct).
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand).take(2).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand).take(2).to_array())
        var m3  <- (arr |> _distinct_by(_.brand) |> take(2))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_order_by_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._order_by(_.price).take(3).to_array())
        var m3  <- (arr |> _order_by(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

// ===== Deferred-materialization (handle-buffering) bounded-heap variants — see linq_fold.md =====

[test]
def parity_order_by_descending_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by_descending(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._order_by_descending(_.price).take(3).to_array())
        var m3  <- (arr |> _order_by_descending(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_distinct_by_order_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand)._order_by(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand)._order_by(_.price).take(3).to_array())
        var m3  <- (arr |> _distinct_by(_.brand) |> _order_by(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_where_order_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 100.0)._order_by(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._where(_.price > 100.0)._order_by(_.price).take(3).to_array())
        var m3  <- (arr |> _where(_.price > 100.0) |> _order_by(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_distinct_by_order_to_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand)._order_by(_.price).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand)._order_by(_.price).to_array())
        var m3  <- (arr |> _distinct_by(_.brand) |> _order_by(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_where_order(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 100.0)._order_by(_.price).to_array())
        var m3f <- _fold(each(arr)._where(_.price > 100.0)._order_by(_.price).to_array())
        var m3  <- (arr |> _where(_.price > 100.0) |> _order_by(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_order_by_first(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let m5f = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by(_.price).first())
        let m3f = _fold(each(arr)._order_by(_.price).first())
        t |> success(m5f.id == m3f.id && m5f.price == m3f.price && m5f.brand == m3f.brand, "order|first m5f == m3f")
    }
}

[test]
def parity_order_by_descending_first(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let m5f = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by_descending(_.price).first())
        let m3f = _fold(each(arr)._order_by_descending(_.price).first())
        t |> success(m5f.id == m3f.id && m5f.price == m3f.price && m5f.brand == m3f.brand, "order_desc|first m5f == m3f")
    }
}

[test]
def parity_select_order_distinct_take(t : T?) {
    // Leading `_select(_.brand)` projects the source, then order |> distinct |> take. Fuses via
    // ProjectedSourceAdapter (the srcsel slot) — single-pass dedup-walk + bounded top-N, field-pruned to brand.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand)._order_by(_).distinct().take(2).to_array())
        var m3f <- _fold(each(arr)._select(_.brand)._order_by(_).distinct().take(2).to_array())
        t |> success(strings_equal(m5f, m3f), "select|order|distinct|take m5f == m3f")
        t |> equal(length(m5f), 2)
        t |> equal(m5f[0], "Ferrari")   // distinct sorted brands: Ferrari, Ford, Honda, Toyota → take 2
        t |> equal(m5f[1], "Ford")
        delete m5f; delete m3f
    }
}

[test]
def parity_select_order_take(t : T?) {
    // Leading `_select` + order + take (no distinct) — bounded heap over projected brands.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand)._order_by(_).take(3).to_array())
        var m3f <- _fold(each(arr)._select(_.brand)._order_by(_).take(3).to_array())
        t |> success(strings_equal(m5f, m3f), "select|order|take m5f == m3f")
        t |> equal(length(m5f), 3)
        delete m5f; delete m3f
    }
}

[test]
def parity_group_by_sum(t : T?) {
    // group_by + per-group reduction (sum) — the field-pruning win shape (reads only brand + qty).
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._group_by(_.brand)._select((b = _._0, total = _._1 |> _select(_.qty) |> sum())).to_array())
        var m3f <- _fold(each(arr)._group_by(_.brand)._select((b = _._0, total = _._1 |> _select(_.qty) |> sum())).to_array())
        t |> equal(length(m5f), length(m3f))
        var s5 : table<string; int>
        var sf : table<string; int>
        for (g in m5f) { s5[g.b] = g.total }
        for (g in m3f) { sf[g.b] = g.total }
        t |> equal(length(s5), length(sf))
        for (k, v in keys(s5), values(s5)) {
            t |> equal(v, sf?[k] ?? -1)
        }
        delete m5f; delete m3f
    }
}

[test]
def parity_group_by(t : T?) {
    // group_by emits a per-key bucket; assert XML fold matches array fold by (brand,count) pairs.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._group_by(_.brand)._select((b = _._0, n = _._1 |> count())).to_array())
        var m3f <- _fold(each(arr)._group_by(_.brand)._select((b = _._0, n = _._1 |> count())).to_array())
        t |> equal(length(m5f), length(m3f))
        // build brand->count maps and compare (group order is not guaranteed)
        var c5 : table<string; int>
        var cf : table<string; int>
        for (g in m5f) { c5[g.b] = g.n }
        for (g in m3f) { cf[g.b] = g.n }
        t |> equal(length(c5), length(cf))
        for (k, v in keys(c5), values(c5)) {
            t |> equal(v, cf?[k] ?? -1)
        }
        delete m5f; delete m3f
    }
}

[test]
def parity_group_by_first(t : T?) {
    // group_by + first-per-group — the deferred-materialization shape: the whole row escapes (FirstCar) but
    // only #brands first-seen rows survive. m5f builds each survivor from the node handle in the miss-branch.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._group_by(_.brand)._select((b = _._0, first = _._1 |> first())).to_array())
        var m3f <- _fold(each(arr)._group_by(_.brand)._select((b = _._0, first = _._1 |> first())).to_array())
        t |> equal(length(m5f), length(m3f))
        var f5 : table<string; int>
        var ff : table<string; int>
        for (g in m5f) {
            f5[g.b] = g.first.id
            t |> equal(g.first.brand, g.b)   // materialized survivor's own brand must match its group key
        }
        for (g in m3f) { ff[g.b] = g.first.id }
        t |> equal(length(f5), length(ff))
        for (k, v in keys(f5), values(f5)) {
            t |> equal(v, ff?[k] ?? -1)
        }
        delete m5f; delete m3f
    }
}
