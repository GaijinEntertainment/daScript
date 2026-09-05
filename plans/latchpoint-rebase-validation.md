# Latchpoint master rebase validation

Rebased onto `a8727a4af` (master after #3943). All three original commits applied without
conflicts; `git range-diff` reported unchanged patches. The pre-rebase tip remains at
`backup/latchpoint-before-master-rebase`.

## Build

The complete configured Windows Release build succeeded after repairing generated MSVC
artifacts. Initial LNK1103 failures required regenerating named objects and affected targets'
incremental `.iobj`/`.ipdb` linker caches. No compiler/build source change was required for
that repair. Runtime and shared modules were rebuilt in this worktree, not borrowed from
another checkout. Evidence is under `logs/latchpoint-rebase/`.

## Compatibility fixes

- The new job-queue guard exposed missing context-local fork-pool setup in server ASR workers:
  the first real ASR request failed while the main server queue was already configured.
  ASR, TTS and media worker entry points now enable their own fork pools and preserve the
  shared queue policy. A model-free worker test covers a fresh thread performing a counted
  dispatch without changing the shared FIFO setting.
- The new bare-queue test's Windows child command began with a quoted executable and failed
  in the shell before reaching its fixture. Explicitly pinning the escape hatch off with an
  environment prefix fixes the launch and prevents inherited opt-out settings masking the test.

## Results

- Game compiles; 77 daslang cases and 59 Python reference tests pass. Game lint is clean.
- Native C++ small suite: 115 cases / 1,104,044 assertions passed; one case skipped.
- dasLLAMA model-free suite: 65 files, initially 808 passing cases, one failed Windows
  launch case, three skipped cases. The only failed file was rerun after its fix: 3/3 pass.
  The entire 65-file sweep was not repeated after this test-only correction.
- New server worker regression: 1/1 pass under JIT. Changed test files lint clean.
- Game, dasLLAMA and server review gates pass.
- Rebuilt live game renders; inspected HDR readback reports no invalid values or GL errors.
- Real chat and TTS completed on the rebuilt server. After applying worker setup and restarting,
  the first ASR request in the replacement server completed successfully. A dasOPENAI smoke
  request transcribed the retained test clip exactly and generated a nonempty WAV response.

This is not a claim that the historical GC/reload failures are fixed, nor a long soak result.
The full stocked-model suite and M5/M4 runs remain outstanding. No PR was opened or pushed.
