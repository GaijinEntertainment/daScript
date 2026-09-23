# daslib architecture notes - syntax twins

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.
The pairs below are daslib facts a C++ site must agree with, and nothing fails on a mismatch:
no lint, no compile error, no default-suite test.

## 42. The raw assignment spellings

- **`das_source_formatter.das`, `greedy_match_safe_tokens`** lists `!==`, `!<-` and `!:=` as
  three-character tokens. Its twin is the lexer, `src/parser/ds2_lexer.lpp` (repo root): the
  rules returning `NOTEQUEQU`, `NOTLARROW` and `NOTCLONEEQU`. A spelling the lexer stops
  accepting, or a new raw spelling it adds, changes both lists.
- **`ast_print.das`, the `preVisitExprCopyRight` / `preVisitExprMoveRight` /
  `preVisitExprCloneRight` overrides** print ` !== `, ` !<- ` and ` !:= ` when the node's
  `no_promotion` bit is set. Their twin is the parser, `src/parser/ds2_parser.ypp` (repo root),
  the `expr_assign_no_bracket` rules that build the node from those tokens: what the printer
  writes must parse back to the same node.
- **`builtin.das`, the `copy_to_move` / `copy_to_move_ref` / `move_to_move` /
  `move_to_move_ref` generics** are the names inference emits for a promoted initializer. Their
  twin is `promoteInitToAssign` and `isAssignInitCall` in `src/ast/ast_infer_type_function.cpp`
  (repo root), described in `src/ast/ARCHITECTURE_INFER.md` sec.5: the helper picked by the
  source's shape, and the names the re-promotion guard recognizes.
