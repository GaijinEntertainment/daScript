# Latchpoint dialogue research archive

The branch `archive/latchpoint-python-experiments` preserves the complete prototype at
`c6ee122465158a97bb70e810f2ae3ddd4daf84af`, before the native-game PR removed the Python
dialogue experiments. It is a research snapshot, not a second supported game version.
It includes the native game at that point; do not merge the branch wholesale to recover
one experiment. Later native-game fixes do not automatically appear on the archive branch.

## What is preserved

All paths below are under `examples/games/latchpoint/` on the archive branch.

| Files | Purpose |
| --- | --- |
| `dialogue_trial.py` | Terminal conversations, native tool calls, bounded reply guards, resident histories and scenario timing |
| `gameplay_dialogue.py` | Intent classification, deterministic action receipts and practical replies |
| `verified_dialogue.py` | Separate action extraction, dialogue drafting and reply verification; bounded recovery attempts |
| `scene_bridge.py` | Earlier Python HTTP bridge, asynchronous jobs and speech receipts; superseded by the native runtime |
| `voice_trial.py` | TTS-to-ASR round-trip experiments with saved clips and transcription comparisons |
| Six `test_*.py` files | Deterministic reference tests for those experiments, including rejected claims and save behavior |
| `scenario.json`, `mischief.json`, `player_regression.json` | Objective routes, adversarial/silly requests and playtest regression inputs |
| `verified_smoke.json`, `verified_power_route.json`, `verified_repair_route.json` | Inputs for the experimental verifier and alternate objective routes |
| `README.md`, `plans/latchpoint*.md` (repository root) | Original invocation examples, design decisions, evaluation notes and limitations |

The live native game still uses `characters.json` and `world_knowledge.json`; those stay
in the game PR. Native gameplay, save state, dialogue, audio and input do not require the
archived Python code. The separate inference server still uses the repository's Python watchdog.

The archive has useful reference behavior, not release-level coverage. Its 59 Python tests
passed, but the PR audit found many unexecuted branches. Model-backed trials also exposed
false-positive reply rejection and malformed action plans. A verifier response is not proof
that a player authorized an action, and transcript agreement is not a voice-quality score.
Model files, local configurations, saves, recorded audio and diagnostic logs are not in Git.

## Returning to the experiments

Use a separate worktree at the archive branch so research cannot overwrite the native game's
save or local server configuration. The archive's README contains the original commands.
For example, from the archive checkout's root, the deterministic reference tests need no model:

```sh
python -B -m unittest discover -s examples/games/latchpoint -p 'test_*.py'
```

Live trials need separately configured local model paths and dasllama-server. Use fresh
temporary directories for trial saves, reports and speech clips, not the playable game's saves.
The archived drivers are not assumed compatible with future server or native save schemas.

## Candidate next iteration

First inventory which experiments still answer a current gameplay question. Port the useful
ones to daslang incrementally, using dasOPENAI and the native runtime's authoritative action
and save interfaces instead of maintaining another world implementation. Candidate work:

- Bring reproducible dialogue scenarios and state assertions into a native test driver.
- Evaluate whether bounded request verification earns its latency and false-rejection cost.
- Keep TTS-to-ASR validation as an intelligibility check alongside human listening.
- Preserve regressions for rudeness, silliness, negation, stale memories and false action claims.
- Remove superseded bridge code, duplicated state machinery and irrelevant scenarios from
  the new working version after comparison; keep this archive snapshot as the reference.

Porting or pruning is future work, not part of the native-game PR. Each retained behavior
needs focused tests and real-model evaluation where deterministic tests cannot settle it.
