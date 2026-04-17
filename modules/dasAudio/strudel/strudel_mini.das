options gen2
options indenting = 4

module strudel_mini shared

require strudel/strudel_event
require strudel/strudel_time
require strudel/strudel_pattern
require math
require strings  // to_int, to_float, to_double, slice

// ─── Tokenizer ───

enum TokenKind {
    WORD        // bd, sd, c3, sawtooth
    NUMBER      // 3, 0.5
    TILDE       // ~ (silence)
    LBRACKET    // [
    RBRACKET    // ]
    LANGLE      // <
    RANGLE      // >
    LPAREN      // (
    RPAREN      // )
    COMMA       // ,
    STAR        // *
    SLASH       // /
    BANG        // !
    AT          // @
    QUESTION    // ?
    COLON       // :
    PIPE        // |
    EOF_TOKEN
}

struct Token {
    kind : TokenKind
    text : string
    pos  : int
}

// Tokenize a mini-notation string.
// Uses peek_data for O(1) per-character access instead of character_at (which is O(n)).
def tokenize(input : string) : array<Token> {
    var tokens : array<Token>
    peek_data(input) $(data) {
        var i = 0
        let n = length(data)
        while (i < n) {
            let ch = int(data[i])
            if (ch == ' ' || ch == '\t' || ch == '\n') {
                i ++
            } elif (ch == '~') {
                tokens |> push(Token(kind = TokenKind.TILDE, text = "~", pos = i))
                i ++
            } elif (ch == '[') {
                tokens |> push(Token(kind = TokenKind.LBRACKET, text = "[", pos = i))
                i ++
            } elif (ch == ']') {
                tokens |> push(Token(kind = TokenKind.RBRACKET, text = "]", pos = i))
                i ++
            } elif (ch == '<') {
                tokens |> push(Token(kind = TokenKind.LANGLE, text = "<", pos = i))
                i ++
            } elif (ch == '>') {
                tokens |> push(Token(kind = TokenKind.RANGLE, text = ">", pos = i))
                i ++
            } elif (ch == '(') {
                tokens |> push(Token(kind = TokenKind.LPAREN, text = "(", pos = i))
                i ++
            } elif (ch == ')') {
                tokens |> push(Token(kind = TokenKind.RPAREN, text = ")", pos = i))
                i ++
            } elif (ch == ',') {
                tokens |> push(Token(kind = TokenKind.COMMA, text = ",", pos = i))
                i ++
            } elif (ch == '*') {
                tokens |> push(Token(kind = TokenKind.STAR, text = "*", pos = i))
                i ++
            } elif (ch == '/') {
                tokens |> push(Token(kind = TokenKind.SLASH, text = "/", pos = i))
                i ++
            } elif (ch == '!') {
                tokens |> push(Token(kind = TokenKind.BANG, text = "!", pos = i))
                i ++
            } elif (ch == '@') {
                tokens |> push(Token(kind = TokenKind.AT, text = "@", pos = i))
                i ++
            } elif (ch == '?') {
                tokens |> push(Token(kind = TokenKind.QUESTION, text = "?", pos = i))
                i ++
            } elif (ch == ':') {
                tokens |> push(Token(kind = TokenKind.COLON, text = ":", pos = i))
                i ++
            } elif (ch == '|') {
                tokens |> push(Token(kind = TokenKind.PIPE, text = "|", pos = i))
                i ++
            } elif (ch >= '0' && ch <= '9') {
                let start = i
                while (i < n) {
                    let c = int(data[i])
                    if ((c >= '0' && c <= '9') || c == '.') {
                        i ++
                    } else {
                        break
                    }
                }
                tokens |> push(Token(kind = TokenKind.NUMBER, text = slice(input, start, i), pos = start))
            } elif ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
                let start = i
                while (i < n) {
                    let c = int(data[i])
                    // Include '#' so note names like "d#3" parse as a single WORD token
                    // (mini-notation treats # as a step char).
                    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '#') {
                        i ++
                    } else {
                        break
                    }
                }
                tokens |> push(Token(kind = TokenKind.WORD, text = slice(input, start, i), pos = start))
            } else {
                i ++  // skip unknown
            }
        }
    }
    tokens |> push(Token(kind = TokenKind.EOF_TOKEN, text = "", pos = length(input)))
    return <- tokens
}

// ─── Parser ───
// Recursive descent: input → Pattern directly (no intermediate AST).
//
// Grammar:
//   pattern   = stack_expr
//   stack_expr = seq_expr ("," seq_expr)*
//   seq_expr  = (element | "_")+   — "_" extends previous element's duration
//   element   = atom_expr modifier* ("@" NUMBER)?
//   atom_expr = WORD | NUMBER | "~" | "[" pattern "]" | "<" seq_expr+ ">"
//   modifier  = "*" NUMBER | "/" NUMBER | "!" NUMBER | "?" NUMBER?

struct Parser {
    tokens : array<Token>
    pos    : int = 0
}

def peek(p : Parser) : Token {
    return p.tokens[p.pos]
}

def advance(var p : Parser) : Token {
    let tok = p.tokens[p.pos]
    p.pos ++
    return tok
}

def expect_token(var p : Parser; kind : TokenKind) : Token {
    let tok = peek(p)
    if (tok.kind != kind) {
        panic("mini-notation parse error at {tok.pos}: expected {kind}, got {tok.kind} '{tok.text}'")
    }
    return advance(p)
}

def at_end(p : Parser) : bool {
    return p.tokens[p.pos].kind == TokenKind.EOF_TOKEN
}

// Is the current token the start of an element?
def is_element_start(p : Parser) : bool {
    let k = peek(p).kind
    return k == TokenKind.WORD || k == TokenKind.NUMBER || k == TokenKind.TILDE || k == TokenKind.LBRACKET || k == TokenKind.LANGLE
}

// Forward declarations via mutual recursion — daScript handles this.

// parse_pattern → stack_expr
def parse_pattern(var p : Parser) : Pattern {
    var first <- parse_seq(p)
    if (peek(p).kind != TokenKind.COMMA) {
        return <- first
    }
    // comma-separated = stack
    var layers : array<Pattern>
    layers |> emplace <| first
    while (peek(p).kind == TokenKind.COMMA) {
        advance(p)
        var next <- parse_seq(p)
        layers |> emplace <| next
    }
    return <- stack(layers)
}

// parse_seq → element+ → fastcat (or weighted_fastcat if _ or @ used)
def parse_seq(var p : Parser) : Pattern {
    var elements : array<Pattern>
    var weights : array<float>
    var has_weights = false
    while (is_element_start(p) || (peek(p).kind == TokenKind.WORD && peek(p).text == "_")) {
        // underscore = elongation: extends previous element's weight
        if (peek(p).kind == TokenKind.WORD && peek(p).text == "_") {
            advance(p)
            if (length(weights) > 0) {
                weights[length(weights) - 1] += 1.0
                has_weights = true
            }
            continue
        }
        var elem <- parse_element(p)
        var w = 1.0
        // @N after element = weight
        if (!at_end(p) && peek(p).kind == TokenKind.AT) {
            advance(p)
            if (!at_end(p) && peek(p).kind == TokenKind.NUMBER) {
                w = to_float(advance(p).text)
                has_weights = true
            }
        }
        elements |> emplace <| elem
        weights |> push(w)
    }
    if (length(elements) == 0) {
        return <- silence()
    }
    if (length(elements) == 1) {
        return <- elements[0]
    }
    if (has_weights) {
        return <- weighted_fastcat(elements, weights)
    }
    return <- fastcat(elements)
}

// parse_element → atom_expr modifier*
def parse_element(var p : Parser) : Pattern {
    var pat <- parse_atom(p)
    // apply modifiers
    while (!at_end(p)) {
        let k = peek(p).kind
        if (k == TokenKind.STAR) {
            advance(p)
            let num = expect_token(p, TokenKind.NUMBER)
            let n = to_double(num.text)
            pat <- pat |> fast(n)
        } elif (k == TokenKind.SLASH) {
            advance(p)
            let num = expect_token(p, TokenKind.NUMBER)
            let n = to_double(num.text)
            pat <- pat |> slow(n)
        } elif (k == TokenKind.BANG) {
            advance(p)
            let num = expect_token(p, TokenKind.NUMBER)
            let n = to_int(num.text)
            // replicate: "bd!3" ≈ fast(3, bd) — approximation since patterns aren't copyable
            pat <- pat |> fast(double(n))
        } elif (k == TokenKind.QUESTION) {
            advance(p)
            var prob = 0.5
            if (peek(p).kind == TokenKind.NUMBER) {
                prob = to_float(expect_token(p, TokenKind.NUMBER).text)
            }
            pat <- pat |> degrade(prob)
        } elif (k == TokenKind.COLON) {
            advance(p)
            let num = expect_token(p, TokenKind.NUMBER)
            let idx = to_int(num.text)
            let fn <- @capture(= idx) (e : Event) : Event {
                var r = e; r.n = idx; return r
            }
            pat <- pat |> fmap(fn)
        } else {
            break
        }
    }
    return <- pat
}

// parse_atom → WORD | NUMBER | ~ | [ pattern ] | < seq+ >
def parse_atom(var p : Parser) : Pattern {
    let tok = peek(p)
    if (tok.kind == TokenKind.WORD) {
        advance(p)
        // check if it's a note name like c3, eb4, fs2
        let midi = parse_note_name(tok.text)
        if (midi >= 0.0) {
            return <- pure(Event(s = "sine", note = midi, attack = 0.01, release = 0.2))
        }
        return <- atom(tok.text)
    } elif (tok.kind == TokenKind.NUMBER) {
        advance(p)
        let note = to_float(tok.text)
        return <- pure(Event(note = note))
    } elif (tok.kind == TokenKind.TILDE) {
        advance(p)
        return <- silence()
    } elif (tok.kind == TokenKind.LBRACKET) {
        // [ pattern ] = subsequence (subdivide into current slot)
        advance(p)
        var inner <- parse_pattern(p)
        expect_token(p, TokenKind.RBRACKET)
        return <- inner
    } elif (tok.kind == TokenKind.LANGLE) {
        // < a b c > = alternate per cycle (slowcat)
        // each space-separated item is one alternative
        advance(p)
        var alts : array<Pattern>
        while (peek(p).kind != TokenKind.RANGLE && !at_end(p)) {
            var alt <- parse_element(p)
            alts |> emplace <| alt
        }
        expect_token(p, TokenKind.RANGLE)
        return <- cat(alts)
    }
    panic("mini-notation parse error at {tok.pos}: unexpected '{tok.text}'")
    return <- silence()
}

// ─── Note Name Parsing ───

// Try to parse a note name like "c3", "eb4", "fs2" → MIDI number.
// Returns -1.0 if not a note name.
// Parse a note name with explicit octave. "d3", "eb4", "fs3". Returns -1 on miss.
// Used by the mini-notation tokenizer — bare letters like "d" are treated as sounds,
// not notes, so `s("a e i")` can be vowels or sound names without collision.
def parse_note_name(name : string) : float {
    let n = length(name)
    if (n < 2 || n > 3) {
        return -1.0
    }
    var result = -1.0
    peek_data(name) $(bytes) {
        let ch = int(bytes[0])
        var semitone = -1
        if (ch == 'c') { semitone = 0; }
        elif (ch == 'd') { semitone = 2; }
        elif (ch == 'e') { semitone = 4; }
        elif (ch == 'f') { semitone = 5; }
        elif (ch == 'g') { semitone = 7; }
        elif (ch == 'a') { semitone = 9; }
        elif (ch == 'b') { semitone = 11; }
        if (semitone < 0) {
            return
        }
        var octaveIdx = 1
        if (n == 3) {
            let mod = int(bytes[1])
            if (mod == 's' || mod == '#') {
                semitone ++
            } elif (mod == 'b') {
                semitone --
            } else {
                return
            }
            octaveIdx = 2
        }
        let octCh = int(bytes[octaveIdx])
        if (octCh < '0' || octCh > '9') {
            return
        }
        let octave = octCh - int('0')
        result = float((octave + 1) * 12 + semitone)
    }
    return result
}

// Parse a note name allowing octave to be omitted (defaults to default_oct,
// e.g. noteToMidi(note, defaultOctave=3)). "d", "d#", "d#3", "eb".
// Returns -1 if the string isn't a valid note.
def parse_note_name_default(name : string; default_oct : int) : float {
    let n = length(name)
    if (n < 1 || n > 3) {
        return -1.0
    }
    var result = -1.0
    peek_data(name) $(bytes) {
        let ch = int(bytes[0])
        var semitone = -1
        if (ch == 'c') { semitone = 0; }
        elif (ch == 'd') { semitone = 2; }
        elif (ch == 'e') { semitone = 4; }
        elif (ch == 'f') { semitone = 5; }
        elif (ch == 'g') { semitone = 7; }
        elif (ch == 'a') { semitone = 9; }
        elif (ch == 'b') { semitone = 11; }
        if (semitone < 0) {
            return
        }
        var octave = default_oct
        var nextIdx = 1
        if (n >= 2) {
            let mod = int(bytes[1])
            if (mod == 's' || mod == '#') {
                semitone ++
                nextIdx = 2
            } elif (mod == 'b') {
                semitone --
                nextIdx = 2
            }
        }
        if (nextIdx < n) {
            let octCh = int(bytes[nextIdx])
            if (octCh < '0' || octCh > '9') {
                return
            }
            octave = octCh - int('0')
        }
        result = float((octave + 1) * 12 + semitone)
    }
    return result
}

// ─── Bar-aware parsing ───

// Parse notation with | bar separators.
// "A B | C D E" = cat([fastcat([A, B]), fastcat([C, D, E])])
// Each bar plays for one cycle; notes within a bar subdivide evenly.
def parse_bars(var p : Parser) : Pattern {
    // check if there are any PIPE tokens — if not, fall back to regular parsing
    var has_pipes = false
    for (tok in p.tokens) {
        if (tok.kind == TokenKind.PIPE) {
            has_pipes = true
            break
        }
    }
    if (!has_pipes) {
        return <- parse_pattern(p)
    }
    // collect bars separated by |
    var bars : array<Pattern>
    while (!at_end(p)) {
        if (is_element_start(p)) {
            var bar <- parse_pattern(p)
            bars |> emplace <| bar
        } else {
            // empty bar before pipe or at end
            var sil <- silence()
            bars |> emplace <| sil
        }
        if (peek(p).kind == TokenKind.PIPE) {
            advance(p)
        } else {
            break
        }
    }
    if (length(bars) == 0) {
        return <- silence()
    }
    if (length(bars) == 1) {
        return <- bars[0]
    }
    return <- cat(bars)
}

// ─── Public API ───

// Parse mini-notation and return a Pattern of sound events.
// s("bd sd [hh hh] cp") → fastcat([atom("bd"), atom("sd"), fastcat([atom("hh"), atom("hh")]), atom("cp")])
def s(notation : string) : Pattern {
    var tokens <- tokenize(notation)
    var p = Parser()
    p.tokens <- tokens
    return <- parse_pattern(p)
}

// Parse mini-notation as numeric values (note field holds the number).
// n("<2 16>") → pattern alternating 2.0 and 16.0 per cycle.
// Useful as patternified parameter: slow(pat, n("<2 16>"))
def n(notation : string) : Pattern {
    return <- s(notation)
}

// pat |> set_vowel("a") — static vowel, or
// pat |> set_vowel("<a e i o u>") — cycling vowel from mini-notation.
// Single token (no spaces, brackets, or angle brackets) → static fmap.
// Otherwise → parse as mini-notation pattern, sample at each event's onset.
def set_vowel(var pat : Pattern; notation : string) : Pattern {
    // detect mini-notation: contains space, <, >, [, ], *, or comma
    var is_pattern = false
    for (ch in notation) {
        if (ch == ' ' || ch == '<' || ch == '>' || ch == '[' || ch == ']' || ch == '*' || ch == ',') {
            is_pattern = true
            break
        }
    }
    if (!is_pattern) {
        // static vowel — simple fmap
        let v = notation
        return fmap(pat, @capture(= v) (e : Event) : Event {
            var r = e; r.vowel = v; return r
        })
    }
    // mini-notation pattern — parse and combineWithStr
    let vowel_pat <- s(notation)
    return combineWithStr(pat, vowel_pat, @(e : Event; val : string) : Event {
        var r = e; r.vowel = val; return r
    })
}

// pat |> set_vowel(s("<a e i o u>") |> slow(5.0lf)) — set vowel from a pre-built pattern.
// Use when the vowel pattern needs transforms (slow, fast, rev) before applying.
def set_vowel(var pat : Pattern; var vowel_pat : Pattern) : Pattern {
    return combineWithStr(pat, vowel_pat, @(e : Event; val : string) : Event {
        var r = e; r.vowel = val; return r
    })
}

// pat |> set_s("<supersaw sawtooth>") — strudel-style .s() with mini-notation.
// Single token → static set_sound. Mini-notation → parse and sample at onset.
def set_s(var pat : Pattern; notation : string) : Pattern {
    // detect mini-notation: contains space, <, >, [, ], *, or comma
    var is_pattern = false
    for (ch in notation) {
        if (ch == ' ' || ch == '<' || ch == '>' || ch == '[' || ch == ']' || ch == '*' || ch == ',') {
            is_pattern = true
            break
        }
    }
    if (!is_pattern) {
        return set_sound(pat, notation)
    }
    let sound_pat <- s(notation)
    return set_sound(pat, sound_pat)
}

// Parse mini-notation for note patterns.
// note("c3 ~ e3 g3") → pattern with note events (sound defaults to "sine")
def note_pattern(notation : string; sound : string = "sine") : Pattern {
    let pat <- s(notation)
    // Post-process: any event whose .s is a note name (including bare "d", "d#"
    // without octave) gets converted to .note via default-octave parsing.
    // parse_atom already converts tokens with explicit octave ("d3"); this pass
    // handles the shorthand where the octave is implicit (default 3).
    let converted <- pat |> fmap(@(e : Event) : Event {
        var r = e
        if (!empty(r.s)) {
            let midi = parse_note_name_default(r.s, 3)
            if (midi >= 0.0) {
                r.note = midi
                r.s = ""
            }
        }
        return r
    })
    return <- converted |> set_sound(sound)
}

// Parse mini-notation with | bar separators for sequential melodies.
// "e4 e4 f4 g4 | g4 f4 e4 d4" → each bar is one cycle, notes subdivide evenly.
def seq(notation : string; sound : string = "sine") : Pattern {
    var tokens <- tokenize(notation)
    var p = Parser()
    p.tokens <- tokens
    let pat <- parse_bars(p)
    return <- pat |> set_sound(sound)
}

// ─── every() convenience helpers ───
// These re-parse the notation to create two independent patterns,
// since lambdas aren't copyable.

// every_fast(4, 2.0, "bd sd hh cp") — double speed every 4th cycle
def every_fast(n : int; factor : double; notation : string) : Pattern {
    let on <- s(notation) |> fast(factor)
    let off <- s(notation)
    return <- every(n, on, off)
}

// every_slow(4, 2.0, "bd sd hh cp") — half speed every 4th cycle
def every_slow(n : int; factor : double; notation : string) : Pattern {
    let on <- s(notation) |> slow(factor)
    let off <- s(notation)
    return <- every(n, on, off)
}

// every_degrade(4, 0.5, "bd sd hh cp") — randomly drop events every 4th cycle
def every_degrade(n : int; prob : float; notation : string) : Pattern {
    let on <- s(notation) |> degrade(prob)
    let off <- s(notation)
    return <- every(n, on, off)
}

// ─── Bare-Name Aliases ───

// note("c3 e3 g3") — alias for note_pattern
def note(notation : string; sound : string = "sine") : Pattern { return <- note_pattern(notation, sound); }

// vowel("a") or vowel("<a e i>") — alias for set_vowel (string overload)
def vowel(var pat : Pattern; notation : string) : Pattern { return <- set_vowel(pat, notation); }

// vowel(s("<a e i>") |> slow(4.0lf)) — alias for set_vowel (Pattern overload)
def vowel(var pat : Pattern; var vowel_pat : Pattern) : Pattern { return <- set_vowel(pat, vowel_pat); }
