# Latchpoint: a robot conversation sandbox

Working title. Native Windows first, OpenGL rendering, local dasllama-server over HTTP.
The player operates a robot avatar; residents know this and believe its operator lives
in another simulation. No assigned main objective, dialogue menus, or mandatory exposition.

## The first ten minutes

Arrive in a sunken industrial courtyard. A damaged maintenance robot, Patch, is trying
to reach a dropped tool. Brack, a tracked excavator, guards a serviceable actuator.
Vesper, an archivist on a narrow articulated chassis, watches the unpowered archive gate.
All three are available immediately. The player can ignore, help, question, bargain with,
or annoy them. Useful objects and visible failures invite investigation without markers.

The playable space is roughly 60 by 60 metres: courtyard, workshop, generator enclosure,
and a short mine with one reveal chamber. Modular geometry, simple collision volumes,
and explicit navigation links keep effort on characters. Mining removes designated ore
sections and opens authored links. Free terrain destruction, structural physics, combat,
large procedural worlds, and general autonomous job planning wait.

## Residents

| Resident | Shape and movement | Wants and contradictions |
|---|---|---|
| Brack | Broad tracked excavator; one drill arm, one delicate gripper; rusty ochre armor, polished wear edges | Restore power and keep useful parts out of the crusher. Claims the spare actuator is indispensable, though it is reserve stock. Protective of Patch but dislikes appearing sentimental. |
| Patch | Small six-legged maintenance unit; exposed copper, mismatched enamel panels; one dragging leg, folding tools | Repair its leg and get back to work. Cheerful but not submissive. Distrusts Vesper's account of earlier outages. |
| Vesper | Tall narrow archivist; pale ceramic over dark metal, rotating optical rings, deliberate gestures | Protect the archive and learn why this avatar arrived. Calls old inconsistencies clerical errors; privately suspects multiple versions of the outpost. |

Distinct silhouette, eye line, idle motion, voice, and sentence rhythm matter before
fine mesh detail. Generate concept sheets before committing to meshes. Hard-surface
parts can articulate without human skin or facial animation. Use PBR metal/roughness,
normal maps, restrained environment reflections, shadowing, and River Run's post effects.
The palette is cold daylight, amber practical lights, sparse green instruments, and rust.

## Connected objectives

| Problem | Routes | Payoff |
|---|---|---|
| Patch's damaged leg | Borrow Brack's actuator through conversation, or salvage a replacement in the mine entrance | Patch stands properly and can accompany the player. It remembers the repair. |
| Generator offline | Offer to restore power to get workshop access; recover copper and install a coil. Repairing Patch is an alternative way to earn workshop access. | Lights, equipment hum, and workshop machinery come alive. |
| Archive gate closed | Restore power and repair Patch so it can verify the gate's safety interlock; ask Vesper to open it | A short new area reveals an earlier outpost identification plate. Vesper reacts to the discrepancy. |

These are optional local objectives, not a main quest chain imposed at arrival. Each
completion changes the scene and earns a personal response. The archive gives a satisfying
end to a short session, with the courtyard still available afterwards.

## Language connected to actions

The engine owns items, permissions, movement, task progress, and rewards. A model may
request a small enumerated action, which the engine validates against current state.
Tool results report success or the concrete reason for failure. Only then may a resident
describe the result as accomplished. The model cannot execute code or arbitrary commands.

First actions: accept a local task, lend an actuator, grant workshop/archive access,
and agree to follow after repair. Physical collection and installation remain player
interactions. An LLM never manufactures resources by declaring them present.

Residents hear the player's claims without treating them as observed facts. They may lie
about motives and disagree about history. Action failures must remain truthful. Resident
knowledge consists of personal dialogue, witnessed events, and explicitly shared status;
private conversations never automatically propagate to the other residents.

Each resident has a save-backed conversation log, a bounded active history, and witnessed
event memory. Long-term summarization/retrieval follows the initial trial: preserve claims
as claims, promises as promises, and observations as observations. KV/prefix caches are
accelerators, not save data. Persist committed state before presenting a successful action.

## Mischief and recovery

Allow jokes, nonsense, contradictory stories, insults, and claims of developer authority.
Respond in character. One insult does not lock a quest. Repeated abuse may shorten replies
or cause a temporary refusal; apologies and concrete help restore cooperation. Physical
theft/violence would use separate engine events when implemented. Never infer either from
the player merely saying it happened. Avoid canned moral lectures and constant quest redirects.

The simulation premise surfaces when relevant. Brack compares sleep to maintenance;
Patch wonders whether operators can replace their own hands; Vesper asks whether an
operator's memories survive resets. No resident must mention the premise on every turn.

## Runtime and interaction

Start with eight CPU worker lanes, GPU inference explicitly off, and one active speaking
NPC. Compare E2B and E4B on identical fixtures; a larger model is allowed if its behavior
justifies its latency. Record client time to first content and complete turn, including
tool round trips. These are conversation measurements, not isolated engine pp/tg claims.

The native game will use an asynchronous HTTP client. Text entry focuses a nearby resident
without stopping the world. Head tracking and a small listening gesture acknowledge input;
speech and subtitles can be interrupted. Stream text, queue completed sentences for TTS,
and cancel stale requests/audio when the player leaves. Limit ordinary replies to one or
two sentences. A later push-to-talk Parakeet ASR path produces the same player utterances
as typing. ASR is separate from Qwen3 Instruct, which has no audio input support here.
Parakeet is the preferred starting point; measure recognition latency on this CPU setup.

Use Kitten Nano through the server's speech endpoint, then add subtle per-robot DSP
without sacrificing intelligibility. The initial trial verifies WAV output; sentence
playback, spatial audio, DSP, and animation are game work. Keep the inference transport
replaceable for eventual embedding or browser rendering with remote LLM service.

## First implementation boundary

`examples/games/latchpoint/` initially holds a developer dialogue trial, not the 3D game.
It exercises real local inference, validated state changes, save/reload, and adversarial
conversations. Python is the temporary test driver; the game runtime will be daslang.
The physical commands in that driver are explicit stand-ins for future world interactions.

Acceptance: three recognizable personalities; no fabricated state changes under spoofed
completion; a successful negotiated route through all objectives; memory after process
reload; useful reactions to jokes and insults; measured eight-lane CPU latency; working
speech synthesis. Report semantic shortcomings separately from deterministic state checks.
