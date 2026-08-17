# dasVulkan Tutorials Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../CLAUDE.md`. A recording-pipeline change — a `record_*.das` driver, wherever the diff puts
it, or the shared `recording/` harness — is reviewed with `skills/internal/vulkan_recording.md`
(repo root).

**A change that adds a tutorial folder ships its `<NN_name>/window/show_<name>.das` viewer in the
same change** (`<name>` = the folder name without the numeric prefix) — a real GLFW window
presenting the live animation. The offscreen pixel-oracle test proves correctness; nothing else
lets a human see the result.
