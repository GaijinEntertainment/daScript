---
slug: are-imgui-inputtext-combo-callbacks-fired-synchronously-inside-the-c-call-can-i-stack-allocate-the-callback-thunk
title: Are ImGui InputText/Combo callbacks fired synchronously inside the C call? Can I stack-allocate the callback thunk?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Yes, fully synchronous.** ImGui invokes `ImGuiInputTextCallback` (and `ImGuiComboGetter`-style item callbacks) from inside the `InputText()` / `Combo()` call itself, on the same thread, in the same C frame. There is no queueing, no deferred dispatch, no other-thread invocation. The callback's `data` pointer is also only valid for the duration of that one call.

This collapses what looks like a complex ABI problem (binding a daslang lambda to a C function pointer) into a stack-allocated thunk. You don't need:

- a daslang struct that mirrors a C struct holding the lambda (the `DasImguiInputText` v1 pattern)
- ABI pinning between daslang field order and C field order
- a long-lived registration table

You only need a stack-local POD plus one trampoline:

```cpp
struct InputTextLambdaThunk {
    Context *  context;
    LineInfo * at;
    Lambda     lambda;       // by value — capture pointer
};

int InputTextLambdaTrampoline(ImGuiInputTextCallbackData * data) {
    auto t = (InputTextLambdaThunk *) data->UserData;
    if ( !t->lambda.capture ) {
        t->context->throw_error_at(t->at, "InputText callback: lambda has no capture");
    }
    return das_invoke_lambda<int>::invoke<ImGuiInputTextCallbackData *>(
        t->context, t->at, t->lambda, data);
}

bool InputTextCb(uint8_t * buf, int buf_size, const char * label, ImGuiInputTextFlags_ flags,
                 Lambda cb, Context * context, LineInfoArg * at) {
    InputTextLambdaThunk thunk { context, at, cb };
    return ImGui::InputText(label, (char *)buf, buf_size, flags,
                             &InputTextLambdaTrampoline, &thunk);
}
```

The thunk is on the C stack. ImGui calls the trampoline 0..N times before `InputText` returns. Then the stack frame unwinds; the thunk is gone; the daslang lambda's capture-block was never referenced after the C call returned.

The same shape works for `ImGui::Combo(label, &cur, &getter_fn, &thunk, items_count, ...)` and any other "callback fires inside the call" ImGui surface (`SetNextWindowSizeConstraints`, `Plot*`, etc.). The legacy `DasImguiInputText` / `ImGuiComboGetter` daslang↔C++ mirror structs in `modules/dasImgui/src/dasIMGUI.main.cpp` exist because the original 2018-era pattern stored callbacks in long-lived struct fields without realizing the lifetime requirement was actually one C frame.

**Watch out** for the *non-synchronous* exceptions: any callback you pass to ImGui that's expected to fire on a *future* frame (e.g. tool-callback hooks set via `io.ConfigCallback*`, custom drawcall callbacks added via `AddCallback` and rendered later by `RenderDrawData`) DOES outlive the call that registered it — those still need a long-lived struct or registration table. The InputText/Combo/SizeConstraints/Plot family is the synchronous one.

**Found 2026-05-10**, dasImgui Phase 0b.4. Pattern verified working with stack-thunk replacement of `DasImguiInputText` for `input_text` / `input_text_with_hint` / `input_text_multiline` / `input_text_growable` (CallbackResize) + `combo_getter`.

## See also
- `daslang-strings-cannot-carry-embedded-nulls-use-array-uint8-and-reinterpret-string-temp-for-c-api-zero-separated-buffers` — companion pattern for InputText buffer storage

## Questions
- Are ImGui InputText/Combo callbacks fired synchronously inside the C call? Can I stack-allocate the callback thunk?
