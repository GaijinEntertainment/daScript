options gen2
options indenting = 4
options persistent_heap = true
options gc
options _allow_glfw_calls = true

require imgui/imgui_harness
require imgui/imgui_live
require imgui/imgui_file_dialog
require opengl/opengl_boost
require live/glfw_live
require gltf/gltf_boost
require gltf/gltf_gl
require gltf/gltf_pbr
require daslib/safe_addr
require daslib/fio
require math
require daslib/math_boost
require strings

// dasGLTF mesh viewer (OpenGL + Dear ImGui). Loads a .glb/.gltf, orbits it under the metallic-roughness
// PBR shader, and exposes the model transform, animation playback and lighting through docked ImGui panels.
// The 3D is drawn between the GL clear and ImGui's RenderDrawData (a manual harness_end_frame split), with a
// PassthruCentralNode dockspace so the panels dock left and the render shows through the centre. Drag to
// orbit, scroll to zoom; "Open..." picks another model. Defaults to the shipped BoomBox (full PBR).

// ----- viewer state (all main-thread; init() loads, update() drives) -----
var g_scene    : GltfScene
var g_model    : GltfGlModel
var g_renderer : GltfPbrRenderer

var g_center : float3
var g_radius = 1.0f

// orbit camera (mouse drag + scroll over the central viewport)
var g_cam_yaw   = 0.6f
var g_cam_pitch = 0.3f
var g_cam_dist  = 2.2f

// model transform (Model window): euler degrees + uniform scale, applied about the model centre
var g_rot_euler   : float3
var g_scale       = 1.0f
var g_auto_rotate = false

// animation (Model window)
var g_clip       = 0
var g_playing    = true
var g_anim_time  = 0.0f
var g_clip_names : array<string>

// lighting (Lighting window) — flat globals so safe_addr binds a stable top-level address
var g_light_dir       = float3(0.3, 0.7, 0.5)
var g_light_color     = float3(1.0, 1.0, 1.0)
var g_light_intensity = 3.0f
var g_light_ambient   = float3(0.15, 0.15, 0.18)
var g_env_amount      = 1.0f
var g_env_tex         = 0u

var g_model_name = ""
var g_load_error = ""

def media_dir : string {
    return "{get_das_root()}/examples/gltf/media"
}

def model_path : string {
    // get_command_line_arguments() includes the interpreter + script + harness flags, so pick the first
    // argument that actually names a glTF file; otherwise fall back to the shipped BoomBox.
    for (a in get_command_line_arguments()) {
        if (ends_with(a, ".glb") || ends_with(a, ".gltf")) {
            return a
        }
    }
    return "{media_dir()}/BoomBox.glb"
}

// Load a model, validate it, then swap it in (keeping the old one on failure) and reframe the camera. Shared
// by init() and the "Open..." dialog. Needs a current GL context — upload_gltf allocates GL buffers/textures.
def load_model(path : string) {
    if (!stat(path).is_valid) {
        g_load_error = "not found: {base_name(path)}"
        return
    }
    var scene <- load_gltf(path)
    if (empty(scene.meshes)) {
        delete scene
        g_load_error = "no meshes in {base_name(path)}"
        return
    }
    var model <- upload_gltf(scene)
    delete g_model
    delete g_scene
    g_scene <- scene   // nolint:PERF030 - released by the delete above; the rule only tracks array/table finalize, not struct
    g_model <- model   // nolint:PERF030 - released by the delete above; the rule only tracks array/table finalize, not struct

    let bounds = gltf_scene_bounds(g_scene)
    g_center = (bounds._0 + bounds._1) * 0.5f
    g_radius = max(length(bounds._1 - bounds._0) * 0.5f, 0.001f)
    g_cam_dist = g_radius * 2.2f

    g_model_name = base_name(path)
    g_clip = 0
    g_anim_time = 0.0f
    g_playing = true
    g_rot_euler = float3(0.0, 0.0, 0.0)
    g_scale = 1.0f
    g_clip_names |> clear()
    g_clip_names |> reserve(length(g_scene.animations))
    for (a in g_scene.animations) {
        g_clip_names |> push(empty(a.name) ? "clip {length(g_clip_names)}" : a.name)
    }
    g_load_error = ""
}

def open_model_dialog {
    file_dialog_open(FileDialogConfig(title = "Open glTF model", start_dir = media_dir(),
        filters <- [FileDialogFilter(caption = "glTF (*.glb *.gltf)", ext = "glb;gltf")]))
}

// Restore the lighting controls to the GltfPbrLight defaults (single source of truth).
def reset_lighting {
    let d = GltfPbrLight()
    g_light_dir = d.direction
    g_light_color = d.color
    g_light_intensity = d.intensity
    g_light_ambient = d.ambient
    g_env_amount = d.environment
}

// T(centre) * R * S * T(-centre): rotate/scale about the model centre so the framed model stays framed.
def model_root_transform : float4x4 {
    let q = quat_from_euler(g_rot_euler * (PI / 180.0f))
    let rs = compose(float3(0.0, 0.0, 0.0), q, float3(g_scale))
    return translation(g_center) * rs * translation(-g_center)
}

// Dock the two panels down the left column (Model over Lighting); the central node stays passthru for the 3D.
def setup_default_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var left_id = 0u
    var center_id = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.24f, left_id, center_id)
    var left_top = 0u
    var left_bottom = 0u
    DockBuilderSplitNode(left_id, ImGuiDir.Up, 0.5f, left_top, left_bottom)
    DockBuilderDockWindow("Model", left_top)
    DockBuilderDockWindow("Lighting", left_bottom)
    DockBuilderFinish(dock_id)
}

def build_ui {
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            setup_default_layout(DOCK_ROOT.dock_id)
            DOCK_ROOT.has_initial_layout = true
        }
        dock_window(MODEL_WIN, (text = "Model", closable = false, flags = ImGuiWindowFlags.None)) {
            text("{g_model_name}")
            same_line(SL_OPEN)
            if (button(OPEN_BTN, (text = "Open..."))) {
                open_model_dialog()
            }
            if (!empty(g_load_error)) {
                text("{g_load_error}")
            }
            text("meshes {length(g_scene.meshes)}   materials {length(g_scene.materials)}   nodes {length(g_scene.nodes)}")
            separator(SEP_ANIM)
            if (!empty(g_scene.animations)) {
                edit_combo(safe_addr(g_clip), (id = "CLIP", text = "Clip", items = g_clip_names))
                g_clip = clamp(g_clip, 0, length(g_scene.animations) - 1)
                edit_checkbox(safe_addr(g_playing), (id = "PLAY", text = "Play"))
                edit_slider_float(safe_addr(g_anim_time),
                    (id = "TIME", text = "Time", v_min = 0.0f, v_max = max(g_scene.animations[g_clip].duration, 0.001f)))
            } else {
                text("(no animations)")
            }
            separator(SEP_XFORM)
            edit_slider_float3(safe_addr(g_rot_euler), (id = "ROT", text = "Rotation", v_min = -180.0f, v_max = 180.0f))
            edit_slider_float(safe_addr(g_scale), (id = "SCALE", text = "Scale", v_min = 0.05f, v_max = 4.0f))
            edit_checkbox(safe_addr(g_auto_rotate), (id = "AUTOROT", text = "Auto-rotate"))
            separator(SEP_HINT)
            text("drag: orbit   scroll: zoom")
        }
        dock_window(LIGHT_WIN, (text = "Lighting", closable = false, flags = ImGuiWindowFlags.None)) {
            edit_slider_float3(safe_addr(g_light_dir), (id = "LDIR", text = "Direction", v_min = -1.0f, v_max = 1.0f))
            edit_slider_float(safe_addr(g_light_intensity), (id = "LINT", text = "Intensity", v_min = 0.0f, v_max = 10.0f))
            edit_color_edit3(safe_addr(g_light_color), (id = "LCOL", text = "Color"))
            edit_color_edit3(safe_addr(g_light_ambient), (id = "LAMB", text = "Ambient"))
            edit_slider_float(safe_addr(g_env_amount), (id = "ENV", text = "Environment", v_min = 0.0f, v_max = 2.0f))
            separator(SEP_LIGHT)
            if (button(RESET_LIGHT_BTN, (text = "Reset"))) {
                reset_lighting()
            }
        }
    }
}

// Mouse orbit + scroll zoom while the pointer isn't over an ImGui window, then advance/pose the animation.
def update_input_and_anim(dt : float) {
    let io & = unsafe(GetIO())
    if (!io.WantCaptureMouse) {
        if (IsMouseDown(ImGuiMouseButton.Left)) {
            g_cam_yaw -= io.MouseDelta.x * 0.01f
            g_cam_pitch = clamp(g_cam_pitch + io.MouseDelta.y * 0.01f, -1.5f, 1.5f)
        }
        if (io.MouseWheel != 0.0f) {
            g_cam_dist = clamp(g_cam_dist * (io.MouseWheel > 0.0f ? 0.9f : 1.1f), g_radius * 1.1f, g_radius * 40.0f)
        }
    }
    if (g_auto_rotate) {
        g_cam_yaw += dt * 0.5f
    }
    let anim_count = length(g_scene.animations)
    if (anim_count > 0) {
        let dur = g_scene.animations[g_clip].duration
        if (g_playing) {
            g_anim_time += dt
        }
        if (dur > 0.0f && g_anim_time >= dur) {
            g_anim_time -= dur * floor(g_anim_time / dur)
        }
        evaluate_animation(g_scene, g_clip, g_anim_time)
    }
}

def draw_scene {
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClearDepth(1.0lf)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LESS)

    let aspect = h != 0 ? float(w) / float(h) : 1.0f
    let cp = cos(g_cam_pitch)
    let eye = g_center + float3(cp * sin(g_cam_yaw), sin(g_cam_pitch), cp * cos(g_cam_yaw)) * g_cam_dist
    let view = look_at_rh(eye, g_center, float3(0.0, 1.0, 0.0))
    let proj = perspective_rh_minus1_to_1(45.0f * PI / 180.0f, aspect, g_radius * 0.05f, g_radius * 60.0f)
    let light = GltfPbrLight(direction = g_light_dir, color = g_light_color,
        intensity = g_light_intensity, ambient = g_light_ambient, environment = g_env_amount)

    gltf_pbr_render(g_renderer, g_model, g_scene, model_root_transform(), view, proj, eye, light)

    // hand a clean state back to the ImGui GL3 backend (it sets up its own, but leave depth/cull/blend off)
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glDisable(GL_BLEND)
}

[export]
def init {
    let path = model_path()
    harness_init("dasGLTF Mesh Viewer", 1280, 720)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable

    g_renderer <- gltf_pbr_create()
    // equirect HDR environment for image-based ambient (metals reflect it); CC0, shared with tutorial 12
    let env = gltf_gl_load_hdr_env("{get_das_root()}/tutorials/_assets/hdri/cannon_2k.hdr")
    g_env_tex = env._0
    gltf_pbr_set_environment(g_renderer, env._0, env._1)
    load_model(path)
}

[export]
def update {
    if (!harness_begin_frame()) {
        return
    }
    harness_new_frame()

    build_ui()
    if (file_dialog_draw() == FileDialogResult.confirmed) {
        load_model(file_dialog_selected_path())
    }
    let io & = unsafe(GetIO())
    update_input_and_anim(io.DeltaTime)
    draw_scene()

    end_of_frame()
    Render()
    live_imgui_render()
    live_end_frame()
}

[export]
def shutdown {
    if (g_env_tex != 0u) {
        glDeleteTextures(1, safe_addr(g_env_tex))
    }
    delete g_renderer
    delete g_model
    delete g_scene
    harness_shutdown()
}

[export]
def main {
    init()
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
    }
    shutdown()
}
