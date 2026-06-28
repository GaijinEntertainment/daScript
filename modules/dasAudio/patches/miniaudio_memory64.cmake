# Idempotent patch for the fetched miniaudio.h: make its emscripten WebAudio
# (ScriptProcessorNode) backend memory64-aware.
#
# Under -sMEMORY64=1, wasm exports take i64 (BigInt) pointers; under wasm32 they
# take i32 (Number). miniaudio's onaudioprocess / unlock JS calls the
# _ma_device_process_pcm_frames_*__webaudio and _ma_device__on_notification_unlocked
# wasm exports passing the device / intermediary-buffer pointers as plain JS
# Numbers, which throws "Cannot convert <n> to a BigInt" every audio buffer on a
# wasm64 build (fatal under load). We install a window.miniaudio.toPtr() helper
# (BigInt-wrap iff sizeof(void*)==8, a compile-time constant passed as an EM_ASM
# arg) and route those pointer args through it. Identity on wasm32.
#
# Invoked at configure time: cmake -DMINIAUDIO_H=<path> -P miniaudio_memory64.cmake
# Each string(REPLACE) only fires when the original text is present, so re-running
# (or running against an already-patched copy) is a no-op.

if(NOT EXISTS "${MINIAUDIO_H}")
    message(FATAL_ERROR "miniaudio_memory64.cmake: file not found: ${MINIAUDIO_H}")
endif()

file(READ "${MINIAUDIO_H}" _ma)

# Idempotency guard: key on the LAST-added patch marker (the worklet non-blocking
# patch), not the first (toPtr). Otherwise a tree already patched by an older
# version of this script (toPtr only) would early-return and never receive a
# newly-added block. With this marker, a toPtr-only tree still runs the script;
# the toPtr string(REPLACE)s are no-ops (targets already gone) and only the new
# worklet block applies.
string(FIND "${_ma}" "daslang non-blocking patch (see miniaudio_memory64.cmake)" _already)
if(_already GREATER -1)
    message(STATUS "miniaudio_memory64.cmake: already patched, skipping ${MINIAUDIO_H}")
    return()
endif()

# 1) Install the toPtr helper right after the device-state setup ($5 = sizeof(void*)==8).
string(REPLACE
[==[            window.miniaudio.device_state.started = $4;]==]
[==[            window.miniaudio.device_state.started = $4;

            /* memory64: wasm exports take i64 (BigInt) pointers, wasm32 takes i32
               (Number). $5 = (sizeof(void*)==8). Wrap any pointer passed to a _ma_*
               wasm export with toPtr() so the same JS works on both ABIs. */
            window.miniaudio.toPtr = $5 ? (function(p){ return BigInt(p); }) : (function(p){ return p; });]==]
    _ma "${_ma}")

# 2) Pass sizeof(void*)==8 as the extra EM_ASM arg ($5) of the context-init block.
string(REPLACE
[==[    }, ma_device_type_playback, ma_device_type_capture, ma_device_type_duplex, ma_device_state_stopped, ma_device_state_started);]==]
[==[    }, ma_device_type_playback, ma_device_type_capture, ma_device_type_duplex, ma_device_state_stopped, ma_device_state_started, (int)(sizeof(void*) == 8));]==]
    _ma "${_ma}")

# 3) on_notification_unlocked (unlock handler).
string(REPLACE
[==[                            _ma_device__on_notification_unlocked(device.pDevice);]==]
[==[                            _ma_device__on_notification_unlocked(miniaudio.toPtr(device.pDevice));]==]
    _ma "${_ma}")

# 4) capture callback.
string(REPLACE
[==[                    _ma_device_process_pcm_frames_capture__webaudio(pDevice, bufferSize, pIntermediaryBuffer);]==]
[==[                    _ma_device_process_pcm_frames_capture__webaudio(window.miniaudio.toPtr(pDevice), bufferSize, window.miniaudio.toPtr(pIntermediaryBuffer));]==]
    _ma "${_ma}")

# 5) playback callback.
string(REPLACE
[==[                    _ma_device_process_pcm_frames_playback__webaudio(pDevice, bufferSize, pIntermediaryBuffer);]==]
[==[                    _ma_device_process_pcm_frames_playback__webaudio(window.miniaudio.toPtr(pDevice), bufferSize, window.miniaudio.toPtr(pIntermediaryBuffer));]==]
    _ma "${_ma}")

# 6) AudioWorklet non-blocking init. miniaudio's worklet ma_device_init busy-waits
# `while (initResult==MA_BUSY) emscripten_sleep(1);` for the async worklet to spin
# up — the ONE thing that forces -sASYNCIFY, which is incompatible with our
# -fwasm-exceptions build (binaryen's asyncify pass crashes on the memory64+EH
# module). Replace the wait + BUSY error-check with a descriptor pre-fill from the
# requested config so ma_device_init returns immediately; the worklet connects
# asynchronously (the device callback isn't invoked until it does) and the
# processor-created callback re-fills these descriptors with identical values.
# Only compiled when MA_USE_AUDIO_WORKLETS (the threaded web build); inert
# otherwise. Verified end-to-end: a 440Hz worklet tone plays in Chrome on
# memory64+pthread+wasm-EH with no asyncify.
string(REPLACE
[==[        while (pDevice->webaudio.initResult == MA_BUSY) { emscripten_sleep(1); }    /* We must wait for initialization to complete. We're just spinning here. The emscripten_sleep() call is why we need to build with `-sASYNCIFY`. */

        /* Initialization is now complete. Descriptors were updated when the worklet was initialized. */
        if (pDevice->webaudio.initResult != MA_SUCCESS) {
            ma_free(pStackBuffer, &pDevice->pContext->allocationCallbacks);
            emscripten_destroy_audio_context(pDevice->webaudio.audioContext);
            return pDevice->webaudio.initResult;
        }]==]
[==[        /* daslang non-blocking patch (see miniaudio_memory64.cmake): drop the
           emscripten_sleep busy-wait that forces -sASYNCIFY. Pre-fill descriptors
           from config; the worklet connects asynchronously. */
        {
            ma_uint32 awCh = (pDescriptorPlayback != NULL && pDescriptorPlayback->channels > 0) ? pDescriptorPlayback->channels : MA_DEFAULT_CHANNELS;
            if (pDescriptorPlayback != NULL) {
                pDescriptorPlayback->format             = ma_format_f32;
                pDescriptorPlayback->channels           = awCh;
                ma_channel_map_init_standard(ma_standard_channel_map_webaudio, pDescriptorPlayback->channelMap, ma_countof(pDescriptorPlayback->channelMap), pDescriptorPlayback->channels);
                pDescriptorPlayback->periodSizeInFrames = 128;
                pDescriptorPlayback->periodCount        = 1;
            }
            if (pDescriptorCapture != NULL) {
                ma_uint32 awCapCh = (pDescriptorCapture->channels > 0) ? pDescriptorCapture->channels : MA_DEFAULT_CHANNELS;
                pDescriptorCapture->format              = ma_format_f32;
                pDescriptorCapture->channels            = awCapCh;
                ma_channel_map_init_standard(ma_standard_channel_map_webaudio, pDescriptorCapture->channelMap, ma_countof(pDescriptorCapture->channelMap), pDescriptorCapture->channels);
                pDescriptorCapture->periodSizeInFrames  = 128;
                pDescriptorCapture->periodCount         = 1;
            }
            /* The descriptors are LOCALS of the generic ma_device_init and die when
               it returns (right after this non-blocking init). The generic init has
               already read back our pre-filled values above, so NULL the worklet
               callback's copies (it null-guards them) to stop it writing through the
               now-dangling pointers when it fires asynchronously. */
            pInitParameters->pDescriptorPlayback = NULL;
            pInitParameters->pDescriptorCapture  = NULL;
        }]==]
    _ma "${_ma}")

# 7) AudioWorklet dangling-config fix. The processor-created callback runs long
# after ma_device_init returns (non-blocking, block 6), so pConfig — a local of
# the generic ma_device_init — is dead. Read the device type from the long-lived
# pDevice instead, or the `if (deviceType == playback) connect(destination)` is
# skipped (garbage type) and the worklet node is created but never connected ->
# silent. string(REPLACE) hits all four reads in the callback.
string(REPLACE
[==[pParameters->pConfig->deviceType]==]
[==[pParameters->pDevice->type]==]
    _ma "${_ma}")

file(WRITE "${MINIAUDIO_H}" "${_ma}")

string(FIND "${_ma}" "window.miniaudio.toPtr" _has)
if(_has GREATER -1)
    message(STATUS "miniaudio_memory64.cmake: memory64 WebAudio pointer patch present in ${MINIAUDIO_H}")
else()
    message(WARNING "miniaudio_memory64.cmake: patch anchors not found — miniaudio version may have changed (expected 0.11.25)")
endif()
