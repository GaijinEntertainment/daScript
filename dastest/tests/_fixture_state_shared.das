options gen2

module _fixture_state_shared shared

//! the global two together-fixtures both claim as fresh: a shared
//! context would carry the first file's write into the second
var public g_hits = 0
