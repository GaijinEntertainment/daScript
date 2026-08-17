options gen2
options _comment_hygiene = true
options indenting = 4

// Hand-written boost runtime primitives that the GENERATED boost (handles,
// structs, commands) depends on. Kept separate from vulkan_boost.das so the
// generated files can require it without a cycle (vulkan_boost re-exports the
// generated files, so it sits above them).

module vulkan_runtime shared

require vulkan public

//! Result-checking convention used by every generated creator/command.
//! `out == null` -> assert SUCCESS (panic on any failure).
//! `out != null` -> write the result through the pointer; the caller decides.
def public vk_check(r : VkResult; var out : VkResult?) {
    if (out != null) {
        unsafe {
            *out = r
        }
    } elif (r != VkResult.SUCCESS) {
        panic("Vulkan call failed: {r}")
    }
}

//! Typed pointer to the first element of an array (vk_view marshalling). Empty -> null.
def public array_addr(var a : array<auto(TT)>) : TT? {
    if (empty(a)) {
        return null
    }
    return unsafe(addr(a[0]))
}

//! Intentional non-owning copy of an ownership wrapper: copy the struct, then
//! clear its `_needs_delete` so only the original frees the handle. Wrapper
//! structs store handles as uint64, so the copy is friction-free.
def public weak_copy(b : auto(TT)) : TT {
    var c = b
    static_if (typeinfo has_field<_needs_delete>(c)) {
        c._needs_delete = false
    }
    return c
}

//! VK_MAKE_API_VERSION(0, major, minor, patch)
def public make_api_version(major, minor, patch : uint) : uint {
    return (major << 22u) | (minor << 12u) | patch
}
