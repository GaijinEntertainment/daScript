// Companion fixture for test_annotation_lifetime.cpp.
// Deliberately NO `options rtti` — annotation reflection must work from the
// context debug heap alone, after the Program is released.

options gen2

require daslib/rtti

[comment(tag = "alive", level = 3)]
struct Annotated {
    @marker = 42 x : int
    y : int
}

[export]
def check : bool {
    let s = Annotated()
    let sinfo = (typeinfo rtti_typeinfo(s)).structType
    if (sinfo == null) {
        return false
    }
    var struct_ok = false
    for (ann in each_annotation(*sinfo)) {
        if (ann.name == "comment" && ann.count == 2u) {
            var tag_ok = false
            var level_ok = false
            for (arg in each_annotation_argument(ann)) {
                if (arg.name == "tag" && arg.sValue == "alive") {
                    tag_ok = true
                } elif (arg.name == "level" && arg.iValue == 3) {
                    level_ok = true
                }
            }
            struct_ok = tag_ok && level_ok
        }
    }
    var field_ok = false
    unsafe {
        for (fld in *sinfo) {
            if (fld.name == "x" && fld.annotation_argument_count == 1u) {
                for (arg in each_annotation_argument(fld)) {
                    if (arg.name == "marker" && arg.iValue == 42) {
                        field_ok = true
                    }
                }
            }
        }
    }
    return struct_ok && field_ok
}
