options gen2
options persistent_heap
require dastest/testing_boost public
require daslib/defer
require glfw/glfw_boost
require opengl/opengl_boost

var @uniform retirement_color : float4
var @out retirement_output : float4
[vertex_program]
def retirement_vs {gl_Position = float4(0.0, 0.0, 0.0, 1.0)}
[fragment_program]
def retirement_fs {retirement_output = retirement_color}

def private fill_and_retire(program : uint){
    gl_binding_begin()
    gl_use_program_filtered(program)
    glUniformBound(program, "retirement_color", float4(0.5))
    gl_binding_end()
    gl_binding_forget_program(program)
}

[test]
def retired_program_releases_nested_uniform_storage(t : T?){
    if (glfwInit() == 0){t |> skip("GLFW display unavailable"); return }
    glfwInitOpenGL(3, 3)
    glfwWindowHint(int(GLFW_VISIBLE), GLFW_FALSE)
    let window = glfwCreateWindow(32, 32, "Binding retirement", null, null)
    if (window == null){glfwTerminate(); t |> skip("OpenGL context unavailable"); return }
    glfwMakeContextCurrent(window)
    let program = create_shader_program(@@retirement_vs, @@retirement_fs)
    defer(){gl_binding_clear(); glDeleteProgram(program); glfwDestroyWindow(window); glfwTerminate()}
    t |> success(program != 0u)
    if (program == 0u){return }
    var recovered = false
    try {gl_binding_scope(true) $ {panic("scope failure")}} recover {recovered = true}
    t |> success(recovered)
    t |> success(!gl_binding_filter_active, "scope relinquishes ownership on recovered panic")
    if (gl_binding_filter_active){gl_binding_end()}
    fill_and_retire(program)
    let before = heap_bytes_allocated()
    for (_i in range(20)){fill_and_retire(program)}
    let after = heap_bytes_allocated()
    t |> equal(after, before, "retired uniform snapshots are freed without GC")
    t |> equal(glGetError(), uint(GL_NO_ERROR))
}
