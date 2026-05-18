---
slug: what-ci-checks-must-pass-when-i-regenerate-doc-source-stdlib-via-das2rst-das
title: What CI checks must pass when I regenerate doc/source/stdlib/ via das2rst.das?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

CI workflow `.github/workflows/doc.yml` runs three sequential gates after `./bin/daslang doc/reflections/das2rst.das`:

1. **`// stub` check** — `grep -rl '// stub' doc/source/stdlib/handmade/` must return nothing. das2rst writes `// stub` placeholders for any public function/struct/enum without a handmade description.

2. **Untracked-files check** — `git ls-files --others --exclude-standard doc/source/stdlib/` must be empty. Newly-generated RST files must be committed.

3. **`sphinx-build -W --keep-going -b latex`** — warnings-as-errors LaTeX build. Catches dangling `:ref:`s, malformed tables, duplicate labels.

**Local workflow before pushing:**
```bash
./bin/daslang doc/reflections/das2rst.das
grep -rl "// stub" doc/source/stdlib/handmade/   # must be empty
git add doc/source/stdlib/handmade/              # if new files appeared
~/Library/Python/3.11/bin/sphinx-build -W -b latex -d doc/sphinx-build doc/source /tmp/site_doc   # must exit 0
```

**Gotchas:**
- For **daslang modules** (`daslib/*.das`, `modules/*/*.das`): fix stubs by adding `//!` comments in the `.das` source — handmade files for daslang modules are ignored. See `skills/documentation_rst.md`.
- For **C++ builtin modules** (math, strings, audio, etc.): fix stubs by editing `doc/source/stdlib/handmade/*.rst` directly.
- When bulk-filling many stubs by copying from siblings (e.g. when a binary layout change shifts function hash buckets and regenerates 158 new RST files), **hand-check math overloads** — vector-specific descriptions may not match the scalar overload, and some library descriptions are technically wrong (e.g. `mad` is *not* always fused FMA; `round` halfway behavior is *not* nearest-even on the vector path).

Reference: PR #2707 round 3 — `mad` and `round` doc fixes after Copilot caught the incorrect inherited descriptions.</body>
<slug>ci-doc-workflow-gates-stubs-untracked-sphinx-w</slug>
</invoke>

## Questions
- What CI checks must pass when I regenerate doc/source/stdlib/ via das2rst.das?
