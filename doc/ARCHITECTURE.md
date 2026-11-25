# doc - the daslang.io documentation tree

This document follows `ARCHITECTURE_COMMON.md` (repo root). The rules a diff here is reviewed
against are in `REVIEW.md`.

## Files

- `source/` - the Sphinx tree the site is built from; `source/conf.py` configures the build.
- `source/stdlib/generated/` - the stdlib pages a generator writes: `reflections/das2rst.das`
  for `daslib/` and the in-tree modules, `imgui2rst` and `vulkan2rst` (under their modules) for
  dasImgui and dasVulkan. They are tracked in git and never edited by hand.
- `source/stdlib/handmade/` - the hand-written stubs, one per module and per symbol, that
  `das2rst.das` splices into the generated pages. `das2rst.das` writes a `// stub` entry for a
  symbol with no stub yet.
- `reflections/das2rst.das` - the stdlib generator, run as
  `daslang -documentation doc/reflections/das2rst.das` from the repo root (the
  `check_docs_generated` target). It also writes the module digest
  `skills/daslang/references/everything.md` (repo root).
- `check_docs_fresh.cmake` - the freshness guards the `check_docs_fresh` target runs after every
  generator: no `// stub` entry left in a handmade stub, no `Uncategorized` section in a
  generated page, no generated page untracked in git.
- `REVIEW.das` - the mechanical half of `REVIEW.md`: every host a built page loads from without
  a click is in `ALLOWED_HOSTS`, and `daslib/rst_comment` is the first require of `das2rst.das`.

## What the build proves

The doc build proves that every page renders. It does not run what a page shows a reader, and
it does not fetch what a page points a reader at. A page that shows a name the tree no longer
has, or a download that is gone, still builds. So the evidence that a shown name works, or that
a pointed-at URL exists, is a test or a check the PR names, not the build.

`rst_comment` installs the `//!` comment reader when it compiles. A module `das2rst.das`
requires before it compiles without that reader, and its page carries no `//!` text.

`daslang.io` serves the built tree, and jsDelivr serves the DocSearch bundle and its base
stylesheet. The theme carries its own fonts and images, so a built page loads nothing else
off-site.
