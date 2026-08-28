# dasVulkan Tutorials Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. A recording-pipeline change - a `record_*.das` driver, wherever the diff puts
it, or the shared `recording/` harness - is reviewed with `skills/internal/vulkan_recording.md`
(repo root).

**A diff that adds a tutorial folder also adds its `<NN_name>/window/show_<name>.das` viewer, in
the same change** (`<name>` is the folder name without the numeric prefix) - a GLFW window that
presents the tutorial's rendered output live. The offscreen test proves correctness; only the
window lets a human see the result.
