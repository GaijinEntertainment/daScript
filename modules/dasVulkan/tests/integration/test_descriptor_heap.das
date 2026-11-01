options gen2
options _comment_hygiene = true
options indenting = 4

// The descriptor-heap example squares HEAP_N indices with a storage
// buffer the shader reaches through a resource heap rather than a
// descriptor set; the result is exact integer arithmetic, so every
// element is checked. Skips (reported as skipped) where
// VK_EXT_descriptor_heap is absent. The heap layout arithmetic is
// pure and is checked on hand-built property values, so it runs
// everywhere.

require dastest/testing_boost public
require ../../examples/descriptor_heap.das

[test]
def test_descriptor_heap_square(t : T?) {
    if (!descriptor_heap_available()) {
        t |> skip("VK_EXT_descriptor_heap not advertised by this device")
        return
    }
    let values <- run_descriptor_heap_square()
    t |> equal(length(values), HEAP_N, "the readback holds every element")
    var wrong = 0
    for (i in range(length(values))) {
        if (values[i] != uint(i * i)) {
            wrong++
        }
    }
    t |> equal(wrong, 0, "every out[i] is i*i")
}

//! property values shaped like a real device's
def heap_props(reserved, buf_size, buf_align, heap_align,
               max_size : uint64) : VkPhysicalDeviceDescriptorHeapPropertiesEXT {
    var p = VkPhysicalDeviceDescriptorHeapPropertiesEXT()
    p.minResourceHeapReservedRange = reserved
    p.bufferDescriptorSize = buf_size
    p.bufferDescriptorAlignment = buf_align
    p.resourceHeapAlignment = heap_align
    p.maxResourceHeapSize = max_size
    return p
}

def panics(blk : block<() : void>) : bool => !empty(panic_text(blk))

[test]
def test_resource_heap_layout(t : T?) {
    let big = 0x2000000ul
    t |> run("reserved range first, descriptors aligned behind it, size rounded up") @(t : T?) {
        let lay = resource_heap_layout(heap_props(0x17a04ul, 16ul, 8ul, 64ul, big), 3)
        t |> equal(lay.reserved, 0x17a04ul)
        t |> equal(lay.first, 0x17a08ul, "the first descriptor sits at the next alignment")
        t |> equal(lay.size, 0x17a40ul, "3 x 16 bytes after 0x17a08 is 0x17a38, rounded to 64")
    }
    t |> run("zero descriptors still carry the reserved range") @(t : T?) {
        let lay = resource_heap_layout(heap_props(0x200ul, 16ul, 8ul, 64ul, big), 0)
        t |> equal(lay.first, 0x200ul)
        t |> equal(lay.size, 0x200ul)
    }
    t |> run("a heap past maxResourceHeapSize is refused") @(t : T?) {
        let props = heap_props(0x200ul, 16ul, 8ul, 64ul, 0x400ul)
        t |> equal(resource_heap_layout(props, 32).size, 0x400ul, "32 descriptors fill the limit")
        t |> success(panics($ { resource_heap_layout(props, 33) }), "the 33rd crosses the limit")
    }
    t |> run("zeroed props and a negative count are refused") @(t : T?) {
        let zeroed = VkPhysicalDeviceDescriptorHeapPropertiesEXT()
        t |> success(panics($ { resource_heap_layout(zeroed, 1) }), "props without the extension")
        let props = heap_props(0x200ul, 16ul, 8ul, 64ul, big)
        t |> success(panics($ { resource_heap_layout(props, -1) }), "a negative count")
    }
}
