# examples/games Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff that deletes a check in `REVIEW.das` (beside this file), narrows what it walks, or
drops a finding it used to report is a defect** - updating a constant so a check keeps
matching the tree is not.

**A `.das` a diff adds or edits beside a game declares `module <name> public`, never
`shared`** - a shared module survives a live reload, and the late-bound GL calls inside it fail
their second compile.

**A shader a diff adds or edits in a game uses only GLSL ES 3.00 features** - the same file
runs in the playground on WebGL2.

**A colour attachment a diff creates or retypes in a format other than `GL_RGBA8` checks
framebuffer completeness at creation and falls back to `GL_RGBA8` when the framebuffer is
incomplete** - WebGL2 does not guarantee a float colour attachment.

**A `.das` a diff adds to a game that has a playground copy is named in the sample's `files`
list (`web/examples/ui/samples/data.json`) in the same change** - the port gate checks the
staged copy is byte-identical, not that the page mounts it, and the playground sample fails to
load with `missing prerequisite`.

