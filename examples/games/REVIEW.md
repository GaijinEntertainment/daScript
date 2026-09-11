# examples/games Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff that deletes a check in `REVIEW.das` (beside this file), narrows what it walks, or
drops a finding it used to report is a defect** - updating a constant so a check keeps
matching the tree is not.

**A `.das` a diff adds beside a game declares `module <name> public`, never `shared`** - a
shared module survives a live reload as the promoted copy, and the late-bound GL calls inside
it fail their second compile.

**A shader a diff adds to a game uses only GLSL ES 3.00 features** - the same file runs in the
playground on WebGL2.

**A colour attachment a diff creates in a format other than `GL_RGBA8` is probed at creation,
with a `GL_RGBA8` fallback on an incomplete framebuffer** - WebGL2 does not guarantee a float
colour attachment.

**A `.das` a diff adds to a game that has a playground copy is named in the sample's `files`
list (`web/examples/ui/samples/data.json`) and in `GAME_FILES` of `site/examples/_interp.html`
in the same change** - the port gate checks the staged copy is byte-identical, not that the
page mounts it, and an unmounted module fails the card at `missing prerequisite`.

