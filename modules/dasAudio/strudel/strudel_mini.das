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
    peek_data(input) <| $(data) {
        var i = 0
        let n = length(data)
        while (i < n) {
            let ch = int(data[i])
            if (ch == ' ' || ch == '\t' || ch == '\n') {
                i += 1
            } elif (ch == '~') {
                tokens |> push(Token(kind = TokenKind.TILDE, text = "~", pos = i))
                i += 1
            } elif (ch == '[') {
                tokens |> push(Token(kind = TokenKind.LBRACKET, text = "[", pos = i))
                i += 1
            } elif (ch == ']') {
                tokens |> push(Token(kind = TokenKind.RBRACKET, text = "]", pos = i))
                i += 1
            } elif (ch == '<') {
                tokens |> push(Token(kind = TokenKind.LANGLE, text = "<", pos = i))
                i += 1
            } elif (ch == '>') {
                tokens |> push(Token(kind = TokenKind.RANGLE, text = ">", pos = i))
                i += 1
            } elif (ch == '(') {
                tokens |> push(Token(kind = TokenKind.LPAREN, text = "(", pos = i))
                i += 1
            } elif (ch == ')') {
                tokens |> push(Token(kind = TokenKind.RPAREN, text = ")", pos = i))
                i += 1
            } elif (ch == ',') {
                tokens |> push(Token(kind = TokenKind.COMMA, text = ",", pos = i))
                i += 1
            } elif (ch == '*') {
                tokens |> push(Token(kind = TokenKind.STAR, text = "*", pos = i))
                i += 1
            } elif (ch == '/') {
                tokens |> push(Token(kind = TokenKind.SLASH, text = "/", pos = i))
                i += 1
            } elif (ch == '!') {
                tokens |> push(Token(kind = TokenKind.BANG, text = "!", pos = i))
                i += 1
            } elif (ch == '@') {
                tokens |> push(Token(kind = TokenKind.AT, text = "@", pos = i))
                i += 1
            } elif (ch == '?') {
                tokens |> push(Token(kind = TokenKind.QUESTION, text = "?", pos = i))
                i += 1
            } elif (ch == ':') {
                tokens |> push(Token(kind = TokenKind.COLON, text = ":", pos = i))
                i += 1
            } elif (ch >= '0' && ch <= '9') {
                let start = i
                while (i < n) {
                    let c = int(data[i])
                    if ((c >= '0' && c <= '9') || c == '.') {
                        i += 1
                    } else {
                        break
                    }
                }
                tokens |> push(Token(kind = TokenKind.NUMBER, text = slice(input, start, i), pos = start))
            } elif ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
                let start = i
                while (i < n) {
                    let c = int(data[i])
                    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
                        i += 1
                    } else {
                        break
                    }
                }
                tokens |> push(Token(kind = TokenKind.WORD, text = slice(input, start, i), pos = start))
            } else {
                i += 1  // skip unknown
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
//   seq_expr  = element+
//   element   = atom_expr modifier*
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
    p.pos += 1
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

// parse_seq → element+ → fastcat
def parse_seq(var p : Parser) : Pattern {
    var elements : array<Pattern>
    while (is_element_start(p)) {
        var elem <- parse_element(p)
        elements |> emplace <| elem
    }
    if (length(elements) == 0) {
        return <- silence()
    }
    if (length(elements) == 1) {
        return <- elements[0]
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
            pat <- fast(n, pat)
        } elif (k == TokenKind.SLASH) {
            advance(p)
            let num = expect_token(p, TokenKind.NUMBER)
            let n = to_double(num.text)
            pat <- slow(n, pat)
        } elif (k == TokenKind.BANG) {
            advance(p)
            let num = expect_token(p, TokenKind.NUMBER)
            let n = to_int(num.text)
            // replicate: "bd!3" ≈ fast(3, bd) — approximation since patterns aren't copyable
            pat <- fast(double(n), pat)
        } elif (k == TokenKind.QUESTION) {
            advance(p)
            var prob = 0.5
            if (peek(p).kind == TokenKind.NUMBER) {
                prob = to_float(expect_token(p, TokenKind.NUMBER).text)
            }
            pat <- degrade(prob, pat)
        } elif (k == TokenKind.COLON) {
            advance(p)
            let num = expect_token(p, TokenKind.NUMBER)
            let idx = to_int(num.text)
            var fn <- @capture(= idx) (e : Event) : Event {
                var r = e; r.n = idx; return r
            }
            pat <- fmap(fn, pat)
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
            return <- pure(Event(s = "sine", note = midi, gain = 0.3, attack = 0.01, release = 0.2))
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
def parse_note_name(name : string) : float {
    let n = length(name)
    if (n < 2 || n > 3) {
        return -1.0
    }
    let ch = first_character(name)
    var semitone = -1
    if (ch == 'c') { semitone = 0; }
    elif (ch == 'd') { semitone = 2; }
    elif (ch == 'e') { semitone = 4; }
    elif (ch == 'f') { semitone = 5; }
    elif (ch == 'g') { semitone = 7; }
    elif (ch == 'a') { semitone = 9; }
    elif (ch == 'b') { semitone = 11; }
    if (semitone < 0) {
        return -1.0
    }
    var octaveIdx = 1
    if (n == 3) {
        let mod = character_at(name, 1)
        if (mod == 's' || mod == '#') {
            semitone += 1
        } elif (mod == 'b') {
            semitone -= 1
        } else {
            return -1.0
        }
        octaveIdx = 2
    }
    let octCh = character_at(name, octaveIdx)
    if (octCh < '0' || octCh > '9') {
        return -1.0
    }
    let octave = int(octCh) - int('0')
    return float((octave + 1) * 12 + semitone)
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

// Parse mini-notation for note patterns.
// note("c3 ~ e3 g3") → pattern with note events (sound defaults to "sine")
def note_pattern(notation : string; sound : string = "sine") : Pattern {
    var pat <- s(notation)
    return <- set_sound(sound, pat)
}

// ─── every() convenience helpers ───
// These re-parse the notation to create two independent patterns,
// since lambdas aren't copyable.

// every_fast(4, 2.0, "bd sd hh cp") — double speed every 4th cycle
def every_fast(n : int; factor : double; notation : string) : Pattern {
    var on <- fast(factor, s(notation))
    var off <- s(notation)
    return <- every(n, on, off)
}

// every_slow(4, 2.0, "bd sd hh cp") — half speed every 4th cycle
def every_slow(n : int; factor : double; notation : string) : Pattern {
    var on <- slow(factor, s(notation))
    var off <- s(notation)
    return <- every(n, on, off)
}

// every_degrade(4, 0.5, "bd sd hh cp") — randomly drop events every 4th cycle
def every_degrade(n : int; prob : float; notation : string) : Pattern {
    var on <- degrade(prob, s(notation))
    var off <- s(notation)
    return <- every(n, on, off)
}
