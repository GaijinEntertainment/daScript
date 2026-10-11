# dasVulkan Tutorials Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE.md`, `../ARCHITECTURE_RENDERING.md`. A `record_*.das` recording driver and
the shared `recording/` harness are reviewed with `skills/internal/vulkan_recording.md` (repo
root).

**A diff that adds a tutorial folder or a present method adds a GLFW viewer per new present
method in the tutorial's `window/`, in the same change.** A present method is how the output
reaches the swapchain: a direct draw, a blit, or a sampling full-screen draw.

**A diff that adds a viewer names it `show_<name>.das`, or `show_<name>_<method>.das` in a
tutorial with more than one present method** - `<name>` is the tutorial folder's name without its
numeric prefix, `<method>` names the present method.
