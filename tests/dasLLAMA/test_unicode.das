options gen2

require dastest/testing_boost public
require dasllama/dasllama_unicode

[test]
def test_unicode(t : T?) {
    // Category classification must match llama.cpp's unicode_cpt_flags_from_cpt.
    t |> run("letters") @(t : T?) {
        t |> success(cpt_is_letter(65))      // 'A'
        t |> success(cpt_is_letter(122))     // 'z'
        t |> success(cpt_is_letter(196))     // 'Ä' U+00C4
        t |> success(!cpt_is_letter(48))     // '0'
        t |> success(!cpt_is_letter(32))     // space
        t |> success(!cpt_is_letter(33))     // '!'
    }
    t |> run("numbers") @(t : T?) {
        t |> success(cpt_is_number(48))      // '0'
        t |> success(cpt_is_number(57))      // '9'
        t |> success(cpt_is_number(189))     // '½' U+00BD (category No)
        t |> success(!cpt_is_number(65))     // 'A'
        t |> success(!cpt_is_number(32))     // space
    }
    t |> run("whitespace") @(t : T?) {
        t |> success(cpt_is_whitespace(32))  // space
        t |> success(cpt_is_whitespace(9))   // tab
        t |> success(cpt_is_whitespace(10))  // newline
        t |> success(cpt_is_whitespace(13))  // CR
        t |> success(cpt_is_whitespace(12288)) // U+3000 ideographic space
        t |> success(!cpt_is_whitespace(65)) // 'A'
        t |> success(!cpt_is_whitespace(48)) // '0'
    }
    // UTF-8 codec round-trips, including a multi-byte codepoint.
    t |> run("utf8 round-trip") @(t : T?) {
        let cpts <- utf8_to_cpts("Äpfel")
        t |> success(length(cpts) == 5)
        t |> success(cpts[0] == 196)         // Ä
        t |> success(cpts[1] == 112)         // p
        var rebuilt = ""
        for (c in cpts) {
            rebuilt += cpt_to_utf8(c)
        }
        t |> success(rebuilt == "Äpfel")
    }
}
