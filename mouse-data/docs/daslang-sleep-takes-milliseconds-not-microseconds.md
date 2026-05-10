---
slug: daslang-sleep-takes-milliseconds-not-microseconds
title: In daslang's daslib/fio, what unit does `sleep(...)` take — microseconds or milliseconds?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Milliseconds.** The signature is `def sleep(msec : uint const)` (`fio_core` builtin, C++ `builtin_sleep`).

Easy to misread because Unix `usleep`/`sleep` and many other languages take microseconds or seconds. Daslang's `sleep` is plain milliseconds.

```das
sleep(100u)      // 100 ms
sleep(1000u)     // 1 second
sleep(60000u)    // 1 minute
```

**Failure mode:** I once wrote `sleep(100000u)` thinking microseconds (= 100 ms in microseconds). That's actually 100 SECONDS = 1.6 minutes per call, which masquerades as a "hung test" because polling loops never time out. Burned ~15 minutes diagnosing what looked like an HTTP hang in an integration test before noticing.

When sleeping in a poll loop, define the constant explicitly with `_MS` suffix and a one-line comment so the unit is loud at callsites:

```das
let READY_POLL_INTERVAL_MS : uint = 100u   //! ms between /status polls
...
sleep(READY_POLL_INTERVAL_MS)
```

Found 2026-05-09 while building the dasImgui playwright-style integration harness.

## Questions
- In daslang's daslib/fio, what unit does `sleep(...)` take — microseconds or milliseconds?
