---
slug: sphinx-w-pygments-json-lexer-fails-on-ellipsis-placeholders
title: Sphinx -W docs CI fails with Pygments JSON lexer warning on placeholder ellipsis — what's the fix?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

## Symptom

Docs CI's `sphinx-build -W --keep-going` fails the build with output like:

    /.../tutorials/driving_outside.rst:82: WARNING: Lexing literal_block '...
    "bbox": [...], "...":"..."
    ...' as "json" resulted in an error at token: '.'. Retrying in relaxed mode.

The build still emits HTML, but `-W` escalates the warning to an error → `Process completed with exit code 1`.

## Cause

Pygments' JSON lexer is strict — `[...]`, `"...":"..."`, and similar ellipsis placeholder tokens you sprinkle into "shape of the response" examples aren't valid JSON. Pygments falls back to relaxed lexing and emits the warning. Under `-W` the warning is fatal.

## Fix

Switch the code-block language to `text`:

    .. code-block:: text

       {
         "frame": 412,
         "bbox": [...],
         "...": "..."
       }

You lose JSON syntax highlighting but the block keeps the same shape and the warning goes away. This is the right move whenever the example is illustrative (uses elisions, comments, trailing commas, etc.) rather than a literal valid-JSON sample.

Alternatives if you want highlighting:
1. **Make the JSON valid** — replace `[...]` with concrete `[0, 0, 0, 0]`, drop the `"...": "..."` rows entirely.
2. **Use `.. parsed-literal::`** — no lexing at all, but you lose the boxed code-block look.

In practice `text` is the lowest-friction option.

## Real example

dasImgui CI failed on `doc/source/tutorials/driving_outside.rst:82` after commit `4afaddc` (2026-05-13). Fixed in `022419f` — single-line `code-block:: json` → `code-block:: text` swap. Local `sphinx-build -W --keep-going` clean afterward; CI green on next push.

## Questions
- Sphinx -W docs CI fails with Pygments JSON lexer warning on placeholder ellipsis — what's the fix?
