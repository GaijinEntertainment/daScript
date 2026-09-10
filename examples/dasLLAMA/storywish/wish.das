options gen2

require strings
require daslib/strings_boost

// The request side of storywish, pure: the typed line becomes the words the corpus spells, the
// words become the prompt in the corpus's own layout, and a line the model writes is recognized
// as a field the corpus puts after a story. main.das drives the screen and the speech thread;
// modules/dasLLAMA/tests/test_storywish.das tests these three directly.

//! the corpus orders its fields at random, so a story is sometimes followed by another field: a line opening with one ends the story
let FIELD_PREFIXES <- ["Summary:", "Words:", "Features:", "Random sentence:", "Story:"]

//! the typed line as the corpus spells a request: lower case, one space after each comma, no empties
def public wish_words(typed : string) : string {
    var words = ""
    for (part in split(to_lower(typed), ",")) {
        let w = strip(part)
        if (!empty(w)) {
            words = empty(words) ? "{w}" : "{words}, {w}"
        }
    }
    return words
}

//! the request in the corpus's own layout - the fields, then `Story:` and its line break
def public build_prompt(words : string; dialogue : bool) : string {
    return "{dialogue ? "Features: Dialogue\n" : ""}Words: {words}\nStory: \n"
}

//! a line the model has written so far opens with a field name: it has moved on from the story
def public line_is_a_field(line : string) : bool {
    let t = strip(line)
    for (prefix in FIELD_PREFIXES) {
        return true if (starts_with(t, prefix))
    }
    return false
}
