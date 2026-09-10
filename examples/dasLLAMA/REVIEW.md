# examples/dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

A browser example is a subfolder here with a `web_shell.html` (`ARCHITECTURE.md` sec. 2); the
rules below bind browser examples.

**Never install a GLFW callback - any `glfwSet*Callback` - in a browser example; poll each input
every frame and edge-detect it - `glfwGetKey` for keys, `glfwGetMouseButton` for buttons.** In the
browser build a callback fires outside any frame of the program and the program traps
(`ARCHITECTURE.md` sec. 3.3).

**A browser example's `web_shell.html` gives the canvas element `max-width` / `max-height` so the
element's box is exactly the rendered image - never `object-fit` on a canvas stretched to fill the
page area around it.** A click maps through the element's box with one ratio per axis, so a
stretched box mis-maps every click (`ARCHITECTURE.md` sec. 3.3).

**A browser example's `web_shell.html` must reload a page the browser restored from its
back-forward cache - a `pageshow` handler that reloads when `persisted` is set.** Such a page
comes back with its workers and audio output frozen out of step (`ARCHITECTURE.md` sec. 3.1).

**A status box a browser example attaches to a sound (`set_status_update`) serves that one sound:
a diff that attaches a box a sound already reported into to another sound is a defect - create
one per sound and release it when the sound ends.** The mixer's last report for the first sound
lands after the second attaches, and reads as the second's stop (`ARCHITECTURE.md` sec. 3.7).

**A diff that changes or drops a witness line - a line a browser example logs under its own name -
updates every test under `modules/dasLLAMA/tests/` that matches it, in the same change.** The
smoke tests match witness lines as substrings, so the words and their order are an interface
(`ARCHITECTURE.md` sec. 2).

**A diff that adds a model file to a browser example's `models.json` names its sha256 and a
location that cannot move - a Hugging Face repository, or a repo-relative path in this repository
under `tree` - never a machine-local path or a branch name.** The deploy fetches by that name and
refuses a file whose hash moved (`ARCHITECTURE.md` sec. 3.4).
