options gen2

require dastest/testing_boost public
require daslib/linq_das
require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require daslib/strings_boost

// linq_das over an XML source: `from c : Car in <node>` →
// `from_in(node, type<Car>)` → `unsafe(from_xml_node(node,
// type<Car>))`, which `_fold` fuses via the XmlAdapter. The node is
// passed inline (a `let`-bound node would be `const&` and rejected —
// see PUGIXML linq_fold tests).

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

[test(description = "a linq query over an XML node filters rows with where and projects a field with select")]
def test_xml_where_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let brands <- %linq! from c : Car in doc.document_element where c.price > 99.0 select c.brand %%
        t |> equal(length(brands), 2, "xml where/select")
        t |> equal(brands[0], "Ford")
        t |> equal(brands[1], "Honda")
    }
}

[test(description = "two where clauses over an XML source both apply")]
def test_xml_multiple_where(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // two `where`s prune the DOM walk: price>99 (car1 Ford, car2 Honda)
        // AND brand Ford (car1).
        let ids <- %linq! from c : Car in doc.document_element where c.price > 99.0 where c.brand == "Ford" select c.id %%
        t |> equal(length(ids), 1, "both predicates apply over the XML rows")
        t |> equal(ids[0], 1)
    }
}

[test(description = "selecting the row variable over an XML source returns the whole rows")]
def test_xml_identity_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let rows <- %linq! from c : Car in doc.document_element where c.price >= 100.0 select c %%
        t |> equal(length(rows), 2, "xml identity select returns rows")
        t |> equal(rows[0].id, 1)
        t |> equal(rows[1].id, 2)
    }
}

[test(description = "a linq query over an XML source can end in an iterator")]
def test_xml_iterator_output(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let brands <- [for (b in %linq! from c : Car in doc.document_element where c.price > 99.0 select c.brand iterator %%); b]
        t |> equal(length(brands), 2, "xml iterator output")
    }
}

[test(description = "orderby sorts XML rows ascending by a field")]
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

[test(description = "orderby descending sorts XML rows by a field from high to low")]
def test_xml_orderby_descending(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let ids <- %linq! from c : Car in doc.document_element orderby c.price descending select c.id %%
        t |> equal(ids[0], 2)   // 300
        t |> equal(ids[2], 3)   // 50
    }
}

[test(description = "orderby with two keys sorts XML rows by brand and then by price descending")]
def test_xml_orderby_multi_key(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // brand asc, price desc → Ford(id1@100, id3@50) then Honda(id2@300) →
        // ids 1, 3, 2
        let ids <- %linq! from c : Car in doc.document_element orderby c.brand, c.price descending select c.id %%
        t |> equal(length(ids), 3)
        t |> equal(ids[0], 1)   // Ford, 100
        t |> equal(ids[1], 3)   // Ford, 50
        t |> equal(ids[2], 2)   // Honda, 300
    }
}

[test(description = "group by over an XML source buckets rows by brand")]
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

[test(description = "joining an XML source to an array pairs every car with its brand headquarters")]
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

[test(description = "a where before a join over an XML source prunes rows ahead of the join")]
def test_xml_join_pre_join_where(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // pre-join where filters the XML rows (price > 99 → car1 Ford, car2
        // Honda) before the join
        let rows <- %linq! from c : Car in doc.document_element where c.price > 99.0 join b in brands on c.brand equals b.brand select (Id = c.id, Country = b.country) %%
        t |> equal(length(rows), 2, "pre-join where prunes the DOM walk")
    }
}

[test(description = "orderby after a join over an XML source sorts the joined rows")]
def test_xml_join_orderby(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // post-join orderby → transparent-id carry; sort the joined pairs by
        // price
        let ids <- %linq! from c : Car in doc.document_element join b in brands on c.brand equals b.brand orderby c.price select c.id %%
        t |> equal(length(ids), 3)
        t |> equal(ids[0], 3, "car3 price 50 first")
        t |> equal(ids[2], 2, "car2 price 300 last")
    }
}

[test(description = "group by after a join over an XML source buckets the joined rows")]
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

// ===== multiple `from`: XML left source × in-memory array right
// source (uncorrelated cross product). The XML source is the unfused
// `_cross_join` chain head — `from_xml_node` is passed as a raw
// argument (not a fused for-loop source), so it relies on the unsafe
// wrap surviving from_in's `$c` expansion. =====

[test(description = "a second from clause over an XML source yields the cartesian product of rows")]
def test_xml_multifrom_tuple(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let rows <- %linq! from c : Car in doc.document_element from b in brands select (Id = c.id, Country = b.country) %%
        t |> equal(length(rows), 6, "3 XML cars × 2 brands cartesian product")
    }
}

[test(description = "a where before a second from clause shrinks the XML side of the cartesian product")]
def test_xml_multifrom_pre_where(t : T?) {
    let brands <- mk_brands()
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // pre-from where prunes the DOM walk (price > 99 → car1, car2) before
        // the cross
        let rows <- %linq! from c : Car in doc.document_element where c.price > 99.0 from b in brands select (Id = c.id, Country = b.country) %%
        t |> equal(length(rows), 4, "2 filtered XML cars × 2 brands")
    }
}

// ===== let bindings over an XML source (inlined computed expression
// over the row attributes) =====

[test(description = "a let binding over an XML source is inlined into the order key and the projection")]
def test_xml_let(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // half = price/2 → 50, 150, 25 → orderby asc → ids 3, 1, 2; the `let`
        // is inlined into both the order key and the projection.
        let rows <- %linq! from c : Car in doc.document_element let half = c.price / 2.0
            orderby half select (Id = c.id, Half = half) %%
        t |> equal(length(rows), 3)
        t |> equal(rows[0].Id, 3)         // 25
        t |> equal(rows[0].Half, 25.0)
        t |> equal(rows[2].Id, 2)         // 150
        t |> equal(rows[2].Half, 150.0)
    }
}

// XML LINQ fold (pass 2a): `_fold` over a `from_xml_node(...)` chain
// emits an inlined DOM child-walk via XmlAdapter. Each lane below
// must match the unfused reference (a plain `for` over
// from_xml_node). The source is `[unsafe_outside_of_for]`, so a fold
// chain wraps it in `unsafe(...)`; the node is passed by value (`var
// root`) because `_fold`'s macro-argument inference does not insert
// the const&->value copy.

struct XCar {
    id    : int
    price : float
    qty   : int
}

// 4 car elements (prices 10/20/30/40, qty 2/4/6/8) with interleaved
// PCDATA noise the element walk skips.
let TEST_XML = ("<cars>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\"/>noise" +
    "<car id=\"2\" price=\"20.0\" qty=\"4\"/>more" +
    "<car id=\"3\" price=\"30.0\" qty=\"6\"/>" +
    "<car id=\"4\" price=\"40.0\" qty=\"8\"/>" +
    "</cars>")

// cars + a non-car element, to exercise the named (tag-filtered)
// overload.
let NAMED_XML = ("<root>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\"/>" +
    "<other id=\"99\" price=\"99.0\" qty=\"0\"/>" +
    "<car id=\"2\" price=\"20.0\" qty=\"4\"/>" +
    "</root>")

[test(description = "_fold over from_xml_node counts rows with and without a predicate")]
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

[test(description = "_fold over from_xml_node projects, filters and sums a field")]
def test_xml_fold_select_where_sum(t : T?) {
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        // project price, filter > 15, sum -> 20 + 30 + 40
        let s = _fold(unsafe(from_xml_node(root, type<XCar>))._select(_.price)._where(_ > 15.0).sum())
        t |> equal(s, 90.0)
    }
}

[test(description = "_fold over from_xml_node computes min, max and average of a projected field")]
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

[test(description = "_fold over from_xml_node finds the first match and answers any without scanning past it")]
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

[test(description = "_fold over from_xml_node caps a count with take")]
def test_xml_fold_take(t : T?) {
    parse_xml(TEST_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        // cap at 2 of the 3 matches
        let c = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price > 15.0).take(2).count())
        t |> equal(c, 2)
    }
}

[test(description = "_fold over the element-name overload of from_xml_node skips elements of other names")]
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

[test(description = "a _fold over from_xml_node gives the same count and sum as the plain generator walk")]
def test_xml_fold_matches_unfused_reference(t : T?) {
    // The fold must be observationally equal to the plain (unfused)
    // generator walk.
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
// A struct with a string field (whose clone is pruned away when
// unread) and a non-zero declared default (qty), to exercise the
// pruned reads, default preservation, and whole-row escape.
struct XCarN {
    id    : int
    price : float
    qty   : int = 1
    name  : string
}

// car 2 has no `qty` attribute -> reads must fall back to the
// declared default (1). All rows carry a `name` attr so a whole-row
// escape has a string to clone (and a pruned chain has one to skip).
let PRUNE_XML = ("<cars>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\" name=\"alpha\"/>" +
    "<car id=\"2\" price=\"20.0\" name=\"beta\"/>" +
    "<car id=\"3\" price=\"30.0\" qty=\"6\" name=\"gamma\"/>" +
    "</cars>")

[test(description = "a _fold that reads only a numeric field gives the exact result without reading the string field")]
def test_xml_fold_prunes_unread_string_field(t : T?) {
    // Chain reads only `price` — the `name` string clone is never
    // emitted. Result must still be exact.
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

[test(description = "a pruned _fold read falls back to the declared default for an omitted attribute")]
def test_xml_fold_preserves_declared_default(t : T?) {
    // car 2 omits `qty`; the pruned read must fall back to the declared
    // default (1) — matching build_xml_row.
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

// 3 cars; price 20.0 appears TWICE (ids 2,3) and 10.0 once (id 1) —
// to verify single()'s exactly-one contract over XML actually scans
// past the first match instead of early-returning it. Uses the
// no-string XCar (single()/first() declare an uninitialized `var of
// the element type, which is unsafe for a string field outside
// `options persistent_heap` — the bench uses that; tests should not
// need it).
let DUP_XML = ("<cars>" +
    "<car id=\"1\" price=\"10.0\" qty=\"2\"/>" +
    "<car id=\"2\" price=\"20.0\" qty=\"4\"/>" +
    "<car id=\"3\" price=\"20.0\" qty=\"6\"/>" +
    "</cars>")

[test(description = "_fold single over from_xml_node returns the one matching row")]
def test_xml_fold_single_unique(t : T?) {
    // single() over XML must return the one survivor when exactly one
    // matches.
    parse_xml(DUP_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        let row = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 10.0).single())
        t |> equal(row.id, 1)
        t |> equal(row.qty, 2)
    }
}

[test(description = "_fold single_or_default returns the default when two rows match, proving it scans past the first match")]
def test_xml_fold_single_or_default_enforces_uniqueness(t : T?) {
    // The exactly-one contract: single_or_default must SEE the second
    // match (it does not early-exit on the first) and fall back to the
    // default. This is the proof that single-family scans, not
    // first-match.
    parse_xml(DUP_XML) $(doc, ok) {
        t |> success(ok)
        var root = doc.document_element
        let SENTINEL = XCar(id = -1)
        // exactly one match -> the survivor
        let one = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 10.0).single_or_default(SENTINEL))
        t |> equal(one.id, 1)
        // two matches -> default (proves the second match was observed, i.e.
        // a full scan, not find-first)
        let dup = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 20.0).single_or_default(SENTINEL))
        t |> equal(dup.id, -1)
        // zero matches -> default
        let none = _fold(unsafe(from_xml_node(root, type<XCar>))._where(_.price == 99.0).single_or_default(SENTINEL))
        t |> equal(none.id, -1)
    }
}

[test(description = "a _fold that collects whole rows materializes every field, including the string field")]
def test_xml_fold_whole_row_escape(t : T?) {
    // `.to_array()` pushes the whole row -> the scanner sees a whole-`it`
    // ref -> full build_xml_row, so every field (including the
    // pruned-elsewhere `name` string) materializes.
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

// Regression for the locked-instance-name fallback: the fold macro
// locks the predicate's contains() against the plain-string instance,
// then re-roots `_.brand` onto peek_xml_field's `string const#`
// result. A MUST [inline] splice clones the body (dropping the
// genericFunction short-circuit), the locked name re-matches
// strictly, and pre-fix this was a hard error[30341]:
// __::strings_boost`contains`...(string const#, string const) The
// fallback now re-resolves through the origin generic and binds the
// `#`-flavored sibling.

struct HealCar {
    id    : int
    brand : string
}

let HEAL_XML = ("<cars>" +
    "<car id=\"1\" brand=\"Ford\"/>" +
    "<car id=\"2\" brand=\"Honda\"/>" +
    "<car id=\"3\" brand=\"Fiat\"/>" +
    "</cars>")

[inline]
def fold_contains_o(doc : pugixml::xml_document? const) : array<HealCar> =>
    <- _fold(unsafe(from_xml_node(doc.document_element, type<HealCar>))
        ._where(_.brand |> contains("o")).to_array())

[test(description = "a [inline] _fold calling contains on a temp-string field still compiles and filters correctly")]
def test_must_inline_folded_temp_string_call_heals(t : T?) {
    parse_xml(HEAL_XML) $(doc; ok) {
        t |> success(ok, "xml parses")
        var cars <- fold_contains_o(doc)
        t |> equal(2, length(cars))     // Ford and Honda contain 'o'; Fiat does not
        delete cars
    }
}
