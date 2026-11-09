# daslib architecture notes - the documentation pipeline

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.

## 44. rst_comment {#comment-reader-closure}

The `//!` comment reader is a parser macro: `rst_comment` registers it, through
`add_new_comment_reader`, when its own `[_macro]` setup runs, and the parser collects the
registered readers afresh at the start of every module it parses. A module parsed before that
setup has run - every module in `rst_comment`'s own require closure - is parsed with no reader
and never gets a `detail/` topic for its `//!` comments; nothing re-parses it later, because a
shared module compiles once per process. The closure is therefore kept to builtin modules,
`daslib/ast` (which brings `daslib/rtti`) and `daslib/rst_label`: `rst_label` holds the label
and short-type-name helpers the reader and `rst` share, so the reader needs neither `rst` nor
`ast_boost`, and `rst` re-exports `rst_label` for its callers. `REVIEW.das` in this folder
walks the closure from `rst_comment` through every `daslib/` require and reports any module
outside that set. The three `daslib` modules left in the closure get no `detail/` topics:
`rst_label`'s pages are hand-written under `doc/source/stdlib/handmade/`, where `rst`'s
`is_handmade` list sends its lookups, and the `//!` lines of `ast.das` and `rtti.das` stay
unread. `detail/` is a generated, untracked directory: a stale file there from an earlier run
or an earlier binary serves a later run's lookup, so a digest regenerated on a box with an old
`detail/` differs from one regenerated on a clean checkout, and a regeneration meant to show
what the generator produces deletes `detail/` first.
