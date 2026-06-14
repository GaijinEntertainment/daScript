// Regression test for issue #3065 (fuzzer): a generic function whose tuple-typed
// parameter defaults to an empty make-struct `<- {}` (parsed as table<auto;auto>)
// drove inferGenericType to return null in ExprCall's forward-type loop, where a
// DAS_ASSERTF assumed it never could ("how? it matched during findMatchingFunctions
// the same way"). In release that fell through to a null makeType and crashed in
// ExprMakeStruct::visit. The guard now returns cleanly, so the bad default surfaces
// as ordinary compile errors instead of a crash.
options gen2
expect 30161, 30254, 30341

tuple var0 {}

def var0(v; r : var0 <- {}) {
}

[export]
def n {
    var0(0)
}
