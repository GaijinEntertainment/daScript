options gen2
options indenting = 4

module gltf_parse shared

require gltf/gltf_types
require gltf/gltf_accessor
require gltf/gltf_scene
require math
require daslib/math_boost
require daslib/json_boost
require daslib/base64
require daslib/fio
require daslib/strings
require stbimage/stbimage_boost
require strings

// ===== glTF 2.0 JSON schema (private; mirrors the on-disk keys) =====
// Filled by sscan_json, then lowered to the backend-agnostic GltfScene.

struct private JTexInfo {
    index : int = -1
    texCoord : int = 0
    scale : float = 1.
    strength : float = 1.
}

struct private JPbr {
    baseColorFactor : float4 = float4(1., 1., 1., 1.)
    metallicFactor : float = 1.
    roughnessFactor : float = 1.
    baseColorTexture : JTexInfo = JTexInfo()
    metallicRoughnessTexture : JTexInfo = JTexInfo()
}

struct private JMaterial {
    name : string
    pbrMetallicRoughness : JPbr = JPbr()
    normalTexture : JTexInfo = JTexInfo()
    occlusionTexture : JTexInfo = JTexInfo()
    emissiveTexture : JTexInfo = JTexInfo()
    emissiveFactor : float3 = float3(0., 0., 0.)
    alphaMode : string = "OPAQUE"
    alphaCutoff : float = 0.5
    doubleSided : bool = false
}

struct private JPrimitive {
    attributes : table<string; int>
    indices : int = -1
    material : int = -1
    mode : int = 4
}

struct private JMesh {
    name : string
    primitives : array<JPrimitive>
}

struct private JSparseIndices {
    bufferView : int = -1
    byteOffset : int = 0
    componentType : int = 0
}

struct private JSparseValues {
    bufferView : int = -1
    byteOffset : int = 0
}

struct private JSparse {
    count : int = 0
    indices : JSparseIndices = JSparseIndices()
    values : JSparseValues = JSparseValues()
}

struct private JAccessor {
    bufferView : int = -1
    byteOffset : int = 0
    componentType : int = 0
    count : int = 0
    @rename = "type" _type : string = "SCALAR"
    normalized : bool = false
    sparse : JSparse = JSparse()
}

struct private JBufferView {
    buffer : int = -1
    byteOffset : int = 0
    byteLength : int = 0
    byteStride : int = 0
    target : int = 0
}

struct private JBuffer {
    uri : string
    byteLength : int = 0
}

struct private JNode {
    name : string
    children : array<int>
    mesh : int = -1
    skin : int = -1
    matrix : array<float>
    translation : float3 = float3(0., 0., 0.)
    rotation : float4 = float4(0., 0., 0., 1.)
    scale : float3 = float3(1., 1., 1.)
}

struct private JScene {
    nodes : array<int>
}

struct private JSkin {
    inverseBindMatrices : int = -1
    joints : array<int>
    skeleton : int = -1
}

struct private JTexture {
    source : int = -1
    sampler : int = -1
}

struct private JImage {
    uri : string
    bufferView : int = -1
    @rename = "mimeType" mimeType : string
}

struct private JSampler {
    magFilter : int = 0
    minFilter : int = 0
    wrapS : int = 10497
    wrapT : int = 10497
}

struct private JAnimTarget {
    node : int = -1
    path : string = "translation"
}

struct private JAnimChannel {
    sampler : int = -1
    target : JAnimTarget = JAnimTarget()
}

struct private JAnimSampler {
    input : int = -1
    output : int = -1
    interpolation : string = "LINEAR"
}

struct private JAnimation {
    name : string
    samplers : array<JAnimSampler>
    channels : array<JAnimChannel>
}

struct private JGltf {
    scene : int = -1
    scenes : array<JScene>
    nodes : array<JNode>
    meshes : array<JMesh>
    accessors : array<JAccessor>
    bufferViews : array<JBufferView>
    buffers : array<JBuffer>
    materials : array<JMaterial>
    textures : array<JTexture>
    images : array<JImage>
    samplers : array<JSampler>
    skins : array<JSkin>
    animations : array<JAnimation>
    extensionsUsed : array<string>
    extensionsRequired : array<string>
}

// ===== string → enum helpers =====

def private parse_alpha_mode(s : string) : GltfAlphaMode {
    if (s == "MASK") return GltfAlphaMode.mask
    if (s == "BLEND") return GltfAlphaMode.blend
    return GltfAlphaMode.opaque
}

// glTF primitive.mode -> one of our three supported topologies. dasGLTF targets indexed TRIANGLES
// (what glTF exporters emit almost universally); the strip/fan/loop modes (LINE_LOOP/LINE_STRIP 2/3,
// TRIANGLE_STRIP/FAN 5/6) are NOT supported and collapse to their base lines/triangles here — an
// asset that actually relies on strip/fan connectivity will render with wrong topology.
def private parse_mode(m : int) : GltfPrimitiveMode {
    if (m == 0) return GltfPrimitiveMode.points
    if (m == 1 || m == 2 || m == 3) return GltfPrimitiveMode.lines
    return GltfPrimitiveMode.triangles
}

def private parse_interp(s : string) : GltfInterp {
    if (s == "STEP") return GltfInterp.step
    if (s == "CUBICSPLINE") return GltfInterp.cubicspline
    return GltfInterp.linear
}

def private parse_path(s : string) : GltfPath {
    if (s == "rotation") return GltfPath.rotation
    if (s == "scale") return GltfPath.scale
    if (s == "weights") return GltfPath.weights
    return GltfPath.translation
}

def private attr(prim : JPrimitive; key : string) : int {
    return prim.attributes?[key] ?? -1
}

// dasGLTF implements no glTF extensions yet; when one lands, list its name here.
[unused_argument(name)]
def private extension_supported(name : string) : bool => false

// ===== accessor decode =====
// One float4 per element (missing lanes zero); handles interleaved byteStride,
// integer-lane / normalized componentTypes, and sparse overrides.

// Validate that the byte span [base, base+span) fits inside buffers[bufIdx]. A malformed/hostile
// glTF can name an out-of-range buffer or a span past the buffer end; failing this leaves the decode
// zero-filled (or skips the sparse/image patch) rather than panicking, keeping load_gltf's documented
// fail-closed contract (return an empty scene on bad input).
def private buffer_range_ok(buffers : array<array<uint8>>; bufIdx : int; base, span : int64) : bool {
    return bufIdx >= 0 && bufIdx < length(buffers) && base >= 0l && base + span <= long_length(buffers[bufIdx])
}

def private decode_floats(gltf : JGltf; buffers : array<array<uint8>>; ai : int) : array<float4> {
    var result : array<float4>
    if (ai < 0 || ai >= length(gltf.accessors)) return <- result
    let acc = gltf.accessors[ai]
    let ncomp = gltf_type_ncomp(acc._type)
    result |> resize(acc.count)
    // unknown accessor type -> ncomp 0 makes elemSize/span degenerate; leave the result zero-filled
    if (ncomp <= 0) return <- result
    if (acc.bufferView >= 0 && acc.bufferView < length(gltf.bufferViews)) {
        let bv = gltf.bufferViews[acc.bufferView]
        let compSize = gltf_comp_size(acc.componentType)
        let elemSize = compSize * int64(ncomp)
        let stride = bv.byteStride != 0 ? int64(bv.byteStride) : elemSize
        let base = int64(bv.byteOffset) + int64(acc.byteOffset)
        let span = acc.count > 0 ? int64(acc.count - 1) * stride + elemSize : 0l
        if (buffer_range_ok(buffers, bv.buffer, base, span)) {
            for (i in range(acc.count)) {
                let eoff = base + int64(i) * stride
                var f0 = 0.; var f1 = 0.; var f2 = 0.; var f3 = 0.
                f0 = gltf_read_comp_float(buffers[bv.buffer], eoff, acc.componentType, acc.normalized)
                if (ncomp > 1) { f1 = gltf_read_comp_float(buffers[bv.buffer], eoff + compSize, acc.componentType, acc.normalized) }
                if (ncomp > 2) { f2 = gltf_read_comp_float(buffers[bv.buffer], eoff + compSize * 2l, acc.componentType, acc.normalized) }
                if (ncomp > 3) { f3 = gltf_read_comp_float(buffers[bv.buffer], eoff + compSize * 3l, acc.componentType, acc.normalized) }
                result[i] = float4(f0, f1, f2, f3)
            }
        }
    }
    apply_sparse_floats(gltf, buffers, acc, result)
    return <- result
}

def private apply_sparse_floats(gltf : JGltf; buffers : array<array<uint8>>; acc : JAccessor; var result : array<float4>) {
    if (acc.sparse.count <= 0) return
    let sp = acc.sparse
    let ncomp = gltf_type_ncomp(acc._type)
    // fail-closed: a malformed sparse accessor can name out-of-range bufferViews/buffers
    if (sp.indices.bufferView < 0 || sp.indices.bufferView >= length(gltf.bufferViews)
            || sp.values.bufferView < 0 || sp.values.bufferView >= length(gltf.bufferViews)) return
    let ibv = gltf.bufferViews[sp.indices.bufferView]
    let iCompSize = gltf_comp_size(sp.indices.componentType)
    let iStride = ibv.byteStride != 0 ? int64(ibv.byteStride) : iCompSize
    let iBase = int64(ibv.byteOffset) + int64(sp.indices.byteOffset)
    let vbv = gltf.bufferViews[sp.values.bufferView]
    let vCompSize = gltf_comp_size(acc.componentType)
    let vElem = vCompSize * int64(ncomp)
    let vStride = vbv.byteStride != 0 ? int64(vbv.byteStride) : vElem
    let vBase = int64(vbv.byteOffset) + int64(sp.values.byteOffset)
    let iSpan = sp.count > 0 ? int64(sp.count - 1) * iStride + iCompSize : 0l
    let vSpan = sp.count > 0 ? int64(sp.count - 1) * vStride + vElem : 0l
    if (!buffer_range_ok(buffers, ibv.buffer, iBase, iSpan) || !buffer_range_ok(buffers, vbv.buffer, vBase, vSpan)) return
    for (k in range(sp.count)) {
        let target = int(gltf_read_comp_uint(buffers[ibv.buffer], iBase + int64(k) * iStride, sp.indices.componentType))
        let voff = vBase + int64(k) * vStride
        var f0 = 0.; var f1 = 0.; var f2 = 0.; var f3 = 0.
        f0 = gltf_read_comp_float(buffers[vbv.buffer], voff, acc.componentType, acc.normalized)
        if (ncomp > 1) { f1 = gltf_read_comp_float(buffers[vbv.buffer], voff + vCompSize, acc.componentType, acc.normalized) }
        if (ncomp > 2) { f2 = gltf_read_comp_float(buffers[vbv.buffer], voff + vCompSize * 2l, acc.componentType, acc.normalized) }
        if (ncomp > 3) { f3 = gltf_read_comp_float(buffers[vbv.buffer], voff + vCompSize * 3l, acc.componentType, acc.normalized) }
        if (target >= 0 && target < length(result)) { result[target] = float4(f0, f1, f2, f3) }
    }
}

def private decode_uints(gltf : JGltf; buffers : array<array<uint8>>; ai : int) : array<uint> {
    var result : array<uint>
    if (ai < 0 || ai >= length(gltf.accessors)) return <- result
    let acc = gltf.accessors[ai]
    result |> resize(acc.count)
    if (acc.bufferView >= 0 && acc.bufferView < length(gltf.bufferViews)) {
        let bv = gltf.bufferViews[acc.bufferView]
        let compSize = gltf_comp_size(acc.componentType)
        let stride = bv.byteStride != 0 ? int64(bv.byteStride) : compSize
        let base = int64(bv.byteOffset) + int64(acc.byteOffset)
        let span = acc.count > 0 ? int64(acc.count - 1) * stride + compSize : 0l
        if (buffer_range_ok(buffers, bv.buffer, base, span)) {
            for (i in range(acc.count)) {
                result[i] = gltf_read_comp_uint(buffers[bv.buffer], base + int64(i) * stride, acc.componentType)
            }
        }
    }
    apply_sparse_uints(gltf, buffers, acc, result)
    return <- result
}

// Sparse override for a uint (index) accessor, mirroring apply_sparse_floats. Rare for index
// accessors, but glTF allows it and decode_floats already honors sparse, so keep the two consistent.
def private apply_sparse_uints(gltf : JGltf; buffers : array<array<uint8>>; acc : JAccessor; var result : array<uint>) {
    if (acc.sparse.count <= 0) return
    let sp = acc.sparse
    if (sp.indices.bufferView < 0 || sp.indices.bufferView >= length(gltf.bufferViews)
            || sp.values.bufferView < 0 || sp.values.bufferView >= length(gltf.bufferViews)) return
    let ibv = gltf.bufferViews[sp.indices.bufferView]
    let iCompSize = gltf_comp_size(sp.indices.componentType)
    let iStride = ibv.byteStride != 0 ? int64(ibv.byteStride) : iCompSize
    let iBase = int64(ibv.byteOffset) + int64(sp.indices.byteOffset)
    let vbv = gltf.bufferViews[sp.values.bufferView]
    let vCompSize = gltf_comp_size(acc.componentType)
    let vStride = vbv.byteStride != 0 ? int64(vbv.byteStride) : vCompSize
    let vBase = int64(vbv.byteOffset) + int64(sp.values.byteOffset)
    let iSpan = sp.count > 0 ? int64(sp.count - 1) * iStride + iCompSize : 0l
    let vSpan = sp.count > 0 ? int64(sp.count - 1) * vStride + vCompSize : 0l
    if (!buffer_range_ok(buffers, ibv.buffer, iBase, iSpan) || !buffer_range_ok(buffers, vbv.buffer, vBase, vSpan)) return
    for (k in range(sp.count)) {
        let target = int(gltf_read_comp_uint(buffers[ibv.buffer], iBase + int64(k) * iStride, sp.indices.componentType))
        let v = gltf_read_comp_uint(buffers[vbv.buffer], vBase + int64(k) * vStride, acc.componentType)
        if (target >= 0 && target < length(result)) {
            result[target] = v
        }
    }
}

def private decode_mat4(gltf : JGltf; buffers : array<array<uint8>>; ai : int) : array<float4x4> {
    var result : array<float4x4>
    if (ai < 0 || ai >= length(gltf.accessors)) return <- result
    let acc = gltf.accessors[ai]
    // glTF requires MAT4 accessors to be FLOAT (5126); the reads below are gltf_rd_f32 (4 bytes),
    // so reject any other componentType rather than mis-sizing the span check.
    if (acc.componentType != 5126) return <- result
    result |> resize(acc.count)
    if (acc.bufferView >= 0 && acc.bufferView < length(gltf.bufferViews)) {
        let bv = gltf.bufferViews[acc.bufferView]
        let compSize = gltf_comp_size(acc.componentType)
        let elemSize = compSize * 16l
        let stride = bv.byteStride != 0 ? int64(bv.byteStride) : elemSize
        let base = int64(bv.byteOffset) + int64(acc.byteOffset)
        let span = acc.count > 0 ? int64(acc.count - 1) * stride + elemSize : 0l
        if (buffer_range_ok(buffers, bv.buffer, base, span)) {
            for (i in range(acc.count)) {
                let eoff = base + int64(i) * stride
                var m : float4x4
                for (col in range(4)) {
                    m[col] = float4(
                        gltf_rd_f32(buffers[bv.buffer], eoff + int64(col * 4 + 0) * compSize),
                        gltf_rd_f32(buffers[bv.buffer], eoff + int64(col * 4 + 1) * compSize),
                        gltf_rd_f32(buffers[bv.buffer], eoff + int64(col * 4 + 2) * compSize),
                        gltf_rd_f32(buffers[bv.buffer], eoff + int64(col * 4 + 3) * compSize))
                }
                result[i] = m
            }
        }
    }
    return <- result
}

def private mat4_from_array(m : array<float>) : float4x4 {
    var r : float4x4
    for (col in range(4)) {
        r[col] = float4(m[col * 4 + 0], m[col * 4 + 1], m[col * 4 + 2], m[col * 4 + 3])
    }
    return r
}

// ===== buffer / image sources =====

def private decode_data_uri(uri : string; var out : array<uint8>) : bool {
    let comma = find(uri, "base64,")
    if (comma < 0) return false
    base64_decode(slice(uri, comma + length("base64,")), out)
    return true
}

def private decode_buffers(gltf : JGltf; var glbBin : array<uint8>; baseDir : string; var buffers : array<array<uint8>>) {
    var glbUsed = false
    buffers |> reserve(length(gltf.buffers))
    for (buf in gltf.buffers) {
        var b : array<uint8>
        if (empty(buf.uri)) {
            if (!glbUsed) {
                b <- glbBin  // nolint:PERF030 - b is fresh this iteration
                glbUsed = true
            }
        } elif (starts_with(buf.uri, "data:")) {
            decode_data_uri(buf.uri, b)
        } else {
            let p = path_join(baseDir, buf.uri)
            fopen(p, "rb") $(f) {
                if (f != null) {
                    fmap(f) $(var data : array<uint8>#) {
                        b := data
                    }
                }
            }
        }
        buffers |> emplace(b)
    }
}

def private decode_image(gltf : JGltf; buffers : array<array<uint8>>; ji : JImage; baseDir : string; var img : GltfImage) {
    var encoded : array<uint8>
    if (!empty(ji.uri)) {
        if (starts_with(ji.uri, "data:")) {
            decode_data_uri(ji.uri, encoded)
        } else {
            let p = path_join(baseDir, ji.uri)
            fopen(p, "rb") $(f) {
                if (f != null) {
                    fmap(f) $(var data : array<uint8>#) {
                        encoded := data
                    }
                }
            }
        }
    } elif (ji.bufferView >= 0 && ji.bufferView < length(gltf.bufferViews)) {
        let bv = gltf.bufferViews[ji.bufferView]
        if (buffer_range_ok(buffers, bv.buffer, int64(bv.byteOffset), int64(bv.byteLength))) {
            encoded |> resize(bv.byteLength)
            for (k in range(bv.byteLength)) {
                encoded[k] = buffers[bv.buffer][int64(bv.byteOffset) + int64(k)]
            }
        }
    }
    if (!empty(encoded)) {
        var stb = Image()
        let r = stb.load_from_memory(encoded, 4)
        if (r._0) {
            img.width = stb.width
            img.height = stb.height
            img.pixels <- stb.bytes
        } else {
            to_log(LOG_ERROR, "gltf: image decode failed: {r._1}\n")
        }
    }
    delete encoded
}

// ===== JGltf → GltfScene =====

def private build_scene(gltf : JGltf; buffers : array<array<uint8>>; baseDir : string; var scene : GltfScene) {  // nolint:STYLE038 - one linear pass per scene facet
    // materials
    scene.materials |> reserve(length(gltf.materials))
    for (jm in gltf.materials) {
        var m = GltfMaterial()
        m.name = jm.name
        m.baseColorFactor = jm.pbrMetallicRoughness.baseColorFactor
        m.metallicFactor = jm.pbrMetallicRoughness.metallicFactor
        m.roughnessFactor = jm.pbrMetallicRoughness.roughnessFactor
        m.baseColorTex = jm.pbrMetallicRoughness.baseColorTexture.index
        m.metalRoughTex = jm.pbrMetallicRoughness.metallicRoughnessTexture.index
        m.normalTex = jm.normalTexture.index
        m.normalScale = jm.normalTexture.scale
        m.occlusionTex = jm.occlusionTexture.index
        m.occlusionStrength = jm.occlusionTexture.strength
        m.emissiveTex = jm.emissiveTexture.index
        m.emissiveFactor = jm.emissiveFactor
        m.alphaMode = parse_alpha_mode(jm.alphaMode)
        m.alphaCutoff = jm.alphaCutoff
        m.doubleSided = jm.doubleSided
        scene.materials |> emplace(m)
    }
    // meshes / primitives
    scene.meshes |> reserve(length(gltf.meshes))
    for (jmesh in gltf.meshes) {
        var mesh = GltfMesh()
        mesh.name = jmesh.name
        mesh.primitives |> reserve(length(jmesh.primitives))
        for (jp in jmesh.primitives) {
            var prim = GltfPrimitive()
            prim.material = jp.material
            prim.mode = parse_mode(jp.mode)
            let posAcc = attr(jp, "POSITION")
            let vcount = (posAcc >= 0 && posAcc < length(gltf.accessors)) ? gltf.accessors[posAcc].count : 0
            prim.vertices <- [for (_iv in range(vcount)); GltfVertex()]
            fill_vec3(gltf, buffers, posAcc, prim.vertices, GltfVertexField.position)
            fill_vec3(gltf, buffers, attr(jp, "NORMAL"), prim.vertices, GltfVertexField.normal)
            fill_vec4(gltf, buffers, attr(jp, "TANGENT"), prim.vertices, GltfVertexField.tangent)
            fill_vec2(gltf, buffers, attr(jp, "TEXCOORD_0"), prim.vertices, GltfVertexField.uv0)
            fill_vec2(gltf, buffers, attr(jp, "TEXCOORD_1"), prim.vertices, GltfVertexField.uv1)
            fill_color(gltf, buffers, attr(jp, "COLOR_0"), prim.vertices)
            fill_vec4(gltf, buffers, attr(jp, "JOINTS_0"), prim.vertices, GltfVertexField.joints0)
            fill_vec4(gltf, buffers, attr(jp, "WEIGHTS_0"), prim.vertices, GltfVertexField.weights0)
            if (jp.indices >= 0) {
                prim.indices <- decode_uints(gltf, buffers, jp.indices)
            } else {
                prim.indices |> resize(vcount)
                for (i in range(vcount)) {
                    prim.indices[i] = uint(i)
                }
            }
            mesh.primitives |> emplace(prim)
        }
        scene.meshes |> emplace(mesh)
    }
    // nodes
    scene.nodes |> reserve(length(gltf.nodes))
    for (jn in gltf.nodes) {
        var n = GltfNode()
        n.name = jn.name
        n.translation = jn.translation
        n.rotation = jn.rotation
        n.scale = jn.scale
        n.mesh = jn.mesh
        n.skin = jn.skin
        n.children := jn.children
        if (length(jn.matrix) == 16) {
            n.local = mat4_from_array(jn.matrix)
        } else {
            n.local = compose(jn.translation, jn.rotation, jn.scale)
        }
        scene.nodes |> emplace(n)
    }
    // skins
    scene.skins |> reserve(length(gltf.skins))
    for (js in gltf.skins) {
        var sk = GltfSkin()
        sk.joints := js.joints
        sk.skeleton = js.skeleton
        if (js.inverseBindMatrices >= 0) {
            sk.inverseBind <- decode_mat4(gltf, buffers, js.inverseBindMatrices)
        }
        scene.skins |> emplace(sk)
    }
    // animations
    scene.animations |> reserve(length(gltf.animations))
    for (ja in gltf.animations) {
        var an = GltfAnimation()
        an.name = ja.name
        var maxT = 0.
        an.samplers |> reserve(length(ja.samplers))
        for (jsamp in ja.samplers) {
            var s = GltfAnimSampler()
            s.interp = parse_interp(jsamp.interpolation)
            var inF <- decode_floats(gltf, buffers, jsamp.input)
            s.input |> resize(length(inF))
            for (i in range(length(inF))) {
                s.input[i] = inF[i].x
                maxT = max(maxT, inF[i].x)
            }
            delete inF
            s.output <- decode_floats(gltf, buffers, jsamp.output)
            an.samplers |> emplace(s)
        }
        an.channels |> reserve(length(ja.channels))
        for (jc in ja.channels) {
            var ch = GltfAnimChannel()
            ch.sampler = jc.sampler
            ch.targetNode = jc.target.node
            ch.targetPath = parse_path(jc.target.path)
            an.channels |> emplace(ch)
        }
        an.duration = maxT
        scene.animations |> emplace(an)
    }
    // textures / samplers / images
    scene.textures |> reserve(length(gltf.textures))
    for (jt in gltf.textures) {
        scene.textures |> push(GltfTexture(image = jt.source, sampler = jt.sampler))
    }
    scene.samplers |> reserve(length(gltf.samplers))
    for (jsm in gltf.samplers) {
        scene.samplers |> push(GltfSampler(wrapS = jsm.wrapS, wrapT = jsm.wrapT, minFilter = jsm.minFilter, magFilter = jsm.magFilter))
    }
    scene.images |> reserve(length(gltf.images))
    for (ji in gltf.images) {
        var img : GltfImage
        decode_image(gltf, buffers, ji, baseDir, img)
        scene.images |> emplace(img)
    }
    // default scene roots
    let sceneIdx = max(gltf.scene, 0)
    if (sceneIdx >= 0 && sceneIdx < length(gltf.scenes)) {
        scene.roots := gltf.scenes[sceneIdx].nodes
    }
}

enum private GltfVertexField {
    position
    normal
    tangent
    uv0
    uv1
    joints0
    weights0
}

def private fill_vec3(gltf : JGltf; buffers : array<array<uint8>>; ai : int; var verts : array<GltfVertex>; field : GltfVertexField) {
    if (ai < 0) return
    var d <- decode_floats(gltf, buffers, ai)
    let n = min(length(verts), length(d))
    for (i in range(n)) {
        if (field == GltfVertexField.position) {
            verts[i].position = d[i].xyz
        } else {
            verts[i].normal = d[i].xyz
        }
    }
    delete d
}

def private fill_vec2(gltf : JGltf; buffers : array<array<uint8>>; ai : int; var verts : array<GltfVertex>; field : GltfVertexField) {
    if (ai < 0) return
    var d <- decode_floats(gltf, buffers, ai)
    let n = min(length(verts), length(d))
    for (i in range(n)) {
        if (field == GltfVertexField.uv0) {
            verts[i].uv0 = d[i].xy
        } else {
            verts[i].uv1 = d[i].xy
        }
    }
    delete d
}

def private fill_vec4(gltf : JGltf; buffers : array<array<uint8>>; ai : int; var verts : array<GltfVertex>; field : GltfVertexField) {
    if (ai < 0) return
    var d <- decode_floats(gltf, buffers, ai)
    let n = min(length(verts), length(d))
    for (i in range(n)) {
        if (field == GltfVertexField.tangent) {
            verts[i].tangent = d[i]
        } elif (field == GltfVertexField.joints0) {
            verts[i].joints0 = d[i]
        } else {
            verts[i].weights0 = d[i]
        }
    }
    delete d
}

def private fill_color(gltf : JGltf; buffers : array<array<uint8>>; ai : int; var verts : array<GltfVertex>) {
    if (ai < 0) return
    var d <- decode_floats(gltf, buffers, ai)
    let isVec3 = gltf.accessors[ai]._type == "VEC3"
    let n = min(length(verts), length(d))
    for (i in range(n)) {
        verts[i].color0 = isVec3 ? float4(d[i].x, d[i].y, d[i].z, 1.) : d[i]
    }
    delete d
}

// ===== GLB container =====

def private glb_chunk_string(bytes : array<uint8>; start : int64; len : int64) : string {
    var sub : array<uint8>
    sub |> resize(len)
    for (i in range64(len)) {
        sub[i] = bytes[start + i]
    }
    let s = string(sub)
    delete sub
    return s
}

def private parse_glb(bytes : array<uint8>; var jsonText : string&; var binBytes : array<uint8>) {
    // fail-closed: a malformed/hostile .glb can declare a total or chunk length past the actual
    // file; clamp the declared total to the real size and stop at the first chunk that runs past it,
    // so the header/chunk reads below never index out of bounds.
    let n = long_length(bytes)
    if (n < 12l) return
    let total = min(int64(gltf_rd_u32(bytes, 8l)), n)
    var off = 12l
    while (off + 8l <= total) {
        let clen = int64(gltf_rd_u32(bytes, off))
        let ctype = gltf_rd_u32(bytes, off + 4l)
        let dataStart = off + 8l
        if (dataStart + clen > total) break
        if (ctype == 0x4E4F534Au) {          // "JSON"
            jsonText = glb_chunk_string(bytes, dataStart, clen)
        } elif (ctype == 0x004E4942u) {       // "BIN\0"
            binBytes |> resize(clen)
            for (i in range64(clen)) {
                binBytes[i] = bytes[dataStart + i]
            }
        }
        off = dataStart + clen
    }
}

def private parse_gltf_json(jsonText : string; var glbBin : array<uint8>; baseDir : string) : GltfScene {
    var scene : GltfScene
    var err : string
    let js = read_json(jsonText, err)
    if (js == null) {
        to_log(LOG_ERROR, "gltf: JSON parse failed: {err}\n")
        return <- scene
    }
    // from_JV (NOT sscan_json): sscan_json zero-fills array-of-struct elements and
    // ignores field defaults, so omitted glTF defaults (node rotation/scale, primitive
    // mode, texture index -1, ...) would come out zero. from_JV honors the defaults.
    var doc = from_JV(js, type<JGltf>)
    // fail closed on extensionsRequired (per spec): decoding an asset whose required
    // extension we don't implement (e.g. KHR_draco_mesh_compression) would silently
    // yield degenerate zero-geometry, not a recognizable error
    for (ext in doc.extensionsRequired) {
        if (!extension_supported(ext)) {
            to_log(LOG_ERROR, "gltf: asset requires unsupported extension {ext}\n")
            return <- scene
        }
    }
    for (ext in doc.extensionsUsed) {
        if (!extension_supported(ext)) {
            to_log(LOG_WARNING, "gltf: optional extension {ext} not supported; ignoring\n")
        }
    }
    var buffers : array<array<uint8>>
    decode_buffers(doc, glbBin, baseDir, buffers)
    build_scene(doc, buffers, baseDir, scene)
    update_world_transforms(scene)
    delete buffers
    return <- scene
}

def load_gltf_from_memory(bytes : array<uint8>; baseDir : string = "") : GltfScene {
    //! Load a glTF 2.0 document (.gltf JSON, .glb binary, or base64-embedded
    //! .gltf) from an in-memory byte buffer. `baseDir` resolves external
    //! buffer/image URIs (pass "" for a self-contained .glb). Returns an empty
    //! scene on failure.
    var scene : GltfScene
    var jsonText : string
    var glbBin : array<uint8>
    if (length(bytes) >= 12 && gltf_rd_u32(bytes, 0l) == 0x46546C67u) {
        parse_glb(bytes, jsonText, glbBin)
    } else {
        jsonText = string(bytes)
    }
    scene <- parse_gltf_json(jsonText, glbBin, baseDir)
    delete glbBin
    return <- scene
}

def load_gltf(path : string) : GltfScene {
    //! Load a glTF 2.0 file (.gltf, .glb, or base64-embedded .gltf) into a
    //! backend-agnostic `GltfScene`. Returns an empty scene on failure.
    var scene : GltfScene
    var fileBytes : array<uint8>
    var haveFile = false
    fopen(path, "rb") $(f) {
        if (f != null) {
            fmap(f) $(var data : array<uint8>#) {
                fileBytes := data
                haveFile = true
            }
        }
    }
    if (!haveFile) {
        to_log(LOG_ERROR, "gltf: cannot open {path}\n")
        return <- scene
    }
    scene <- load_gltf_from_memory(fileBytes, dir_name(path))
    delete fileBytes
    return <- scene
}
