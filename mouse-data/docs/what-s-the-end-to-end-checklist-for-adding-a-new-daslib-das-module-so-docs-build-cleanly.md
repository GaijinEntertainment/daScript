---
slug: what-s-the-end-to-end-checklist-for-adding-a-new-daslib-das-module-so-docs-build-cleanly
title: What's the end-to-end checklist for adding a new daslib/*.das module so docs build cleanly?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Four things to update, in order:

**1. `doc/reflections/das2rst.das`** — add a `require daslib/<modname>` near the other daslib requires, write a `document_module_<modname>(root : string)` function modeled on a sibling (e.g. `document_module_linq_boost`), and call it from the dispatcher block near the end. Minimal form for a module with mostly-private internals:

```daslang
def document_module_my_new_module(root : string) {
    var mod = find_module("my_new_module")
    var groups : array<DocGroup>
    document("Short description", mod, "my_new_module.rst", groups)
}
```

For modules with many public functions, copy the `linq_boost` pattern and add `group_by_regex(...)` entries for each named group — anything left over lands in "Uncategorized" and **fails CI**.

**2. `doc/source/stdlib/handmade/module-<modname>.rst`** — `das2rst` auto-creates this as `// stub\nModule <modname>`. Replace the **whole file** with a plain-text description (1-2 paragraphs, with a `.. code-block:: das` require + minimal example). See `module-linq.rst` / `module-linq_boost.rst` for the convention.

**3. `doc/source/stdlib/sec_*.rst`** — find the section your module belongs in (e.g. `sec_algorithms.rst` for linq family, `sec_strings.rst` for strings, etc.) and add `generated/<modname>.rst` to its `.. toctree::`. Without this the page builds but isn't linked.

**4. Regenerate + verify:**

```bash
./bin/daslang doc/reflections/das2rst.das        # picks up new module + handmade stub
grep -rl "// stub" doc/source/stdlib/handmade/   # must be empty after step 2
grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'  # must be empty
rm -rf doc/sphinx-build site/doc                 # clean cache (cached builds hide warnings)
sphinx-build -b html -d doc/sphinx-build doc/source site/doc 2>&1 | tee /tmp/sphinx_out.txt
grep -iE "warning:|error:" /tmp/sphinx_out.txt   # must be empty
```

`doc/source/stdlib/generated/*.rst` and `generated/detail/*.rst` are **gitignored** — only commit (1) das2rst.das, (2) the handmade module-<modname>.rst, and (3) the sec_*.rst toctree update.

## Questions
- What's the end-to-end checklist for adding a new daslib/*.das module so docs build cleanly?
- Where do I register a new daslib module in das2rst.das?
- Why does my new module appear as `// stub` in the generated RST?

## Questions
- What's the end-to-end checklist for adding a new daslib/*.das module so docs build cleanly?
