---
slug: dasimgui-imtextureid-type-and-where-to-get-a-real-texture-handle
title: In dasImgui, what's ImTextureID — what daslang type does ImageButton's user_texture_id take, and where do I get a real texture handle to pass?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Type:** `void?` in the daslang binding. There's no `ImTextureID` typedef on the daslang side — `ImageButton(label, user_texture_id : void?, size, uv0, uv1, bg_col, tint_col)` and `GetTexID()`/`SetTexID()` all use `void?` directly.

**Source of a real handle for testing/demos:** `unsafe(GetIO()).Fonts.TexID` — the font atlas texture is always available after the renderer backend has called `NewFrame()` once. It's a `void?` field (set by the renderer; non-null after first frame). Used by `modules/dasImgui/example/imgui_demo.das:5127` and the 0b.1 trigger smoke `modules/dasImgui/examples/features/triggers.das`.

**Backend-specific underneath:** for the OpenGL backend, the `void?` wraps a `GLuint`; for D3D it wraps a different pointer. ImGui treats it as opaque — your widget code never inspects it.

**Don't pass `null`:** ImageButton with `null` user_texture_id is undefined behavior in most backends — likely segfault in the GL render path. Always guard `if (font_tex != null) { image_button(...) }` if the texture might not be ready yet.

**Phase 0b.1 widget signature:**
```das
[widget]
def image_button(var state : ClickState; text : string; user_texture_id : void?; size : float2;
                 uv0 : float2 = float2(0.0f, 0.0f);
                 uv1 : float2 = float2(1.0f, 1.0f);
                 bg_col : float4 = float4(0.0f, 0.0f, 0.0f, 0.0f);
                 tint_col : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)) : bool
```

## Questions
- In dasImgui, what's ImTextureID — what daslang type does ImageButton's user_texture_id take, and where do I get a real texture handle to pass?
