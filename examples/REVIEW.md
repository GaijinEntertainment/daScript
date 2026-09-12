# examples Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**

**A `.das` in this folder tree that a game's `main.das` requires, directly or through another
required file, answers to `games/REVIEW.md` too - wherever the diff puts it.**

**A folder with a `web_shell.html`, and every `.das` and `models.json` in it, answers to
`dasLLAMA/REVIEW.md` too, wherever the diff puts it.**

**A diff that drops `require live/audio_live` from a program states, in the PR body, why that
program's audio is meant to die with a reload** - `audio_live` is what carries the audio
thread's stream and channels across a reload, so without it the program creates and finalizes
its own audio system on every one.
