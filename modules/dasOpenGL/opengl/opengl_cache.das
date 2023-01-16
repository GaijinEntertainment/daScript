options indenting = 4

module opengl_cache shared private

require debugapi

require daslib/apply_in_context
require daslib/safe_addr

require opengl/opengl_boost public
require opengl/opengl_ttf public

var programs : table<uint64; uint>
var images : table<string; uint>
var fonts : table<string; Font?>

[apply_in_context(opengl_cache)]
def public cache_font(name:string implicit) : Font?
    if ! fonts |> key_exists(name)
        create_ttf_adapter()
        var font <- new [[Font]]
        *font <- load_ttf(name)
        fonts[clone_string(name)] = font
        return font
    return fonts[name]

def public cache_ttf_objects
    let shaders = get_ttf_program()
    let prog = cache_shader_program(shaders.vtx,shaders.frag)
    set_ttf_objects(prog)

[apply_in_context(opengl_cache)]
def public cache_shader_program(vtx,frag:string# implicit) : uint
    let phash = (uint64(hash(vtx))<<32ul) | uint64(hash(frag))
    if ! programs |> key_exists(phash)
        programs[phash] = create_shader_program(vtx,frag)
    return programs[phash]

[apply_in_context(opengl_cache)]
def public cache_image(_filename:string# implicit) : uint
    var value = 0u
    let filename = clone_string(_filename)
    images |> find_if_exists(filename) <| $ ( pimg )
        value = pimg
    if value==0u
        value = load_image_from_file(filename, true)
        images[clone_string(filename)] = value
    return value

def private has_agent
    return has_debug_agent_context("opengl_cache")

def private install_agent
    install_new_debug_agent(new DapiDebugAgent(), "opengl_cache")

def public init_opengl_cache
    if !has_agent()
        fork_debug_agent_context(@@install_agent)

[apply_in_context(opengl_cache)]
def public reset_opengl_cache : bool
    for prog in values(programs)
        glDeleteProgram(prog)
    delete programs
    for img in values(images)
        glDeleteTextures(1,safe_addr(img))
    delete images
    for fnt in values(fonts)
        unsafe
            delete fnt
        fnt = null
    delete fonts
    return true
