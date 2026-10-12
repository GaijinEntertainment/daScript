# dasImgui third-party notices

This module binds, statically links, or ships the following third-party works.
The referenced license texts are checked in under `licenses/` and `font/` and
install with the SDK.

## Dear ImGui (MIT)

The bound library itself (https://github.com/ocornut/imgui), Copyright (c)
2014-2026 Omar Cornut, MIT License - see `licenses/IMGUI.txt`. Fetched at build
time (v1.92.6-docking); the fetched tree bundles the stb `imstb_rectpack.h` /
`imstb_textedit.h` / `imstb_truetype.h` headers (public domain / MIT dual, Sean
Barrett) and the Khronos `glcorearb.h` header (MIT-style).

## FreeType (FTL)

Font rasterization statically links FreeType (https://freetype.org), fetched at
build time. FreeType is dual-licensed; this module uses it under the FreeType
Project License - see `licenses/FREETYPE_FTL.txt`.

Portions of this software are copyright (c) The FreeType Project
(www.freetype.org). All rights reserved.

## MD4C (MIT)

The markdown layer statically links MD4C (https://github.com/mity/md4c),
Copyright (c) 2016-2024 Martin Mitas, MIT License - see `licenses/MD4C.txt`.
Fetched at build time (release-0.5.3).

## JetBrains Mono (SIL OFL 1.1)

The runtime font `font/JetBrainsMono-Regular.ttf`
(https://github.com/JetBrains/JetBrainsMono), Copyright 2020 The JetBrains Mono
Project Authors, SIL Open Font License 1.1 - see `font/OFL.txt` and
`font/AUTHORS.txt`. The license and authors files install beside the font.

## gl3w (public domain) + Khronos glcorearb.h (MIT)

The OpenGL loader under `gl3w/` is generated output of the gl3w generator
(https://github.com/skaslev/gl3w), released into the public domain (Unlicense).
It compiles against this module's copy of the Khronos `glcorearb.h` header
(Copyright 2013-2020 The Khronos Group Inc., MIT) - the notice installs as
`KHRONOS_GL.LICENSE`.

## Input example fonts

`examples/features/text_input/fonts/` contains unmodified Noto Sans 2.008 and Noto
Sans CJK SC 2.004 under SIL OFL 1.1, plus Twemoji Mozilla 0.7.0 in COLR/CPAL format.
Noto Sans is Copyright 2015-2021 Google LLC; Noto Sans CJK is Copyright 2014-2021
Adobe. Their original licence files are beside the fonts.

Twemoji artwork is by Twitter and contributors (https://github.com/twitter/twemoji),
under CC BY 4.0 (https://creativecommons.org/licenses/by/4.0/). Mozilla's font tooling
is Copyright 2016-2018 Mozilla Foundation, Apache 2.0. The upstream licence notice and
full Apache 2.0/CC BY 4.0 texts are in the `twemoji/` subdirectory. The example displays
attribution, and `fonts/manifest.json` records exact source URLs, versions and SHA256.
The font files, notices and manifest travel with both native and web releases.
