//! intptr of a scalar does not compile: it takes a pointer,
//! smart_ptr, function or lambda, and a scalar carries no address
// intptr takes a pointer, a smart_ptr, a function or a lambda - a
// scalar carries no address
options gen2
// coverage instrumentation re-infers after its pass and would wrap
// the expected 31400 in an extra 31207 (macro failed to infer) -
// instrumenting a deliberately-failing compile is meaningless
options no_coverage = true

expect 31400    // concept_failed

def test {
    debug(intptr(13))
}
