options gen2
options rtti
require dastest/testing_boost public
require daslib/rtti

struct ScanDestination {
    value : int
}

def private scan_scalar(text : string; var value : float&) : bool =>
    sscan_json(text, value)

def private scan_struct(text : string; var value : ScanDestination) : bool =>
    sscan_json(text, value)

def private scan_array(text : string; var value : array<int>) : bool =>
    sscan_json(text, value)

def private scan_table(text : string; var value : table<string; int>) : bool =>
    sscan_json(text, value)

def private scan_fixed_array(text : string; var value : int[2]) : bool =>
    sscan_json(text, value)

def private with_destination(var value : ScanDestination;
                             body : block<(var item : ScanDestination)>
                             ) {
    body |> invoke(value)
}

[test]
def test_scalar_scan_destinations(t : T?) {
    var number = 17
    t |> success(sscan_json("42", number))
    t |> equal(number, 42)
    var flag = false
    t |> success(sscan_json("true", flag))
    t |> equal(flag, true)
    var text = "before"
    t |> success(sscan_json("\"after\"", text))
    t |> equal(text, "after")
    var amount = 3.5
    t |> success(sscan_json("null", amount))
    t |> equal(amount, 3.5)
    t |> success(sscan_json([json = "2.5", value = amount]))
    t |> equal(amount, 2.5)
}

[test]
def test_forwarded_scan_destinations(t : T?) {
    var scalar = 0.0
    t |> success(scan_scalar("1.5", scalar))
    t |> equal(scalar, 1.5)
    var value : ScanDestination
    t |> success(scan_struct("\{\"value\":7\}", value))
    t |> equal(value.value, 7)
    var items : array<int>
    t |> success(scan_array("[3,4]", items))
    t |> equal(items, [3, 4])
    var mapping : table<string; int>
    t |> success(scan_table("\{\"value\":7\}", mapping))
    t |> equal(mapping?["value"] ?? 0, 7)
    var fixed : int[2]
    t |> success(scan_fixed_array("[3,4]", fixed))
    t |> equal(fixed[0], 3)
    t |> equal(fixed[1], 4)
    with_destination(value) $(var item) {
        t |> success(sscan_json("\{\"value\":9\}", item))
    }
    t |> equal(value.value, 9)
}

[test]
def test_pointer_and_fixed_scan_destinations(t : T?) {
    var value : ScanDestination?
    t |> strictEqual(sscan_json("\{\"value\":7\}", value), true)
    t |> strictEqual(value != null, true)
    t |> equal(value.value, 7)
    var points : float[2]
    t |> success(sscan_json("[1.5,2.5]", points))
    t |> equal(points[0], 1.5)
    t |> equal(points[1], 2.5)
    var scalar = 0.0
    let address = unsafe(addr(scalar))
    t |> success(sscan_json_at("1.5", address,
        typeinfo rtti_typeinfo(scalar)))
    t |> equal(scalar, 1.5)
}
