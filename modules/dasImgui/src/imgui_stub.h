#include <imgui.h>
// internal API surface — the binder now cherry-picks symbols from
// imgui_internal.h (Render* draw primitives, DockBuilder*, ...), so the
// generated func glue needs these declarations visible at compile time.
#include <imgui_internal.h>
