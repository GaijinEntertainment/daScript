---
slug: how-do-i-marshal-an-inputtext-buffer-between-daslang-array-uint8-and-imgui-s-char-without-the-legacy-dasimguiinputtext-mirror-st
title: How do I marshal an InputText buffer between daslang `array<uint8>` and ImGui's `char *` without the legacy `DasImguiInputText` mirror struct?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Pass `addr(buffer[0])` directly; the C++ side casts `uint8_t*` → `char*`.** The whole `DasImguiInputText` struct in `modules/dasImgui/src/dasIMGUI.main.cpp` exists to wrap a `TArray<uint8_t> buffer` field for the v1 `ImGuiInputTextBuffer` daslang surface. For new widget code, drop the wrapper and keep the buffer as a plain `array<uint8>` field on the state struct.

**C++ side** (one line per overload, no struct):
```cpp
bool InputTextBasic ( uint8_t * buf, int buf_size, const char * label, ImGuiInputTextFlags_ flags ) {
    return ImGui::InputText(label, (char *)buf, buf_size, flags);
}
```
Bind it the usual way:
```cpp
addExtern<DAS_BIND_FUN(das::InputTextBasic)>(*this, lib, "_builtin_InputText_basic",
    SideEffects::worstDefault, "das::InputTextBasic");
```

**Daslang side** (state owns the buffer + a string mirror; widget body marshals):
```das
struct InputTextState {
    @safe_when_uninitialized @live value : string         //! mirror of buffer-as-string
    @live capacity : int                                  //! buffer.size; promoted from 0 → 256 first frame
    @optional has_pending : bool
    @safe_when_uninitialized @optional pending_value : string
    @optional changed : bool
    @optional buffer : array<uint8>                       // transient working buffer
}

def private input_text_buffer_init(var state : InputTextState) {
    if (state.capacity == 0) { state.capacity = 256 }
    if (length(state.buffer) != state.capacity) {
        state.buffer |> resize(state.capacity)
        sync_buffer_from_value(state)
    }
}

def private sync_buffer_from_value(var state : InputTextState) {
    for (i in range(state.capacity)) { state.buffer[i] = 0u8 }
    let want = length(state.value)
    if (want > 0) {
        let n = min(want, state.capacity - 1)
        unsafe { memcpy(addr(state.buffer[0]), reinterpret<void?>(state.value), n) }
    }
}

def private sync_value_from_buffer(var state : InputTextState) {
    unsafe {
        let pbuf = addr(state.buffer[0])
        state.value := clone_string(reinterpret<string>(pbuf))
    }
}

[widget]
def input_text(var state : InputTextState; text : string;
               flags : ImGuiInputTextFlags = ImGuiInputTextFlags.None) : bool {
    input_text_buffer_init(state)
    apply_pending_text(state)        // imgui_set L3 path
    let changed = unsafe(_builtin_InputText_basic(
        addr(state.buffer[0]), state.capacity, text, flags))
    state.changed = changed
    if (changed) { sync_value_from_buffer(state) }
    text_value_finalize(widget_ident, "input_text", state)
    return changed
}
```

**Why these moving pieces:**

- **`@optional buffer`** is dropped on hot-reload. `length(state.buffer) != state.capacity` triggers re-allocate-and-refill from `state.value` on the first post-reload frame. Same path covers initial frame.
- **`sync_buffer_from_value` zeros first** so the trailing null is preserved (ImGui scans for null to determine end-of-string). `min(want, capacity - 1)` reserves the byte.
- **`sync_value_from_buffer` only runs on `changed`** (one extra clone per *user keystroke*, not per frame). Keystroke rates won't notice.
- **`memcpy(addr(buf[0]), reinterpret<void?>(state.value), n)`** is the daslang idiom for "copy N bytes from a string into a uint8 array". `reinterpret<void?>(state.value)` exposes the string's underlying pointer for the byte copy.

**For the growable variant** (CallbackResize), use `_builtin_InputText_cb` (the `_cb` forwarder takes a `Lambda` parameter and stack-allocates a thunk for the callback's lifetime — see `are-imgui-inputtext-combo-callbacks-fired-synchronously-inside-the-c-call-can-i-stack-allocate-the-callback-thunk`):

```das
unsafe {
    changed = _builtin_InputText_cb(addr(state.buffer[0]), state.capacity, text, cb_flags,
        @ capture(& state) (var data : ImGuiInputTextCallbackData) : int {
            if (data.EventFlag == ImGuiInputTextFlags.CallbackResize) {
                state.buffer |> resize(data.BufSize)
                state.capacity = data.BufSize
                data.Buf = unsafe(reinterpret<int8?>(addr(state.buffer[0])))
            }
            return 0
        })
}
```

`data.Buf` is the writable `char*` field on the bound `ImGuiInputTextCallbackData`; daslang exposes it as `int8?`. After resize, repoint Buf to the new array start, return 0, ImGui continues with the larger buffer.

**Found 2026-05-10**, dasImgui Phase 0b.4. Replaces the 80-line `DasImguiInputText` shim machinery in the v1 `daslib/imgui_boost.das` surface (which stays untouched as the v1 v1 surface — only new widget code uses this pattern).

## See also
- `are-imgui-inputtext-combo-callbacks-fired-synchronously-inside-the-c-call-can-i-stack-allocate-the-callback-thunk` — the lifetime contract that lets the callback variant work without a struct mirror
- `how-do-i-expose-a-daslang-string-value-through-a-widget-state-struct-without-tripping-error-31014-uninitialized-variable-is-unsafe-at-compile-time` — companion: why the `value` field needs `@safe_when_uninitialized`

## Questions
- How do I marshal an InputText buffer between daslang `array<uint8>` and ImGui's `char *` without the legacy `DasImguiInputText` mirror struct?
