options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options _nolint = "STYLE042"
// STYLE030 over a generic body: a function with an `array<uint8> | #`
// parameter is a generic, and a lint of this file alone never instantiates
// it, so a global read inside it has no resolved Variable - only a spelled
// name. The name must keep its provider module alive, exactly as a spelled
// call name does. Only the genuinely unused module fires.

expect 31209:1

require daslib/style_lint
require _style030_global_table   // read ONLY from the generic body below - must NOT fire
require _style030_unused         // genuinely unused source module (FIRES)

[export]
def level_of(bytes : array<uint8> | #; i : int) : int {
    return STYLE030_LEVELS[int(bytes[int64(i)]) & 0xF]
}
