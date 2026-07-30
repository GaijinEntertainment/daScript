options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: tree_node — expandable branch with auto-paired TreePop.
//
//   tree_node(IDENT, (text = "..", flags = ImGuiTreeNodeFlags....)) { body }
//
// The [container] form. Body runs only while ImGui reports the node open
// (chevron expanded). The wrapper handles TreePop automatically when the
// body's been invoked. Three distinct knobs:
//
//   1. flags — ImGuiTreeNodeFlags bitfield. DefaultOpen, OpenOnArrow,
//      OpenOnDoubleClick, Leaf, Bullet, Framed, NoTreePushOnOpen.
//
//   2. state.pending_open / pending_close — flips SetNextItemOpen(true/false,
//      Always) on the NEXT frame's TreeNodeEx call. The live commands
//      imgui_open / imgui_close ride this channel.
//
//   3. state.opened — per-frame read-only mirror of TreeNodeEx's bool
//      return. A driver checks "did the user actually expand this?" by
//      reading state.opened on the snapshot.
//
// tree_node vs tree_node_ex:
//   - tree_node (this tutorial) — [container] block-arg, auto-pairs TreePop.
//   - tree_node_ex — [widget] leaf form, caller drives TreePop manually
//     (see examples/features/tree_node_ex.das). Use it when the body needs
//     to render OUTSIDE the open conditional, e.g. same_line() with a
//     sibling button on the header row.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/tree_node.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/tree_node.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_open","args":{"target":"TN_WIN/PHYSICS_TREE"}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_close","args":{"target":"TN_WIN/PHYSICS_TREE"}}' \
//        localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui tree_node tutorial", 720, 540)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    PHYSICS_GRAVITY.bounds = (0.0f, 50.0f)
    PHYSICS_DENSITY.bounds = (0.0f, 10.0f)
    RENDER_FOV.bounds = (30.0f, 120.0f)

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(680.0f, 500.0f), ImGuiCond.Always)
    window(TN_WIN, (text = "tree_node tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Three patterns - DefaultOpen, nested tree_nodes, and OpenOnArrow.")
        separator()

        // ---- A: DefaultOpen (chevron down from frame 1) ----
        tree_node(PHYSICS_TREE, (text = "Physics",
                                 flags = ImGuiTreeNodeFlags.DefaultOpen)) {
            slider_float(PHYSICS_GRAVITY, (text = "gravity"))
            slider_float(PHYSICS_DENSITY, (text = "density"))
            text("PHYSICS_TREE.opened = {PHYSICS_TREE.opened}")
        }

        // ---- B: nested tree_nodes — Render > [Camera | Mesh] ----
        tree_node(RENDER_TREE, (text = "Render",
                                flags = ImGuiTreeNodeFlags.None)) {
            tree_node(CAMERA_SUBTREE, (text = "Camera",
                                       flags = ImGuiTreeNodeFlags.DefaultOpen)) {
                slider_float(RENDER_FOV, (text = "fov"))
                checkbox(CAMERA_ORTHO, (text = "Orthographic"))
            }
            tree_node(MESH_SUBTREE, (text = "Mesh",
                                     flags = ImGuiTreeNodeFlags.None)) {
                checkbox(MESH_WIRE, (text = "Wireframe"))
                checkbox(MESH_NORMALS, (text = "Show normals"))
            }
        }

        // ---- C: OpenOnArrow — clicking the label doesn't expand ----

        // DefaultOpen keeps ASSETS_HINT in the registry from frame 1. OpenOnArrow
        // is still shown: collapse via chevron, then click the label — it stays
        // collapsed; only the arrow toggles.
        tree_node(ASSETS_TREE, (text = "Assets (OpenOnArrow + DefaultOpen)",
                                flags = ImGuiTreeNodeFlags.OpenOnArrow | ImGuiTreeNodeFlags.DefaultOpen)) {
            text(ASSETS_HINT, (text = "OpenOnArrow flag - clicking the LABEL doesn't toggle."))
            text("Click the arrow chevron specifically. Useful for")
            text("selectable-row trees where the label is the selection.")
        }
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
