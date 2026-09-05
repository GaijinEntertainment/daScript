options gen2
options persistent_heap
options stack = 262144
require dastest/testing_boost public
require glfw/glfw_boost
require opengl/opengl_boost
require opengl/opengl_ttf
require opengl/opengl_cache
require daslib/fio
require math

[test]
def test_core_profile_cleanup(t : T?) {
    if (glfwInit() == 0) { t |> skip("No window system available for the core-profile check"); return }
    glfwInitOpenGL(3, 3)
    glfwWindowHint(int(GLFW_VISIBLE), GLFW_FALSE)
    glfwWindowHint(int(GLFW_OPENGL_PROFILE), int(GLFW_OPENGL_CORE_PROFILE))
    let window = glfwCreateWindow(64, 64, "core-state-test", null, null)
    if (window == null) { glfwTerminate(); t |> skip("No OpenGL core context available"); return }
    glfwMakeContextCurrent(window)
    for (_i in range(8)) { glGetError() }
    glBindVertexArray(0u)
    glDisableVertexAttribArray(0u)
    t |> equal(glGetError(), GL_INVALID_OPERATION, "control: mutating vertex attributes without a VAO is invalid")
    bind_ffp()
    t |> equal(glGetError(), 0u, "generic cleanup must not mutate VAO zero")
    create_ttf_objects()
    var font <- load_ttf(path_join(get_das_root(), "modules/dasStbImage/fonts/droidsansmono.ttf"))
    if (font.bitmap.width > 0) {
        upload_font_texture(font)
        var quads <- font |> create_quads("core")
        font |> draw_quads(quads, identity4x4(), float3(1.0))
        delete quads
        destroy_font_texture(font)
        t |> equal(glGetError(), 0u, "font draw and cleanup are core-profile safe")
    } else {
        t |> failure("Could not load the test font")
    }
    delete font
    glfwDestroyWindow(window)
    glfwTerminate()
}
