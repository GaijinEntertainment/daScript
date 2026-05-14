---
slug: bulk-fix-lint-style005-with-perl-multiline-regex
title: Bulk-fix `if (cond) { return X }` → `if (cond) return X` (STYLE005) across many files — what's a safe perl one-liner that won't mangle elif chains or multi-line bodies?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

A multi-line regex restricted to the strict three-line `if (cond) {\n<indent+>BODY\n<indent>}\n` shape, with body limited to bare `return X` / `break` / `continue`. Indent-matching prevents elif and false brace matches.

```perl
# /tmp/fix_style005.pl
use strict; use warnings;
die "usage: $0 <file>...\n" unless @ARGV;
for my $file (@ARGV) {
    open(my $fh, "<", $file) or die "$file: $!";
    local $/;
    my $src = <$fh>;
    close $fh;
    my $orig = $src;
    # Re-run until idempotent — handles patterns that nest after each pass.
    for (my $iter = 0; $iter < 5; $iter++) {
        my $count = ($src =~ s{
            ^([ \t]+)if \((.+?)\) \{\n
            \1[ \t]+(return\b[^\n]*?|break|continue)\n
            \1\}\n
        }{$1if ($2) $3\n}mgx);
        last if $count == 0;
    }
    if ($src ne $orig) {
        open(my $out, ">", $file) or die;
        print $out $src;
    }
}
```

**Why it works:**
- `^([ \t]+)` captures indent, `\1` reuses it on body and `\}` — bodies indented further (which is the standard) match; bodies at the same indent don't.
- Body alternation `(return\b[^\n]*?|break|continue)` matches exactly one terminator line; multi-line bodies don't match (the next non-blank line wouldn't be the `}` at indent `\1`).
- `\}\n` requires the closing brace on its own line — `} elif (...)` and `} else {` are left alone (correctly — those need bigger refactors).
- `x` mode lets you write the regex with whitespace + comments.

**Hit rate**: on `modules/dasSQLITE/daslib/` this collapsed ~326 of ~336 STYLE005 warnings in one pass (97%). The residual ~10 were elif chains and multi-line-body cases that needed manual rewrites.

**Companion script for STYLE022/PERF013/PERF017** in the same vein:
```perl
# STYLE022 — bit-set/clear → field-assign (end-of-line shape, single bit only)
s/^([ \t]+)((?:[\w.]|\[[^\]]*\])+) \|= (\w+)\.(\w+)$/$1$2.$4 = true/mg;
s/^([ \t]+)((?:[\w.]|\[[^\]]*\])+) &= ~(\w+)\.(\w+)$/$1$2.$4 = false/mg;
# PERF013 — +=/-= 1
s/^([ \t]+)((?:[\w.]|\[[^\]]*\])+) \+= 1$/$1$2++/mg;
s/^([ \t]+)((?:[\w.]|\[[^\]]*\])+) -= 1$/$1$2--/mg;
# PERF017 — length(x) == 0 etc.
my $len = qr/length\(((?:[^()]|\([^()]*\))+)\)/;
s/$len == 0\b/empty($1)/g;  # plus !=, >, <=, >=1, <1, and Yoda variants
```

**Always verify with `mcp__daslang__compile_check`** after the bulk pass — these regexes can match unexpected shapes if your indent style is non-standard. For dasSQLITE (440+ warnings) this got the warning count from 568 to ~32 in 2 minutes of perl runtime, leaving the semantic residuals (STYLE016/STYLE005-elif/PERF018) for hand-fixing.

## Questions
- Bulk-fix `if (cond) { return X }` → `if (cond) return X` (STYLE005) across many files — what's a safe perl one-liner that won't mangle elif chains or multi-line bodies?
