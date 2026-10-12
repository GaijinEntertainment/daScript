options gen2

module _style030_global_table

// A module whose only public symbol is a global: the importer reads it from a
// generic body (one with a `| #` parameter), where the read stays an unresolved
// name until the generic is instantiated - which a lint of the importer alone
// never does. STYLE030 must still count the module as used.
let STYLE030_LEVELS <- [
    -127, -104, -83, -65, -49, -35, -22, -10, 1, 13, 25, 38, 53, 69, 89, 113,
]
