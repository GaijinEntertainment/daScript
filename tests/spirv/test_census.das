options gen2
options indenting = 4

// Census gate (gate B): the set of opcodes the emitter actually produces for the square must
// equal the declared Phase-1 set, in both directions. Stronger than line coverage — it catches
// an opcode whose emit line ran but was never asserted, and a declared opcode that stopped firing.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis

[test]
def test_opcode_census(t : T?) {
    t |> run("emitter opcode census == declared Phase-1 set") <| @(t : T?) {
        var words <- square_words()
        var present <- opcode_set(words)
        var declared <- phase1_emitter_opcodes()
        for (op in keys(present)) {
            t |> success(key_exists(declared, op), "emitted opcode {op} is in the declared set")
        }
        for (op in keys(declared)) {
            t |> success(key_exists(present, op), "declared opcode {op} is actually emitted")
        }
        delete present
        delete declared
        delete words
    }
}
