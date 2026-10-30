# dasVulkan - the device feature sets the compute creators enable

**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** Companion to `ARCHITECTURE.md`;
a section is cited by its anchor.

## The 8/16-bit storage set {#storage-8-16}

`shaderInt16` is a core `VkPhysicalDeviceFeatures` bit, set on `f2.features` directly - it is
not in the Vulkan11 or Vulkan12 chain that carries `storageBuffer16BitAccess`,
`storageBuffer8BitAccess`, `shaderFloat16` and `shaderInt8`. It belongs with them all the same:
a shader declaring an `int16` SSBO member pulls the SPIR-V `Int16` capability, which needs
`shaderInt16`. Those five bits are one set, and the layer treats them as one.

`storage_8_16_supported` probes the whole set. Each `create_device_storage_8_16*` creator
enables every bit that probe reads, threading `VkPhysicalDeviceVulkan11Features` into
`VkPhysicalDeviceVulkan12Features` through the features2 overload and setting `shaderInt16` on
the core block. `cooperative_matrix2_supported` probes the same core `shaderInt16` bit, because
the cm2 device its name gates is a superset of this one.

The probe and the creators are two halves of one claim. A bit that one side names and the other
misses either fails `vkCreateDevice` or lets a kernel use a feature the device never enabled.

## The Vulkan 1.3 subgroup pair {#subgroup-pair}

`subgroupSizeControl` and `computeFullSubgroups` are a pair: a compute pipeline may set
`REQUIRE_FULL_SUBGROUPS` only on a device where both are enabled. When the coopmat2 creator
chains that 1.3 struct it also carries `shaderIntegerDotProduct` on it: the feature was promoted
in 1.3, and a chain holding both the 1.3 struct and the standalone dot-product struct is invalid,
so the standalone struct serves only a device below 1.3.
`compute_full_subgroups_supported` reports the pair, returning false below API 1.3 before it
reads any feature bit, because the struct carrying them is 1.3 core.

Every optional feature block joins a creator's pNext chain only when its extension is enabled
and the device reports the bit: a struct for an extension the device did not enable fails
`vkCreateDevice`, so each creator queries first and chains second.

Every ladder creator that can enable the pair does so opportunistically: it queries the pair
first, and chains a `VkPhysicalDeviceVulkan13Features` onto the tail of its pNext chain only
when the device reports both bits, after whichever optional struct that creator chained last. A
device missing the pair degrades to the same device without it rather than failing to create.
That tail position is why the chaining is a per-creator ladder: only the creator knows which of
its own optional blocks is last. `create_device_storage_8_16_int_dot_coopmat2` runs one rung
past the pair: when the device reports `VK_NV_cooperative_matrix_decode_vector` it chains
`VkPhysicalDeviceCooperativeMatrixDecodeVectorFeaturesNV` after the 1.3 block, or after
whichever optional block is last when the pair is missing. That rung is the creator's tail, so
the next optional block chains after it. `create_device_coopmat_full_subgroups`, which predates the
ladder, is the other shape: it chains its `VkPhysicalDeviceVulkan13Features` unconditionally in
mid-chain and copies the reported bits into it - a 1.3 core struct, so the chain is legal
either way, and a missing pair leaves the bits off.

`create_compute_pipeline_full_subgroups` is the consumer: `create_compute_pipeline` with
`REQUIRE_FULL_SUBGROUPS` set on the stage, which lets the shader compiler drop its
partial-subgroup guards. A pipeline it returns is valid only on a device from a creator that
enabled the pair, and only for a workgroup width that is a multiple of the subgroup size.

`create_compute_pipeline_subgroup` is the pair's other consumer: it chains a
`VkPipelineShaderStageRequiredSubgroupSizeCreateInfo` on the stage, so a kernel written for one
width - a coopmat tile whose 128 threads own four fragments as four 32-lane subgroups - runs at
that width on a device whose default is another (RDNA runs 32 or 64 lanes and defaults to 64).
`subgroup_size_control_range` reports the widths a device admits and whether its compute stage
may pin at all, and `compute_subgroup_size_pinnable` asks for one width - a power of two inside
the range, the only widths `requiredSubgroupSize` admits; a pin outside the range, not a power of
two, or on a device without the pair, is an invalid pipeline create, so a caller probes first. The pin is the
pair's `subgroupSizeControl` bit, so every creator that enables the pair enables the pin. The
plain, specialized, full-subgroups and pinned creators are one private builder.
