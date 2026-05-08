---
slug: malformed
title: Frontmatter without closing fence

Body that should still be readable as the body, but the parser should
flag a parse_error because the closing `---` is missing.

## Questions
- This bullet should NOT be extracted because the frontmatter never closed.
