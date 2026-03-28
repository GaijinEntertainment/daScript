# MIDI SF2 Demo

Plays a MIDI file using a SoundFont (SF2) for instrument sounds.

## Setup

Download one (or both) of these GM SoundFonts and place them in this folder:

- **FluidR3_GM.sf2** (~141 MB) - full GM bank, best quality
  - https://sourceforge.net/projects/androidframe/files/soundfonts/FluidR3_GM.sf2/download
  - https://musical-artifacts.com/artifacts/738

- **GeneralUser GS v1.471.sf2** (~30 MB) - lighter alternative
  - https://raw.githubusercontent.com/ROCKNIX/generaluser-gs/main/GeneralUser%20GS%20v1.471.sf2

The demo tries FluidR3 first, then GeneralUser. If neither is found it falls back to basic oscillators.

## Usage

```
daslang.exe examples/daStrudel/midi_sf2_demo/main.das
daslang.exe examples/daStrudel/midi_sf2_demo/main.das -- path/to/song.mid
```
