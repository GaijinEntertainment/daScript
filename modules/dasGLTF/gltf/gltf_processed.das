options gen2
options persistent_heap
module gltf_processed shared public
require gltf/gltf_boost public
require stbimage/texture_blocks public
require gltf/gltf_skinning public
require meshoptimizer
require math
require daslib/math_boost
require daslib/fio
require daslib/json_boost
require daslib/f16_cvt
require strings

let DAS_GLTF_VERSION = 2
let DAS_GLTF_PROCESSOR_VERSION = 3
let VERTEX_UV = 1
let VERTEX_TANGENT = 2
let VERTEX_SKIN = 4
let VERTEX_UV1 = 8
let VERTEX_COLOR = 16
struct ProcessingProfile {
    //! Controls which optional vertex streams and storage encodings survive preprocessing.
    keep_uv1, keep_color : bool //! Preserve secondary UVs and vertex colors when true.
    compress_storage : bool = true //! Encode triangle geometry with meshoptimizer when true.
    texture_cache : string = ".jitted_scripts/assets/textures" //! Directory for content-addressed block textures; empty disables cache I/O.
}
struct ProcessedPrimitive {
    //! Portable packed geometry for one primitive, optionally meshoptimizer-encoded.
    material : int = -1 //! Index into the processed scene material array, or -1.
    mode : GltfPrimitiveMode = GltfPrimitiveMode.triangles //! Original glTF primitive topology.
    attributes, stride, vertex_count, index_count : int //! Attribute flags, byte stride, and decoded element counts.
    encoded : bool //! True when both payload arrays use meshoptimizer codecs.
    vertices, indices : array<uint8> //! Packed vertex bytes and uint32 index bytes, or their encoded forms.
    lo, hi : float3 //! Object-space bounds of source positions, including unreferenced vertices.
}
struct ProcessedMesh {
    //! A named mesh containing portable processed primitives.
    name : string //! Source mesh name.
    primitives : array<ProcessedPrimitive> //! Processed primitives in source order.
}
struct ProcessedAsset {
    //! Backend-neutral processed glTF data: scene metadata, packed geometry, and BC textures.
    version : int //! Serialized asset layout version.
    key : uint64 //! Content and processing-profile key used by manifests and caches.
    source : string //! Source identifier included in the key and diagnostics.
    scene : GltfScene //! Nodes, materials, skins, animations, samplers, and texture references.
    meshes : array<ProcessedMesh> //! Portable geometry addressed by scene nodes.
    images : array<BlockTexture> //! Deduplicated compressed images addressed by scene textures.
    lo, hi : float3 //! World-space bounds computed from the source scene.
    source_vertices, processed_vertices, texture_hits, texture_misses : int //! Processing statistics for geometry and the texture cache.
}
struct ProcessedCatalogEntry {
    //! Identifies one processed asset and the key that produced it.
    path : string //! Asset path interpreted by the catalog consumer.
    key : uint64 //! Expected processed asset key.
}
struct ProcessedCatalog {
    //! Versioned list of processed assets for tooling and package manifests.
    version : int //! Catalog schema version.
    assets : array<ProcessedCatalogEntry> //! Catalog entries in author-defined order.
}
def prepare_processed_catalog(var catalog : ProcessedCatalog; full_regeneration : bool) {
    //! Reject partial updates across format versions; a full source-tree conversion starts a fresh catalog.
    if (catalog.version != DAS_GLTF_VERSION && (catalog.version != 0 || !empty(catalog.assets))) {
        if (!full_regeneration) { panic("Processed catalog version changed; rerun without explicit input files") }
        clear(catalog.assets)
    }
    catalog.version = DAS_GLTF_VERSION
}
struct ProcessedManifest {
    //! Split-file manifest for geometry and independently shared texture payloads.
    version : int //! Manifest schema version.
    key : uint64 //! Expected key of the geometry payload.
    geometry : string //! Geometry filename relative to the manifest directory.
    textures : array<string> //! Texture filenames relative to the manifest directory.
}
def processed_stride(flags : int) : int {
    //! Return the packed vertex stride for the `VERTEX_*` attribute bit mask. Position and normal
    //! are always present; optional streams add their fixed portable widths.
    return 16 + ((flags & VERTEX_UV) != 0 ? 4 : 0) + ((flags & VERTEX_TANGENT) != 0 ? 4 : 0) + ((flags & VERTEX_SKIN) != 0 ? 8 : 0) + ((flags & VERTEX_UV1) != 0 ? 4 : 0) + ((flags & VERTEX_COLOR) != 0 ? 4 : 0)
}
[arch(at="../ARCHITECTURE.md#processed-vertex-portability")]
def private pack_uv(v : float2) : uint {return f32_to_f16(v.x) | (f32_to_f16(v.y) << 16)}
def private unpack_uv(v : uint) : float2 {return float2(f16_to_f32(v & 65535u), f16_to_f32(v >> 16))}
def private pack_direction(v : float4) : uint {
    let x = int(round(clamp(v.x, -1.0, 1.0) * 511.0))
    let y = int(round(clamp(v.y, -1.0, 1.0) * 511.0))
    let z = int(round(clamp(v.z, -1.0, 1.0) * 511.0))
    let w = v.w < 0.0 ? 3u : (v.w > 0.0 ? 1u : 0u)
    return (uint(x) & 1023u) | ((uint(y) & 1023u) << 10) | ((uint(z) & 1023u) << 20) | (w << 30)
}
def private unpack_direction(bits : uint) : float4 {
    let x = int(bits << 22) >> 22; let y = int(bits << 12) >> 22; let z = int(bits << 2) >> 22; let w = int(bits) >> 30
    return float4(float(x) / 511.0, float(y) / 511.0, float(z) / 511.0, float(w))
}
def private pack_vertices(vertices : array<GltfVertex>; flags : int) : array<uint8> {
    let stride = processed_stride(flags); var words : array<uint>; words |> reserve(length(vertices) * stride / 4); words |> resize(length(vertices) * stride / 4)
    for (v, i in vertices, count()){
        var at = i * stride / 4
        words[at++] = unsafe(reinterpret<uint>(v.position.x)); words[at++] = unsafe(reinterpret<uint>(v.position.y)); words[at++] = unsafe(reinterpret<uint>(v.position.z))
        words[at++] = pack_direction(float4(v.normal, 0.0))
        if ((flags & VERTEX_UV) != 0){words[at++] = pack_uv(v.uv0)}
        if ((flags & VERTEX_TANGENT) != 0){words[at++] = pack_direction(v.tangent)}
        if ((flags & VERTEX_SKIN) != 0){
            let skin = skin_pack8(v.joints0, v.weights0)
            words[at++] = skin.x; words[at++] = skin.y
        }
        if ((flags & VERTEX_UV1) != 0){words[at++] = pack_uv(v.uv1)}
        if ((flags & VERTEX_COLOR) != 0){words[at++] = unsafe(reinterpret<uint>(ubyte4(uint4(round(clamp(v.color0, float4(0.0), float4(1.0)) * 255.0)))))}
    }
    var bytes : array<uint8>; bytes |> reserve(length(words) * 4); bytes |> resize(length(words) * 4)
    if (!empty(bytes)){unsafe{memcpy(addr(bytes[0]), addr(words[0]), length(bytes))}}
    delete words; return <- bytes
}
def processed_vertex_bytes(p : ProcessedPrimitive) : array<uint8> {
    //! Return decoded packed vertex bytes for `p`. Panics when an encoded payload fails validation.
    var out : array<uint8>
    if (p.encoded){if (!meshopt_decode_vertices(out, p.vertices, uint(p.vertex_count), uint(p.stride))){panic("Invalid meshopt vertex payload")}}
    else{out <- clone(p.vertices)}
    return <- out
}
def processed_indices(p : ProcessedPrimitive) : array<uint> {
    //! Return decoded uint32 indices for `p`, rejecting codec failures and indices outside the
    //! declared vertex range with a panic.
    var out : array<uint>
    if (p.encoded){if (!meshopt_decode_indices(out, p.indices, uint(p.index_count))){panic("Invalid meshopt index payload")}}
    else{out |> reserve(p.index_count); out |> resize(p.index_count); if (!empty(out)){unsafe{memcpy(addr(out[0]), addr(p.indices[0]), p.index_count * 4)}}}
    for (index in out){if (index >= uint(p.vertex_count)){panic("Processed index outside vertex buffer")}}
    return <- out
}
def process_primitive(p : GltfPrimitive; flags : int; encode : bool; optimize_order : bool = true) : ProcessedPrimitive {
    //! Pack one glTF primitive according to `flags`, deduplicate triangle vertices, and optionally
    //! optimize ordering and encode storage. Non-triangle or unindexed primitives remain raw.
    var out = ProcessedPrimitive(material = p.material, mode = p.mode, attributes = flags, stride = processed_stride(flags), lo = float3(1.0e20), hi = float3(-1.0e20))
    for (v in p.vertices){out.lo = min(out.lo, v.position); out.hi = max(out.hi, v.position)}
    if (empty(p.vertices)){out.lo = float3(0.0); out.hi = float3(0.0); return <- out}
    var bytes <- pack_vertices(p.vertices, flags); var indices <- clone(p.indices)
    if (p.mode == GltfPrimitiveMode.triangles && !empty(indices)){
        var remap : array<uint>; let count = meshopt_generate_remap(remap, indices, bytes, uint(out.stride))
        var compact : array<uint8>; var remapped : array<uint>
        meshopt_remap_vertices(compact, bytes, remap, uint(out.stride), count)
        meshopt_remap_indices(remapped, indices, remap)
        if (optimize_order){meshopt_optimize_cache(remapped, count)}
        meshopt_optimize_fetch(remapped, compact, uint(out.stride))
        delete bytes; delete indices; bytes <- compact; indices <- remapped; delete remap
    }
    out.vertex_count = length(bytes) / out.stride; out.index_count = length(indices)
    out.encoded = encode && p.mode == GltfPrimitiveMode.triangles && !empty(indices)
    if (out.encoded){meshopt_encode_vertices(out.vertices, bytes, uint(out.stride)); meshopt_encode_indices(out.indices, indices, uint(out.vertex_count))}
    else{
        out.vertices <- bytes; out.indices |> reserve(length(indices) * 4); out.indices |> resize(length(indices) * 4)
        if (!empty(indices)){unsafe{memcpy(addr(out.indices[0]), addr(indices[0]), length(out.indices))}}
    }
    delete bytes; delete indices; return <- out
}
def private process_gltf_textures(raw : GltfScene; profile : ProcessingProfile; var out : ProcessedAsset) {
    out.images |> reserve(length(out.scene.textures))
    for (tex, ti in out.scene.textures, count()){
        if (tex.image < 0 || tex.image >= length(raw.images)){tex.image = -1; continue}
        let image & = unsafe(raw.images[tex.image])
        if (empty(image.pixels)){tex.image = -1; continue}
        var srgb = false
        for (m in raw.materials){if (m.baseColorTex == ti || m.emissiveTex == ti){srgb = true}}
        var format = 0
        for (i in range(image.width * image.height)){if (image.pixels[i * 4 + 3] != 255u8){format = 1; break}}
        let key = block_texture_key(image.pixels, image.width, image.height, format, srgb)
        var found = -1
        for (img, i in out.images, count()){if (img.key == key){found = i; break}}
        if (found < 0){
            var hit = false
            var compressed <- cached_block_texture(image.pixels, image.width, image.height, format, srgb, profile.texture_cache, hit)
            if (hit){out.texture_hits++}else{out.texture_misses++}
            found = length(out.images); out.images |> emplace(compressed)
        }
        tex.image = found
    }
}
def process_gltf(raw : GltfScene; source : string; profile : ProcessingProfile) : ProcessedAsset {
    //! Convert a loaded scene into backend-neutral processed geometry and block textures. `source`
    //! participates in the asset key; the returned scene retains hierarchy, materials, skins, and animation.
    var out = ProcessedAsset(version = DAS_GLTF_VERSION, key = hash(raw) ^ hash((version = DAS_GLTF_VERSION, processor = DAS_GLTF_PROCESSOR_VERSION, uv1 = profile.keep_uv1, color = profile.keep_color, storage = profile.compress_storage)) ^ hash(source), source = clone(source))
    let bounds = gltf_scene_bounds(raw); out.lo = bounds._0; out.hi = bounds._1
    out.scene.materials <- clone(raw.materials); out.scene.nodes <- clone(raw.nodes); out.scene.roots <- clone(raw.roots)
    out.scene.skins <- clone(raw.skins); out.scene.animations <- clone(raw.animations); out.scene.samplers <- clone(raw.samplers); out.scene.textures <- clone(raw.textures)
    out.meshes |> reserve(length(raw.meshes))
    for (mesh, mi in raw.meshes, count()){
        var result = ProcessedMesh(name = clone(mesh.name)); var skinned = false
        for (node in raw.nodes){if (node.mesh == mi && node.skin >= 0){skinned = true}}
        result.primitives |> reserve(length(mesh.primitives))
        for (p in mesh.primitives){
            var flags = (skinned ? VERTEX_SKIN : 0) | (profile.keep_uv1 ? VERTEX_UV1 : 0) | (profile.keep_color ? VERTEX_COLOR : 0)
            if (p.material >= 0 && p.material < length(raw.materials)){
                let m = raw.materials[p.material]
                if (m.baseColorTex >= 0 || m.metalRoughTex >= 0 || m.normalTex >= 0 || m.occlusionTex >= 0 || m.emissiveTex >= 0){flags |= VERTEX_UV}
                if (m.normalTex >= 0){flags |= VERTEX_TANGENT}
            }
            let ordered = p.material >= 0 && p.material < length(raw.materials) && raw.materials[p.material].alphaMode == GltfAlphaMode.blend
            var processed <- process_primitive(p, flags, profile.compress_storage, !ordered)
            out.source_vertices += length(p.vertices); out.processed_vertices += processed.vertex_count; result.primitives |> emplace(processed)
        }
        out.meshes |> emplace(result)
    }
    process_gltf_textures(raw, profile, out)
    return <- out
}
def unpack_processed_geometry(asset : ProcessedAsset) : GltfScene {
    //! Reconstruct ordinary `GltfPrimitive` vertex and index arrays from a processed asset while
    //! cloning its scene metadata. Panics if an encoded geometry payload cannot be decoded.
    var out <- clone(asset.scene)
    out.meshes |> reserve(length(out.meshes) + length(asset.meshes))
    for (mesh in asset.meshes){
        var result = GltfMesh(name = clone(mesh.name))
        result.primitives |> reserve(length(mesh.primitives))
        for (p in mesh.primitives){
            var prim = GltfPrimitive(material = p.material, mode = p.mode)
            var bytes <- processed_vertex_bytes(p); var words : array<uint>; words |> reserve(length(bytes) / 4); words |> resize(length(bytes) / 4)
            if (!empty(bytes)){unsafe{memcpy(addr(words[0]), addr(bytes[0]), length(bytes))}}
            prim.vertices |> reserve(p.vertex_count); prim.vertices |> resize(p.vertex_count)
            for (v, i in prim.vertices, count()){
                var at = i * p.stride / 4
                v.position = float3(unsafe(reinterpret<float>(words[at])), unsafe(reinterpret<float>(words[at + 1])), unsafe(reinterpret<float>(words[at + 2]))); at += 3
                v.normal = unpack_direction(words[at++]).xyz
                if ((p.attributes & VERTEX_UV) != 0){v.uv0 = unpack_uv(words[at++])}
                if ((p.attributes & VERTEX_TANGENT) != 0){v.tangent = unpack_direction(words[at++])}
                if ((p.attributes & VERTEX_SKIN) != 0){
                    let skin = skin_unpack8(uint2(words[at], words[at + 1])); at += 2
                    v.joints0 = skin.joints; v.weights0 = skin.weights
                }
                if ((p.attributes & VERTEX_UV1) != 0){v.uv1 = unpack_uv(words[at++])}
                v.color0 = (p.attributes & VERTEX_COLOR) != 0 ? float4(uint4(unsafe(reinterpret<ubyte4>(words[at++])))) / 255.0 : float4(1.0)
            }
            prim.indices <- processed_indices(p); result.primitives |> emplace(prim); delete bytes; delete words
        }
        out.meshes |> emplace(result)
    }
    return <- out
}
def processed_valid(a : ProcessedAsset) : bool {
    //! Check serialized version, size limits, packed strides and raw byte counts, compressed textures,
    //! and scene references. This is a cheap structural check; `load_processed` additionally decodes
    //! encoded geometry and validates every index before returning untrusted serialized data.
    if (a.version != DAS_GLTF_VERSION || length(a.meshes) > 65536){return false}
    for (mesh in a.meshes){for (p in mesh.primitives){
        if (p.attributes < 0 || p.attributes > 31 || p.stride != processed_stride(p.attributes) || p.vertex_count < 0 || p.index_count < 0 || p.vertex_count > 10000000 || p.index_count > 30000000 ||
            (!p.encoded && (long_length(p.vertices) != int64(p.vertex_count) * int64(p.stride) || long_length(p.indices) != int64(p.index_count) * 4l))){return false}
    }}
    for (image in a.images){if (!block_texture_valid(image)){return false}}
    for (texture in a.scene.textures){if (texture.image < -1 || texture.image >= length(a.images)){return false}}
    for (node in a.scene.nodes){if (node.mesh < -1 || node.mesh >= length(a.meshes)){return false}}
    return true
}
def private processed_payloads_valid(a : ProcessedAsset) : bool {
    for (mesh in a.meshes){for (p in mesh.primitives){
        var vertices : array<uint8>
        if (p.encoded && !meshopt_decode_vertices(vertices, p.vertices, uint(p.vertex_count), uint(p.stride))){delete vertices; return false}
        if (!p.encoded && (p.attributes & VERTEX_SKIN) != 0){vertices <- clone(p.vertices)}
        if ((p.attributes & VERTEX_SKIN) != 0){
            let skin_offset = 16 + ((p.attributes & VERTEX_UV) != 0 ? 4 : 0) + ((p.attributes & VERTEX_TANGENT) != 0 ? 4 : 0)
            for (vertex in range(p.vertex_count)){
                let weights = vertex * p.stride + skin_offset + 4
                if (uint(vertices[weights]) + uint(vertices[weights + 1]) + uint(vertices[weights + 2]) + uint(vertices[weights + 3]) != 255u ||
                    uint(vertices[weights]) < uint(vertices[weights + 1]) || uint(vertices[weights + 1]) < uint(vertices[weights + 2]) || uint(vertices[weights + 2]) < uint(vertices[weights + 3])){delete vertices; return false}
            }
        }
        delete vertices
        var indices : array<uint>
        if (p.encoded){
            if (p.mode != GltfPrimitiveMode.triangles || p.index_count == 0 || p.index_count % 3 != 0 ||
                !meshopt_decode_indices(indices, p.indices, uint(p.index_count))){delete indices; return false}
        }else{
            indices |> reserve(p.index_count); indices |> resize(p.index_count)
            if (!empty(indices)){unsafe{memcpy(addr(indices[0]), addr(p.indices[0]), p.index_count * 4)}}
        }
        for (index in indices){if (index >= uint(p.vertex_count)){delete indices; return false}}
        delete indices
    }}
    return true
}
def private save_binary_file(path : string; data : auto) : bool {
    let parent = dir_name(path); mkdir_rec(parent)
    let temporary = "{path}.tmp-{ref_time_ticks()}"; var ok = false
    fopen(temporary, "wb") $(f){if (f != null){ok = fsave(f, data) > 0}}
    if (ok){var error : string; ok = rename(temporary, path, error)}
    if (!ok){remove(temporary)}
    return ok
}
def save_processed(asset : ProcessedAsset; path : string; shared_directory : string = "") : bool {
    //! Save a validated asset as one `.das_glb` binary or as a `.das_gltf` manifest plus geometry
    //! and texture files. `shared_directory` allows textures to be shared across manifests.
    if (!processed_valid(asset)){return false}
    if (ends_with(path, ".das_glb")){return save_binary_file(path, asset)}
    if (!ends_with(path, ".das_gltf")){return false}
    let dir = dir_name(path)
    var manifest = ProcessedManifest(version = DAS_GLTF_VERSION, key = asset.key, geometry = "{asset.key}.das_mesh")
    var geometry <- clone(asset); delete geometry.images
    var ok = save_binary_file("{dir}/{manifest.geometry}", geometry); delete geometry
    manifest.textures |> reserve(length(asset.images))
    for (image in asset.images){
        let storage = empty(shared_directory) ? dir : shared_directory
        let destination = "{storage}/textures/{image.key}.das_tex"
        var error : string; let file = relative(destination, dir, error)
        if (!empty(error) || empty(file)){ok = false; break}
        manifest.textures |> push(file)
        if (!save_binary_file(destination, image)){ok = false}
    }
    if (ok){ok = fwrite(path, sprint_json(manifest, false))}
    delete manifest; return ok
}
def load_processed(path : string) : ProcessedAsset {
    //! Load and validate a `.das_glb` binary or split `.das_gltf` manifest. Panics on missing,
    //! malformed, mismatched, or unsupported data.
    var out : ProcessedAsset; var ok = false
    if (ends_with(path, ".das_glb")){fopen(path, "rb") $(f){if (f != null){ok = fload(f, out)}}}
    elif (ends_with(path, ".das_gltf")){
        var m : ProcessedManifest
        if (sscan_json(fread(path), m) && m.version == DAS_GLTF_VERSION){
            let dir = dir_name(path)
            fopen("{dir}/{m.geometry}", "rb") $(f){if (f != null){ok = fload(f, out)}}
            if (ok && out.key == m.key){out.images |> reserve(length(out.images) + length(m.textures)); for (file in m.textures){
                var image : BlockTexture; var loaded = false
                fopen("{dir}/{file}", "rb") $(f){if (f != null){loaded = fload(f, image)}}
                if (!loaded){ok = false}; out.images |> emplace(image)
            }}else{ok = false}
        }
        delete m
    }
    if (!ok || !processed_valid(out) || !processed_payloads_valid(out)){delete out; panic("Invalid processed glTF: {path}")}
    return <- out
}
def load_and_process_gltf(path : string; profile : ProcessingProfile = ProcessingProfile()) : ProcessedAsset {
    //! Load an existing processed file by extension, otherwise parse and process a source glTF.
    //! Panics when the source is missing, unreadable, or contains no nodes, meshes, or animations.
    if (ends_with(path, ".das_gltf") || ends_with(path, ".das_glb")){return <- load_processed(path)}
    if (!stat(path).is_valid){panic("Asset not found: {path}")}
    var source <- load_gltf(path)
    if (empty(source.nodes) && empty(source.meshes) && empty(source.animations)){delete source; panic("Empty or unreadable glTF: {path}")}
    var result <- process_gltf(source, path, profile)
    delete source; return <- result
}
