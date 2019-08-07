expect 30108:4

require UnitTest

struct StructureNotLocal
    a:TestObjectNotLocal

[export]
def test:bool
    var a:TestObjectNotLocal            // 30108
    var b:StructureNotLocal             // 30108
    var c:array<StructureNotLocal>      // 30108
    var d:table<int,StructureNotLocal>  // 30108
    return true

