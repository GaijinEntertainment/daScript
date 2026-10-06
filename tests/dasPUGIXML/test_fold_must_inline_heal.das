options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require daslib/strings_boost
require dastest/testing_boost public

// Regression for the locked-instance-name fallback: the fold macro locks the
// predicate's contains() against the plain-string instance, then re-roots
// `_.brand` onto peek_xml_field's `string const#` result. A MUST [inline]
// splice clones the body (dropping the genericFunction short-circuit), the
// locked name re-matches strictly, and pre-fix this was a hard error[30341]:
//   __::strings_boost`contains`...(string const#, string const)
// The fallback now re-resolves through the origin generic and binds the
// `#`-flavored sibling.

struct Car {
    id    : int
    brand : string
}

let XML = ("<cars>" +
    "<car id=\"1\" brand=\"Ford\"/>" +
    "<car id=\"2\" brand=\"Honda\"/>" +
    "<car id=\"3\" brand=\"Fiat\"/>" +
    "</cars>")

[inline]
def fold_contains_o(doc : pugixml::xml_document? const) : array<Car> =>
    <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand |> contains("o")).to_array())

[test]
def test_must_inline_folded_temp_string_call_heals(t : T?) {
    parse_xml(XML) $(doc; ok) {
        t |> success(ok, "xml parses")
        var cars <- fold_contains_o(doc)
        t |> equal(2, length(cars))     // Ford and Honda contain 'o'; Fiat does not
        delete cars
    }
}
