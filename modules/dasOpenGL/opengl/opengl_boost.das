options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module opengl_boost shared

require opengl/opengl public

require daslib/safe_addr
require daslib/contracts
require daslib/array_boost

require glsl/glsl_opengl public

require opengl/opengl_state
require opengl/opengl_boost_internal public

require stbimage

require math

typedef GLenum = uint


def glTexParameteri(target : uint; pname : uint; param : uint) {
    glTexParameteri(target, pname, int(param))
}

def getTexture2DSize(tex : uint; mipLevel : int = 0) {
    var w, h : int
    glBindTexture(GL_TEXTURE_2D, tex)
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_WIDTH, safe_addr(w))
    glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_HEIGHT, safe_addr(h))
    return int2(w, h)
}

def write_ssbo(ssbo : uint; data : auto(TT)) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo)
    var p_mat = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY)
    if (p_mat == null) {
        panic("can't map ssbo")
    }
    unsafe { // nolint:STYLE025 — expression-form unsafe loses the reinterpret in generic instances (31004)
        *(reinterpret<TT -const?> p_mat) = data
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER)
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u)
}

def read_ssbo(ssbo : uint; var data : auto(TT)) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo)
    var p_mat = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY)
    if (p_mat == null) {
        panic("can't map ssbo")
    }
    unsafe { // nolint:STYLE025 — expression-form unsafe loses the reinterpret in generic instances (31004)
        data = *(reinterpret<TT?> p_mat)
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER)
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u)
}

def bind_sampler_2d(unit : int; tex : uint) {
    glActiveTexture(GL_TEXTURE0 + uint(unit))
    glBindTexture(GL_TEXTURE_2D, tex)
}

def bind_sampler_3d(unit : int; tex : uint) {
    glActiveTexture(GL_TEXTURE0 + uint(unit))
    glBindTexture(GL_TEXTURE_3D, tex)
}

def bind_sampler_cube(unit : int; tex : uint) {
    glActiveTexture(GL_TEXTURE0 + uint(unit))
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex)
}

def bind_sampler_2d_array(unit : int; tex : uint) {
    glActiveTexture(GL_TEXTURE0 + uint(unit))
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex)
}

// sampler2DShadow binds an ordinary 2D depth texture; the GL_TEXTURE_COMPARE_MODE / depth format are
// set when the texture is created (not at bind time), so the bind itself matches bind_sampler_2d.
def bind_sampler_2d_shadow(unit : int; tex : uint) {
    glActiveTexture(GL_TEXTURE0 + uint(unit))
    glBindTexture(GL_TEXTURE_2D, tex)
}

def bind_image_any(unit : int; tex : uint; format : uint; access : uint = GL_READ_WRITE) {
    glBindImageTexture(uint(unit), tex, 0, false, 0, access, format)
}

def bind_ffp() {
    glUseProgram(0u)
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    glBindVertexArray(0u)
    // glBindBuffer(GL_ARRAY_BUFFER, 0u)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u)
    glActiveTexture(GL_TEXTURE0)
    glBindTexture(GL_TEXTURE_2D, 0u)
    var maxAttr = 16
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, safe_addr(maxAttr))
    for (va in range(maxAttr)) {
        glDisableVertexAttribArray(uint(va))
    }
}

def glUniformAny(location : int; value : int) {
    glUniform1i(location, value)
}

def glUniformAny(location : int; value : int[]) {
    glUniform1iv(location, length(value), unsafe(addr(value[0])))
}

def glUniformAny(location : int; value : float) {
    glUniform1f(location, value)
}

def glUniformAny(location : int; value : float[]) {
    glUniform1fv(location, length(value), unsafe(addr(value[0])))
}

def glUniformAny(location : int; value : float2) {
    glUniform2fv(location, 1, unsafe(addr(value.x)))
}

def glUniformAny(location : int; value : float3) {
    glUniform3fv(location, 1, unsafe(addr(value.x)))
}

def glUniformAny(location : int; value : float4) {
    glUniform4fv(location, 1, unsafe(addr(value.x)))
}

def glUniformAny(location : int; value : float4[]) {
    glUniform4fv(location, length(value), unsafe(addr(value[0].x)))
}

def glUniformAny(location : int; value : float4x4) {
    glUniformMatrix4fv(location, value)
}

// float3x3 is 36 bytes = 9 tight column-major floats (3x float3, no std140 padding), so it uploads
// straight via glUniformMatrix3fv -- matches a `uniform mat3` shader field (e.g. a normal matrix).
def glUniformAny(location : int; value : float3x3) {
    glUniformMatrix3fv(location, value)
}

def glUniformAny(location : int; value : int2) {
    glUniform2iv(location, 1, unsafe(addr(value.x)))
}

def glUniformAny(location : int; value : int3) {
    glUniform3iv(location, 1, unsafe(addr(value.x)))
}

def glUniformAny(location : int; value : int4) {
    glUniform4iv(location, 1, unsafe(addr(value.x)))
}

def glUniformMatrix4fv(location : int; value : float4x4) {
    unsafe {
        glUniformMatrix4fv(location, 1, false, addr(value[0][0]))
    }
}

def glUniformMatrix3fv(location : int; value : float3x3) {
    unsafe {
        glUniformMatrix3fv(location, 1, false, addr(value[0][0]))
    }
}

[expect_any_array(arr)]
def glBufferData(target : GLenum; arr; usage : GLenum) {
    unsafe {
        assert(!empty(arr))
        glBufferData(target, int64(length(arr) * typeinfo sizeof(arr[0])), reinterpret<void?> addr(arr[0]), usage)
    }
}

// ===== std140 UBO runtime (the GL analogue of dasVulkan's write_field / with_mapped_memory) =====
// A struct @uniform packed as a std140 interface block binds through these three helpers, emitted by
// glsl_opengl's bind-uniform macro: std140_reset sizes a per-(program, block) CPU staging buffer,
// std140_set writes each field at its compile-time std140 offset, std140_flush uploads the bytes to a
// per-(program, block) GL uniform buffer and binds blockName -> bindingPoint -> buffer. Keeping all
// state in these tables lets the generated bind body stay a flat list of calls (no local buffer).
// GL/WebGL2 blocks carry no layout(binding=), so the binding point is assigned host-side at flush.
var private g_std140_staging : table<string; array<uint8>>
var private g_std140_ubos : table<string; uint>

// Resize the staging buffer for (program, blockName) to `size` bytes (the std140 block size, padded to
// 16). Unwritten bytes are std140 padding the shader never reads, so they need no zeroing.
def std140_reset(program : uint; blockName : string; size : int) {
    let key = "{program}/{blockName}"
    g_std140_staging[key] |> resize(size)
}

// memcpy a 32-bit scalar / vector / float4x4 field into the staging buffer at its std140 byte offset.
// Mirrors dasVulkan's write_field; float3x3 / composites are repacked/recursed by the bind macro.
def std140_set(program : uint; blockName : string; offset : uint; var value : auto(TT) const) {
    let key = "{program}/{blockName}"
    unsafe {
        let dst = reinterpret<void?>(addr(g_std140_staging[key][int(offset)]))
        memcpy(dst, reinterpret<void?>(addr(value)), typeinfo sizeof(type<TT>))
    }
}

// Upload the staging buffer to a per-(program, block) GL uniform buffer (lazily created) and bind the
// block to `bindingPoint`. glUniformBlockBinding wires the program's block to the point; glBindBufferRange
// binds the buffer there.
def std140_flush(program : uint; blockName : string; bindingPoint : int) {
    let key = "{program}/{blockName}"
    var ubo = g_std140_ubos?[key] ?? 0u
    if (ubo == 0u) {
        glGenBuffers(1, safe_addr(ubo))
        g_std140_ubos[key] = ubo
    }
    glBindBuffer(GL_UNIFORM_BUFFER, ubo)
    glBufferData(GL_UNIFORM_BUFFER, g_std140_staging[key], GL_DYNAMIC_DRAW)
    let blockIndex = glGetUniformBlockIndex(program, blockName)
    if (blockIndex != GL_INVALID_INDEX) {
        let nbytes = int64(length(g_std140_staging[key]))
        glUniformBlockBinding(program, blockIndex, uint(bindingPoint))
        glBindBufferRange(GL_UNIFORM_BUFFER, uint(bindingPoint), ubo, int64(0), nbytes)
    }
}

def glVertexAttribPointer(index : uint; size : int; tp : GLenum; normalized : bool; stride : int; offset : int) {
    unsafe {
        glVertexAttribPointer(index, size, tp, normalized, stride, reinterpret<void?> offset)
    }
}

def glVertexAttribPointer(index : uint; size : int; tp : GLenum; normalized : bool; stride : int; ptrd : void?; offset : int) {
    unsafe {
        var poffset = reinterpret<int8?> ptrd
        poffset += offset
        glVertexAttribPointer(index, size, tp, normalized, stride, poffset)
    }
}

def create_shader(src : string implicit; shaderType : uint; canfail : bool = false) {
    if (src == "") {
        panic("create_shaer with empty shader")
    }
    let s = glCreateShader(shaderType)
    glShaderSource(s, 1, unsafe(addr(src)), null)
    glCompileShader(s)
    var status : int
    glGetShaderiv(s, GL_COMPILE_STATUS, safe_addr(status))
    if (status == 0) {
        if (!canfail) {
            unsafe {
                var log : array<uint>
                resize(log, 16384)
                let slog = reinterpret<string> addr(log[0])
                glGetShaderInfoLog(s, 16384, null, slog)
                to_log(LOG_ERROR, "Shader source:\n{src}\n")
                panic("{slog}\n\nFAILED TO CREATE SHADER ({status})\n")
            }
        }
        return 0u
    } else {
        return s
    }
}

def create_shader_program(vertex, fragment : uint) {
    let program = glCreateProgram()
    glAttachShader(program, vertex)
    glAttachShader(program, fragment)
    return program
}

def create_compute_shader_program(compute : uint) {
    let program = glCreateProgram()
    glAttachShader(program, compute)
    return program
}

def link_shader(program : uint; canfail : bool = false) {
    glLinkProgram(program)
    var status : int
    glGetProgramiv(program, GL_LINK_STATUS, safe_addr(status))
    if (status == 0) {
        if (!canfail) {
            unsafe {
                var log : uint8[512]
                let slog = reinterpret<string> addr(log[0])
                glGetProgramInfoLog(program, 512, null, slog)
                panic("FAILED TO LINK SHADER PROGRAM ({status}):\n{slog}\n")
            }
        }
        return false
    } else {
        return true
    }
}

def create_shader_program(vsrc, fsrc : string implicit) {
    let vertex = create_shader(vsrc, GL_VERTEX_SHADER)
    let fragment = create_shader(fsrc, GL_FRAGMENT_SHADER)
    let program = create_shader_program(vertex, fragment)
    link_shader(program)
    glDeleteShader(vertex)
    glDeleteShader(fragment)
    return program
}

def create_compute_shader_program(csrc : string implicit) {
    let compute = create_shader(csrc, GL_COMPUTE_SHADER)
    let program = create_compute_shader_program(compute)
    link_shader(program)
    glDeleteShader(compute)
    return program
}

def create_ssbo(var value : auto&) {
    var ssbo : uint
    glGenBuffers(1, safe_addr(ssbo))
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0u, ssbo)
    glBufferData(GL_SHADER_STORAGE_BUFFER, int64(typeinfo sizeof(value)), unsafe(addr(value)), GL_DYNAMIC_COPY)
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u)
    return ssbo
}

def load_image_from_bytes(width, height : int; data : void?) {
    var texture : uint
    glGenTextures(1, safe_addr(texture))
    glBindTexture(GL_TEXTURE_2D, texture)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGBA), width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glBindTexture(GL_TEXTURE_2D, GL_INVALID_VALUE)
    return texture
}

def load_image_from_file(fname : string implicit; canfail : bool = false) {
    var x, y, comp : int
    let data = stbi_load(fname, safe_addr(x), safe_addr(y), safe_addr(comp), 4)
    if (data == null) {
        if (!canfail) {
            let reason = stbi_failure_reason()
            panic(reason)
        }
        return 0u
    } else {
        let texture_id = load_image_from_bytes(x, y, data)
        stbi_image_free(data)
        return texture_id
    }
}

def load_image_from_file(fname : string implicit; dir1, dir2 : string; can_fail : bool = false) {
    var img = load_image_from_file("{dir1}/{fname}", true)
    if (img == 0u) {
        img = load_image_from_file("{dir2}/{fname}", true)
    }
    if (!can_fail && img == 0u) {
        let reason = stbi_failure_reason()
        panic(reason)
    }
    return img
}




