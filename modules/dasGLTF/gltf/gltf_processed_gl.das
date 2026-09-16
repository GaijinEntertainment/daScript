options gen2
options persistent_heap
module gltf_processed_gl shared public
require gltf/gltf_gl public
require gltf/gltf_processed public
require daslib/safe_addr
require math
require strings

[arch(at="../ARCHITECTURE.md#compressed-texture-capability")]
def private compressed_format_supported(format : uint) : bool {
    var count = 0; glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, safe_addr(count))
    var formats : array<int>; formats |> resize(max(count, 1))
    if (count > 0){glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, unsafe(addr(formats[0])))}
    var found = false; for (i in range(count)){if (uint(formats[i]) == format){found = true}}
    delete formats
    if (!found && (format == 0x8c4cu || format == 0x8c4fu || format == 0x83f0u || format == 0x83f3u)){
        var s3tc = false; var srgb = false; var extensions = 0
        glGetIntegerv(GL_NUM_EXTENSIONS, safe_addr(extensions))
        for (i in range(extensions)){
            let p = glGetStringi(GL_EXTENSIONS, uint(i))
            let name = p == null ? "" : clone_string(unsafe(reinterpret<string>(p)))
            if (ends_with(name, "texture_compression_s3tc") || ends_with(name, "compressed_texture_s3tc")){s3tc = true}
            if (ends_with(name, "texture_sRGB") || ends_with(name, "compressed_texture_s3tc_srgb") || ends_with(name, "texture_compression_s3tc_srgb")){srgb = true}
        }
        let p = glGetString(GL_VERSION); let version = p == null ? "" : clone_string(unsafe(reinterpret<string>(p)))
        if (starts_with(version, "3.") || starts_with(version, "4.")){srgb = true}
        found = s3tc && ((format == 0x83f0u || format == 0x83f3u) || srgb)
    }
    return found
}
def private block_gl_format(t : BlockTexture) : uint {
    if (t.format == 0){return t.srgb ? 0x8c4cu : 0x83f0u}
    if (t.format == 1){return t.srgb ? 0x8c4fu : 0x83f3u}
    return t.format == 2 ? 0x8dbbu : 0x8dbdu
}
[arch(at="../ARCHITECTURE.md#compressed-texture-capability")]
def private upload_blocks(t : BlockTexture; sampler : GltfSampler; var bytes : uint64&) : uint {
    let format = block_gl_format(t)
    var compressed = compressed_format_supported(format)
    for (m in t.mips){if ((m.width % 4 != 0 && m.width != 1 && m.width != 2) || (m.height % 4 != 0 && m.height != 1 && m.height != 2)){compressed = false}}
    var texture = 0u; glGenTextures(1, safe_addr(texture)); glBindTexture(GL_TEXTURE_2D, texture); bytes = 0ul
    for (m, level in t.mips, count()){
        if (compressed){
            glCompressedTexImage2D(GL_TEXTURE_2D, level, format, m.width, m.height, 0, length(m.data), unsafe(addr(m.data[0])))
            bytes += uint64(long_length(m.data))
        }else{
            var pixels <- decode_block_mip(t, level)
            glTexImage2D(GL_TEXTURE_2D, level, int(t.srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8), m.width, m.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unsafe(addr(pixels[0])))
            bytes += uint64(long_length(pixels)); delete pixels
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.minFilter == 0 ? int(GL_LINEAR_MIPMAP_LINEAR) : sampler.minFilter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.magFilter == 0 ? int(GL_LINEAR) : sampler.magFilter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS == 0 ? int(GL_REPEAT) : sampler.wrapS)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT == 0 ? int(GL_REPEAT) : sampler.wrapT)
    glBindTexture(GL_TEXTURE_2D, 0u); return texture
}
def private packed_attribute(index : uint; count : int; kind : uint; normalized : bool; stride, offset : int){
    glEnableVertexAttribArray(index); glVertexAttribPointer(index, count, kind, normalized, stride, null, offset)
}
//! Return the generic vertex attributes a processed primitive must restore at draw time.
def gltf_processed_missing_attributes(attributes : int) : uint {
    var missing = 0u
    if ((attributes & VERTEX_TANGENT) == 0){missing |= GLTF_GL_ATTR_TANGENT}
    if ((attributes & VERTEX_UV) == 0){missing |= GLTF_GL_ATTR_UV0}
    if ((attributes & VERTEX_UV1) == 0){missing |= GLTF_GL_ATTR_UV1}
    if ((attributes & VERTEX_COLOR) == 0){missing |= GLTF_GL_ATTR_COLOR0}
    if ((attributes & VERTEX_SKIN) == 0){missing |= GLTF_GL_ATTR_JOINTS0 | GLTF_GL_ATTR_WEIGHTS0}
    return missing
}
//! Derive the packed vertex layout for a mesh instance and material in a scene.
def gltf_packed_primitive_attributes(scene : GltfScene; mesh_index, material_index : int) : int {
    var attributes = VERTEX_UV
    for (node in scene.nodes){if (node.mesh == mesh_index && node.skin >= 0){attributes |= VERTEX_SKIN}}
    if (material_index >= 0 && material_index < length(scene.materials) && scene.materials[material_index].normalTex >= 0){attributes |= VERTEX_TANGENT}
    return attributes
}
def upload_processed_primitive(p : ProcessedPrimitive) : GltfGlPrimitive {
    var out = GltfGlPrimitive(material = p.material, indexCount = p.index_count, mode = p.mode == GltfPrimitiveMode.triangles ? GL_TRIANGLES : (p.mode == GltfPrimitiveMode.lines ? GL_LINES : GL_POINTS))
    out.missingAttributes = gltf_processed_missing_attributes(p.attributes)
    var vertices <- processed_vertex_bytes(p); var indices <- processed_indices(p)
    glGenVertexArrays(1, unsafe(addr(out.vao))); glBindVertexArray(out.vao)
    glGenBuffers(1, unsafe(addr(out.vbo))); glBindBuffer(GL_ARRAY_BUFFER, out.vbo); glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)
    out.vertexBytes = uint64(long_length(vertices))
    packed_attribute(0u, 3, GL_FLOAT, false, p.stride, 0)
    packed_attribute(1u, 4, GL_INT_2_10_10_10_REV, true, p.stride, 12)
    var at = 16
    if ((p.attributes & VERTEX_UV) != 0){packed_attribute(3u, 2, GL_HALF_FLOAT, false, p.stride, at); at += 4}
    if ((p.attributes & VERTEX_TANGENT) != 0){packed_attribute(2u, 4, GL_INT_2_10_10_10_REV, true, p.stride, at); at += 4}
    if ((p.attributes & VERTEX_SKIN) != 0){packed_attribute(6u, 4, GL_UNSIGNED_SHORT, false, p.stride, at); packed_attribute(7u, 4, GL_UNSIGNED_SHORT, true, p.stride, at + 8); at += 16}
    if ((p.attributes & VERTEX_UV1) != 0){packed_attribute(4u, 2, GL_HALF_FLOAT, false, p.stride, at); at += 4}
    if ((p.attributes & VERTEX_COLOR) != 0){packed_attribute(5u, 4, GL_UNSIGNED_BYTE, true, p.stride, at)}
    glGenBuffers(1, unsafe(addr(out.ebo))); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.ebo)
    if (p.vertex_count <= 65536){
        var short_indices : array<uint16>; short_indices |> reserve(length(indices)); short_indices |> resize(length(indices))
        for (index, i in indices, count()){short_indices[i] = uint16(index)}
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, short_indices, GL_STATIC_DRAW); out.indexType = GL_UNSIGNED_SHORT; out.indexBytes = uint64(long_length(indices)) * 2ul
        delete short_indices
    }else{glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW); out.indexType = GL_UNSIGNED_INT; out.indexBytes = uint64(long_length(indices)) * 4ul}
    glBindVertexArray(0u); delete vertices; delete indices; return out
}
def upload_processed(asset : ProcessedAsset; var pool : GltfGlTexturePool) : GltfGlModel {
    if (!processed_valid(asset)){panic("Invalid processed asset upload")}
    var model <- upload_gltf(asset.scene)
    model.texturePool = unsafe(addr(pool)); model.textureKeys |> resize(length(asset.scene.textures))
    for (texture, ti in asset.scene.textures, count()){
        if (texture.image < 0){continue}
        let image & = unsafe(asset.images[texture.image])
        var sampler = texture.sampler >= 0 && texture.sampler < length(asset.scene.samplers) ? asset.scene.samplers[texture.sampler] : GltfSampler()
        if (sampler.wrapS == 0){sampler.wrapS = int(GL_REPEAT)}; if (sampler.wrapT == 0){sampler.wrapT = int(GL_REPEAT)}
        if (sampler.minFilter == 0){sampler.minFilter = int(GL_LINEAR_MIPMAP_LINEAR)}; if (sampler.magFilter == 0){sampler.magFilter = int(GL_LINEAR)}
        let key = (image.key ^ hash(sampler)) | 1ul
        if (!key_exists(pool.entries, key)){
            var bytes = 0ul; let id = upload_blocks(image, sampler, bytes)
            pool.entries[key] = GltfGlTextureEntry(texture = id, bytes = bytes)
        }
        pool.entries[key].references++; model.textures[ti] = pool.entries[key].texture; model.textureKeys[ti] = key
    }
    model.meshes |> reserve(length(model.meshes) + length(asset.meshes))
    for (mesh in asset.meshes){
        var glmesh : GltfGlMesh
        glmesh.primitives |> reserve(length(mesh.primitives))
        for (p in mesh.primitives){glmesh.primitives |> emplace(upload_processed_primitive(p))}
        model.meshes |> emplace(glmesh)
    }
    return <- model
}
def upload_gltf_geometry_packed(scene : GltfScene; texture_owner : GltfGlModel) : GltfGlModel {
    var model = GltfGlModel(borrowsTextures = true, whiteTex = texture_owner.whiteTex, flatNormalTex = texture_owner.flatNormalTex)
    model.textures <- clone(texture_owner.textures)
    model.meshes |> reserve(length(model.meshes) + length(scene.meshes))
    for (mesh, mi in scene.meshes, count()){
        var glmesh : GltfGlMesh
        glmesh.primitives |> reserve(length(mesh.primitives))
        for (p in mesh.primitives){
            let flags = gltf_packed_primitive_attributes(scene, mi, p.material)
            let ordered = p.material >= 0 && p.material < length(scene.materials) && scene.materials[p.material].alphaMode == GltfAlphaMode.blend
            var processed <- process_primitive(p, flags, false, !ordered)
            glmesh.primitives |> emplace(upload_processed_primitive(processed)); delete processed
        }
        model.meshes |> emplace(glmesh)
    }
    return <- model
}
