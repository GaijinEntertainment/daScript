options gen2
options _comment_hygiene = true
options _dasllama_internal = true

//! dasLLAMA - the engine (``dasllama/dasllama_core``) plus its file decoders: audio files to PCM
//! through miniaudio (``audio`` module) and images to RGB8 through stb_image (``stbimage`` module).
//! A program that hands the engine samples and pixels requires ``dasllama/dasllama_core`` instead
//! and needs neither module.

module dasllama shared public

require dasllama/dasllama_core public
require dasllama/dasllama_audio_io public      // audio decode (wav/mp3/flac/ogg) to mono f32 PCM at 16 kHz or any asked rate
require dasllama/dasllama_vision_io public     // image decode to RGB8 VisionImage - the only stbimage seam
