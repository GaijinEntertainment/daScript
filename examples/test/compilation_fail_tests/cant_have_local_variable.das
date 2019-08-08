expect 30108:4, 30101:3

require UnitTest

struct StructureNotLocal
    a:TestObjectNotLocal                // 30101: structure field has to be 'local'

[export]
def test:bool
    var a:TestObjectNotLocal            // 30108
    var b:StructureNotLocal             // 30108
    var c:array<StructureNotLocal>      // 30108
                                        // 30101: array type has to be 'local'
    var d:table<int,StructureNotLocal>  // 30108
                                        // 30101: table value has to be 'local'
    return true

