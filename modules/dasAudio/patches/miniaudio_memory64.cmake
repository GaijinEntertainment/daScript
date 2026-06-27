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

# Idempotency guard: if the helper is already present, this copy is patched.
string(FIND "${_ma}" "window.miniaudio.toPtr" _already)
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

file(WRITE "${MINIAUDIO_H}" "${_ma}")

string(FIND "${_ma}" "window.miniaudio.toPtr" _has)
if(_has GREATER -1)
    message(STATUS "miniaudio_memory64.cmake: memory64 WebAudio pointer patch present in ${MINIAUDIO_H}")
else()
    message(WARNING "miniaudio_memory64.cmake: patch anchors not found — miniaudio version may have changed (expected 0.11.25)")
endif()
