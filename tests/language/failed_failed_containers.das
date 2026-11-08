//! pushing or emplacing a temporary into an array of a struct does
//! not compile - the containers suite recompiled with FAIL_TO_COMPILE
//! on
expect 30341:4

options gen2
let FAIL_TO_COMPILE = true

include containers.inc
