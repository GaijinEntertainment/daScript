options gen2
options rtti
require dastest/testing_boost public

struct ScanDestination {
    value : int
}

struct DerivedDestination : ScanDestination {
    extra : int
}

def private scan_selected(text : string; choose_first : bool;
                          var first, second : auto(T)) : bool =>
    sscan_json(text, choose_first ? first : second)

def private scan_cast(text : string; var value : DerivedDestination) : bool =>
    sscan_json(text, cast<ScanDestination>(value))

def private scan_fallback(text : string; var value : ScanDestination?;
                          var fallback : ScanDestination) : bool =>
    sscan_json(text, value ?? fallback)

[test]
def test_selected_scan_storage(t : T?) {
    var first, second : ScanDestination
    t |> success(scan_selected("\{\"value\":7\}", true, first, second))
    t |> equal(first.value, 7)
    t |> equal(second.value, 0)
    t |> success(scan_selected("\{\"value\":9\}", false, first, second))
    t |> equal(first.value, 7)
    t |> equal(second.value, 9)
    var items, other_items : array<int>
    t |> success(scan_selected("[3,4]", false, items, other_items))
    t |> equal(length(items), 0)
    t |> equal(other_items, [3, 4])
    var mapping, other_mapping : table<string; int>
    t |> success(scan_selected("\{\"value\":7\}", true,
        mapping, other_mapping))
    t |> equal(mapping?["value"] ?? 0, 7)
    t |> equal(length(other_mapping), 0)
    var fixed, other_fixed : int[2]
    t |> success(scan_selected("[3,4]", true, fixed, other_fixed))
    t |> equal(fixed[0], 3)
    t |> equal(fixed[1], 4)
    t |> equal(other_fixed[0], 0)
}

[test]
def test_cast_scan_storage(t : T?) {
    var value = DerivedDestination(extra = 11)
    t |> success(scan_cast("\{\"value\":7\}", value))
    t |> equal(value.value, 7)
    t |> equal(value.extra, 11)
}

[test]
def test_fallback_scan_storage(t : T?) {
    var fallback : ScanDestination
    var pointer : ScanDestination?
    t |> success(scan_fallback("\{\"value\":7\}", pointer, fallback))
    t |> equal(fallback.value, 7)
    var value : ScanDestination
    t |> success(scan_fallback("\{\"value\":9\}", unsafe(addr(value)),
        fallback))
    t |> equal(value.value, 9)
    t |> equal(fallback.value, 7)
}
