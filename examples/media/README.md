# Media Samples

CC0 (public domain) audio samples for daStrudel examples and MIDI player.

## Drums — SamsSonor

Sam Greene's Sonor Force 3001 drumkit.
Source: https://github.com/sfzinstruments/SamsSonor
License: CC0 1.0 Universal (Public Domain)

GM percussion sounds:

| Directory | GM Note | Sound | Source Sample |
|---|---|---|---|
| `bd/` | 35-36 | Bass Drum | KickSamples |
| `sd/` | 38-40 | Snare | SnareLeftHand |
| `hh/` | 42 | Closed Hi-Hat | HHClosed |
| `hh_pedal/` | 44 | Pedal Hi-Hat | HHPedal |
| `hh_open/` | 46 | Open Hi-Hat | HHOpenClang |
| `cp/` | 49 | Crash Cymbal | CrashClang |
| `ride/` | 51 | Ride Cymbal | Ride |
| `ride_bell/` | 53 | Ride Bell | RideBell |
| `sidestick/` | 37 | Side Stick | SnareRightHand |
| `tom_low/` | 43 | Low Tom | TomLow |
| `tom_high/` | 45 | High Tom | TomHigh |

Each directory contains 3 OGG variations (velocity layers).

### Drum conversion process

Source FLAC samples are converted to mono OGG with linear peak normalization:
```
ffmpeg -y -i Source.flac -ac 1 -c:a libvorbis -q:a 4 output.ogg
```

Do NOT use `-af loudnorm` — it introduces echoes/artifacts on percussive sounds.
For volume balancing between drum types, use linear scaling:
```
ffmpeg -y -i Source.flac -ac 1 -af "volume=0.8" -c:a libvorbis -q:a 4 output.ogg
```

## Piano — jRhodes3c

1977 Rhodes Mark I Stage 73 electric piano by jlearman.
Source: https://github.com/sfzinstruments/jlearman.jRhodes3c
License: CC0 1.0 Universal (Public Domain)

15 pitch centers covering MIDI notes 29-96 (F1 to C7), 3 velocity layers each:

| Directory | MIDI | Note | Velocity Layers |
|---|---|---|---|
| `note_29/` | 29 | F1 | soft, mid, loud |
| `note_35/` | 35 | B1 | soft, mid, loud |
| `note_40/` | 40 | E2 | soft, mid, loud |
| `note_45/` | 45 | A2 | soft, mid, loud |
| `note_50/` | 50 | D3 | soft, mid, loud |
| `note_55/` | 55 | G3 | soft, mid, loud |
| `note_59/` | 59 | B3 | soft, mid, loud |
| `note_62/` | 62 | D4 | soft, mid, loud |
| `note_65/` | 65 | F4 | soft, mid, loud |
| `note_71/` | 71 | B4 | soft, mid, loud |
| `note_76/` | 76 | E5 | soft, mid, loud |
| `note_81/` | 81 | A5 | soft, mid, loud |
| `note_86/` | 86 | D6 | soft, mid, loud |
| `note_91/` | 91 | G6 | soft, mid, loud |
| `note_96/` | 96 | C7 | soft, mid, loud |

### Piano conversion process

Source mono FLAC samples converted to OGG:
```
ffmpeg -y -i As_NNN__Note_Vel-mono.flac -ac 1 -c:a libvorbis -q:a 4 output.ogg
```

## MIDI Files

Public domain MIDI files for testing the MIDI player:

| File | Piece | Source |
|---|---|---|
| `midi/fur_elise.mid` | Beethoven — Fur Elise | midiworld.com |
