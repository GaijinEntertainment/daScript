# dasVulkan Tutorials Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. A `record_*.das` recording driver and the shared `recording/` harness
are reviewed with `skills/internal/vulkan_recording.md` (repo root).

**A diff that gives a tutorial a new way for its output to reach the swapchain - a direct draw, a
blit, or a full-screen draw that samples the image - adds a GLFW viewer for that way in the
tutorial's `window/`, in the same change.** The viewer's name is the `gate_tutorial_viewers` check
in `../REVIEW.das`.
