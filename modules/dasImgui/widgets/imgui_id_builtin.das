options gen2
options indenting = 4
options no_unused_block_arguments = false

module imgui_id_builtin shared

require imgui public
require imgui/imgui_boost_runtime public

//! `with_id(s) { ... }` — explicit ID-stack push for the cases where the
//! indexed-widget form (`IDENT[i]`/`IDENT[key]`, API_REWORK §4.4) can't
//! reach: composed sub-panels rendered by a helper function that doesn't
//! know which "slot" it's in, or data-driven structure the call_macro can't
//! trace lexically. Per API_REWORK §4.8.
//!
//! Effect: pushes `s` onto both ImGui's internal ID stack (so widgets
//! rendered inside the block get a distinct ImGui hash from siblings under
//! a different `with_id`) AND the boost's container-path stack (so leaf
//! telemetry paths and registry keys include `s` as a segment). Both pop
//! on block return.
//!
//! `check_unique_render` keys off the full path, so two render sites of
//! the same single-global widget under different `with_id` scopes don't
//! trip the once-per-frame panic — but the widget's state global is still
//! shared. To get distinct state per scope, use the indexed form instead.

def public with_id(s : string; blk : block) {
    container_path_push(s)
    PushID(s)
    invoke(blk)
    PopID()
    container_path_pop()
}
