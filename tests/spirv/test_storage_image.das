options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_shader
require spirv/spirv_builtins public

// Phase-7.1b storage image: an image2D global lowers to a UniformConstant OpTypeImage with a KNOWN
// Rgba8 format and sampled=2 (read-write) + DescriptorSet/Binding; imageLoad -> OpImageRead,
// imageStore -> OpImageWrite. A known format means no without-format capability is required. spirv-val'd.
def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test(description = "An image2D lowers to a read-write Rgba8 OpTypeImage with imageLoad and imageStore.")]
def test_storage_image_compute(t : T?) {
    t |> run("storage image: image2D -> read-write OpTypeImage (Rgba8) + imageLoad/imageStore") <| @(t : T?) {
        var words <- storimg_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.GLCompute),
            "storimg: EntryPoint model is GLCompute")
        t |> success(has_op(words, SpvOp.TypeImage), "storimg: emits OpTypeImage")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ms, sampled, format].
        // sampled=2 (read-write) at index 6, format=Rgba8 (known, no without-format cap) at index 7.
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 2u),
            "storimg: OpTypeImage sampled=2 (read-write storage image)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 7, uint(SpvImageFormat.Rgba8)),
            "storimg: OpTypeImage format is Rgba8 (known format)")
        // UniformConstant-storage OpVariable (the storage-image descriptor) at operand index 2
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.UniformConstant)),
            "storimg: has a UniformConstant-storage OpVariable")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 0u),
            "storimg: DescriptorSet 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "storimg: Binding 0")
        t |> success(has_op(words, SpvOp.ImageRead), "storimg: imageLoad -> OpImageRead")
        t |> success(has_op(words, SpvOp.ImageWrite), "storimg: imageStore -> OpImageWrite")
        validate(t, words, "storimg")
        delete words
    }
}

[test(description = "imageSize on a storage image emits OpImageQuerySize and the ImageQuery capability.")]
def test_image_size_query(t : T?) {
    t |> run("imageSize(storage image) -> OpImageQuerySize + ImageQuery capability") <| @(t : T?) {
        var words <- imgquery_words()
        t |> success(has_op(words, SpvOp.ImageQuerySize), "imgquery: imageSize -> OpImageQuerySize")
        // the no-LOD query: must NOT emit the LOD variant (that one is textureSize's, Sampled=1 images)
        t |> success(!has_op(words, SpvOp.ImageQuerySizeLod),
            "imgquery: storage-image query is the no-LOD OpImageQuerySize, not OpImageQuerySizeLod")
        // OpImageQuerySize takes (result_type, result_id, image) -- the image operand is the loaded
        // storage image, so an OpLoad of the UniformConstant image must precede it
        t |> success(has_op(words, SpvOp.Load), "imgquery: loads the storage image before the query")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ImageQuery)),
            "imgquery: imageSize pulls in OpCapability ImageQuery")
        validate(t, words, "imgquery")
        delete words
    }
}

[test(description = "Module-scope let constants fold to OpConstant and OpConstantComposite.")]
def test_module_scope_const(t : T?) {
    t |> run("module-scope `let` constants fold to OpConstant / OpConstantComposite") <| @(t : T?) {
        var words <- modconst_words()
        // Compilation succeeding at all is the fold proof: a non-folded module-scope const falls through
        // to the fail-closed `unsupported global` error (see test_fail_closed.das for the `var` rejection).
        // Here we pin the positive structure -- the folded scalar consts become OpConstant, the const-
        // vector becomes OpConstantComposite, referenced straight by id (no OpVariable / OpLoad).
        t |> success(has_op(words, SpvOp.Constant), "modconst: scalar module consts -> OpConstant")
        t |> success(has_op(words, SpvOp.ConstantComposite),
            "modconst: const-vector module const -> OpConstantComposite")
        validate(t, words, "modconst")
        delete words
    }
}

[test(description = "An unannotated image2D stays Rgba8 while @format=r32f yields an R32f OpTypeImage.")]
def test_storage_image_format(t : T?) {
    t |> run("storage-image @format: default image2D stays Rgba8, @format=r32f -> OpTypeImage R32f") <| @(t : T?) {
        var words <- imgfmt_words()
        // two storage images of different formats -> two distinct OpTypeImage (deduped on the format operand)
        t |> success(count_op(words, SpvOp.TypeImage) == 2,
            "imgfmt: two distinct-format storage images -> 2 OpTypeImage")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ms, sampled, format].
        // format at operand index 7: the @format=r32f image carries R32f; the bare one keeps Rgba8.
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 7, uint(SpvImageFormat.R32f)),
            "imgfmt: @format=r32f image2D -> OpTypeImage format R32f")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 7, uint(SpvImageFormat.Rgba8)),
            "imgfmt: bare image2D stays Rgba8 (back-compat default)")
        // the format change leaves the read-write storage classification (sampled=2 at index 6) intact
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 2u),
            "imgfmt: storage images stay sampled=2 (read-write)")
        t |> success(has_op(words, SpvOp.ImageRead), "imgfmt: imageLoad -> OpImageRead")
        t |> success(has_op(words, SpvOp.ImageWrite), "imgfmt: imageStore -> OpImageWrite")
        validate(t, words, "imgfmt")
        delete words
    }
}

// Storage images beyond image2D, and the writeonly access qualifier.
//
// image3D / image2DArray / imageCubeArray lower to a read-write OpTypeImage (Sampled = 2) that differs
// from image2D only in Dim and Arrayed, and each is addressed with an int3.  Two capabilities ride
// along: a cube-ARRAYED storage image needs ImageCubeArray, and an @format outside SPIR-V's core
// storage set (rgba16 unorm) needs StorageImageExtendedFormats.  `writeonly` becomes a NonReadable
// decoration on the VARIABLE, the mirror of @readonly -> NonWritable.
//
// The asserts pin the whole OpTypeImage operand tuple, not one operand at a time -- a per-operand
// check would pass on a module that merely contains a 3D image somewhere and an Rgba16 one elsewhere.
// The tuples here are the ones glslang emits for these compute shaders.
// writeonly, the way a LUT-baking compute shader declares its outputs
var @binding = 0 @format = "rgba32f" @writeonly g_tex_big : image2DArray
var @binding = 1 @format = "rgba16f" @writeonly g_tex_noise : image3D
// the cube-arrayed + extended-format pair these shaders declare
var @binding = 2 @format = "rgba8" @writeonly g_tex_cube : imageCubeArray
var @binding = 3 @format = "rgba16" @writeonly g_tex_rgba16 : image2DArray
// a plain read-write image3D: imageLoad + imageSize must stay legal on it
var @binding = 4 @format = "rgba32f" g_tex_rw : image3D
// a RUNTIME-SIZED descriptor array of storage images: the texture-pool surface, indexed by slot.
// This is how a compute pass reaches its texture pools (`uniform image3D tex3d_rgba32f0[]`).
var @binding = 5 @format = "rgba32f" g_pool : array<image3D>

[compute_shader(local_size_x=64, name="storage_img_spv"), marker(no_coverage)]
def storage_images {
    let p = int3(gl_GlobalInvocationID)
    let slot = int(gl_WorkGroupID.x)
    let texel = imageLoad(g_tex_rw, p)
    let dims = imageSize(g_tex_rw)
    // read one pool slot, write another: a get_texel / set_texel shape
    let pooled = imageLoad(g_pool[slot], p)
    let v = texel + pooled + float4(float(dims.z), 0.0f, 0.0f, 0.0f)
    imageStore(g_pool[slot], p, v)
    imageStore(g_tex_big, p, v)
    imageStore(g_tex_noise, p, v)
    imageStore(g_tex_cube, p, v)
    imageStore(g_tex_rgba16, p, v)
}

// OpTypeImage operands: result_id(0), sampled_type(1), Dim(2), Depth(3), Arrayed(4), MS(5),
// Sampled(6), Format(7). SAMPLED_STORAGE = 2 is the read-write (non-sampled) class.
let DIM_OPERAND = 2
let ARRAYED_OPERAND = 4
let SAMPLED_OPERAND = 6
let FORMAT_OPERAND = 7
let SAMPLED_STORAGE = 2u

// The capability is operand 0 of OpCapability; positional, so an id equal to the enum can't match.
let CAP_OPERAND = 0

// words[1] is the SPIR-V version word of the module header (magic, version, generator, bound, schema).
let VERSION_WORD = 1

// True iff SOME single OpTypeImage carries this whole tuple -- Dim, Arrayed, Sampled and Format on the
// one instruction, so two different images cannot together satisfy a check neither one meets alone.
def private has_storage_image(words : array<uint>; dim : SpvDim; arrayed : uint; fmt : SpvImageFormat) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (opcode != uint(SpvOp.TypeImage) || n <= FORMAT_OPERAND) return
        if (w[s + DIM_OPERAND] == uint(dim) && w[s + ARRAYED_OPERAND] == arrayed &&
            w[s + SAMPLED_OPERAND] == SAMPLED_STORAGE && w[s + FORMAT_OPERAND] == uint(fmt)) {
            found = true
        }
    }
    return found
}

def private has_cap(words : array<uint>; cap : SpvCapability) : bool =>
    op_has_operand_at(words, SpvOp.Capability, CAP_OPERAND, uint(cap))

// How many OpAccessChain instructions are based on the descriptor-array VARIABLE -- i.e. how many
// pool[idx] accesses lowered. Counting bare OpAccessChain would also catch the builtin component
// reads (gl_WorkGroupID.x), so walk the type chain instead: OpTypeRuntimeArray -> the
// UniformConstant OpTypePointer to it -> the OpVariable of that pointer type -> access chains on it.
def private count_pool_access_chains(words : array<uint>) : int {
    var runtime_arrays, pool_ptrs, pool_vars : table<uint>
    var n = 0
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, cnt : int) {
        if (opcode == uint(SpvOp.TypeRuntimeArray) && cnt >= 1) {
            runtime_arrays |> insert(w[s])                       // result id
        } elif (opcode == uint(SpvOp.TypePointer) && cnt >= 3) {
            // result(0), storage class(1), pointee(2)
            if (w[s + 1] == uint(SpvStorageClass.UniformConstant) && key_exists(runtime_arrays, w[s + 2])) {
                pool_ptrs |> insert(w[s])
            }
        } elif (opcode == uint(SpvOp.Variable) && cnt >= 2) {
            // result type(0), result id(1)
            if (key_exists(pool_ptrs, w[s])) {
                pool_vars |> insert(w[s + 1])
            }
        } elif (opcode == uint(SpvOp.AccessChain) && cnt >= 3) {
            // result type(0), result id(1), base(2)
            if (key_exists(pool_vars, w[s + 2])) {
                n++
            }
        }
    }
    delete runtime_arrays
    delete pool_ptrs
    delete pool_vars
    return n
}

def private count_decoration(words : array<uint>; dec : SpvDecoration) : int =>
    count_op_operand_at(words, SpvOp.Decorate, 1, uint(dec))

[test(description = "image3D, image2DArray and imageCubeArray lower to the OpTypeImage operand tuples glslang emits.")]
def test_storage_images(t : T?) {
    t |> run("image3D / image2DArray / imageCubeArray lower to the OpTypeImage tuples glslang emits") <| @(t : T?) {
        var words <- clone_to_move(storage_img_spv)

        // each declaration's full tuple, matched against the glslang reference
        t |> success(has_storage_image(words, SpvDim._2D, 1u, SpvImageFormat.Rgba32f),
            "image2DArray rgba32f -> OpTypeImage 2D Arrayed=1 Sampled=2 Rgba32f")
        t |> success(has_storage_image(words, SpvDim._3D, 0u, SpvImageFormat.Rgba16f),
            "image3D rgba16f -> OpTypeImage 3D Arrayed=0 Sampled=2 Rgba16f")
        t |> success(has_storage_image(words, SpvDim.Cube, 1u, SpvImageFormat.Rgba8),
            "imageCubeArray rgba8 -> OpTypeImage Cube Arrayed=1 Sampled=2 Rgba8")
        t |> success(has_storage_image(words, SpvDim._2D, 1u, SpvImageFormat.Rgba16),
            "image2DArray rgba16 -> OpTypeImage 2D Arrayed=1 Sampled=2 Rgba16")
        t |> success(has_storage_image(words, SpvDim._3D, 0u, SpvImageFormat.Rgba32f),
            "image3D rgba32f (read-write) -> OpTypeImage 3D Arrayed=0 Sampled=2 Rgba32f")

        // the two capabilities the new dims / formats pull in, and neither one before its trigger
        t |> success(has_cap(words, SpvCapability.ImageCubeArray),
            "a cube-arrayed storage image declares ImageCubeArray")
        t |> success(has_cap(words, SpvCapability.StorageImageExtendedFormats),
            "the rgba16 format declares StorageImageExtendedFormats")
        t |> success(count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ImageCubeArray)) == 1,
            "ImageCubeArray is emitted exactly once")

        // writeonly -> NonReadable on the variable: the four @writeonly images carry it, g_tex_rw does not
        t |> success(count_decoration(words, SpvDecoration.NonReadable) == 4,
            "each @writeonly storage image is NonReadable, and only those")
        t |> success(count_decoration(words, SpvDecoration.NonWritable) == 0,
            "no storage image is NonWritable here")

        // the int3 surface: five stores, two loads, one size query
        t |> success(count_op(words, SpvOp.ImageWrite) == 5, "five imageStore -> five OpImageWrite")
        t |> success(count_op(words, SpvOp.ImageRead) == 2, "two imageLoad -> two OpImageRead")
        t |> success(count_op(words, SpvOp.ImageQuerySize) == 1, "imageSize -> OpImageQuerySize")
        t |> success(has_cap(words, SpvCapability.ImageQuery), "imageSize declares ImageQuery")

        // ----- the runtime-sized descriptor array of storage images -----
        t |> success(has_op(words, SpvOp.TypeRuntimeArray), "array<image3D> -> OpTypeRuntimeArray")
        // the pool is indexed: one OpAccessChain off the pool variable per access (a load and a store)
        t |> success(count_pool_access_chains(words) == 2, "pool[slot] -> one OpAccessChain per access")
        t |> success(has_cap(words, SpvCapability.RuntimeDescriptorArray),
            "a runtime-sized descriptor array declares RuntimeDescriptorArray")
        // RuntimeDescriptorArray is core only from SPIR-V 1.5; the emitter raises the module rather
        // than emit SPV_EXT_descriptor_indexing, which the glslang reference does not carry either
        t |> success(words[VERSION_WORD] == SPV_VERSION_1_5, "a descriptor array raises the module to SPIR-V 1.5")
        t |> success(!has_op(words, SpvOp.Extension), "no OpExtension: descriptor indexing is core at 1.5")
        // an opaque element has no byte layout, so the runtime array carries NO ArrayStride
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.ArrayStride)) == 0,
            "the opaque descriptor array carries no ArrayStride")
        // the array is NOT NonUniform-indexed here: a uniform slot needs no decoration or extra cap
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.NonUniform)) == 0,
            "a uniformly-indexed pool needs no NonUniform decoration")

        // the descriptor array raised the module to 1.5, so vulkan1.1 (SPIR-V 1.3) would reject the
        // binary on its version word alone -- validate against the target Vulkan environment
        let r = validate_spirv(words, "vulkan1.2")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.2): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
