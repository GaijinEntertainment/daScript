dasVideo
========

dasVideo is the daslang module for **video playback**: decode a clip to frames plus
audio and hand them to your renderer, with A/V sync. Its design is "the player owns the
decoded data, the consumer borrows it" — each frame's pixels and each audio batch are
borrowed through RAII block accessors (no copy between the decoder and the GPU), and
audio is the master clock the video paces to. ``video_open`` auto-detects the container
from its magic bytes, so your code never names the codec; it plays from a file or from a
borrowed in-memory buffer.

Only royalty-free, permissively-licensed codecs are bound — both the code license and the
codec patents have to be clear.

Where to go
-----------

* **Documentation**: https://borisbat.github.io/dasVideo/
* **Repository**: https://github.com/borisbat/dasVideo

What's there
------------

* ``video`` — the playback surface: ``video_open`` / ``video_decode`` / ``video_rewind``,
  stream info getters, and the borrow accessors — ``get_data`` (packed RGBA8 **or** the
  decoder's native YUV420p planes, zero-copy) and ``get_audio_data`` (interleaved float
  PCM). Audio is opt-in (``video_enable_audio``), and a ``video_open(data, size)``
  overload plays from an in-memory buffer.
* Codecs: **MPEG-1** video + MP2 audio (pl_mpeg, the always-on zero-dependency base), and
  **AV1** video (dav1d) — as a raw ``.ivf`` elementary stream or inside a **WebM** container
  (nestegg demux) alongside **Opus** audio (libopus). Each codec is vendor-and-compiled
  from source under a single CMake flag, so a consumer needs only CMake + a C compiler.
* Examples under ``examples/`` — windowed GL players (RGBA and a YUV→RGB shader path),
  A/V-synced audio playback, and a video-textured spinning cube decoded straight from
  memory. The decoded frames go to ``dasGlfw`` + ``dasOpenGL`` today; the borrow API is
  ``dasImgui``-ready.
* A seven-step tutorial ladder (open → decode → borrow → screen → GPU → sound → texture on
  geometry) and headless decode tests that run in CI without a display.

Licensing
---------

pl_mpeg is MIT, dav1d is BSD-2, nestegg is ISC, and libopus is BSD — all permissive, all
royalty-free. FFmpeg, VP9, and Theora are deliberately **not** bound here; bind those
yourself if you need them.
