# dasLR1 architecture

This document follows the repository's `ARCHITECTURE_COMMON.md` contract. dasLR1 is a pure daslang
module: an LALR(1) parser generator over byte terminals built at run time, an incremental acceptor
over its tables, a reader for GBNF grammar text, and a JSON-schema compiler that emits GBNF. Its one
consumer in this tree is the token constraint of `utils/dasllama-server`, and it depends on nothing
under `modules/`.

## File charters

- `lr1/lr1.das` - the grammar data (`GrammarBuilder`, `Grammar`), the LALR(1) table builder and the
  acceptor (`LrStack`, `lr_start`, `lr_step`, `lr_accepts`, `lr_at_end`, `lr_next_bytes`,
  `lr_step_bytes`, `lr_matches`). Knows nothing of grammar text or schemas.
- `lr1/gbnf.das` - GBNF text to a `Grammar` (`gbnf_grammar`), the repetition cap `GBNF_REPEAT_CAP`
  and the hex-digit reader `hex_digit_value`. Owns the dialect: literals, classes, repetition
  suffixes, groups, comments, rule continuation. Never a parser of anything but grammar text.
- `lr1/json_schema.das` - a JSON schema to GBNF text (`json_schema_to_gbnf`, `gbnf_literal`) and to a
  `Grammar` (`json_schema_grammar`), over its own order-keeping schema tree (`parse_schema`, `SNode`).
- `tests/` - the module's tests, the lifted corpora they run (`_gbnf_corpus.json`,
  `_schema_corpus.json`, `_gbnf/*.gbnf`) and the program that lifts them (`_lift_corpora.das`).

## Byte classes as terminals

A terminal is a byte class, not a byte. The builder partitions the 256 byte values into the finest
classes the grammar's byte sets agree on, so two bytes no production tells apart share one class and one
action column. A JSON grammar needs about forty classes where it would need 256 byte columns, and the
action table (`action[state * (nclasses + 1) + cls]`, the end-of-input column last) stays small enough to
build per request. `byte_class` maps a byte to its class at step time. Item keys are 64-bit: a
production index times 4096 plus the dot, times 1024 plus the lookahead, which a grammar of a thousand
rules puts past 32 bits.

## Conflicts kept as a set of stacks {#stack-set}

The tables are LALR(1): LR(0) kernels with lookaheads propagated the dragon-book way. A cell that ends
up with more than one action is not an error. The first action stays in `action`, the others in `alt`
keyed by the cell index, `conflicts` counts such cells and `first_conflict` names the first. The acceptor
is then a set of LR stacks (`LrStack.stacks`): at a conflicting cell it forks, a stack that cannot act
dies, and two stacks with the same states are one. The language accepted is exactly the grammar's, so a
grammar author - a schema compiler, or a hand-written GBNF with adjacent optionals or repeated sets -
never has to make the grammar LALR(1). A deterministic grammar runs one stack and pays nothing for the
fork machinery. The set is capped at `LR_MAX_STACKS`: a byte whose reduce chains would grow it past the
cap is refused and the set stays as it was, never trimmed, because a trimmed set could drop the one
stack that completes the sentence and later refuse a byte of the grammar's own language. The refusal
is the acceptor's limit, not the grammar's: a grammar whose readings multiply along the input - a
right-recursive rule with two spellings of the same byte - hits it after a few bytes, and so does a
rule that derives the empty string through a cycle of itself. A grammar the schema compiler emits is
deterministic and never comes near it.

## The viable-prefix property {#viable-prefix}

A byte is accepted when some stack can, after its chain of reduces, shift the byte's class, and refused
the moment no stack can. For that acceptance to mean "some sentence of the grammar extends this prefix"
every rule the root reaches must derive some terminal string, so `build_grammar` refuses a grammar
whose reachable rule is unproductive (`Grammar.err` names it) and ignores rules the root cannot reach.
The property is what makes the acceptor usable as a token mask: a sampler that admits a token only while
every byte of it is accepted never has to undo an emitted prefix. `lr_at_end` reads acceptance at the
end-of-input column the same way, through the reduce chains, so a stop token is admitted exactly where
the sentence is complete; `lr_accepts` and `lr_next_bytes` answer for a byte, or for all 256, over the
same chains without moving the set.

## Probing on a copy

`lr_step` moves the stack set; a probe for "may these bytes come next" copies `LrStack.stacks` and
steps the copy (`lr_step_bytes` on the copy, as the server's token constraint does). The copy is the
whole state - the tables are read-only - so a probe costs the stacks' depth and nothing else, and the
sampler can probe a thousand candidates against one state.

## GBNF, the dialect read

The dialect is the one llama.cpp's grammar files are written in, so its `grammars/*.gbnf` and its grammar
test corpus run unchanged: `name ::= alternatives`, `"literals"`, `[character classes]` with ranges and
negation, rule names, `( groups )`, `.` (any character, a newline included), the suffixes `* + ? {m} {m,}
{m,n}`, `#` comments, and `|` continuing a rule on the next line. Terminals are code points: a literal
or a class that reaches past ASCII compiles into the UTF-8 byte sequences of its ranges, so the grammar
it builds stays a byte grammar and a multi-byte character is accepted one continuation byte at a time.
Token references (`<[id]>`, `<name>`, `!<...>`) are refused: the acceptor sees bytes, and a caller that
wants to speak in tokens maps them to bytes itself. Two set rules with the same byte set are one rule.
A repetition `{m,n}` becomes `m` copies and a chain of `n - m` optional rules, one rule a copy, so a
bound past `GBNF_REPEAT_CAP` (1024) is refused: grammar text reaches the reader from a network request,
and a bound is the one knob that buys an unbounded build.

## The schema compiler is a port of the reference converter {#ported-converter}

`json_schema_to_gbnf` follows llama.cpp's `json-schema-to-grammar` rule for rule and emits the same
text, so the reference's conversion corpus is the compiler's oracle and a behavioral difference shows up as
a text difference. Properties come out in declaration order with the required ones first and the optional
ones as a chain of optional tails; `enum` and `const` as literals; integer `minimum`/`maximum` as digit-range
alternatives; `minLength`/`maxLength` and `minItems`/`maxItems` as repetitions; a local `$ref` by
definition; `anyOf`/`oneOf` as alternatives, `allOf` merged; a string `pattern` as a regular expression
over the string's characters; `additionalProperties` through a trie that excludes the declared keys; the
`date`, `time` and `uuid` formats as fixed rules; a `type` that is neither a name nor a list of names is
refused as the reference refuses it. A repetition the schema names (`minItems`, `maxItems`,
`minLength`, `maxLength`, a pattern's `{m,n}`) is held to the reader's `GBNF_REPEAT_CAP` before the text
is emitted, for the same reason the reader holds it. Keys keep their declaration order, which daslib's
JSON tables drop, so the schema is parsed into the module's own `SNode` tree. The file raises the
per-module `_cyclomatic_complexity` and `_function_length` options because the port keeps the reference's
visitor and pattern-lexer shape; that raise covers the ported code alone, and is the one such raise under
`lr1/`.

## Corpora

The two JSON corpora are written by `tests/_lift_corpora.das` over a llama.cpp checkout
(`daslang modules/dasLR1/tests/_lift_corpora.das -- <llama.cpp root>`; the committed files come from
commit 98c4764b6):

- `tests/_gbnf_corpus.json` - every `test_grammar` and `test_schema` call of
  `tests/test-grammar-integration.cpp`: the grammar or schema text, the strings it accepts, the strings
  it refuses. The two grammar cases naming token references are left out, since the acceptor sees bytes.
- `tests/_schema_corpus.json` - every `test({...})` case of `tests/test-json-schema-to-grammar.cpp`: its
  verdict, the schema, the grammar text the reference emits, compared line by line with the indentation
  stripped.
- `tests/_gbnf/*.gbnf` - llama.cpp's sample grammars, copied whole, each of which must build.

Each is MIT-licensed by the ggml authors: the notice sits in the `.LICENSE` file beside each JSON corpus
and in `tests/_gbnf/LICENSE` for the grammar files.
