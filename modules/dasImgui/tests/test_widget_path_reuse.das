options gen2
options persistent_heap
options gc
require dastest/testing_boost public
require imgui/imgui_boost_runtime

var retained_widget_path : string

[test]
def repeated_widget_paths_do_not_allocate_strings(t : T?) {
    imgui_boost_runtime::begin_frame()
    container_path_push("Panel")
    let warm = widget_path_key("Value")
    let before = string_heap_allocation_count()
    var same = true
    for (_frame in range(3)) {
        container_path_pop()
        imgui_boost_runtime::begin_frame()
        container_path_push("Panel")
        for (_i in range(100)) { same = same && widget_path_key("Value") == warm }
    }
    let allocations = string_heap_allocation_count() - before
    container_path_pop()
    t |> success(same, "repeated keys retain their full path")
    t |> equal(allocations, 0ul, "steady-state widget lookup does not allocate strings")
}

[test]
def path_reuse_preserves_container_identity(t : T?) {
    imgui_boost_runtime::begin_frame()
    t |> equal(widget_path_key("Value"), "Value")
    container_path_push("Left")
    let left = widget_path_key("Value")
    container_path_push("Details")
    let nested = widget_path_key("Value")
    container_path_pop()
    t |> equal(widget_path_key("Value"), left)
    container_path_pop()
    container_path_push("Right")
    let right = widget_path_key("Value")
    container_path_pop()
    t |> equal(left, "Left/Value")
    t |> equal(nested, "Left/Details/Value")
    t |> equal(right, "Right/Value")
    container_path_push("Unfinished")
    imgui_boost_runtime::begin_frame()
    t |> equal(widget_path_key("Value"), "Value")
    container_path_push("Left")
    t |> equal(widget_path_key("Value"), left)
    container_path_pop()
}

[test, no_jit]
def changing_widget_ids_preserve_paths_across_cache_turnover(t : T?) {
    imgui_boost_runtime::begin_frame()
    container_path_push("Transient")
    let retained = widget_path_key("Retained")
    retained_widget_path = retained
    let starting_heap = heap_bytes_allocated()
    var valid = true
    for (i in range(10000)) {
        let leaf = "Row{i}"
        valid = valid && widget_path_key(leaf) == "Transient/{leaf}"
    }
    let full_heap = heap_bytes_allocated()
    for (i in range(10000, 20000)) {
        let leaf = "Row{i}"
        valid = valid && widget_path_key(leaf) == "Transient/{leaf}"
    }
    let extra_heap = heap_bytes_allocated() - full_heap
    container_path_pop()
    imgui_boost_runtime::begin_frame()
    let released_heap = heap_bytes_allocated()
    t |> success(released_heap <= starting_heap, "rollover releases retained table storage")
    container_path_push("Transient")
    let warm = widget_path_key("After churn")
    let before = string_heap_allocation_count()
    for (_i in range(100)) { valid = valid && widget_path_key("After churn") == warm }
    let allocations = string_heap_allocation_count() - before
    container_path_pop()
    unsafe(heap_collect(true, true))
    t |> success(valid, "changing ids retain exact path identity")
    t |> equal(extra_heap, 0ul, "retained path tables stop growing under transient ids")
    t |> equal(retained, "Transient/Retained", "previously returned paths remain valid")
    t |> equal(retained_widget_path, retained,
        "caller-held paths survive cache turnover and collection")
    t |> equal(allocations, 0ul, "steady-state reuse resumes after transient ids")
    retained_widget_path = ""
}

[test]
def borrowed_widget_identifiers_are_not_retained(t : T?) {
    imgui_boost_runtime::begin_frame()
    var parent = fixed_array(65u8, 108u8, 112u8, 104u8, 97u8, 0u8)
    var leaf = fixed_array(73u8, 116u8, 101u8, 109u8, 0u8)
    var retained : string
    unsafe {
        container_path_push(reinterpret<string>(addr(parent[0])))
        retained = widget_path_key(reinterpret<string>(addr(leaf[0])))
        container_path_pop()
    }
    container_path_push("Alpha")
    for (i in range(64)) { let path = widget_path_key("Sibling{i}") }
    container_path_pop()
    for (i in range(64)) {
        container_path_push("Parent{i}")
        let path = widget_path_key("Value")
        container_path_pop()
    }
    parent[0] = 90u8
    leaf[0] = 90u8
    container_path_push("Alpha")
    let before = string_heap_allocation_count()
    let reused = widget_path_key("Item")
    let allocated = string_heap_allocation_count() - before
    container_path_pop()
    t |> equal(reused, retained, "key value survives reused caller storage")
    t |> equal(allocated, 0ul, "cached identity owns both borrowed key components")
    container_path_push("Cleanup")
    for (i in range(10000)) { let path = widget_path_key("Cleanup{i}") }
    container_path_pop()
    imgui_boost_runtime::begin_frame()
}

[test, no_jit]
def cached_dynamic_keys_survive_collection(t : T?) {
    imgui_boost_runtime::begin_frame()
    var parent = clone_string("CollectedParent")
    var leaf = clone_string("CollectedLeaf")
    container_path_push(parent)
    let warm = widget_path_key(leaf)
    container_path_pop()
    parent = ""; leaf = "" // nolint:LINT010 drop caller roots before collection
    unsafe(heap_collect(true, true))
    container_path_push("CollectedParent")
    let before = string_heap_allocation_count()
    let cached = widget_path_key("CollectedLeaf")
    let allocations = string_heap_allocation_count() - before
    container_path_pop()
    t |> equal(cached, warm, "lookup after collection retains cached identity")
    t |> equal(allocations, 0ul, "collection does not turn cached keys into misses")
}
