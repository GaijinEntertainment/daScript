---
slug: my-dasimgui-apng-recording-is-committing-as-100-mb-and-github-warns-about-file-size-how-do-i-shrink-it-without-sacrificing-reade
title: My dasImgui APNG recording is committing as 100+ MB and GitHub warns about file size — how do I shrink it without sacrificing reader pacing?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

**Three knobs in order of cost/benefit: shrink the host window, drop fps to 20, and check whether you're recording per-frame animations.**

Existing dasImgui APNGs sit in a 7.5 MB → 77 MB band (`visual_aids_tour.apng` is the largest committed). GitHub's CLI warns at >50 MB but doesn't block. The sweet spot for new recordings is **≤75 MB**, matching the existing ceiling.

## Knob 1: window dimensions (biggest lever)

File size scales roughly linearly with pixel count. The two narrative+edit_external recordings in PR 5b/5c each shrunk from 900×640 / 820×600 down to **720×480** with `FontGlobalScale = 1.15` (down from 1.4) — cut the file roughly in half. Live_create_window dimensions only take effect on init, so a `live_reload` doesn't pick up the change — full restart required.

In the feature file:

    live_create_window("dasImgui my_tour", 720, 480)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.15

Also shrink the inside-window `SetNextWindowSize` to fit (e.g. 680×440 inside a 720×480 frame).

## Knob 2: recording fps

`record_start(fps = 20)` instead of `fps = 30` cuts frames by 33% → file by roughly 30%. Cursor tracking is still smooth enough for readers at 20 fps.

    post_command(app, "record_start", JV((file = OUTPUT, fps = 20, max_seconds = 75)))

## Knob 3: per-frame animations defeat compression

APNG uses delta compression between frames. If your feature file animates something on EVERY frame (per-tick progress_bar fill, real-time clock, rotating gauge), every frame is "different" and the delta isn't compressed — file size balloons.

Hard to fix without changing the feature semantics, but worth knowing: a tour with one animating progress_bar will be ~30% larger than the same tour with all static widgets.

## Combine all three

`narrative_layout_tour.apng` started at 124 MB (900×640, fps=30, animating progress_bar). After knob 1 (→720×480) it was 90 MB. After knob 2 (→fps=20) it was 60 MB. That's the same story arc, just lighter.

## Verify size before committing

    ls -la doc/source/_static/tutorials/your_apng.apng

Bracket: 7.5 MB → 75 MB OK; >75 MB consider knob 1+2 above; <5 MB suspicious (probably under-paced — too few frames per stage).

## Related

- `tests/integration/record_boost_narrative_layout.das` / `record_edit_external.das` — both record at fps=20 for size reasons.
- `examples/tutorial/narrative_layout_tour.das` / `editing_external.das` — both shrunk to 720×480 / FontGlobalScale=1.15.
- mouse card `imgui-narrate-frames-vs-recorder-fps` — pacing constants are independent of recorder fps.

## Questions
- My dasImgui APNG recording is committing as 100+ MB and GitHub warns about file size — how do I shrink it without sacrificing reader pacing?
