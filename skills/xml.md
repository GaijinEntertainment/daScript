# XML in daslang (dasPUGIXML)

Read this skill before writing or editing `.das` code that parses, builds, queries, or serializes XML. There is one path: `dasPUGIXML` (a binding around the C++ pugixml library) plus its `PUGIXML_boost` daslib layer with iterators, RAII helpers, and the builder EDSL. Read [tutorials/dasPUGIXML/](tutorials/dasPUGIXML/) for runnable examples (4 tutorials, in order).

## Module & build flag

```das
require pugixml/PUGIXML_boost
```

`PUGIXML_boost` re-exports the C++ `pugixml` module — never `require` both.

The module is **gated by CMake**: `DAS_PUGIXML_DISABLED` (default `ON` = disabled). To use it locally, set `DAS_PUGIXML_DISABLED=OFF` in `cmake.configureSettings` of `.vscode/settings.json` and reconfigure. See `CLAUDE.md` § "Build Configurations" for the workflow.

## Loading & parsing — RAII blocks

All three entry points take a block that runs with a live document; the document is freed automatically at block exit.

```das
parse_xml(xml_str) $(doc, ok) {
    if (!ok) { return ; }
    let root = doc.document_element
    // ... work with doc/root ...
}

open_xml("path/to/file.xml") $(doc, ok) {
    if (!ok) { return ; }
    // ...
}

with_doc() $(doc) {
    // empty document, build it via tag()/attr() ...
    print(to_string(doc))
}
```

**Always check `ok` first** — pugixml returns errors as a bool flag, not exceptions. Reading from a doc whose parse failed is undefined.

**Don't escape document/node pointers past the block.** They become dangling once the RAII block exits. Materialize what you need (strings, structs, your own data) inside the block, and return that.

## Iteration

Two styles for the same iteration — pick whichever reads cleaner at the call site:

```das
// Iterator (preferred for plain reads)
for (ch in each_child(node))            { ... }    // all children
for (ch in each_child(node, "name"))    { ... }    // children with this tag
for (a  in each_attribute(node))        { ... }

// Block (required when you need a `var` child for mutation)
node |> for_each_child()       $(ch)      { ... }
node |> for_each_child("name") $(ch)      { ... }
node |> for_each_attribute()   $(a)       { ... }
```

Manual `node.first_child` / `node.next_sibling` walking still works but is rarely the right choice.

## LINQ source — `from_xml_node`

`from_xml_node(root, type<Row>)` is a typed, lazy iterator over `root`'s child elements: each child is materialized into a `Row` by reading **same-named attributes**, so comprehensions and `daslib/linq_boost` queries run straight over an XML document.

```das
struct Car {
    id : int
    make : string
    price : float
    year : int = 2000        // default kept when the attribute is absent
}

for (car in from_xml_node(root, type<Car>)) { ... }                 // all children
let makes <- [for (car in from_xml_node(root, type<Car>));          // comprehension
    car.make; where car.price < 30000.0]
let cars <- unsafe(from_xml_node(root, "car", type<Car>) |> to_array())   // tag-filtered + collect
```

- **Field mapping (v1):** every struct field reads from an attribute of the same name. Supported scalar types: `int`, `uint`, `float`, `double`, `bool`, `string`. Fields of other types keep their default. Child-element / text mapping (via `@xml_*` field annotations) is a planned growth path.
- **Defaults:** a missing attribute leaves the field at its declared default (`year : int = 2000` above), because the field value is passed as the accessor's fallback.
- **Lifetime-safe:** rows are owned values — string fields are cloned out of the document — so results collected with `to_array` / a comprehension stay valid **past** the `parse_xml` / `open_xml` RAII block. (Contrast: a raw `xml_node` must not escape the block.)
- **`unsafe` outside a `for`:** `from_xml_node` is `[unsafe_outside_of_for]`. A `for` loop and a comprehension are safe; piping it into `to_array` / `linq_boost` outside a `for` needs an `unsafe` block.
- **Fused `_fold` lane (pass 2a/2b):** `_fold(unsafe(from_xml_node(root, type<Row>))._where(...)._select(...).count()/sum()/min()/max()/average()/any()/first()/take(N)...)` emits an inlined DOM child-walk via `XmlAdapter` (`require ?pugixml pugixml/linq_fold_xml`, gated by `static_if (typeinfo builtin_module_exists(pugixml))` in `daslib/linq_fold`) — no generator, no intermediate array. Pass the node by value (`var root`, not `let root`) since `_fold`'s macro-arg inference skips the const&→value copy. **Field-pruning (pass 2b):** the walk reads only the `Row` fields the chain actually references (via `read_xml_field` into scalar locals) — unread fields, especially `string` fields whose `clone_string` is the alloc cost, are never touched, so a float-only chain is alloc-free and JIT beats the equivalent SQLite query. A whole-row escape (`to_array` / identity `_select(_)` / pass-to-fn) falls back to the full `build_xml_row`. Non-`loop_or_count` shapes (order_by / distinct / group_by / join / reverse) still take the un-fused tier-2 path. See [tutorials/dasPUGIXML/05_linq_over_xml.das](tutorials/dasPUGIXML/05_linq_over_xml.das).

## Quick accessors (with defaults)

```das
let title  = node_text(root, "title")                       // child element text
let desc   = node_text(root, "desc", "no description")
let id     = node_attr(root, "id")                          // attribute as string
let count  = node_attr_int(root, "count", 0)
let ratio  = node_attr_float(root, "ratio", 1.0)
let active = node_attr_bool(root, "active", false)
```

These read; they do not mutate. Missing data returns the supplied default.

## Typed `is` / `as` on attributes and text

`PUGIXML_boost` overloads `operator []` on `xml_node` to return an `xml_attribute`, and overloads `is`/`as` for `int / uint / float / double / bool / string` on both `xml_attribute` and `xml_text`.

```das
let x_attr = root["x"]                          // xml_attribute
let x_val  = root["x"] as int                   // 0 if missing — does NOT crash
let label  = root["label"] as string

// Existence check on attribute / non-empty check on text
if (root["x"] is int)        { ... }
if (count_node.text is int)  { let n = count_node.text as int }

// Whole-node serialization
let xml_string = (some_node as string)
```

A missing attribute reads as a "falsy" `xml_attribute` (its `.ok` field is false) — typed `as` returns the type's default rather than erroring.

## Building XML — the `tag` / `attr` EDSL

`tag()` appends a child and runs a block inside it. `attr()` appends an attribute and returns its parent for chaining. Both pipe naturally.

```das
with_doc() $(doc) {
    doc |> tag("library") $(var library) {
        library |> tag("book") $(var book) {
            book |> tag("title", "The C Programming Language")     // child + text
            book |> tag("author", "Brian W. Kernighan")
            book |> attr("isbn",  "978-0131103627")                // attribute
            book |> attr("year",  1988)                            // typed: int
            book |> attr("price", 67.99)                           // typed: float
        }
    }
    print(to_string(doc))
}
```

- `tag(name, blk)` — append child, run block on it, return the child.
- `tag(name, text)` — append child with text content (no nested block).
- `tag(name)` — append a bare child.
- `attr(name, value)` — string / int / float / bool overloads; returns the parent for chaining.
- Low-level: `add_child`, `add_child_ex`, `add_attr` — use only when the EDSL doesn't fit.

## Serializing

```das
let s = to_string(doc)        // serialize whole document
let s = to_string(some_node)  // serialize a subtree
```

2-space indent, UTF-8 (auto-detected for input; emitted by default for output).

## XPath

```das
let first_name = select_text(root, "product[1]/name")
let isbn       = select_value(root, "book[1]/@isbn")     // attribute or text

root |> for_each_select("book[@year>'2000']") $(xn) {
    let n = xn.node
    print("{n.name}\n")
}

// Reuse a compiled query
with_xpath("book[@in_stock='true']") $(q) {
    // pass q to evaluate_node_set / iterate
}
```

`for_each_select` auto-frees the result set; the block parameter is an `xpath_node` (`.node` is the underlying `xml_node`). Compile a query once with `with_xpath` if you'll evaluate it many times.

## Struct ↔ XML

Round-trip arbitrary struct values:

```das
let player = Player(name = "Hero", hp = 100)
let xml_str = to_XML(player, "player")               // string
let restored = from_XML(xml_str, type<Player>)       // string → struct
let same_via_node = from_XML(some_node, type<Player>) // node → struct
```

Supports nested structs, enums, arrays, tables, tuples, variants, vector types (`floatN`, `intN`). The same field annotations as JSON apply (`@rename`, `@enum_as_int`, `@unescape`).

## Common gotchas

- **Errors are `ok` bool flags, not exceptions** — always branch on the second block parameter before using `doc`.
- **No escape past the RAII block** — `xml_document?` and any `xml_node` derived from it are valid only inside the block. Returning them past the block exit is use-after-free.
- **Missing attributes don't error**: `root["does_not_exist"] as int` returns `0`. Use the `is` test if you need to distinguish "missing" from "zero".
- **Build flag**: if `require pugixml/PUGIXML_boost` fails at compile time, the module is disabled — flip `DAS_PUGIXML_DISABLED=OFF` (`CLAUDE.md` § "Build Configurations").
- **CMake install for tutorial data**: a tutorial that ships sample XML (e.g. [tutorials/dasPUGIXML/books.xml](tutorials/dasPUGIXML/books.xml)) needs a `*.xml` glob in the install rule — see `skills/tutorials.md`.

## Reference

- Tutorials (read in order, runnable):
  - [tutorials/dasPUGIXML/01_parsing_and_navigation.das](tutorials/dasPUGIXML/01_parsing_and_navigation.das) — parsing, iteration, quick accessors, `is`/`as`
  - [tutorials/dasPUGIXML/02_building_xml.das](tutorials/dasPUGIXML/02_building_xml.das) — `with_doc`, `tag`/`attr` EDSL, serialization
  - [tutorials/dasPUGIXML/03_xpath.das](tutorials/dasPUGIXML/03_xpath.das) — XPath queries, compiled XPath
  - [tutorials/dasPUGIXML/04_serialization.das](tutorials/dasPUGIXML/04_serialization.das) — `to_XML`/`from_XML` round-trip
  - [tutorials/dasPUGIXML/05_linq_over_xml.das](tutorials/dasPUGIXML/05_linq_over_xml.das) — `from_xml_node` LINQ source (typed rows from attributes)
- daslib helpers (the source of truth for the EDSL): [modules/dasPUGIXML/daslib/PUGIXML_boost.das](modules/dasPUGIXML/daslib/PUGIXML_boost.das)
- C++ binding (for adding new functions): [modules/dasPUGIXML/src/dasPUGIXML.h](modules/dasPUGIXML/src/dasPUGIXML.h), `dasPUGIXML.cpp`
- Tests with patterns: [tests/dasPUGIXML/](tests/dasPUGIXML/) — `test_pugixml_core.das`, `test_pugixml_mutation.das`, `test_pugixml_boost.das`, `test_serial_*.das`
- Auto-generated RST: `doc/source/stdlib/handmade/function-pugixml-*.rst`, `structure_annotation-pugixml-*.rst`, `Variable-pugixml-*.rst`
