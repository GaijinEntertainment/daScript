options gen2
options _comment_hygiene = true
options indenting = 4

// Canary for #56's single-view semantics: vk_view_create_unsafe clone-copies an
// opt-struct-ptr field into scratch and CONSUMES (nulls) the pointer, so finalize
// keeps off the caller's local and a re-view sees the field as absent. If re-view
// support is ever added (scratch-side ownership flag), this test flips and must be
// updated deliberately. Host-side only -- no instance, no device.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_structs

[test]
def test_opt_ptr_view_semantics(t : T?) {
    var vp : PipelineViewportStateCreateInfo
    var vport = Viewport(width = 1.0f, height = 1.0f, maxDepth = 1.0f)
    vp.pViewports |> push(vport)

    var gp : GraphicsPipelineCreateInfo
    var st : PipelineShaderStageCreateInfo      // null module -- never handed to a driver
    st.stage.vertex = true
    gp.pStages <- [st]
    unsafe {
        gp.pViewportState = addr(vp)
    }
    t |> success(gp.pViewportState != null, "opt-ptr field set before viewing")

    let vk1 = vk_view_create_unsafe(gp)
    t |> equal(int(vk1.stageCount), 1)          // stageCount = 0 falls back to length(pStages)
    t |> success(vk1.pViewportState != null, "the view carries the cloned viewport state")
    t |> success(gp.pViewportState == null, "vk_view_create_unsafe consumes the opt-ptr field")
    t |> success(gp._vk_view_pViewportState_present, "scratch clone is marked present")
    vk_view_destroy(gp)
    t |> success(!gp._vk_view_pViewportState_present, "vk_view_destroy releases the scratch clone")

    // the canary proper: a second view of the same struct no longer sees the state
    let vk2 = vk_view_create_unsafe(gp)
    t |> success(vk2.pViewportState == null, "a re-view sees the consumed field as absent")
    vk_view_destroy(gp)

    // count-from-length on the batch allocate-info shape
    let dsl_a : DescriptorSetLayout
    let dsl_b : DescriptorSetLayout
    var dsai = DescriptorSetAllocateInfo(pSetLayouts <- [dsl_a, dsl_b])
    let vdsai = vk_view_create_unsafe(dsai)
    t |> equal(int(vdsai.descriptorSetCount), 2)
    vk_view_destroy(dsai)

    // fixed-array member copy: blendConstants float[4] must land in the raw struct element-exact and in
    // order. Host-side and driver-independent, so it must pass on EVERY CI lane -- the marshal half of
    // test_pipeline_boost_structs' CONSTANT_COLOR oracle, whose GPU half llvmpipe fails driver-side.
    var cb : PipelineColorBlendStateCreateInfo
    cb.blendConstants = fixed_array(0.25f, 0.5f, 0.75f, 1.0f)
    let vcb = vk_view_create_unsafe(cb)
    t |> equal(vcb.blendConstants[0], 0.25f)
    t |> equal(vcb.blendConstants[1], 0.5f)
    t |> equal(vcb.blendConstants[2], 0.75f)
    t |> equal(vcb.blendConstants[3], 1.0f)
    vk_view_destroy(cb)
}
