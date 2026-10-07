# dasLR1 architecture

This document follows the repository's `ARCHITECTURE_COMMON.md` contract. dasLR1 is a pure daslang
module: an LALR(1) parser generator over byte terminals built at run time, an incremental acceptor
over its tables, a reader for GBNF grammar text, and a JSON-schema compiler that emits GBNF. Its one
consumer in this tree is the token constraint of `utils/dasllama-server`, and it depends on nothing
under `modules/`.

## File charters

- `lr1/lr1.das` - the grammar data (`GrammarBuilder`, `Grammar`), the LALR(1) table builder and the
  acceptor (`LrStack`, `lr_start`, `lr_step`, `lr_at_end`, `lr_next_bytes`, `lr_step_bytes`,
  `lr_matches`). Knows nothing of grammar text or schemas.
- `lr1/gbnf.das` - GBNF text to a `Grammar` (`gbnf_grammar`). Owns the dialect: literals, classes,
  repetition suffixes, groups, comments, rule continuation. Never a parser of anything but grammar text.
- `lr1/json_schema.das` - a JSON schema to GBNF text (`json_schema_to_gbnf`, `gbnf_literal`) and to a
  `Grammar` (`json_schema_grammar`), over its own order-keeping schema tree (`parse_schema`, `SNode`).
- `tests/` - the module's tests and the lifted corpora they run (`_gbnf_corpus.json`,
  `_schema_corpus.json`, `_gbnf/*.gbnf`), each corpus beside its license file.

## Byte classes as terminals

A terminal is a byte class, not a byte. The builder partitions the 256 byte values into the finest
classes the grammar's byte sets agree on, so two bytes no production tells apart share one class and one
action column. A JSON grammar needs about forty classes where it would need 256 byte columns, and the
action table (`action[state * (nclasses + 1) + cls]`, the EOF column last) stays small enough to build
per request. `byte_class` maps a byte to its class at step time.

## Conflicts kept as a set of stacks {#stack-set}

The tables are LALR(1): LR(0) kernels with lookaheads propagated the dragon-book way. A cell that ends
up with more than one action is not an error. The first action stays in `action`, the others in `alt`
keyed by the cell index, `conflicts` counts such cells and `first_conflict` names the first. The acceptor
is then a set of LR stacks (`LrStack.stacks`): at a conflicting cell it forks, a stack that cannot act
dies, and the set is capped at `LR_MAX_STACKS`, past which the acceptor refuses the byte. The language
accepted is exactly the grammar's, so a grammar author - a schema compiler, or a hand-written GBNF with
adjacent optionals or repeated sets - never has to make the grammar LALR(1). A deterministic grammar
runs one stack and pays nothing for the fork machinery.

## The viable-prefix property {#viable-prefix}

A byte is accepted when some stack can, after its chain of reduces, shift the byte's class, and refused
the moment no stack can. For that refusal to mean "no sentence of the grammar extends this prefix" every
reachable rule must derive some terminal string, so `build_grammar` refuses a grammar whose reachable rule
is unproductive (`Grammar.err` names it) and ignores rules the root cannot reach. The property is what
makes the acceptor usable as a token mask: a sampler that admits a token only while every byte of it is
accepted never has to undo an emitted prefix. `lr_at_end` reads acceptance at the EOF column the same
way, through the reduce chains, so a stop token is admitted exactly where the sentence is complete.

## Probing on a copy

`lr_step` moves the stack set; a probe for "may these bytes come next" copies `LrStack.stacks` and
steps the copy (`lr_step_bytes` on the copy, as the server's token constraint does). The copy is the
whole state - the tables are read-only - so a probe costs the stacks' depth and nothing else, and the
sampler can probe a thousand candidates against one state.

## GBNF, the dialect read

The dialect is the one llama.cpp's grammar files are written in, so its `grammars/*.gbnf` and its grammar
test corpus run unchanged: `name ::= alternatives`, `"literals"`, `[character classes]` with ranges and
negation, rule names, `( groups )`, `.`, the suffixes `* + ? {m} {m,} {m,n}`, `#` comments, and `|`
continuing a rule on the next line. Terminals are code points: a literal or a class that reaches past
ASCII compiles into the UTF-8 byte sequences of its ranges, so the grammar it builds stays a byte grammar
and a multi-byte character is accepted one continuation byte at a time. Token references (`<[id]>`,
`<name>`, `!<...>`) are refused: the acceptor sees bytes, and a caller that wants to speak in tokens maps
them to bytes itself. Two set rules with the same byte set are one rule.

## The schema compiler is a port of the reference converter {#ported-converter}

`json_schema_to_gbnf` follows llama.cpp's `json-schema-to-grammar` rule for rule and emits the same
text, so the reference's conversion corpus is the compiler's oracle and a behavioral difference shows up as
a text difference. Properties come out in declaration order with the required ones first and the optional
ones as a chain of optional tails; `enum` and `const` as literals; integer `minimum`/`maximum` as digit-range
alternatives; `minLength`/`maxLength` and `minItems`/`maxItems` as repetitions; a local `$ref` by
definition; `anyOf`/`oneOf` as alternatives, `allOf` merged; a string `pattern` as a regular expression
over the string's characters; `additionalProperties` through a trie that excludes the declared keys; the
`date`, `time` and `uuid` formats as fixed rules. Keys keep their declaration order, which daslib's JSON
tables drop, so the schema is parsed into the module's own `SNode` tree. The file raises the
per-module `_cyclomatic_complexity` and `_function_length` options because the port keeps the reference's
visitor and pattern-lexer shape; that raise covers the ported code alone.

## Corpora

- `tests/_gbnf_corpus.json` - the acceptance corpus lifted from llama.cpp's grammar integration test:
  grammar cases and schema cases, each with the strings it accepts and refuses.
- `tests/_schema_corpus.json` - the conversion corpus lifted from llama.cpp's schema-to-grammar test: a
  schema and the grammar text the reference emits, compared line by line with the indentation stripped.
- `tests/_gbnf/*.gbnf` - llama.cpp's sample grammars, each of which must build.

Each is MIT-licensed by the ggml authors, the notice in the `.LICENSE` file beside it.
