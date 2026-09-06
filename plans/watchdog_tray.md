# The watchdog's tray icon

The second half of `plans/tray_icon.md`: the supervisor shows its state in the notification
area through `stddlg`'s tray primitive, and the tray becomes the notification channel.

## Decisions

1. **Opt-in, never fatal.** `--tray` (and the `tray` key in `watchdog.json`) asks for the icon;
   a box with no desktop, no session bus or no backend logs `tray_unavailable` and supervises
   without it. The flag defaults to off; the dasllama-server bundle turns it on in its own
   `watchdog.json`, as the Python era did, and a headless deployment of that bundle gets one
   `tray_unavailable` line per start.
2. **The menu is a pure function of supervisor state.** A disabled status row, `Open <name>`
   (live while the health check is green; with `--no-health` only when `--tray-url` names the
   page, and then while the child runs), a separator, `Shutdown`. A left click opens the page;
   the platform pops the menu on a right click. The exchange rails the Python tray carried
   (adopt a sidecar, run untuned, share) stay cut with the exchange UI.
3. **One status line, three coarse states.** The tooltip and the status row show the same
   text: `stopped`, `restarting in Ns (exit N)`, `starting up - <stage in plain words>`,
   `tuning this box - d/t kernels`, `serving - healthy HhMMm`, `unhealthy (<stage>)`. The
   icon is a procedural mark wearing a badge for the coarse state: plain for serving, an amber
   triangle while working, a red square in trouble. No image file: the static binary carries
   no decoder, and a badge composited at run time gives every deployment status for free.
4. **Notifications go through the tray while it is up.** `notify` calls `tray_notify` when the
   icon exists, and falls back to the spawned helper (PowerShell balloon, osascript,
   notify-send) otherwise, so a headless box keeps the old path.
5. **The page opens through the platform's opener**, spawned from daslang: `open` on macOS,
   `xdg-open` on Linux, `rundll32 url.dll,FileProtocolHandler` on Windows (no console flash, no
   `cmd /c start` quoting). `--tray-url` names the page; unset, it is the health URL's origin.
6. **Pumped from `tick`.** The supervisor polls the tray once per tick, child or no child, and
   acts on what came back: an open request, or a shutdown that enters the same stop ladder as
   Ctrl-C. The tray never calls into the supervisor. The cost: on Windows and macOS the menu is
   modal inside the pump, so supervision pauses while a person holds it open.
7. **Nothing per tick.** The fold is whole units (minutes, seconds, counts), the tray compares
   two folds and builds its strings only on a change, and both hosts collect the heaps between
   ticks (`options gc`, a `heap_collect` every minute or past a megabyte of strings) - the same
   rail dasllama-server runs on, since every log line leaves strings and JSON nodes behind.
8. **Build.** `watchdog.das` requires `stddlg`; `bin/watchdog` links the module's static twin
   beside dasHV's, and the target exists when both module targets do. The block sits above the
   `-exe` utilities' LLVM early return, so the default configure (LLVM off) builds it too.

## Tests

- Pure arms on every platform: the status line and badge for each state, the URL origin, the
  icon mark's badge pixels.
- Linux, headless: the watchdog under the interpreter as a child of the test, on a private
  `dbus-daemon`, supervising the park fixture; `dbus-send` reads the menu layout and the
  tooltip, clicks `Shutdown`, and the log shows `tray_started`, `tray_shutdown_requested`,
  `watchdog_stopped`, exit 0. The bus and osascript helpers move to
  `tests/stddlg/_tray_rig.das`, shared with the tray test.
- macOS: the status item's tooltip read back through System Events. Windows: the icon reaches
  `tray_started` on an interactive desktop. Every platform: a crash-then-ok run under `--tray`
  ends with exit 0 whether the icon exists or not, and a chatty child leaves the string heap
  bounded. The menu clicks are eyeballed on the Mac and on zen2.

## Not this arc

- A brand mark from a shipped image (needs a decoder in the static binary).
- The exchange rails (adopt / untuned / share) - with the exchange UI.
