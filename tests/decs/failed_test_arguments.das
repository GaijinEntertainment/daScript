//! query() blocks with an optional argument that has a default or is
//! not a pointer, or a default or reference on a const argument, must
//! not compile.
options gen2
expect 30123:2, 50503:4

options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_fail_optional(t : T?) {                    // 33101: unsupported call macro query
    // 40104: optional argument i can't have a default value
    query() $(@optional var i : int? = null) {
        // 30103: block argument default value type mismatch int?& -const vs
        // void? const, reference matter
        pass
    }
    query() $(@optional var i : int) {     // 40104: optional argument i must be a pointer
        pass                                // 33101: unsupported call macro query
    }
}

[test]
def test_fail_default(t : T?) {         // 33101: unsupported call macro query
    query() $(var i : int? = null) {   // 40104: argument i has default value, it can't be & or var
        // 30103: block argument default value type mismatch int?& -const vs
        // void? const, reference matters
        pass
    }
    query() $(i : int&) {   // 40104: argument i is both & and constant
        pass                 // 33101: unsupported call macro query
    }
}
