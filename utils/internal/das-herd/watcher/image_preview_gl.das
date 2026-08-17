options gen2
options indenting = 4

module image_preview_gl shared public

require imgui
require opengl/opengl_boost
require daslib/safe_addr

//! Main-thread GL bridge for decoded inspector pixels. Keeping backend calls
//! here preserves imgui_harness' rule that application files use the harness
//! lifecycle rather than reaching into the backend directly.

def public inspector_preview_texture_upload(pixels : array<uint8># const;
                                            width : int; height : int;
                                            var texture : uint&) {
    if (empty(pixels) || width <= 0 || height <= 0) return
    glGenTextures(1, unsafe(addr(texture)))
    glBindTexture(GL_TEXTURE_2D, texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGBA), width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, unsafe(addr(pixels[0])))
}

def public inspector_preview_texture_release(var texture : uint&) {
    if (texture == 0u) return
    glDeleteTextures(1, unsafe(addr(texture)))
    texture = 0u
}

def public inspector_preview_texture_ref(texture : uint) : ImTextureRef {
    unsafe {
        var result : ImTextureRef
        result._TexID = uint64(texture)
        return result
    }
}
