options gen2
options _comment_hygiene = true
options indenting = 4

// Hand-written ergonomic top of the boost: selection helpers, block brackets,
// combined builders, and the composite creators whose CreateInfo the generator
// skips (instance/device/render-pass/shader-module/pipeline). Re-exports the
// generated boost so users `require vulkan/vulkan_boost` and get everything.

module vulkan_boost shared

require vulkan public
require vulkan/vulkan_runtime public
require vulkan/vulkan_ctors public
require vulkan/vulkan_handles public
require vulkan/vulkan_structs public
require vulkan/vulkan_commands public
require vulkan/vulkan_cmds public
require strings
require daslib/defer

// ===== selection =====

def public select_physical_device(instance : Instance) : VkPhysicalDevice {
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        panic("no Vulkan physical devices")
    }
    var devices : array<VkPhysicalDevice>
    devices |> resize(int(count))
    unsafe {
        vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, addr(devices[0]))
    }
    // prefer a discrete GPU
    var best = devices[0]
    for (d in devices) {
        var props : VkPhysicalDeviceProperties
        vkGetPhysicalDeviceProperties(d, props)
        if (props.deviceType == VkPhysicalDeviceType.DISCRETE_GPU) {
            best = d
            break
        }
    }
    return best
}

def public select_graphics_queue_family(phys : VkPhysicalDevice) : uint {
    var count = 0u
    vkGetPhysicalDeviceQueueFamilyProperties(phys, count, null)
    var props : array<VkQueueFamilyProperties>
    props |> resize(int(count))
    unsafe {
        vkGetPhysicalDeviceQueueFamilyProperties(phys, count, addr(props[0]))
    }
    for (i in range(int(count))) {
        if (props[i].queueFlags.graphics) {
            return uint(i)
        }
    }
    panic("no graphics queue family")
    return 0u
}

def public find_memory_type(phys : VkPhysicalDevice; type_bits : uint; want : VkMemoryPropertyFlags) : uint {
    var mp : VkPhysicalDeviceMemoryProperties
    vkGetPhysicalDeviceMemoryProperties(phys, mp)
    for (i in range(int(mp.memoryTypeCount))) {
        let supported = (type_bits & (1u << uint(i))) != 0u
        let has = (uint(mp.memoryTypes[i].propertyFlags) & uint(want)) == uint(want)
        if (supported && has) {
            return uint(i)
        }
    }
    panic("no suitable memory type")
    return 0u
}

// ===== composite creators (CreateInfo skipped by the generator) =====

//! is `name` advertised among the loader's available instance extensions?
def public instance_extension_available(name : string) : bool {
    var count = 0u
    if (vkEnumerateInstanceExtensionProperties("", count, null) != VkResult.SUCCESS || count == 0u) {
        return false
    }
    var props : array<VkExtensionProperties>
    props |> resize(int(count))
    let rc = vkEnumerateInstanceExtensionProperties("", count, unsafe(addr(props[0])))
    if (rc != VkResult.SUCCESS) {
        return false
    }
    var found = false
    for (p in props) {
        unsafe {
            if ("{reinterpret<string>(addr(p.extensionName[0]))}" == name) {
                found = true
                break
            }
        }
    }
    return found
}

//! is `name` advertised among `phys`'s available device extensions? Use this to gate an optional
//! device extension before enabling it (e.g. skip a VK_EXT_mesh_shader test on a driver -- such as an
//! older Mesa lavapipe -- that does not advertise it, rather than crashing vkCreateDevice).
def public device_extension_available(phys : VkPhysicalDevice; name : string) : bool {
    var count = 0u
    if (vkEnumerateDeviceExtensionProperties(phys, "", count, null) != VkResult.SUCCESS || count == 0u) {
        return false
    }
    var props : array<VkExtensionProperties>
    props |> resize(int(count))
    let rc = vkEnumerateDeviceExtensionProperties(phys, "", count, unsafe(addr(props[0])))
    if (rc != VkResult.SUCCESS) {
        return false
    }
    var found = false
    for (p in props) {
        unsafe {
            if ("{reinterpret<string>(addr(p.extensionName[0]))}" == name) {
                found = true
                break
            }
        }
    }
    return found
}

//! True iff ``phys`` advertises ``VK_EXT_mesh_shader`` AND reports the ``meshShader`` + ``taskShader``
//! feature bits (via ``vkGetPhysicalDeviceFeatures2``). The extension string alone is not enough — a
//! driver can list it yet report a bit as 0, still failing ``vkCreateDevice``. Gate mesh setup with this.
def public mesh_shader_supported(phys : VkPhysicalDevice) : bool {
    if (!device_extension_available(phys, "VK_EXT_mesh_shader")) {
        return false
    }
    var mf = VkPhysicalDeviceMeshShaderFeaturesEXT()   // sType pre-filled by the ctor
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(mf)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return mf.meshShader != 0u && mf.taskShader != 0u
}

def public create_instance(application_name : string; api_version : uint; extensions : array<string>) : Instance {
    var ai = VkApplicationInfo()
    ai.apiVersion = api_version
    if (application_name != "") {
        ai.pApplicationName = application_name
    }
    var ci = VkInstanceCreateInfo()
    unsafe {
        ci.pApplicationInfo = addr(ai)
    }
    // Portability drivers (MoltenVK on macOS) reject vkCreateInstance with ERROR_INCOMPATIBLE_DRIVER
    // unless VK_KHR_portability_enumeration is enabled and the matching create flag is set. Added
    // transparently when the loader advertises it, so the same user code runs unchanged everywhere.
    var exts <- [for (e in extensions); e]
    if (instance_extension_available("VK_KHR_portability_enumeration")) {
        if (find_index(exts, "VK_KHR_portability_enumeration") < 0) {
            exts |> push("VK_KHR_portability_enumeration")
        }
        ci.flags.enumerate_portability_khr = true
    }
    // array<string> is contiguous `const char*` pointers == const char* const* (what
    // Vulkan wants); ppEnabledExtensionNames is bound as void? so reinterpret to it.
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkInstance
    vk_check(vkCreateInstance(ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

def public create_instance(application_name : string = ""; api_version : uint = 0u) : Instance {
    let no_ext : array<string>
    return <- create_instance(application_name, api_version, no_ext)
}

// Portability devices (MoltenVK) advertise VK_KHR_portability_subset and the spec REQUIRES
// enabling it whenever advertised — vkCreateDevice fails otherwise. Appended transparently at
// every raw vkCreateDevice site (create_instance's portability_enumeration twin).
def private append_portability_subset(phys : VkPhysicalDevice; var exts : array<string>) {
    if (device_extension_available(phys, "VK_KHR_portability_subset") &&
            find_index(exts, "VK_KHR_portability_subset") < 0) {
        exts |> push("VK_KHR_portability_subset")
    }
}

def public create_device(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>) : Device {
    var no_features : VkPhysicalDeviceFeatures
    return <- create_device(phys, queue_family, extensions, no_features)
}

def public create_device(phys : VkPhysicalDevice; queue_family : uint) : Device {
    let no_ext : array<string>
    var no_features : VkPhysicalDeviceFeatures
    return <- create_device(phys, queue_family, no_ext, no_features)
}

//! Create a device with a caller-supplied ``VkPhysicalDeviceFeatures`` block — for opt-in features like
//! ``largePoints`` (so a vertex shader's ``gl_PointSize`` takes effect under POINT_LIST) or ``wideLines``.
//! Verify support via ``vkGetPhysicalDeviceFeatures`` first: an unsupported bit fails ``vkCreateDevice``.
def public create_device(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>; var features : VkPhysicalDeviceFeatures) : Device {
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(prio)
    }
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    unsafe {
        ci.pQueueCreateInfos = addr(qci)
        ci.pEnabledFeatures = addr(features)
    }
    var exts <- [for (e in extensions); e]
    append_portability_subset(phys, exts)
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

//! Create a device with Vulkan 1.3 features (``dynamicRendering``, ``synchronization2``, …). 1.3
//! promoted these to core but the device must still *opt in* — caller sets the flags, this threads
//! ``VkPhysicalDeviceVulkan13Features`` through pNext. Instance must be api 1.3 or vkCreateDevice fails.
def public create_device(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>;
                         var features13 : VkPhysicalDeviceVulkan13Features) : Device {
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(prio)
    }
    features13.sType = VkStructureType.PHYSICAL_DEVICE_VULKAN_1_3_FEATURES
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    unsafe {
        ci.pQueueCreateInfos = addr(qci)
        ci.pNext = addr<void?>(features13)
    }
    var exts <- [for (e in extensions); e]
    append_portability_subset(phys, exts)
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

//! Create a device with Vulkan 1.2 features (``descriptorIndexing``, ``runtimeDescriptorArray``,
//! ``shaderSampledImageArrayNonUniformIndexing``, …). VK_EXT_descriptor_indexing is 1.2 core, so a 1.2+
//! instance opts into bindless through this pNext struct with no extension string; api < 1.2 fails.
def public create_device(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>;
                         var features12 : VkPhysicalDeviceVulkan12Features) : Device {
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(prio)
    }
    features12.sType = VkStructureType.PHYSICAL_DEVICE_VULKAN_1_2_FEATURES
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    unsafe {
        ci.pQueueCreateInfos = addr(qci)
        ci.pNext = addr<void?>(features12)
    }
    var exts <- [for (e in extensions); e]
    append_portability_subset(phys, exts)
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

//! Create a device from a caller-built ``VkPhysicalDeviceFeatures2`` pNext CHAIN — the multi-struct
//! escape hatch, since the single-struct overloads carry only one (16-bit Vulkan11Features + 8-bit
//! Vulkan12Features must go together). ``pEnabledFeatures`` stays null; ``features2.features`` is base.
def public create_device(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>;
                         var features2 : VkPhysicalDeviceFeatures2) : Device {
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(prio)
    }
    features2.sType = VkStructureType.PHYSICAL_DEVICE_FEATURES_2
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    unsafe {
        ci.pQueueCreateInfos = addr(qci)
        ci.pNext = addr<void?>(features2)
    }
    var exts <- [for (e in extensions); e]
    append_portability_subset(phys, exts)
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

//! The features2 overload with MULTIPLE queue families — one queue per listed (distinct) family.
//! The compute-tier creators use it to add a dedicated transfer queue beside the main one.
def public create_device(phys : VkPhysicalDevice; queue_families : array<uint>; extensions : array<string>;
                         var features2 : VkPhysicalDeviceFeatures2) : Device {
    // Vulkan requires each queueFamilyIndex at most once in pQueueCreateInfos, and an empty list
    // would leave pQueueCreateInfos dangling — assert rather than dedupe, so a caller bug surfaces
    assert(!empty(queue_families), "create_device: queue_families is empty")
    for (i in range(length(queue_families))) {
        for (j in range(i + 1, length(queue_families))) {
            assert(queue_families[i] != queue_families[j], "create_device: duplicate queue family")
        }
    }
    var prio = 1.0f
    var qcis : array<VkDeviceQueueCreateInfo>
    qcis |> resize(length(queue_families))
    for (qf, qc in queue_families, qcis) {
        qc = VkDeviceQueueCreateInfo()
        qc.queueFamilyIndex = qf
        qc.queueCount = 1u
        unsafe {
            qc.pQueuePriorities = addr(prio)
        }
    }
    features2.sType = VkStructureType.PHYSICAL_DEVICE_FEATURES_2
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = uint(length(queue_families))
    unsafe {
        ci.pQueueCreateInfos = addr(qcis[0])
        ci.pNext = addr<void?>(features2)
    }
    var exts <- [for (e in extensions); e]
    append_portability_subset(phys, exts)
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    delete qcis
    return <- b
}

//! True iff ``phys`` supports the 8/16-bit SSBO storage set the dasSpirv storage wave emits:
//! ``storageBuffer16BitAccess`` (Vulkan11Features) + ``storageBuffer8BitAccess`` + ``shaderFloat16`` +
//! ``shaderInt8`` (Vulkan12Features). Gate 8/16-bit kernels — old/software drivers can miss any bit.
def public storage_8_16_supported(phys : VkPhysicalDevice) : bool {
    var f11 = VkPhysicalDeviceVulkan11Features()
    var f12 = VkPhysicalDeviceVulkan12Features()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f11)
        f11.pNext = addr<void?>(f12)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return (f11.storageBuffer16BitAccess != 0u && f12.storageBuffer8BitAccess != 0u
        && f12.shaderFloat16 != 0u && f12.shaderInt8 != 0u)
}

//! Create a device with the 8/16-bit SSBO storage set enabled (the four bits storage_8_16_supported
//! probes), threaded as a Vulkan11Features -> Vulkan12Features chain through the features2 overload.
//! Pairs with dasSpirv's ``float16``/``int8`` SSBO members and ``unpack8``/``pack32``; gate first.
def public create_device_storage_8_16(phys : VkPhysicalDevice; queue_family : uint) : Device {
    let no_ext : array<string>
    return <- create_device_storage_8_16(phys, queue_family, no_ext)
}

def public create_device_storage_8_16(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>) : Device {
    var f11 = VkPhysicalDeviceVulkan11Features()
    var f12 = VkPhysicalDeviceVulkan12Features()
    f11.storageBuffer16BitAccess = 1u
    f12.storageBuffer8BitAccess = 1u
    f12.shaderFloat16 = 1u
    f12.shaderInt8 = 1u
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f11)
        f11.pNext = addr<void?>(f12)
    }
    return <- create_device(phys, queue_family, extensions, f2)
}

//! True iff ``phys`` advertises the cooperative-matrix feature (VK_KHR_cooperative_matrix — Tensor-Core
//! MMA); lavapipe and non-Tensor-Core GPUs miss it. Exact tile shapes come from
//! get_physical_device_cooperative_matrix_properties_k_h_r; every Tensor-Core GPU has the f16 16x16x16 tile.
def public cooperative_matrix_supported(phys : VkPhysicalDevice) : bool {
    var fcm = VkPhysicalDeviceCooperativeMatrixFeaturesKHR()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(fcm)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return fcm.cooperativeMatrix != 0u
}

//! Create a device with the cooperative-matrix feature + its prerequisites: storageBuffer16BitAccess +
//! shaderFloat16 (the f16 tiles) and vulkanMemoryModel (the emitted SPIR-V uses the Vulkan memory model),
//! plus the VK_KHR_cooperative_matrix extension. Gate with ``cooperative_matrix_supported``.
def public create_device_cooperative_matrix(phys : VkPhysicalDevice; queue_family : uint) : Device {
    var f11 = VkPhysicalDeviceVulkan11Features()
    var f12 = VkPhysicalDeviceVulkan12Features()
    var fcm = VkPhysicalDeviceCooperativeMatrixFeaturesKHR()
    f11.storageBuffer16BitAccess = 1u
    f12.shaderFloat16 = 1u
    f12.vulkanMemoryModel = 1u
    fcm.cooperativeMatrix = 1u
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f11)
        f11.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(fcm)
    }
    var ext <- ["VK_KHR_cooperative_matrix"]
    var dev <- create_device(phys, queue_family, ext, f2)
    delete ext
    return <- dev
}

//! True iff ``phys`` advertises VK_KHR_pipeline_executable_properties AND its ``pipelineExecutableInfo``
//! feature bit — drivers can list the extension with the feature off, and enabling both then fails vkCreateDevice.
def public pipeline_exec_props_supported(phys : VkPhysicalDevice) : bool {
    if (!device_extension_available(phys, "VK_KHR_pipeline_executable_properties")) return false
    var fpe = VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(fpe)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return fpe.pipelineExecutableInfo != 0u
}

//! create_device_cooperative_matrix PLUS the Vulkan 1.3 subgroup-size-control pair so pipelines can pin
//! ``requiredSubgroupSize``; each bit is enabled only when the device reports it (degrades to the plain
//! coopmat device). Instance must be api 1.3; opportunistically enables VK_KHR_pipeline_executable_properties.
def public create_device_coopmat_full_subgroups(phys : VkPhysicalDevice; queue_family : uint) : Device {
    var q13 = VkPhysicalDeviceVulkan13Features()
    var q2 = VkPhysicalDeviceFeatures2()
    unsafe {
        q2.pNext = addr<void?>(q13)
    }
    vkGetPhysicalDeviceFeatures2(phys, q2)
    var f11 = VkPhysicalDeviceVulkan11Features()
    var f12 = VkPhysicalDeviceVulkan12Features()
    var f13 = VkPhysicalDeviceVulkan13Features()
    var fcm = VkPhysicalDeviceCooperativeMatrixFeaturesKHR()
    var fpe = VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR()
    f11.storageBuffer16BitAccess = 1u
    f12.shaderFloat16 = 1u
    f12.vulkanMemoryModel = 1u
    // optional 1.3 bits: request only what the queried features report
    f13.subgroupSizeControl = q13.subgroupSizeControl
    f13.computeFullSubgroups = q13.computeFullSubgroups
    fcm.cooperativeMatrix = 1u
    fpe.pipelineExecutableInfo = 1u
    let with_exec_props = pipeline_exec_props_supported(phys)
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f11)
        f11.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(f13)
        f13.pNext = addr<void?>(fcm)
        if (with_exec_props) {
            fcm.pNext = addr<void?>(fpe)
        }
    }
    var ext <- ["VK_KHR_cooperative_matrix"]
    if (with_exec_props) {
        ext |> push("VK_KHR_pipeline_executable_properties")
    }
    var dev <- create_device(phys, queue_family, ext, f2)
    delete ext
    return <- dev
}

//! The physical device's subgroup properties: lane count (``subgroupSize`` — 32 on NV, 32/64 on
//! AMD, 8 on lavapipe), the stages subgroup ops work in, and the supported operation classes.
def public subgroup_properties(phys : VkPhysicalDevice) : VkPhysicalDeviceSubgroupProperties {
    var sp = VkPhysicalDeviceSubgroupProperties()
    var p2 = VkPhysicalDeviceProperties2()
    unsafe {
        p2.pNext = addr<void?>(sp)
    }
    vkGetPhysicalDeviceProperties2(phys, p2)
    return sp
}

//! True iff compute-stage subgroups support the op classes dasSpirv emits: basic +
//! arithmetic (subgroupAdd/Min/Max) + shuffle (subgroupShuffleXor) + ballot
//! (subgroupBroadcastFirst). Gate subgroup kernels with this.
def public subgroup_compute_ops_supported(phys : VkPhysicalDevice) : bool {
    let sp = subgroup_properties(phys)
    return (sp.supportedStages.compute && sp.supportedOperations.basic && sp.supportedOperations.arithmetic
        && sp.supportedOperations.shuffle && sp.supportedOperations.ballot)
}

//! True iff ``phys`` supports ``shaderIntegerDotProduct`` (VK_KHR_shader_integer_dot_product,
//! core in Vulkan 1.3) — the packed OpSDot/OpUDot family dasSpirv's ``sdot4`` emits. Gate dp4a
//! kernels with this; software renderers and pre-1.3 drivers can miss it.
def public integer_dot_product_supported(phys : VkPhysicalDevice) : bool {
    var fdot = VkPhysicalDeviceShaderIntegerDotProductFeatures()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(fdot)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return fdot.shaderIntegerDotProduct != 0u
}

//! Create a device with the 8/16-bit SSBO storage set AND ``shaderIntegerDotProduct`` (a Vulkan11 ->
//! Vulkan12 -> ShaderIntegerDotProduct chain through features2); pre-1.3 devices get the
//! VK_KHR_shader_integer_dot_product string automatically. Pairs with ``sdot4``; gate with both probes.
def public create_device_storage_8_16_int_dot(phys : VkPhysicalDevice; queue_family : uint) : Device {
    var f11 = VkPhysicalDeviceVulkan11Features()
    var f12 = VkPhysicalDeviceVulkan12Features()
    var fdot = VkPhysicalDeviceShaderIntegerDotProductFeatures()
    f11.storageBuffer16BitAccess = 1u
    f12.storageBuffer8BitAccess = 1u
    f12.shaderFloat16 = 1u
    f12.shaderInt8 = 1u
    fdot.shaderIntegerDotProduct = 1u
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f11)
        f11.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(fdot)
    }
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(phys, props)
    var ext : array<string>
    if (props.apiVersion < make_api_version(1u, 3u, 0u)) {
        ext |> push("VK_KHR_shader_integer_dot_product")
    }
    // opportunistic, matching the coopmat twins: mapped-host imports (the .dlim mirror rail)
    // must not silently require a tensor-core device
    if (device_extension_available(phys, "VK_EXT_external_memory_host")) {
        ext |> push("VK_EXT_external_memory_host")
    }
    return <- create_device(phys, queue_family, ext, f2)
}

//! Superset of create_device_storage_8_16_int_dot that ALSO enables cooperative matrix, and OPPORTUNISTICALLY:
//! memory_priority + pageable (residency), timelineSemaphore, external_memory_host. ``transfer_family`` >= 0
//! adds a second queue (pair ``select_transfer_queue_family`` + a timeline semaphore to overlap uploads).
def private push_memory_management_extensions(var ext : array<string>; with_memprio, with_pageable, with_extmemhost : bool) {
    if (with_memprio) {
        ext |> push("VK_EXT_memory_priority")
    }
    if (with_pageable) {
        ext |> push("VK_EXT_pageable_device_local_memory")
    }
    if (with_extmemhost) {
        ext |> push("VK_EXT_external_memory_host")
    }
}

def public create_device_storage_8_16_int_dot_coopmat(phys : VkPhysicalDevice; queue_family : uint; transfer_family : int = -1) : Device {
    var q12 = VkPhysicalDeviceVulkan12Features()
    var qmp = VkPhysicalDeviceMemoryPriorityFeaturesEXT()
    var qpg = VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT()
    var q2 = VkPhysicalDeviceFeatures2()
    unsafe {
        q2.pNext = addr<void?>(q12)
        q12.pNext = addr<void?>(qmp)
        qmp.pNext = addr<void?>(qpg)
    }
    vkGetPhysicalDeviceFeatures2(phys, q2)
    let with_memprio = device_extension_available(phys, "VK_EXT_memory_priority") && qmp.memoryPriority != 0u
    let with_pageable = (with_memprio && device_extension_available(phys, "VK_EXT_pageable_device_local_memory")
        && qpg.pageableDeviceLocalMemory != 0u)
    let with_extmemhost = device_extension_available(phys, "VK_EXT_external_memory_host")
    var f11 = VkPhysicalDeviceVulkan11Features()
    var f12 = VkPhysicalDeviceVulkan12Features()
    var fdot = VkPhysicalDeviceShaderIntegerDotProductFeatures()
    var fcm = VkPhysicalDeviceCooperativeMatrixFeaturesKHR()
    var fmp = VkPhysicalDeviceMemoryPriorityFeaturesEXT()
    var fpg = VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT()
    f11.storageBuffer16BitAccess = 1u
    f12.storageBuffer8BitAccess = 1u
    f12.shaderFloat16 = 1u
    f12.shaderInt8 = 1u
    f12.vulkanMemoryModel = 1u
    f12.timelineSemaphore = q12.timelineSemaphore
    fdot.shaderIntegerDotProduct = 1u
    fcm.cooperativeMatrix = 1u
    fmp.memoryPriority = 1u
    fpg.pageableDeviceLocalMemory = 1u
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f11)
        f11.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(fdot)
        fdot.pNext = addr<void?>(fcm)
        if (with_memprio) {
            fcm.pNext = addr<void?>(fmp)
        }
        if (with_pageable) {
            fmp.pNext = addr<void?>(fpg)
        }
    }
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(phys, props)
    var ext <- ["VK_KHR_cooperative_matrix"]
    if (props.apiVersion < make_api_version(1u, 3u, 0u)) {
        ext |> push("VK_KHR_shader_integer_dot_product")
    }
    push_memory_management_extensions(ext, with_memprio, with_pageable, with_extmemhost)
    var fams <- [queue_family]
    if (transfer_family >= 0 && uint(transfer_family) != queue_family) {
        fams |> push(uint(transfer_family))
    }
    var dev <- create_device(phys, fams, ext, f2)
    delete fams
    delete ext
    return <- dev
}

//! True iff ``phys`` exposes VK_EXT_memory_priority with its feature bit — the exact gate the
//! opportunistic creator blocks use, so a caller may chain ``VkMemoryPriorityAllocateInfoEXT``
//! into its allocations exactly when a creator above enabled the extension.
def public memory_priority_supported(phys : VkPhysicalDevice) : bool {
    if (!device_extension_available(phys, "VK_EXT_memory_priority")) {
        return false
    }
    var qmp = VkPhysicalDeviceMemoryPriorityFeaturesEXT()
    var q2 = VkPhysicalDeviceFeatures2()
    unsafe {
        q2.pNext = addr<void?>(qmp)
    }
    vkGetPhysicalDeviceFeatures2(phys, q2)
    return qmp.memoryPriority != 0u
}

//! True iff ``phys`` can create the cm2 device: VK_NV_cooperative_matrix2 (+ its VK_KHR_cooperative_matrix
//! dependency), the cm2 feature set the dasSpirv rails emit (wg scope, flexible dims, tensor addressing, block
//! loads), and every bit create_device_storage_8_16_int_dot_coopmat2 enables (``cooperativeMatrix``, ``bufferDeviceAddress``).
def public cooperative_matrix2_supported(phys : VkPhysicalDevice) : bool {
    if (!device_extension_available(phys, "VK_NV_cooperative_matrix2")
            || !device_extension_available(phys, "VK_KHR_cooperative_matrix")) {
        return false
    }
    var f12 = VkPhysicalDeviceVulkan12Features()
    var fcm = VkPhysicalDeviceCooperativeMatrixFeaturesKHR()
    var fcm2 = VkPhysicalDeviceCooperativeMatrix2FeaturesNV()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(fcm)
        fcm.pNext = addr<void?>(fcm2)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    if (fcm.cooperativeMatrix == 0u || f12.bufferDeviceAddress == 0u) return false
    return (fcm2.cooperativeMatrixWorkgroupScope != 0u && fcm2.cooperativeMatrixFlexibleDimensions != 0u
        && fcm2.cooperativeMatrixTensorAddressing != 0u && fcm2.cooperativeMatrixBlockLoads != 0u)
}

//! True when the device also carries the cm2 flash-attention trio — reductions, conversions,
//! and per-element operations. ``create_device_storage_8_16_int_dot_coopmat2`` enables them
//! opportunistically; gate shaders using coopmatReduce / coopmatPerElement / use casts on this.
def public cooperative_matrix2_fa_supported(phys : VkPhysicalDevice) : bool {
    if (!cooperative_matrix2_supported(phys)) return false
    var fcm2 = VkPhysicalDeviceCooperativeMatrix2FeaturesNV()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(fcm2)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return (fcm2.cooperativeMatrixReductions != 0u && fcm2.cooperativeMatrixConversions != 0u
        && fcm2.cooperativeMatrixPerElementOperations != 0u)
}

//! The device's cm2 limits: workgroup-scope max workgroup size, max flexible dimension, and the
//! shared memory the driver reserves for workgroup-scope staging (subtract from the 48KB budget).
def public cooperative_matrix2_properties(phys : VkPhysicalDevice) : VkPhysicalDeviceCooperativeMatrix2PropertiesNV {
    var cp = VkPhysicalDeviceCooperativeMatrix2PropertiesNV()
    var p2 = VkPhysicalDeviceProperties2()
    unsafe {
        p2.pNext = addr<void?>(cp)
    }
    vkGetPhysicalDeviceProperties2(phys, p2)
    return cp
}

//! True iff ``phys`` has VK_NV_cooperative_vector with its base feature bit — the Tensor-Core
//! MATVEC path (f16, packed-s8, and fp8-matrix combos come from
//! get_physical_device_cooperative_vector_properties_n_v).
def public cooperative_vector_supported(phys : VkPhysicalDevice) : bool {
    if (!device_extension_available(phys, "VK_NV_cooperative_vector")) return false
    var fcv = VkPhysicalDeviceCooperativeVectorFeaturesNV()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(fcv)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return fcv.cooperativeVector != 0u
}

//! The first DEDICATED transfer queue family (transfer set, graphics+compute clear — the DMA copy
//! engines), or -1 when the device exposes none. Pair with a timeline semaphore to overlap uploads
//! with compute; fall back to the main queue on -1.
def public select_transfer_queue_family(phys : VkPhysicalDevice) : int {
    var qcount = 0u
    vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, null)
    if (qcount == 0u) return -1
    var qprops : array<VkQueueFamilyProperties>
    qprops |> resize(int(qcount))
    unsafe {
        vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, addr(qprops[0]))
    }
    var found = -1
    for (q, qi in qprops, count()) {
        if (found < 0 && q.queueFlags.transfer && !q.queueFlags.graphics && !q.queueFlags.compute) {
            found = qi
        }
    }
    delete qprops
    return found
}

//! Superset of create_device_storage_8_16_int_dot_coopmat that ALSO enables VK_NV_cooperative_matrix2 +
//! ``bufferDeviceAddress`` (gate with ``cooperative_matrix2_supported``), plus the same opportunistic set
//! (coopvec, residency levers, timelineSemaphore, external_memory_host) and ``transfer_family`` second queue.
def public create_device_storage_8_16_int_dot_coopmat2(phys : VkPhysicalDevice; queue_family : uint; transfer_family : int = -1) : Device {   // nolint:STYLE038 — linear feature/extension pNext-chain assembly, one hop per capability
    // query the optional feature bits first — a pNext struct may only join the chain when its
    // extension is actually enabled, so each optional block is gated on ext + reported bit
    var q12q = VkPhysicalDeviceVulkan12Features()
    var qcv = VkPhysicalDeviceCooperativeVectorFeaturesNV()
    var qmp = VkPhysicalDeviceMemoryPriorityFeaturesEXT()
    var qpg = VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT()
    var qcm2 = VkPhysicalDeviceCooperativeMatrix2FeaturesNV()
    var q2 = VkPhysicalDeviceFeatures2()
    unsafe {
        q2.pNext = addr<void?>(q12q)
        q12q.pNext = addr<void?>(qcv)
        qcv.pNext = addr<void?>(qmp)
        qmp.pNext = addr<void?>(qpg)
        qpg.pNext = addr<void?>(qcm2)
    }
    vkGetPhysicalDeviceFeatures2(phys, q2)
    let with_coopvec = device_extension_available(phys, "VK_NV_cooperative_vector") && qcv.cooperativeVector != 0u
    let with_memprio = device_extension_available(phys, "VK_EXT_memory_priority") && qmp.memoryPriority != 0u
    let with_pageable = (with_memprio && device_extension_available(phys, "VK_EXT_pageable_device_local_memory")
        && qpg.pageableDeviceLocalMemory != 0u)
    let with_extmemhost = device_extension_available(phys, "VK_EXT_external_memory_host")
    var f11 = VkPhysicalDeviceVulkan11Features()
    var f12 = VkPhysicalDeviceVulkan12Features()
    var fdot = VkPhysicalDeviceShaderIntegerDotProductFeatures()
    var fcm = VkPhysicalDeviceCooperativeMatrixFeaturesKHR()
    var fcm2 = VkPhysicalDeviceCooperativeMatrix2FeaturesNV()
    var fcv = VkPhysicalDeviceCooperativeVectorFeaturesNV()
    var fmp = VkPhysicalDeviceMemoryPriorityFeaturesEXT()
    var fpg = VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT()
    f11.storageBuffer16BitAccess = 1u
    f12.storageBuffer8BitAccess = 1u
    f12.shaderFloat16 = 1u
    f12.shaderInt8 = 1u
    f12.vulkanMemoryModel = 1u
    f12.bufferDeviceAddress = 1u
    f12.timelineSemaphore = q12q.timelineSemaphore
    fdot.shaderIntegerDotProduct = 1u
    fcm.cooperativeMatrix = 1u
    fcm2.cooperativeMatrixWorkgroupScope = 1u
    fcm2.cooperativeMatrixFlexibleDimensions = 1u
    fcm2.cooperativeMatrixTensorAddressing = 1u
    fcm2.cooperativeMatrixBlockLoads = 1u
    // the flash-attention trio (reduce combine fns, use conversions, per-element callbacks):
    // opportunistic — enabled when reported, gate shader use with cooperative_matrix2_fa_supported
    fcm2.cooperativeMatrixReductions = qcm2.cooperativeMatrixReductions
    fcm2.cooperativeMatrixConversions = qcm2.cooperativeMatrixConversions
    fcm2.cooperativeMatrixPerElementOperations = qcm2.cooperativeMatrixPerElementOperations
    fcv.cooperativeVector = 1u
    fmp.memoryPriority = 1u
    fpg.pageableDeviceLocalMemory = 1u
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f11)
        f11.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(fdot)
        fdot.pNext = addr<void?>(fcm)
        fcm.pNext = addr<void?>(fcm2)
        if (with_coopvec) {
            fcm2.pNext = addr<void?>(fcv)
        }
        if (with_memprio) {
            if (with_coopvec) {
                fcv.pNext = addr<void?>(fmp)
            } else {
                fcm2.pNext = addr<void?>(fmp)
            }
        }
        if (with_pageable) {
            fmp.pNext = addr<void?>(fpg)
        }
    }
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(phys, props)
    var ext <- ["VK_KHR_cooperative_matrix", "VK_NV_cooperative_matrix2"]
    if (props.apiVersion < make_api_version(1u, 3u, 0u)) {
        ext |> push("VK_KHR_shader_integer_dot_product")
    }
    if (with_coopvec) {
        ext |> push("VK_NV_cooperative_vector")
    }
    push_memory_management_extensions(ext, with_memprio, with_pageable, with_extmemhost)
    var fams <- [queue_family]
    if (transfer_family >= 0 && uint(transfer_family) != queue_family) {
        fams |> push(uint(transfer_family))
    }
    var dev <- create_device(phys, fams, ext, f2)
    delete fams
    delete ext
    return <- dev
}

//! True iff ``phys`` reports the core-1.2 timelineSemaphore feature — the compute-tier creators
//! enable it when present; gate transfer-queue overlap schemes on this.
def public timeline_semaphore_supported(phys : VkPhysicalDevice) : bool {
    var q12 = VkPhysicalDeviceVulkan12Features()
    var q2 = VkPhysicalDeviceFeatures2()
    unsafe {
        q2.pNext = addr<void?>(q12)
    }
    vkGetPhysicalDeviceFeatures2(phys, q2)
    return q12.timelineSemaphore != 0u
}

//! True iff ``phys`` exposes VK_EXT_external_memory_host (the compute-tier creators enable it
//! opportunistically) — host-pointer imports (e.g. a file mapping) become possible.
def public external_memory_host_supported(phys : VkPhysicalDevice) : bool
    => device_extension_available(phys, "VK_EXT_external_memory_host")

//! ``minImportedHostPointerAlignment`` for host-pointer imports, or 0 when unsupported.
def public external_memory_host_min_alignment(phys : VkPhysicalDevice) : uint64 {
    if (!external_memory_host_supported(phys)) {
        return 0ul
    }
    var emh = VkPhysicalDeviceExternalMemoryHostPropertiesEXT()
    var p2 = VkPhysicalDeviceProperties2()
    unsafe {
        p2.pNext = addr<void?>(emh)
    }
    vkGetPhysicalDeviceProperties2(phys, p2)
    return emh.minImportedHostPointerAlignment
}

//!@nolint
//! Create a device with the bindless feature set: ``descriptorIndexing`` plus
//! ``shaderSampledImageArrayNonUniformIndexing`` (index a ``sampler2D[N]`` with ``nonuniformEXT``), via
//! the 1.2-core features12 path. **``runtimeDescriptorArray`` deliberately NOT enabled** — use features12.
//! The GPU regression tests under ``tests/integration/test_bindless_*.das`` and
//! ``test_nonuniform_ext.das`` drive this entry point.
def public create_device_descriptor_indexing(phys : VkPhysicalDevice; queue_family : uint) : Device {
    let no_ext : array<string>
    var f12 : VkPhysicalDeviceVulkan12Features
    f12.descriptorIndexing = 1u
    f12.shaderSampledImageArrayNonUniformIndexing = 1u
    return <- create_device(phys, queue_family, no_ext, f12)
}

//!@nolint
//! Create a device with the GPU-driven-rendering set: ``multiDrawIndirect`` (>1 draw per
//! ``vkCmdDrawIndirect``) on the base ``VkPhysicalDeviceFeatures``, plus ``shaderDrawParameters``
//! (``gl_DrawID`` / BuiltIn DrawIndex) via Vulkan11Features — 1.1 core, so no extension string.
//! Exercised by ``tests/integration/test_gl_draw_id.das``.
def public create_device_draw_parameters(phys : VkPhysicalDevice; queue_family : uint) : Device {
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(prio)
    }
    var base : VkPhysicalDeviceFeatures
    base.multiDrawIndirect = 1u
    var f11 : VkPhysicalDeviceVulkan11Features
    f11.sType = VkStructureType.PHYSICAL_DEVICE_VULKAN_1_1_FEATURES
    f11.shaderDrawParameters = 1u
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    unsafe {
        ci.pQueueCreateInfos = addr(qci)
        ci.pEnabledFeatures = addr(base)
        ci.pNext = addr<void?>(f11)
    }
    var exts : array<string>
    append_portability_subset(phys, exts)
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

//! Create a device with ``VK_EXT_mesh_shader``: caller sets the flags (``mf.meshShader``,
//! ``mf.taskShader``) and MUST also pass the ``"VK_EXT_mesh_shader"`` string in ``extensions`` — the
//! flags alone are not enough. Instance must be api >= 1.2 (SPIR-V 1.4); verify the bits or create fails.
def public create_device(phys : VkPhysicalDevice; queue_family : uint; extensions : array<string>;
                         var meshFeatures : VkPhysicalDeviceMeshShaderFeaturesEXT) : Device {
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    unsafe {
        qci.pQueuePriorities = addr(prio)
    }
    meshFeatures.sType = VkStructureType.PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    unsafe {
        ci.pQueueCreateInfos = addr(qci)
        ci.pNext = addr<void?>(meshFeatures)
    }
    var exts <- [for (e in extensions); e]
    append_portability_subset(phys, exts)
    if (!empty(exts)) {
        ci.enabledExtensionCount = uint(length(exts))
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

//! Create a device with the mesh-shader pipeline enabled: ``meshShader`` + ``taskShader`` features plus
//! the ``VK_EXT_mesh_shader`` device-extension string, in one call. This is the entry point Tutorial 13
//! and its pixel-oracle (``tutorials/13_mesh/test_mesh.das``) use. The instance must be at api version >= 1.2 (SPIR-V 1.4).
def public create_device_mesh_shader(phys : VkPhysicalDevice; queue_family : uint) : Device {
    if (!mesh_shader_supported(phys)) {
        panic("create_device_mesh_shader: device does not support the VK_EXT_mesh_shader meshShader+taskShader features")
    }
    var mf : VkPhysicalDeviceMeshShaderFeaturesEXT
    mf.meshShader = 1u
    mf.taskShader = 1u
    return <- create_device(phys, queue_family, ["VK_EXT_mesh_shader"], mf)
}

//! True iff ``phys`` advertises the three RT device extensions (``VK_KHR_acceleration_structure``,
//! ``VK_KHR_ray_tracing_pipeline``, ``VK_KHR_deferred_host_operations``) AND reports the matching
//! feature bits plus ``bufferDeviceAddress``. Gate RT setup — lavapipe fails this, so RT tests skip.
def public rt_supported(phys : VkPhysicalDevice) : bool {
    if (!device_extension_available(phys, "VK_KHR_acceleration_structure") ||
            !device_extension_available(phys, "VK_KHR_ray_tracing_pipeline") ||
            !device_extension_available(phys, "VK_KHR_deferred_host_operations")) {
        return false
    }
    var f12 = VkPhysicalDeviceVulkan12Features()
    var asf = VkPhysicalDeviceAccelerationStructureFeaturesKHR()
    var rtf = VkPhysicalDeviceRayTracingPipelineFeaturesKHR()
    var f2 = VkPhysicalDeviceFeatures2()
    unsafe {
        f2.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(asf)
        asf.pNext = addr<void?>(rtf)
    }
    vkGetPhysicalDeviceFeatures2(phys, f2)
    return f12.bufferDeviceAddress != 0u && asf.accelerationStructure != 0u && rtf.rayTracingPipeline != 0u
}

//!@nolint
//! Create a device with the hardware ray-tracing stack in one call: ``bufferDeviceAddress`` (1.2 core),
//! ``accelerationStructure`` and ``rayTracingPipeline`` through pNext, plus the three extension strings.
//! Instance must be api >= 1.2 (RT shaders are SPIR-V 1.4). Gate with rt_supported first.
//! Pairs with the [vulkan_raygen_shader] / [vulkan_miss_shader] / [vulkan_closest_hit_shader]
//! annotations, build_blas / build_tlas, build_shader_binding_table, and cmd_trace_rays_k_h_r.
def public create_device_ray_tracing(phys : VkPhysicalDevice; queue_family : uint) : Device {
    let no_extra : array<string>
    return <- create_device_ray_tracing(phys, queue_family, no_extra)
}

//! create_device_ray_tracing with additional device extensions on top of the RT set (a windowed
//! RT app passes ``["VK_KHR_swapchain"]``).
def public create_device_ray_tracing(phys : VkPhysicalDevice; queue_family : uint; extra_extensions : array<string>) : Device {
    if (!rt_supported(phys)) {
        panic("create_device_ray_tracing: device does not support VK_KHR_ray_tracing_pipeline + VK_KHR_acceleration_structure + bufferDeviceAddress")
    }
    var prio = 1.0f
    var qci = VkDeviceQueueCreateInfo()
    qci.queueFamilyIndex = queue_family
    qci.queueCount = 1u
    var f12 = VkPhysicalDeviceVulkan12Features()
    f12.bufferDeviceAddress = 1u
    var asf = VkPhysicalDeviceAccelerationStructureFeaturesKHR()
    asf.accelerationStructure = 1u
    var rtf = VkPhysicalDeviceRayTracingPipelineFeaturesKHR()
    rtf.rayTracingPipeline = 1u
    var extensions <- ["VK_KHR_acceleration_structure", "VK_KHR_ray_tracing_pipeline", "VK_KHR_deferred_host_operations"]
    extensions |> push_from(extra_extensions)
    append_portability_subset(phys, extensions)
    var ci = VkDeviceCreateInfo()
    ci.queueCreateInfoCount = 1u
    ci.enabledExtensionCount = uint(length(extensions))
    unsafe {
        qci.pQueuePriorities = addr(prio)
        ci.pQueueCreateInfos = addr(qci)
        ci.pNext = addr<void?>(f12)
        f12.pNext = addr<void?>(asf)
        asf.pNext = addr<void?>(rtf)
        ci.ppEnabledExtensionNames = addr<void?>(extensions[0])
    }
    var h : VkDevice
    vk_check(vkCreateDevice(phys, ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    return <- b
}

def public get_device_queue(device : Device; queue_family, index : uint) : VkQueue {
    var q : VkQueue
    vkGetDeviceQueue(boost_value_to_vk(device), queue_family, index, q)
    return q
}

def public create_shader_module(device : Device; var code : array<uint8>) : ShaderModule {
    var ci = VkShaderModuleCreateInfo()
    ci.codeSize = uint64(long_length(code))
    unsafe {
        ci.pCode = addr<uint?>(code[0])
    }
    var h : VkShaderModule
    vk_check(vkCreateShaderModule(boost_value_to_vk(device), ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! SPIR-V words overload — the form emitted by dasSpirv (`array<uint>`). codeSize is the byte
//! count (4 per word); pCode points straight at the word data, no uint8->uint reinterpret.
def public create_shader_module(device : Device; var code : array<uint>) : ShaderModule {
    var ci = VkShaderModuleCreateInfo()
    ci.codeSize = uint64(long_length(code) * 4l)
    unsafe {
        ci.pCode = addr(code[0])
    }
    var h : VkShaderModule
    vk_check(vkCreateShaderModule(boost_value_to_vk(device), ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

def public create_pipeline_layout(device : Device) : PipelineLayout {
    let ci = VkPipelineLayoutCreateInfo()
    var h : VkPipelineLayout
    vk_check(vkCreatePipelineLayout(boost_value_to_vk(device), ci, null, h), null)
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

def public create_render_pass_single_color(device : Device; format : VkFormat;
                                           final_layout : VkImageLayout = VkImageLayout.TRANSFER_SRC_OPTIMAL) : RenderPass {
    var attach : VkAttachmentDescription
    attach.format = format
    attach.samples._1 = true
    attach.loadOp = VkAttachmentLoadOp.CLEAR
    attach.storeOp = VkAttachmentStoreOp.STORE
    attach.stencilLoadOp = VkAttachmentLoadOp.DONT_CARE
    attach.stencilStoreOp = VkAttachmentStoreOp.DONT_CARE
    attach.initialLayout = VkImageLayout.UNDEFINED
    attach.finalLayout = final_layout
    var ref : VkAttachmentReference
    ref.layout = VkImageLayout.COLOR_ATTACHMENT_OPTIMAL
    var subpass : VkSubpassDescription
    subpass.pipelineBindPoint = VkPipelineBindPoint.GRAPHICS
    subpass.colorAttachmentCount = 1u
    var ci = VkRenderPassCreateInfo()
    ci.attachmentCount = 1u
    ci.subpassCount = 1u
    var h : VkRenderPass
    unsafe {
        subpass.pColorAttachments = addr(ref)
        ci.pAttachments = addr(attach)
        ci.pSubpasses = addr(subpass)
        vk_check(vkCreateRenderPass(boost_value_to_vk(device), ci, null, h), null)
    }
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Depth-only render pass for shadow mapping. One depth attachment: loadOp=CLEAR, storeOp=STORE,
//! finalLayout=DEPTH_STENCIL_READ_ONLY_OPTIMAL so the main pass samples it without a manual barrier.
//! Vertex-only pipelines (no fragment) are valid here — there are no color attachments to write.
def public create_render_pass_depth_only(device : Device; depth_format : VkFormat;
                                         final_layout : VkImageLayout = VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL) : RenderPass {
    var depth : VkAttachmentDescription
    depth.format = depth_format
    depth.samples._1 = true
    depth.loadOp = VkAttachmentLoadOp.CLEAR
    depth.storeOp = VkAttachmentStoreOp.STORE
    depth.stencilLoadOp = VkAttachmentLoadOp.DONT_CARE
    depth.stencilStoreOp = VkAttachmentStoreOp.DONT_CARE
    depth.initialLayout = VkImageLayout.UNDEFINED
    depth.finalLayout = final_layout

    var depth_ref : VkAttachmentReference
    depth_ref.attachment = 0u
    depth_ref.layout = VkImageLayout.DEPTH_STENCIL_ATTACHMENT_OPTIMAL

    var subpass : VkSubpassDescription
    subpass.pipelineBindPoint = VkPipelineBindPoint.GRAPHICS

    var ci = VkRenderPassCreateInfo()
    ci.attachmentCount = 1u
    ci.subpassCount = 1u
    var h : VkRenderPass
    unsafe {
        subpass.pDepthStencilAttachment = addr(depth_ref)
        ci.pAttachments = addr(depth)
        ci.pSubpasses = addr(subpass)
        vk_check(vkCreateRenderPass(boost_value_to_vk(device), ci, null, h), null)
    }
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Two-attachment render pass: color at index 0, depth at index 1. Color clears+stores like the
//! single-color path; depth clears+discards (no read after the pass). Framebuffers built against
//! this must pass `[color_view, depth_view]` in attachment order.
def public create_render_pass_color_depth(device : Device; color_format, depth_format : VkFormat;
                                          color_final_layout : VkImageLayout = VkImageLayout.TRANSFER_SRC_OPTIMAL) : RenderPass {
    var attaches : array<VkAttachmentDescription>
    var color : VkAttachmentDescription
    color.format = color_format
    color.samples._1 = true
    color.loadOp = VkAttachmentLoadOp.CLEAR
    color.storeOp = VkAttachmentStoreOp.STORE
    color.stencilLoadOp = VkAttachmentLoadOp.DONT_CARE
    color.stencilStoreOp = VkAttachmentStoreOp.DONT_CARE
    color.initialLayout = VkImageLayout.UNDEFINED
    color.finalLayout = color_final_layout
    attaches |> push(color)
    var depth : VkAttachmentDescription
    depth.format = depth_format
    depth.samples._1 = true
    depth.loadOp = VkAttachmentLoadOp.CLEAR
    depth.storeOp = VkAttachmentStoreOp.DONT_CARE
    depth.stencilLoadOp = VkAttachmentLoadOp.DONT_CARE
    depth.stencilStoreOp = VkAttachmentStoreOp.DONT_CARE
    depth.initialLayout = VkImageLayout.UNDEFINED
    depth.finalLayout = VkImageLayout.DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    attaches |> push(depth)

    var color_ref : VkAttachmentReference
    color_ref.attachment = 0u
    color_ref.layout = VkImageLayout.COLOR_ATTACHMENT_OPTIMAL
    var depth_ref : VkAttachmentReference
    depth_ref.attachment = 1u
    depth_ref.layout = VkImageLayout.DEPTH_STENCIL_ATTACHMENT_OPTIMAL

    var subpass : VkSubpassDescription
    subpass.pipelineBindPoint = VkPipelineBindPoint.GRAPHICS
    subpass.colorAttachmentCount = 1u
    var ci = VkRenderPassCreateInfo()
    ci.attachmentCount = 2u
    ci.subpassCount = 1u
    var h : VkRenderPass
    unsafe {
        subpass.pColorAttachments = addr(color_ref)
        subpass.pDepthStencilAttachment = addr(depth_ref)
        ci.pAttachments = addr(attaches[0])
        ci.pSubpasses = addr(subpass)
        vk_check(vkCreateRenderPass(boost_value_to_vk(device), ci, null, h), null)
    }
    delete attaches
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! `dynamic_viewport` leaves the viewport/scissor out of the pipeline (VIEWPORT+SCISSOR
//! dynamic state) so the pipeline survives swapchain resize -- set them per frame with
//! cmd_set_viewport / cmd_set_scissor (draw_frame does this when its dynamic_viewport is true).
def public create_graphics_pipeline_simple(device : Device; render_pass : RenderPass; layout : PipelineLayout;
                                           vert : ShaderModule; frag : ShaderModule; width, height : int; dynamic_viewport : bool = false) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.fragment = true
    stages[1].module_ = boost_value_to_vk(frag)
    stages[1].pName = "main"

    var vinput = VkPipelineVertexInputStateCreateInfo()
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport : VkViewport
    viewport.width = float(width)
    viewport.height = float(height)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(width)
    scissor.extent.height = uint(height)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true
    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states : array<VkDynamicState>
    var dynstate = VkPipelineDynamicStateCreateInfo()
    if (dynamic_viewport) {
        dyn_states |> push(VkDynamicState.VIEWPORT)
        dyn_states |> push(VkDynamicState.SCISSOR)
        dynstate.dynamicStateCount = uint(length(dyn_states))
    }

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        if (dynamic_viewport) {
            dynstate.pDynamicStates = addr(dyn_states[0])
            gp.pDynamicState = addr(dynstate)
        } else {
            vpstate.pViewports = addr(viewport)
            vpstate.pScissors = addr(scissor)
        }
        blend.pAttachments = addr(blend_att)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete dyn_states
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Graphics pipeline for VK_EXT_mesh_shader: a task -> mesh -> fragment chain with NO vertex-input and
//! NO input-assembly state (a mesh pipeline amplifies geometry on the GPU, so both are ignored/null);
//! rest matches create_graphics_pipeline_simple. Needs a mesh-shader device; draw cmd_draw_mesh_tasks_e_x_t.
def public create_mesh_pipeline(device : Device; render_pass : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 — linear CreateInfo assembly, the Vulkan pipeline-build shape
                                task : ShaderModule; mesh : ShaderModule; frag : ShaderModule;
                                width, height : int; dynamic_viewport : bool = false;
                                enable_depth : bool = false) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(3)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.task_ext = true
    stages[0].module_ = boost_value_to_vk(task)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.mesh_ext = true
    stages[1].module_ = boost_value_to_vk(mesh)
    stages[1].pName = "main"
    stages[2] = VkPipelineShaderStageCreateInfo()
    stages[2].stage.fragment = true
    stages[2].module_ = boost_value_to_vk(frag)
    stages[2].pName = "main"

    var viewport : VkViewport
    viewport.width = float(width)
    viewport.height = float(height)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(width)
    scissor.extent.height = uint(height)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true
    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    // depth test + write (LESS_OR_EQUAL) when the render pass carries a depth attachment; left off
    // (pDepthStencilState null) by default so the depthless Tutorial 13 path is unchanged.
    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    if (enable_depth) {
        depthstate.depthTestEnable = VK_TRUE
        depthstate.depthWriteEnable = VK_TRUE
        depthstate.depthCompareOp = VkCompareOp.LESS_OR_EQUAL
        depthstate.maxDepthBounds = 1.0f
    }

    var dyn_states : array<VkDynamicState>
    var dynstate = VkPipelineDynamicStateCreateInfo()
    if (dynamic_viewport) {
        dyn_states |> push(VkDynamicState.VIEWPORT)
        dyn_states |> push(VkDynamicState.SCISSOR)
        dynstate.dynamicStateCount = uint(length(dyn_states))
    }

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 3u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        if (dynamic_viewport) {
            dynstate.pDynamicStates = addr(dyn_states[0])
            gp.pDynamicState = addr(dynstate)
        } else {
            vpstate.pViewports = addr(viewport)
            vpstate.pScissors = addr(scissor)
        }
        blend.pAttachments = addr(blend_att)
        gp.pStages = addr(stages[0])
        // NB: pVertexInputState + pInputAssemblyState deliberately left null — ignored for mesh pipelines.
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        if (enable_depth) {
            gp.pDepthStencilState = addr(depthstate)
        }
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete dyn_states
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Graphics pipeline for textured / lit 3D meshes: vertex-input bindings + attributes (a real vertex
//! buffer, not `gl_VertexIndex` fabrication) and depth test + write on by default (LESS_OR_EQUAL) — the
//! render pass MUST have a depth attachment then. `dynamic_viewport=true` for swapchain-resize windowing.
def public create_graphics_pipeline_v3d(device : Device; render_pass : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 — linear CreateInfo assembly, the Vulkan pipeline-build shape
                                        vert : ShaderModule; frag : ShaderModule; width, height : int;
                                        var vertex_bindings : array<VkVertexInputBindingDescription>;
                                        var vertex_attributes : array<VkVertexInputAttributeDescription>;
                                        depth_test_enable : bool = true; depth_write_enable : bool = true;
                                        depth_compare_op : VkCompareOp = VkCompareOp.LESS_OR_EQUAL;
                                        dynamic_viewport : bool = false) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.fragment = true
    stages[1].module_ = boost_value_to_vk(frag)
    stages[1].pName = "main"

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = uint(length(vertex_bindings))
    vinput.vertexAttributeDescriptionCount = uint(length(vertex_attributes))
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport : VkViewport
    viewport.width = float(width)
    viewport.height = float(height)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(width)
    scissor.extent.height = uint(height)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.cullMode.back = true
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = depth_test_enable ? VK_TRUE : VK_FALSE
    depthstate.depthWriteEnable = depth_write_enable ? VK_TRUE : VK_FALSE
    depthstate.depthCompareOp = depth_compare_op
    depthstate.minDepthBounds = 0.0f
    depthstate.maxDepthBounds = 1.0f

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states : array<VkDynamicState>
    var dynstate = VkPipelineDynamicStateCreateInfo()
    if (dynamic_viewport) {
        dyn_states |> push(VkDynamicState.VIEWPORT)
        dyn_states |> push(VkDynamicState.SCISSOR)
        dynstate.dynamicStateCount = uint(length(dyn_states))
    }

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        if (!empty(vertex_bindings)) {
            vinput.pVertexBindingDescriptions = addr(vertex_bindings[0])
        }
        if (!empty(vertex_attributes)) {
            vinput.pVertexAttributeDescriptions = addr(vertex_attributes[0])
        }
        if (dynamic_viewport) {
            dynstate.pDynamicStates = addr(dyn_states[0])
            gp.pDynamicState = addr(dynstate)
        } else {
            vpstate.pViewports = addr(viewport)
            vpstate.pScissors = addr(scissor)
        }
        blend.pAttachments = addr(blend_att)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete dyn_states
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//!@nolint
//! `create_graphics_pipeline_v3d` parallel for the dynamic-rendering path (Vulkan 1.3 core). No
//! `RenderPass` — the pipeline carries a `VkPipelineRenderingCreateInfo` in pNext with the color/depth
//! format and view mask. `samples` flows into both the multisample state and the rendering info.
//! Tutorial 09 drives this with 4x MSAA.
def public create_graphics_pipeline_dyn(device : Device;   // nolint:STYLE038 — linear CreateInfo assembly, the Vulkan pipeline-build shape
                                        color_format, depth_format : VkFormat;
                                        samples : VkSampleCountFlags;
                                        layout : PipelineLayout;
                                        vert : ShaderModule; frag : ShaderModule; width, height : int;
                                        var vertex_bindings : array<VkVertexInputBindingDescription>;
                                        var vertex_attributes : array<VkVertexInputAttributeDescription>;
                                        depth_test_enable : bool = true; depth_write_enable : bool = true;
                                        depth_compare_op : VkCompareOp = VkCompareOp.LESS_OR_EQUAL;
                                        dynamic_viewport : bool = false) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.fragment = true
    stages[1].module_ = boost_value_to_vk(frag)
    stages[1].pName = "main"

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = uint(length(vertex_bindings))
    vinput.vertexAttributeDescriptionCount = uint(length(vertex_attributes))
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport : VkViewport
    viewport.width = float(width)
    viewport.height = float(height)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(width)
    scissor.extent.height = uint(height)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.cullMode.back = true
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples = samples

    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = depth_test_enable ? VK_TRUE : VK_FALSE
    depthstate.depthWriteEnable = depth_write_enable ? VK_TRUE : VK_FALSE
    depthstate.depthCompareOp = depth_compare_op
    depthstate.minDepthBounds = 0.0f
    depthstate.maxDepthBounds = 1.0f

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states : array<VkDynamicState>
    var dynstate = VkPipelineDynamicStateCreateInfo()
    if (dynamic_viewport) {
        dyn_states |> push(VkDynamicState.VIEWPORT)
        dyn_states |> push(VkDynamicState.SCISSOR)
        dynstate.dynamicStateCount = uint(length(dyn_states))
    }

    // dynamic-rendering pipeline carries the color/depth formats in a VkPipelineRenderingCreateInfo
    // attached via pNext (instead of a VkRenderPass handle). We allocate the color format slot
    // separately so addr() is valid through the call.
    var dyn_render = VkPipelineRenderingCreateInfo()
    var cf = color_format
    dyn_render.depthAttachmentFormat = depth_format
    dyn_render.colorAttachmentCount = 1u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    // no render_pass on the dynamic-rendering path; pNext carries the format info instead
    var h : VkPipeline
    unsafe {
        if (!empty(vertex_bindings)) {
            vinput.pVertexBindingDescriptions = addr(vertex_bindings[0])
        }
        if (!empty(vertex_attributes)) {
            vinput.pVertexAttributeDescriptions = addr(vertex_attributes[0])
        }
        if (dynamic_viewport) {
            dynstate.pDynamicStates = addr(dyn_states[0])
            gp.pDynamicState = addr(dynstate)
        } else {
            vpstate.pViewports = addr(viewport)
            vpstate.pScissors = addr(scissor)
        }
        blend.pAttachments = addr(blend_att)
        dyn_render.pColorAttachmentFormats = addr(cf)
        gp.pNext = addr<void?>(dyn_render)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete dyn_states
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! One specialization-constant override: ``id`` is the shader's SpecId (a dasSpirv ``@constant_id``
//! global or a ``local_size_*_id`` axis), ``value`` the 32-bit replacement as raw bits — float via
//! ``reinterpret<uint>``, bool as 0/1; every dasSpirv spec constant is 4 bytes.
struct public SpecConstantValue {
    id    : uint
    value : uint
}

//! create_compute_pipeline with specialization-constant overrides: each entry rewrites the shader's
//! ``OpSpecConstant`` default at pipeline-create time (``VkSpecializationInfo``), so one module backs
//! many pipelines — the [tune]-style GPU kernel-family primitive. Ownership as the base overload.
def public create_compute_pipeline(device : Device; layout : PipelineLayout; shader : ShaderModule;
                                   spec : array<SpecConstantValue>; entry : string = "main") : Pipeline {
    var entries : array<VkSpecializationMapEntry>
    var data : array<uint>
    entries |> reserve(length(spec))
    data |> reserve(length(spec))
    for (i in range(length(spec))) {
        var e = VkSpecializationMapEntry()
        e.constantID = spec[i].id
        e.offset = uint(i * 4)
        e.size = 4ul
        entries |> push(e)
        data |> push(spec[i].value)
    }
    var si = VkSpecializationInfo()
    si.mapEntryCount = uint(length(spec))
    si.dataSize = uint64(long_length(spec) * 4l)
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.compute = true
    stage.module_ = boost_value_to_vk(shader)
    stage.pName = entry
    var cp = VkComputePipelineCreateInfo()
    cp.layout = boost_value_to_vk(layout)
    var h : VkPipeline
    unsafe {
        if (!empty(spec)) {
            si.pMapEntries = addr(entries[0])
            si.pData = addr<void?>(data[0])
            stage.pSpecializationInfo = addr(si)
        }
        cp.stage = stage
        vk_check(vkCreateComputePipelines(boost_value_to_vk(device), null, 1u, addr(cp), null, addr(h)), null)
    }
    delete entries
    delete data
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! create a compute pipeline from a shader module + layout (owns the VkPipeline;
//! finalizes via vkDestroyPipeline). Wraps the batch vkCreateComputePipelines at count 1.
def public create_compute_pipeline(device : Device; layout : PipelineLayout; shader : ShaderModule; entry : string = "main") : Pipeline {
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.compute = true
    stage.module_ = boost_value_to_vk(shader)
    stage.pName = entry
    var cp = VkComputePipelineCreateInfo()
    cp.stage = stage
    cp.layout = boost_value_to_vk(layout)
    var h : VkPipeline
    unsafe {
        vk_check(vkCreateComputePipelines(boost_value_to_vk(device), null, 1u, addr(cp), null, addr(h)), null)
    }
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Record a single-image pipeline barrier that transitions `image` from `old_layout` to `new_layout`,
//! with the given access masks and pipeline stages (color aspect, 1 mip, 1 layer). The building block
//! for storage-image / transfer layout moves; used by compute_to_storage_image below.
def public transition_image(cmd : CommandBuffer; image : Image; old_layout, new_layout : VkImageLayout;
                            src_access, dst_access : VkAccessFlags; src_stage, dst_stage : VkPipelineStageFlags) {
    var barrier = ImageMemoryBarrier(srcAccessMask = src_access, dstAccessMask = dst_access,
        oldLayout = old_layout, newLayout = new_layout, srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, image = weak_copy(image))
    barrier.subresourceRange.aspectMask.color = true
    barrier.subresourceRange.levelCount = 1u
    barrier.subresourceRange.layerCount = 1u
    let no_mem : array<MemoryBarrier>
    let no_buf : array<BufferMemoryBarrier>
    var imgs : array<ImageMemoryBarrier>
    imgs |> emplace(barrier)
    let no_dep : VkDependencyFlags
    cmd_pipeline_barrier(cmd, src_stage, dst_stage, no_dep, no_mem, no_buf, imgs)
}

//! Depth-aspect counterpart of `transition_image` — same barrier mechanics, aspectMask `depth = true`.
//! For depth transitions the render-pass finalLayout doesn't cover (e.g. sampling a shadow map in pass
//! 2). Stencil-combined formats need the stencil bit too: use `transition_image_aspect` below.
def public transition_depth_image(cmd : CommandBuffer; image : Image; old_layout, new_layout : VkImageLayout;
                                  src_access, dst_access : VkAccessFlags; src_stage, dst_stage : VkPipelineStageFlags) {
    var aspect : VkImageAspectFlags
    aspect.depth = true
    transition_image_aspect(cmd, image, old_layout, new_layout, src_access, dst_access, src_stage, dst_stage, aspect)
}

//! Fully general `transition_image` — caller supplies the `aspectMask` (stencil, combined
//! depth+stencil, anything the two convenience helpers miss). `levelCount = layerCount = 1`; for mip
//! chains or array layers drop to a raw `cmd_pipeline_barrier`.
def public transition_image_aspect(cmd : CommandBuffer; image : Image; old_layout, new_layout : VkImageLayout;
                                   src_access, dst_access : VkAccessFlags; src_stage, dst_stage : VkPipelineStageFlags;
                                   aspect : VkImageAspectFlags) {
    var barrier = ImageMemoryBarrier(srcAccessMask = src_access, dstAccessMask = dst_access,
        oldLayout = old_layout, newLayout = new_layout, srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, image = weak_copy(image))
    barrier.subresourceRange.aspectMask = aspect
    barrier.subresourceRange.levelCount = 1u
    barrier.subresourceRange.layerCount = 1u
    let no_mem : array<MemoryBarrier>
    let no_buf : array<BufferMemoryBarrier>
    var imgs : array<ImageMemoryBarrier>
    imgs |> emplace(barrier)
    let no_dep : VkDependencyFlags
    cmd_pipeline_barrier(cmd, src_stage, dst_stage, no_dep, no_mem, no_buf, imgs)
}

def private set_extent_2d(var ici : ImageCreateInfo; width, height : int) {
    ici.extent.width = uint(width)
    ici.extent.height = uint(height)
    ici.extent.depth = 1u
}

//!@nolint
//! Run a compute shader that writes an Rgba8 storage image (set=0/binding=0, 8x8 local size) and return
//! the w*h RGBA8 pixels. One-shot: own instance/device/pipeline/pool; w,h must be positive multiples of 8.
//! `srgb_encode` blits via R8G8B8A8_SRGB for display-ready bytes — storage writes do NOT gamma-encode.
//! For the canonical authoring side see dasSpirv's image2D + imageStore — dasVulkan's storage-image
//! tests and the Mandelbrot tutorial are worked examples.
def public compute_to_storage_image(var words : array<uint>; w, h : int; srgb_encode : bool = false) : array<uint8> {   // nolint:STYLE038 — whole one-shot compute round-trip (device→dispatch→readback), stages inseparable
    assert(w > 0 && h > 0 && (w % 8) == 0 && (h % 8) == 0,
        "compute_to_storage_image: w/h must be positive multiples of 8")
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan compute", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    // the storage image (STORAGE for the compute write + TRANSFER_SRC for the readback), device-local + view
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt, mipLevels = 1u, arrayLayers = 1u,
        tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    set_extent_2d(ici, w, h)
    ici.samples._1 = true
    ici.usage.storage = true
    ici.usage.transfer_src = true
    var inscope image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope imem <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(image), boost_value_to_vk(imem), 0ul), null)
    var vci = ImageViewCreateInfo(image = weak_copy(image), viewType = VkImageViewType._2D, format = fmt)
    vci.subresourceRange.aspectMask.color = true
    vci.subresourceRange.levelCount = 1u
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)

    // optional sRGB readback intermediate: when srgb_encode, the linear storage image is blitted into
    // this R8G8B8A8_SRGB image (hardware applies linear->sRGB at the blit) and THAT is read back. No
    // storage usage -- storage images don't sRGB-encode on write; TRANSFER_DST (blit target) + TRANSFER_SRC.
    let srgb_fmt = VkFormat.R8G8B8A8_SRGB
    var inscope srgb_image : Image
    var inscope srgb_mem : DeviceMemory
    if (srgb_encode) {
        var sici = ImageCreateInfo(imageType = VkImageType._2D, format = srgb_fmt, mipLevels = 1u, arrayLayers = 1u,
            tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
        set_extent_2d(sici, w, h)
        sici.samples._1 = true
        sici.usage.transfer_dst = true
        sici.usage.transfer_src = true
        srgb_image <- create_image(device, sici)
        var sreq : VkMemoryRequirements
        vkGetImageMemoryRequirements(dev, boost_value_to_vk(srgb_image), sreq)
        var swant : VkMemoryPropertyFlags
        swant.device_local = true
        let smai = MemoryAllocateInfo(allocationSize = sreq.size,
            memoryTypeIndex = find_memory_type(phys, sreq.memoryTypeBits, swant))
        srgb_mem <- allocate_memory(device, smai)
        vk_check(vkBindImageMemory(dev, boost_value_to_vk(srgb_image), boost_value_to_vk(srgb_mem), 0ul), null)
    }

    // descriptor set layout + pool: binding 0 = STORAGE_IMAGE, compute
    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u, descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    b0.stageFlags.compute = true
    dslci.pBindings |> emplace(b0)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    let psize = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    dpci.pPoolSizes |> push(psize)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }

    // bind the storage image view (layout GENERAL, no sampler)
    var write = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    let iinfo = DescriptorImageInfo(imageView = weak_copy(view), imageLayout = VkImageLayout.GENERAL)
    write.pImageInfo |> push(iinfo)
    var writes : array<WriteDescriptorSet>
    writes |> emplace(write)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // compute pipeline from the emitted blob
    var inscope shader <- create_shader_module(device, words)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var inscope pipe_layout <- create_pipeline_layout(device, plci)
    var inscope pipeline <- create_compute_pipeline(device, pipe_layout, shader)

    var inscope readback <- create_host_buffer(device, phys, uint64(w) * uint64(h) * 4ul)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var shader_write : VkAccessFlags
        shader_write.shader_write = true
        var to_read : VkAccessFlags
        to_read.transfer_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var comp : VkPipelineStageFlags
        comp.compute_shader = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        // UNDEFINED -> GENERAL for the storage-image write
        transition_image(cmd, image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
            none, shader_write, top, comp)
        cmd_bind_pipeline(cmd, pipeline, VkPipelineBindPoint.COMPUTE)
        var sets : array<DescriptorSet>
        sets |> push(vk_value_to_boost(raw_set))
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, pipe_layout, 0u, sets, no_dyn)
        cmd_dispatch(cmd, uint(w / 8), uint(h / 8), 1u)
        // GENERAL -> TRANSFER_SRC for the readback copy
        transition_image(cmd, image, VkImageLayout.GENERAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            shader_write, to_read, comp, xfer)
        if (srgb_encode) {
            // blit the linear UNORM storage image into the sRGB intermediate (hardware applies the
            // linear->sRGB encode at the format conversion), then read THAT back gamma-encoded.
            var transfer_write : VkAccessFlags
            transfer_write.transfer_write = true
            transition_image(cmd, srgb_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
                none, transfer_write, top, xfer)
            var region : VkImageBlit
            region.srcSubresource.aspectMask.color = true
            region.srcSubresource.layerCount = 1u
            region.srcOffsets[1].x = w
            region.srcOffsets[1].y = h
            region.srcOffsets[1].z = 1
            region.dstSubresource.aspectMask.color = true
            region.dstSubresource.layerCount = 1u
            region.dstOffsets[1].x = w
            region.dstOffsets[1].y = h
            region.dstOffsets[1].z = 1
            unsafe {
                vkCmdBlitImage(boost_value_to_vk(cmd),
                    boost_value_to_vk(image),      VkImageLayout.TRANSFER_SRC_OPTIMAL,
                    boost_value_to_vk(srgb_image), VkImageLayout.TRANSFER_DST_OPTIMAL,
                    1u, addr(region), VkFilter.NEAREST)
            }
            transition_image(cmd, srgb_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
                transfer_write, to_read, xfer, xfer)
            copy_image_to_buffer(cmd, srgb_image, readback, w, h)
        } else {
            copy_image_to_buffer(cmd, image, readback, w, h)
        }
    }
    map_memory_to_array(device, readback.memory, uint64(w) * uint64(h) * 4ul) $(m) {
        pixels := m
    }
    return <- pixels
}

// ===== combined builders =====

//! image + backing device memory + view, owned together (destroyed view->image->memory)
struct public OffscreenTarget {
    image : Image
    memory : DeviceMemory
    view : ImageView
}

def public finalize(var t : OffscreenTarget) {
    delete t.view
    delete t.image
    delete t.memory
}

def public build_offscreen_target(device : Device; phys : VkPhysicalDevice; width, height : int; format : VkFormat) : OffscreenTarget {
    var samples : VkSampleCountFlags
    samples._1 = true
    var extra : VkImageUsageFlags
    extra.transfer_src = true                   // base helper default: blittable to host readback
    return <- build_offscreen_target(device, phys, width, height, format, samples, extra)
}

//!@nolint
//! `build_offscreen_target` overload taking caller-supplied sample count + extra usage flags.
//! `color_attachment` is always set; `extra_usage` adds the rest — `transfer_src` for the
//! offscreen-readback default, `input_attachment` for subpass-input rails (tutorial 10), `sampled` for
//! post-processing chains (tutorial 11), `_4`/`_8` on `samples` for MSAA (tutorial 09).
//! MSAA images CANNOT carry `transfer_src` — the resolve target does.
def public build_offscreen_target(device : Device; phys : VkPhysicalDevice; width, height : int;
                                  format : VkFormat; samples : VkSampleCountFlags;
                                  extra_usage : VkImageUsageFlags) : OffscreenTarget {
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = format, mipLevels = 1u, arrayLayers = 1u,
        samples = samples, tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    set_extent_2d(ici, width, height)
    ici.usage = extra_usage
    ici.usage.color_attachment = true
    var inscope image <- create_image(device, ici)

    var req : VkMemoryRequirements
    vkGetImageMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(image), req)
    var want : VkMemoryPropertyFlags
    want.device_local = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindImageMemory(boost_value_to_vk(device), boost_value_to_vk(image), boost_value_to_vk(memory), 0ul), null)

    var vci = ImageViewCreateInfo(image = weak_copy(image), viewType = VkImageViewType._2D, format = format)
    vci.subresourceRange.aspectMask.color = true
    vci.subresourceRange.levelCount = 1u
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)

    var t = OffscreenTarget(image <- image, memory <- memory, view <- view)
    return <- t
}

//! depth image + backing memory + depth-aspect view, owned together — OffscreenTarget with
//! depth_stencil_attachment usage and a depth-only view aspect. Default `D32_SFLOAT` is one of the two
//! depth-attachment formats Vulkan guarantees on every device (the other is `D32_SFLOAT_S8_UINT`).
struct public OffscreenDepth {
    image : Image
    memory : DeviceMemory
    view : ImageView
}

def public finalize(var d : OffscreenDepth) {
    delete d.view
    delete d.image
    delete d.memory
}

def public build_offscreen_depth(device : Device; phys : VkPhysicalDevice; width, height : int;
                                 format : VkFormat = VkFormat.D32_SFLOAT) : OffscreenDepth {
    let extra : VkImageUsageFlags  // default: empty
    return <- build_offscreen_depth(device, phys, width, height, format, extra)
}

//! `build_offscreen_depth` overload taking caller-supplied extra usage flags. `depth_stencil_attachment`
//! is always set; `extra_usage` adds `sampled` (shadow maps read by a `sampler2DShadow` next pass),
//! `transfer_src` (depth readback), etc. Memory + view stay device-local D32_SFLOAT.
def public build_offscreen_depth(device : Device; phys : VkPhysicalDevice; width, height : int;
                                 format : VkFormat; extra_usage : VkImageUsageFlags) : OffscreenDepth {
    var samples : VkSampleCountFlags
    samples._1 = true
    return <- build_offscreen_depth(device, phys, width, height, format, extra_usage, samples)
}

//! `build_offscreen_depth` overload taking caller-supplied sample count, for MSAA depth attachments:
//! color and depth in one render pass MUST agree on `samples`, so a 4x color target needs a 4x depth
//! target. Takes both `extra_usage` and `samples`; the 5-arg overload defaults extras to empty.
def public build_offscreen_depth(device : Device; phys : VkPhysicalDevice; width, height : int;
                                 format : VkFormat; extra_usage : VkImageUsageFlags;
                                 samples : VkSampleCountFlags) : OffscreenDepth {
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = format, mipLevels = 1u, arrayLayers = 1u,
        samples = samples, tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    set_extent_2d(ici, width, height)
    ici.usage = extra_usage
    ici.usage.depth_stencil_attachment = true
    var inscope image <- create_image(device, ici)

    var req : VkMemoryRequirements
    vkGetImageMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(image), req)
    var want : VkMemoryPropertyFlags
    want.device_local = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindImageMemory(boost_value_to_vk(device), boost_value_to_vk(image), boost_value_to_vk(memory), 0ul), null)

    var vci = ImageViewCreateInfo(image = weak_copy(image), viewType = VkImageViewType._2D, format = format)
    vci.subresourceRange.aspectMask.depth = true
    // include the stencil aspect for combined depth+stencil formats -- omitting it on those leaves
    // the view invalid (VUID-VkImageViewCreateInfo-subresourceRange-01018).
    if (format == VkFormat.D16_UNORM_S8_UINT
            || format == VkFormat.D24_UNORM_S8_UINT
            || format == VkFormat.D32_SFLOAT_S8_UINT) {
        vci.subresourceRange.aspectMask.stencil = true
    }
    vci.subresourceRange.levelCount = 1u
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)

    var d = OffscreenDepth(image <- image, memory <- memory, view <- view)
    return <- d
}

//! host-visible buffer + backing memory, owned together
struct public HostBuffer {
    buffer : Buffer
    memory : DeviceMemory
    size : uint64
}

def public finalize(var hb : HostBuffer) {
    delete hb.buffer
    delete hb.memory
}

def public create_host_buffer(device : Device; phys : VkPhysicalDevice; size : uint64) : HostBuffer {
    var bci : BufferCreateInfo
    bci.size = size
    bci.usage.transfer_dst = true
    var inscope buffer <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(buffer), req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(buffer), boost_value_to_vk(memory), 0ul), null)
    var hb = HostBuffer(buffer <- buffer, memory <- memory, size = size)
    return <- hb
}

//! HostBuffer with caller-chosen usage flags (vertex/index/uniform/storage/transfer_src/...), where the
//! `(device, phys, size)` overload above hardcodes `transfer_dst` for the readback-buffer role.
//! Memory is still HOST_VISIBLE | HOST_COHERENT.
def public create_host_buffer(device : Device; phys : VkPhysicalDevice; size : uint64; usage : VkBufferUsageFlags) : HostBuffer {
    var bci = BufferCreateInfo(size = size, usage = usage)
    var inscope buffer <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(buffer), req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(buffer), boost_value_to_vk(memory), 0ul), null)
    var hb = HostBuffer(buffer <- buffer, memory <- memory, size = size)
    return <- hb
}

//! Typed upload of `array<TT>` into a HostBuffer: maps `length * sizeof<TT>` bytes, memcpys, unmaps.
//! Memory must be HOST_COHERENT (true for `create_host_buffer` allocations). No-op on an empty array.
def public upload_bytes(buf : HostBuffer; device : Device; var data : array<auto(TT)>) {
    if (empty(data)) return
    let nbytes = length(data) * typeinfo sizeof(type<TT>)
    assert(uint64(nbytes) <= buf.size, "upload_bytes: data exceeds destination HostBuffer.size")
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(buf.memory), 0ul, uint64(nbytes), mf, addr(mapped)), null)
        memcpy(mapped, addr<void?>(data[0]), nbytes)
    }
    vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(buf.memory))
}

//! Create a host-visible buffer sized to fit `data` and upload it in one call — "vertex/index buffer
//! from typed array". Panics on empty input: Vulkan requires `VkBufferCreateInfo.size > 0`
//! (VUID-VkBufferCreateInfo-size-00912), so an empty array can't produce a valid buffer.
def public create_host_buffer_from_bytes(device : Device; phys : VkPhysicalDevice;
                                         usage : VkBufferUsageFlags; var data : array<auto(TT)>) : HostBuffer {
    assert(!empty(data), "create_host_buffer_from_bytes: empty data - VkBuffer.size must be > 0")
    let nbytes = uint64(length(data) * typeinfo sizeof(type<TT>))
    var hb <- create_host_buffer(device, phys, nbytes, usage)
    hb |> upload_bytes(device, data)
    return <- hb
}

// ===== block brackets =====

//! allocate a one-shot primary command buffer, record the block, submit, wait idle
def public run_cmd_sync(device : Device; pool : CommandPool; queue : VkQueue; blk : block<(cmd : CommandBuffer) : void>) {
    var ai = VkCommandBufferAllocateInfo()
    ai.commandPool = boost_value_to_vk(pool)
    ai.level = VkCommandBufferLevel.PRIMARY
    ai.commandBufferCount = 1u
    var raw_cmd : VkCommandBuffer
    unsafe {
        vk_check(vkAllocateCommandBuffers(boost_value_to_vk(device), ai, addr(raw_cmd)), null)
    }
    let cmd = vk_value_to_boost(raw_cmd)   // non-owning wrapper (pool-freed below)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw_cmd, begin), null)
    invoke(blk, cmd)
    vk_check(vkEndCommandBuffer(raw_cmd), null)
    var submit = VkSubmitInfo()
    submit.commandBufferCount = 1u
    unsafe {
        submit.pCommandBuffers = addr(raw_cmd)
        vk_check(vkQueueSubmit(queue, 1u, addr(submit), null), null)
    }
    vk_check(vkQueueWaitIdle(queue), null)
    unsafe {
        vkFreeCommandBuffers(boost_value_to_vk(device), boost_value_to_vk(pool), 1u, addr(raw_cmd))
    }
}

//! begin/end a render pass around the block, clearing to `clear`
def public record_render_pass(cmd : CommandBuffer; render_pass : RenderPass; framebuffer : Framebuffer;
                              area : VkRect2D; clear : VkClearValue; blk : block<() : void>) {
    var rp = VkRenderPassBeginInfo()
    rp.renderPass = boost_value_to_vk(render_pass)
    rp.framebuffer = boost_value_to_vk(framebuffer)
    rp.renderArea = area
    rp.clearValueCount = 1u
    var c = clear
    let raw_cmd = boost_value_to_vk(cmd)
    unsafe {
        rp.pClearValues = addr(c)
        vkCmdBeginRenderPass(raw_cmd, rp, VkSubpassContents.INLINE)
    }
    invoke(blk)
    vkCmdEndRenderPass(raw_cmd)
}

//! multi-clear overload: one VkClearValue per attachment, in framebuffer attachment order — use with
//! `create_render_pass_color_depth` (clears = [color, depth]). Empty `clears` is always a caller bug:
//! CLEAR-loadOp attachments would read uninitialized values (VUID-VkRenderPassBeginInfo-clearValueCount-00902).
def public record_render_pass(cmd : CommandBuffer; render_pass : RenderPass; framebuffer : Framebuffer;
                              area : VkRect2D; var clears : array<VkClearValue>; blk : block<() : void>) {
    assert(!empty(clears), "record_render_pass multi-clear: clears must be non-empty (one per attachment)")
    var rp = VkRenderPassBeginInfo()
    rp.renderPass = boost_value_to_vk(render_pass)
    rp.framebuffer = boost_value_to_vk(framebuffer)
    rp.renderArea = area
    rp.clearValueCount = uint(length(clears))
    let raw_cmd = boost_value_to_vk(cmd)
    unsafe {
        rp.pClearValues = addr(clears[0])
        vkCmdBeginRenderPass(raw_cmd, rp, VkSubpassContents.INLINE)
    }
    invoke(blk)
    vkCmdEndRenderPass(raw_cmd)
}

//! Block-form dynamic-rendering wrapper (Vulkan 1.3 core): vkCmdBeginRendering before the block,
//! vkCmdEndRendering after. Replaces `record_render_pass`'s (render_pass + framebuffer) pair with an
//! inline VkRenderingInfo. The caller owns the boost `RenderingAttachmentInfo` values.
def public record_rendering(cmd : CommandBuffer; area : VkRect2D; layer_count : uint;
                            var color_attachments : array<RenderingAttachmentInfo>;
                            var depth_attachment : RenderingAttachmentInfo;
                            blk : block<() : void>) {
    var info = RenderingInfo(renderArea = area, layerCount = layer_count, viewMask = 0u,
        pColorAttachments <- color_attachments)
    unsafe {
        info.pDepthAttachment = addr(depth_attachment)
    }
    cmd_begin_rendering(cmd, info)
    invoke(blk)
    cmd_end_rendering(cmd)
}

//! Color-only `record_rendering` overload -- no depth/stencil attachment. Used by post-process
//! passes that write a fullscreen quad to an offscreen color target (tutorial 11).
def public record_rendering(cmd : CommandBuffer; area : VkRect2D; layer_count : uint;
                            var color_attachments : array<RenderingAttachmentInfo>;
                            blk : block<() : void>) {
    var info = RenderingInfo(renderArea = area, layerCount = layer_count, viewMask = 0u,
        pColorAttachments <- color_attachments)
    cmd_begin_rendering(cmd, info)
    invoke(blk)
    cmd_end_rendering(cmd)
}

//! Block-form device-memory mapping: maps the whole allocation (VK_WHOLE_SIZE), invokes the block with
//! the raw pointer, unmaps on return. **Memory must be HOST_VISIBLE | HOST_COHERENT** — this helper does
//! NOT vkFlushMappedMemoryRanges, so non-coherent memory you allocate yourself needs its own flush.
def public with_mapped_memory(device : Device; memory : DeviceMemory; blk : block<(ptr : void?) : void>) {
    var mapped : void? = null
    let flags : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(memory), 0ul, VK_WHOLE_SIZE, flags, addr(mapped)), null)
    }
    invoke(blk, mapped)
    vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(memory))
}

// Reusable CPU scratch for push-constant staging. Grows to the largest block seen and is never freed —
// a small per-program scratch reused every push, so no per-frame allocation in the record loop. Single-
// threaded command recording + the block below never re-enters, so a module global is safe.
var private g_push_staging : array<uint8>

//! Block-form std140 staging for push constants: sizes the CPU scratch to `size`, hands the block a raw
//! pointer into it, frees nothing (scratch is reused). The block writes each field at its std140 offset
//! then pushes, so the GPU gets std140 bytes — the raw daslang struct image can diverge from std140.
def public with_push_staging(size : uint; blk : block<(ptr : void?) : void>) {
    return if (size == 0u)
    if (int(size) > length(g_push_staging)) {
        g_push_staging |> resize(int(size))
    }
    unsafe {
        invoke(blk, addr<void?>(g_push_staging[0]))
    }
}

//! Write a single typed value at a byte offset into mapped device memory (one memcpy of sizeof<TT>), for
//! std140 field writes at compile-time-computed offsets. TT = any 32-bit scalar / vec / float4x4 /
//! float3x4 whose daslang storage matches std140 directly; float3x3 needs per-column writes, not this.
def public write_field(ptr : void?; offset : uint; var value : auto(TT) const) {
    unsafe {
        let dst = reinterpret<void?>(intptr(ptr) + uint64(offset))
        memcpy(dst, addr<void?>(value), typeinfo sizeof(type<TT>))
    }
}

//! map a device-memory range, expose it as an array<uint8>, unmap after the block
def public map_memory_to_array(device : Device; memory : DeviceMemory; size : uint64; blk : block<(data : array<uint8>) : void>) {
    var mapped : void? = null
    let flags : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(memory), 0ul, size, flags, addr(mapped)), null)
    }
    // array is int-indexed, so a >2GB mapping cannot be represented here at all — assert rather
    // than resize truncated and then memcpy the full width, which would run past the allocation
    assert(size <= uint64(0x7FFFFFFF), "map_memory_to_array: mapping exceeds the int-indexed array cap")
    var arr : array<uint8>
    arr |> resize(int64(size))
    unsafe {
        memcpy(addr<void?>(arr[0]), mapped, size)
    }
    vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(memory))
    invoke(blk, arr)
    delete arr
}

//! map a device-memory range, copy `count` elements of T into an owned array<T>, unmap.
//! The typed/copying readback — no block, no manual memcpy/reinterpret at the call site.
[unused_argument(elem)] def public read_memory(device : Device; memory : DeviceMemory; elem : type<auto(TT)>; count : int) : array<TT> {
    var out : array<TT>
    if (count <= 0) {
        return <- out
    }
    out |> resize(count)
    let nbytes = count * typeinfo sizeof(type<TT>)
    var mapped : void? = null
    let flags : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(memory), 0ul, uint64(nbytes), flags, addr(mapped)), null)
        memcpy(addr<void?>(out[0]), mapped, nbytes)
    }
    vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(memory))
    return <- out
}

// ===== conveniences =====

def public full_area(width, height : int) : VkRect2D {
    var r : VkRect2D
    r.extent.width = uint(width)
    r.extent.height = uint(height)
    return r
}

def public clear_color(r, g, b, a : float) : VkClearValue {
    var c : VkClearValue
    c.color.float32[0] = r
    c.color.float32[1] = g
    c.color.float32[2] = b
    c.color.float32[3] = a
    return c
}

//! `depth` clear in [0, 1] (1.0 is the "far" reset for the standard z-buffer convention).
def public clear_depth(depth : float = 1.0f; stencil : uint = 0u) : VkClearValue {
    var c : VkClearValue
    c.depthStencil.depth = depth
    c.depthStencil.stencil = stencil
    return c
}

//! Depth-compare ("shadow") sampler for `sampler2DShadow` + `textureCompare`. Vulkan's per-tap result
//! is `reference compareOp sampledDepth`, so `LESS` returns 1 = lit when the fragment is nearer the
//! light. LINEAR filtering PCFs the 2x2 footprint; CLAMP_TO_BORDER + white border = off-map is lit.
def public create_sampler_shadow(device : Device; compare_op : VkCompareOp = VkCompareOp.LESS) : Sampler {
    let sci = SamplerCreateInfo(magFilter = VkFilter.LINEAR, minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR, addressModeU = VkSamplerAddressMode.CLAMP_TO_BORDER,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_BORDER, addressModeW = VkSamplerAddressMode.CLAMP_TO_BORDER,
        borderColor = VkBorderColor.FLOAT_OPAQUE_WHITE, compareEnable = VK_TRUE, compareOp = compare_op)
    return <- create_sampler(device, sci)
}

//! bind a pipeline (defaults to the graphics bind point; pass COMPUTE for a compute pipeline)
def public cmd_bind_pipeline(cmd : CommandBuffer; pipeline : Pipeline; bind_point : VkPipelineBindPoint = VkPipelineBindPoint.GRAPHICS) {
    vkCmdBindPipeline(boost_value_to_vk(cmd), bind_point, boost_value_to_vk(pipeline))
}

//! draw (convenience over the generated cmd_draw: defaults instance/first to 0)
def public cmd_draw(cmd : CommandBuffer; vertex_count : uint; instance_count : uint = 1u) {
    vkCmdDraw(boost_value_to_vk(cmd), vertex_count, instance_count, 0u, 0u)
}

//! Bind one vertex buffer at the given binding slot (offset 0 by default). The generated
//! `cmd_bind_vertex_buffers` takes `array<Buffer>` + `array<uint64>` which forces a per-frame
//! allocation for single-binding draws; this overload avoids it.
def public cmd_bind_vertex_buffer(cmd : CommandBuffer; binding : uint; buffer : Buffer; offset : uint64 = 0ul) {
    var raw_buf = boost_value_to_vk(buffer)
    var off = offset
    unsafe {
        vkCmdBindVertexBuffers(boost_value_to_vk(cmd), binding, 1u, addr(raw_buf), addr(off))
    }
}

//!@nolint
//! Typed push-constant upload: `sizeof<TT>` bytes from `value` at `offset` for stages `stages`. TT's
//! daslang layout must match the shader's std430 push block (fine for scalar / vec4 / mat4, watch vec3
//! padding). Vulkan requires both `offset` and the resolved size to be multiples of 4
//! (VUID-vkCmdPushConstants-offset-00370 / VUID-vkCmdPushConstants-size-00369), so both are
//! asserted — a `bool` or `uint8` TT silently trips driver validation otherwise.
def public cmd_push_constants(cmd : CommandBuffer; layout : PipelineLayout; stages : VkShaderStageFlags;
                              offset : uint; var value : auto(TT) const) {
    let sz = uint(typeinfo sizeof(type<TT>))
    assert((offset & 3u) == 0u, "cmd_push_constants: offset must be a multiple of 4")
    assert((sz & 3u) == 0u, "cmd_push_constants: sizeof<TT> must be a multiple of 4")
    unsafe {
        vkCmdPushConstants(boost_value_to_vk(cmd), boost_value_to_vk(layout), stages,
            offset, sz, addr<void?>(value))
    }
}

def public copy_image_to_buffer(cmd : CommandBuffer; image : Image; buffer : HostBuffer; width, height : int) {
    var region : VkBufferImageCopy
    region.imageSubresource.aspectMask.color = true
    region.imageSubresource.layerCount = 1u
    region.imageExtent.width = uint(width)
    region.imageExtent.height = uint(height)
    region.imageExtent.depth = 1u
    unsafe {
        vkCmdCopyImageToBuffer(boost_value_to_vk(cmd), boost_value_to_vk(image), VkImageLayout.TRANSFER_SRC_OPTIMAL,
            boost_value_to_vk(buffer.buffer), 1u, addr(region))
    }
}

// ===== pipeline executable introspection (VK_KHR_pipeline_executable_properties) =====

def private exec_cstr(src : int8[VK_MAX_DESCRIPTION_SIZE]) : string {
    var tmp = src
    unsafe {
        return "{reinterpret<string>(addr(tmp[0]))}"
    }
}

def private blob_text(var blob : array<uint8>) : string {
    if (empty(blob)) {
        return ""
    }
    blob[length(blob) - 1] = uint8(0)   // last byte is the caller-reserved NUL slot, never content
    unsafe {
        return "{reinterpret<string>(addr(blob[0]))}"
    }
}

//! One-string VK_KHR_pipeline_executable_properties report for `pipeline`: per-executable statistics
//! (on NVIDIA: register count, binary size, ...) plus internal representations, `isText` payloads
//! (disassembly) verbatim. Needs the capture_* pipeline flags + a device with pipelineExecutableInfo.
def public dump_pipeline_executables(device : Device; pipeline : Pipeline) : string {
    var execs <- get_pipeline_executable_properties_k_h_r(device, PipelineInfoKHR(pipeline = weak_copy(pipeline)))
    let s = build_string() $(writer) {
        write(writer, "pipeline executables: {length(execs)}\n")
        for (ei in range(length(execs))) {
            write(writer, "== executable {ei}: {exec_cstr(execs[ei].name)} ==\n")
            write(writer, "  {exec_cstr(execs[ei].description)} (stages {uint(execs[ei].stages)}, subgroupSize {int(execs[ei].subgroupSize)})\n")
            let info = PipelineExecutableInfoKHR(pipeline = weak_copy(pipeline), executableIndex = uint(ei))
            var stats <- get_pipeline_executable_statistics_k_h_r(device, info)
            write(writer, "-- statistics ({length(stats)}) --\n")
            for (st in stats) {
                var v = ""
                if (st.format == VkPipelineExecutableStatisticFormatKHR.BOOL32_KHR) {
                    v = st.value.b32 != 0u ? "true" : "false"
                } elif (st.format == VkPipelineExecutableStatisticFormatKHR.INT64_KHR) {
                    v = "{st.value.i64}"
                } elif (st.format == VkPipelineExecutableStatisticFormatKHR.UINT64_KHR) {
                    // decimal via int64 when it fits; raw uint64 (prints 0x-hex) past INT64_MAX
                    v = st.value.u64 <= 0x7FFFFFFFFFFFFFFFl ? "{int64(st.value.u64)}" : "{st.value.u64}"
                } else {
                    v = "{st.value.f64}"
                }
                write(writer, "{exec_cstr(st.name)} = {v}    ({exec_cstr(st.description)})\n")
            }
            delete stats
            // two-call pattern round 2: the generated wrapper filled name/isText/dataSize with pData null;
            // allocate each pData at its reported dataSize and re-query to pull the actual bytes
            var reps <- get_pipeline_executable_internal_representations_k_h_r(device, info)
            write(writer, "-- internal representations ({length(reps)}) --\n")
            var blobs : array<array<uint8>>
            blobs |> reserve(length(reps))
            for (r in reps) {
                var b : array<uint8>
                if (r.dataSize > 0x7FFFFFF0ul) {
                    // int-overflow guard: can't resize() past INT_MAX — leave the blob empty so the fill call skips it
                    write(writer, "-- representation '{exec_cstr(r.name)}' too large ({int64(r.dataSize)} bytes), skipped --\n")
                } elif (int(r.dataSize) > 0) {
                    // text payloads reserve ONE extra byte for the terminating NUL (see blob_text)
                    b |> resize(int(r.dataSize) + (r.isText != 0u ? 1 : 0))
                }
                blobs |> emplace(b)
            }
            if (!empty(reps)) {
                var vkinfo = VkPipelineExecutableInfoKHR()
                vkinfo.pipeline = boost_value_to_vk(pipeline)
                vkinfo.executableIndex = uint(ei)
                var vkcount = uint(length(reps))
                unsafe {
                    for (ri in range(length(reps))) {
                        if (!empty(blobs[ri])) {
                            reps[ri].pData = addr<void?>(blobs[ri][0])
                            reps[ri].dataSize = uint64(long_length(blobs[ri]))   // tell the driver about the reserved byte
                        }
                    }
                    vk_check(vkGetPipelineExecutableInternalRepresentationsKHR(boost_value_to_vk(device), vkinfo, vkcount, addr(reps[0])), null)
                }
            }
            for (ri in range(length(reps))) {
                let is_text = reps[ri].isText != 0u
                // skipped-oversize reps keep the driver-reported size (pData null => driver rewrites it): no int() cast
                let ds = reps[ri].dataSize
                let ds_str = ds > 0x7FFFFFF0ul ? "{int64(ds)} (skipped)" : "{int(ds)}"
                write(writer, "[{ri}] {exec_cstr(reps[ri].name)} isText={is_text} dataSize={ds_str}    ({exec_cstr(reps[ri].description)})\n")
                if (is_text && !empty(blobs[ri])) {
                    write(writer, blob_text(blobs[ri]))
                    write(writer, "\n")
                }
            }
            delete blobs
            delete reps
        }
    }
    delete execs
    return s
}

// ===== hardware ray tracing rails (VK_KHR_ray_tracing_pipeline) =====

//! buffer + memory allocated with the DEVICE_ADDRESS flag + its GPU virtual address, owned
//! together. The building block for everything ray tracing references by VkDeviceAddress:
//! acceleration-structure storage, build scratch, build inputs, shader binding tables.
struct public AddressBuffer {
    buffer : Buffer
    memory : DeviceMemory
    size : uint64
    address : uint64
}

def public finalize(var ab : AddressBuffer) {
    delete ab.buffer
    delete ab.memory
}

//! Create a buffer + memory + device address, adding the ``shader_device_address`` usage bit and the
//! matching DEVICE_ADDRESS allocate flag (an address query without it is invalid). ``host_visible``
//! picks HOST_VISIBLE|HOST_COHERENT (SBTs, AS inputs) vs DEVICE_LOCAL. Needs ``bufferDeviceAddress``.
def public create_address_buffer(device : Device; phys : VkPhysicalDevice; size : uint64;
                                 usage : VkBufferUsageFlags; host_visible : bool) : AddressBuffer {
    var u = usage
    u.shader_device_address = true
    var bci = BufferCreateInfo(size = size, usage = u)
    var inscope buffer <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(buffer), req)
    var want : VkMemoryPropertyFlags
    if (host_visible) {
        want.host_visible = true
        want.host_coherent = true
    } else {
        want.device_local = true
    }
    var flags_info = VkMemoryAllocateFlagsInfo()
    flags_info.flags.device_address = true
    var mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    unsafe {
        mai.next = addr<void?>(flags_info)
    }
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(buffer), boost_value_to_vk(memory), 0ul), null)
    let bdai = BufferDeviceAddressInfo(buffer = weak_copy(buffer))
    let address = get_buffer_device_address(device, bdai)
    var ab = AddressBuffer(buffer <- buffer, memory <- memory, size = size, address = address)
    return <- ab
}

//! Typed upload of `array<TT>` into an AddressBuffer (host_visible allocations only) — same
//! contract as the HostBuffer overload.
def public upload_bytes(buf : AddressBuffer; device : Device; var data : array<auto(TT)>) {
    if (empty(data)) {
        return
    }
    let nbytes = length(data) * typeinfo sizeof(type<TT>)
    assert(uint64(nbytes) <= buf.size, "upload_bytes: data exceeds destination AddressBuffer.size")
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(buf.memory), 0ul, uint64(nbytes), mf, addr(mapped)), null)
        memcpy(mapped, addr<void?>(data[0]), nbytes)
    }
    vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(buf.memory))
}

//! Query the ray-tracing pipeline properties (the shader-group handle size / alignment rules the
//! SBT layout needs) via the ``vkGetPhysicalDeviceProperties2`` pNext chain.
def public get_ray_tracing_properties(phys : VkPhysicalDevice) : VkPhysicalDeviceRayTracingPipelinePropertiesKHR {
    var rtp = VkPhysicalDeviceRayTracingPipelinePropertiesKHR()
    var p2 = VkPhysicalDeviceProperties2()
    unsafe {
        p2.pNext = addr<void?>(rtp)
    }
    vkGetPhysicalDeviceProperties2(phys, p2)
    rtp.pNext = null   // don't leak the dangling chain pointer to the caller
    return rtp
}

//! Fetch a ray-tracing pipeline's opaque shader-group handles (``vkGetRayTracingShaderGroupHandlesKHR``
//! is raw-only in the generated layer — its ``void*`` out-param is not boost-mappable). Returns
//! ``group_count`` records of ``handle_size`` bytes, tightly packed; build_shader_binding_table aligns.
def public get_ray_tracing_shader_group_handles(device : Device; pipeline : Pipeline;
                                                first_group : uint; group_count : uint; handle_size : uint) : array<uint8> {
    var data : array<uint8>
    data |> resize(int(group_count * handle_size))
    unsafe {
        vk_check(vkGetRayTracingShaderGroupHandlesKHR(boost_value_to_vk(device), boost_value_to_vk(pipeline),
            first_group, group_count, uint64(long_length(data)), addr<void?>(data[0])), null)
    }
    return <- data
}

//!@nolint
//! Create a triangle-path ray-tracing pipeline: raygen + N miss + closest-hit, one GENERAL group per
//! general stage plus a TRIANGLES_HIT_GROUP. Group order is [raygen, miss[0..N), hit] — the layout
//! build_shader_binding_table assumes; ``traceRayEXT``'s ``miss_index`` selects within the miss region.
//! Shadow-ray setups take two miss shaders (index 0 = radiance/sky, index 1 = the shadow "lit" flip).
//! ``max_recursion >= 2`` lets closest-hit trace shadow rays.
def public create_ray_tracing_pipeline(device : Device; layout : PipelineLayout;
                                       raygen : ShaderModule; miss : array<ShaderModule>; closest_hit : ShaderModule;
                                       max_recursion : uint = 1u) : Pipeline {
    assert(!empty(miss), "create_ray_tracing_pipeline: need at least one miss shader")
    var ci : RayTracingPipelineCreateInfoKHR
    ci.pStages |> reserve(2 + length(miss))
    ci.pGroups |> reserve(2 + length(miss))
    var st_rgen = PipelineShaderStageCreateInfo(module_ = weak_copy(raygen), pName = "main")
    st_rgen.stage.raygen_khr = true
    ci.pStages |> emplace(st_rgen)
    ci.pGroups |> push(RayTracingShaderGroupCreateInfoKHR(type_ = VkRayTracingShaderGroupTypeKHR.GENERAL_KHR,
        generalShader = 0u, closestHitShader = VK_SHADER_UNUSED_KHR,
        anyHitShader = VK_SHADER_UNUSED_KHR, intersectionShader = VK_SHADER_UNUSED_KHR))
    for (i in range(length(miss))) {
        var st_miss = PipelineShaderStageCreateInfo(module_ = weak_copy(miss[i]), pName = "main")
        st_miss.stage.miss_khr = true
        ci.pStages |> emplace(st_miss)
        ci.pGroups |> push(RayTracingShaderGroupCreateInfoKHR(type_ = VkRayTracingShaderGroupTypeKHR.GENERAL_KHR,
            generalShader = uint(1 + i), closestHitShader = VK_SHADER_UNUSED_KHR,
            anyHitShader = VK_SHADER_UNUSED_KHR, intersectionShader = VK_SHADER_UNUSED_KHR))
    }
    var st_chit = PipelineShaderStageCreateInfo(module_ = weak_copy(closest_hit), pName = "main")
    st_chit.stage.closest_hit_khr = true
    ci.pStages |> emplace(st_chit)
    ci.pGroups |> push(RayTracingShaderGroupCreateInfoKHR(type_ = VkRayTracingShaderGroupTypeKHR.TRIANGLES_HIT_GROUP_KHR,
        generalShader = VK_SHADER_UNUSED_KHR, closestHitShader = uint(1 + length(miss)),
        anyHitShader = VK_SHADER_UNUSED_KHR, intersectionShader = VK_SHADER_UNUSED_KHR))
    ci.maxPipelineRayRecursionDepth = max_recursion
    ci.layout = weak_copy(layout)
    var infos : array<RayTracingPipelineCreateInfoKHR>
    infos |> emplace(ci)
    var no_op : DeferredOperationKHR
    var no_cache : PipelineCache
    var pipes <- create_ray_tracing_pipelines_k_h_r(device, no_op, no_cache, infos)
    unsafe {
        delete infos
    }
    var p <- pipes[0]
    delete pipes
    return <- p
}

//! Single-miss convenience over the N-miss overload: groups [raygen, miss, hit].
def public create_ray_tracing_pipeline(device : Device; layout : PipelineLayout;
                                       raygen : ShaderModule; miss : ShaderModule; closest_hit : ShaderModule;
                                       max_recursion : uint = 1u) : Pipeline {
    var mlist : array<ShaderModule>
    mlist |> push(weak_copy(miss))
    var p <- create_ray_tracing_pipeline(device, layout, raygen, mlist, closest_hit, max_recursion)
    delete mlist
    return <- p
}

//! A built shader binding table: the backing device-address buffer plus the four strided regions
//! ``cmd_trace_rays_k_h_r`` consumes. ``callable`` is a valid empty region (no callable shaders).
struct public ShaderBindingTable {
    buffer : AddressBuffer
    raygen : StridedDeviceAddressRegionKHR
    miss : StridedDeviceAddressRegionKHR
    hit : StridedDeviceAddressRegionKHR
    callable : StridedDeviceAddressRegionKHR
}

def public finalize(var sbt : ShaderBindingTable) {
    delete sbt.buffer
}

def private align_up(v : uint64; alignment : uint64) : uint64 => (v + alignment - 1ul) & ~(alignment - 1ul)

//! Build the shader binding table for a create_ray_tracing_pipeline pipeline (group order
//! [raygen | miss_count × miss | hit_count × hit]): records aligned per ``shaderGroupHandleAlignment``,
//! region starts per ``shaderGroupBaseAlignment``. Keep the table alive for every trace using it.
def public build_shader_binding_table(device : Device; phys : VkPhysicalDevice; pipeline : Pipeline;
                                      miss_count : uint; hit_count : uint) : ShaderBindingTable {
    let props = get_ray_tracing_properties(phys)
    let handle_size = uint64(props.shaderGroupHandleSize)
    let hs = int(props.shaderGroupHandleSize)   // 32-bit twin for loop/index math; the u64 feeds alignment
    let record = align_up(handle_size, uint64(props.shaderGroupHandleAlignment))
    let base_align = uint64(props.shaderGroupBaseAlignment)
    let rgen_size = align_up(record, base_align)
    let miss_size = align_up(uint64(miss_count) * record, base_align)
    let hit_size = align_up(uint64(hit_count) * record, base_align)
    let total = rgen_size + miss_size + hit_size
    let group_count = 1u + miss_count + hit_count
    var handles <- get_ray_tracing_shader_group_handles(device, pipeline, 0u, group_count, uint(handle_size))
    var usage : VkBufferUsageFlags
    usage.shader_binding_table_khr = true
    // over-allocate by base_align so the record block can start base-aligned even when the
    // buffer's device address itself is not (region.deviceAddress % base_align must be 0)
    var sbt = ShaderBindingTable(buffer <- create_address_buffer(device, phys, total + base_align, usage, true))
    let start = align_up(sbt.buffer.address, base_align)
    let pad = int(start - sbt.buffer.address)
    var records : array<uint8>
    records |> resize(pad + int(total))
    for (g in range(int(group_count))) {
        var dst = pad
        if (g == 0) {
            dst += 0
        } elif (g <= int(miss_count)) {
            dst += int(rgen_size) + (g - 1) * int(record)
        } else {
            dst += int(rgen_size + miss_size) + (g - 1 - int(miss_count)) * int(record)
        }
        for (b in range(hs)) {
            records[dst + b] = handles[g * hs + b]
        }
    }
    sbt.buffer |> upload_bytes(device, records)
    delete handles
    delete records
    sbt.raygen = StridedDeviceAddressRegionKHR(deviceAddress = start, stride = rgen_size, size = rgen_size)
    sbt.miss = StridedDeviceAddressRegionKHR(deviceAddress = start + rgen_size, stride = record, size = miss_size)
    sbt.hit = StridedDeviceAddressRegionKHR(deviceAddress = start + rgen_size + miss_size, stride = record, size = hit_size)
    return <- sbt
}

//! Trace using a built ShaderBindingTable (the four strided regions in one argument).
def public cmd_trace_rays(cmd : CommandBuffer; sbt : ShaderBindingTable; width : uint; height : uint; depth : uint = 1u) {
    cmd_trace_rays_k_h_r(cmd, sbt.raygen, sbt.miss, sbt.hit, sbt.callable, width, height, depth)
}

//! Host-side stand-in for ``VkAccelerationStructureInstanceKHR``, which the generator skips (C bitfield
//! members). Byte-identical to the C struct, so an ``array<AccelInstance>`` uploads straight into the
//! TLAS instance buffer. Build with make_accel_instance; set translation via ``inst.transform[r][3]``.
struct public AccelInstance {
    transform : float[3][4]        // row r = [x-axis, y-axis, z-axis, translation][r]
    customIndex24_mask8 : uint     // instanceCustomIndex:24 | mask:8
    sbtOffset24_flags8 : uint      // instanceShaderBindingTableRecordOffset:24 | flags:8
    blasAddress : uint64           // accelerationStructureReference
}

//! An AccelInstance with the identity transform. ``flags`` is VkGeometryInstanceFlagBitsKHR
//! (e.g. 1 = TRIANGLE_FACING_CULL_DISABLE); the default ``mask`` 0xFF passes every trace cull mask.
def public make_accel_instance(blas_address : uint64; custom_index : uint = 0u; mask : uint = 0xFFu;
                               sbt_record_offset : uint = 0u; flags : uint = 0u) : AccelInstance {
    var inst : AccelInstance
    inst.transform[0][0] = 1.0
    inst.transform[1][1] = 1.0
    inst.transform[2][2] = 1.0
    inst.customIndex24_mask8 = (custom_index & 0x00FFFFFFu) | (mask << 24u)
    inst.sbtOffset24_flags8 = (sbt_record_offset & 0x00FFFFFFu) | (flags << 24u)
    inst.blasAddress = blas_address
    return inst
}

//! An acceleration structure with its backing storage buffer and GPU address, owned together
//! (``var inscope tlas <- build_tlas(...)``). For a BLAS, ``address`` is what TLAS instances
//! reference (make_accel_instance takes it).
struct public AccelStructure {
    handle : AccelerationStructureKHR
    buffer : AddressBuffer
    address : uint64
}

def public finalize(var a : AccelStructure) {
    delete a.handle
    delete a.buffer
}

// shared create→sizes→scratch→build→address dance behind build_blas / build_tlas. run_cmd_sync
// waits the queue idle, so the scratch (and the caller's build-input buffers) may be freed right
// after, and a BLAS built this way is complete before a TLAS build that references it starts.
def private build_accel_structure(device : Device; phys : VkPhysicalDevice; pool : CommandPool; queue : VkQueue;
                                  geom : AccelerationStructureGeometryKHR; as_type : VkAccelerationStructureTypeKHR;
                                  prim_count : uint) : AccelStructure {
    var bgi = AccelerationStructureBuildGeometryInfoKHR(type_ = as_type,
        mode = VkBuildAccelerationStructureModeKHR.BUILD_KHR)
    bgi.flags.prefer_fast_trace_khr = true
    bgi.pGeometries |> push(geom)
    var max_prims <- [prim_count]
    let sizes = get_acceleration_structure_build_sizes_k_h_r(device, VkAccelerationStructureBuildTypeKHR.DEVICE_KHR, bgi, max_prims)
    delete max_prims
    var storage_usage : VkBufferUsageFlags
    storage_usage.acceleration_structure_storage_khr = true
    var inscope as_buf <- create_address_buffer(device, phys, sizes.accelerationStructureSize, storage_usage, false)
    let asci = AccelerationStructureCreateInfoKHR(buffer = weak_copy(as_buf.buffer),
        size = sizes.accelerationStructureSize, type_ = as_type)
    var inscope handle <- create_acceleration_structure_k_h_r(device, asci)
    var scratch_usage : VkBufferUsageFlags
    scratch_usage.storage_buffer = true
    var inscope scratch <- create_address_buffer(device, phys, sizes.buildScratchSize, scratch_usage, false)
    bgi.dstAccelerationStructure = weak_copy(handle)
    bgi.scratchData.deviceAddress = scratch.address
    var infos : array<AccelerationStructureBuildGeometryInfoKHR>
    infos |> emplace(bgi)
    var ranges : array<array<AccelerationStructureBuildRangeInfoKHR>>
    ranges |> resize(1)
    ranges[0] |> push(AccelerationStructureBuildRangeInfoKHR(primitiveCount = prim_count))
    run_cmd_sync(device, pool, queue) $(cmd) {
        cmd_build_acceleration_structures_k_h_r(cmd, infos, ranges)
    }
    unsafe {
        delete infos
    }
    delete ranges
    let dai = AccelerationStructureDeviceAddressInfoKHR(accelerationStructure = weak_copy(handle))
    let address = get_acceleration_structure_device_address_k_h_r(device, dai)
    var result = AccelStructure(handle <- handle, buffer <- as_buf, address = address)
    return <- result
}

//! Build a bottom-level acceleration structure over an indexed triangle list, blocking until the GPU
//! build completes. ``vertices`` are xyz positions (R32G32B32_SFLOAT, stride ``sizeof(float3)``);
//! ``indices`` are uint triples. Build inputs are transient — only the AccelStructure stays resident.
def public build_blas(device : Device; phys : VkPhysicalDevice; pool : CommandPool; queue : VkQueue;
                      var vertices : array<float3>; var indices : array<uint>) : AccelStructure {
    assert(!empty(vertices), "build_blas: vertices must be non-empty")
    assert(!empty(indices), "build_blas: indices must be non-empty")
    assert(length(indices) % 3 == 0, "build_blas: indices must be uint triples")
    var input_usage : VkBufferUsageFlags
    input_usage.acceleration_structure_build_input_read_only_khr = true
    let vsize = uint64(long_length(vertices) * int64(typeinfo sizeof(type<float3>)))
    var inscope vbuf <- create_address_buffer(device, phys, vsize, input_usage, true)
    vbuf |> upload_bytes(device, vertices)
    let isize = uint64(long_length(indices) * 4l)
    var inscope ibuf <- create_address_buffer(device, phys, isize, input_usage, true)
    ibuf |> upload_bytes(device, indices)
    var tri = VkAccelerationStructureGeometryTrianglesDataKHR()
    tri.vertexFormat = VkFormat.R32G32B32_SFLOAT
    tri.vertexData.deviceAddress = vbuf.address
    tri.vertexStride = uint64(typeinfo sizeof(type<float3>))
    tri.maxVertex = uint(length(vertices) - 1)
    tri.indexType = VkIndexType.UINT32
    tri.indexData.deviceAddress = ibuf.address
    var geom : AccelerationStructureGeometryKHR
    geom.geometryType = VkGeometryTypeKHR.TRIANGLES_KHR
    geom.geometry.triangles = tri
    geom.flags.opaque_khr = true
    return <- build_accel_structure(device, phys, pool, queue, geom, VkAccelerationStructureTypeKHR.BOTTOM_LEVEL_KHR,
        uint(length(indices) / 3))
}

//! Build a top-level acceleration structure over BLAS instances (make_accel_instance), blocking
//! until the GPU build completes. The referenced BLASes must be fully built — true for build_blas,
//! which waits the queue idle.
def public build_tlas(device : Device; phys : VkPhysicalDevice; pool : CommandPool; queue : VkQueue;
                      var instances : array<AccelInstance>) : AccelStructure {
    static_assert(typeinfo sizeof(type<AccelInstance>) == 64, "AccelInstance must stay wire-identical to VkAccelerationStructureInstanceKHR (64 bytes)")
    assert(!empty(instances), "build_tlas: instances must be non-empty")
    var input_usage : VkBufferUsageFlags
    input_usage.acceleration_structure_build_input_read_only_khr = true
    let isize = uint64(long_length(instances) * int64(typeinfo sizeof(type<AccelInstance>)))
    var inscope ibuf <- create_address_buffer(device, phys, isize, input_usage, true)
    ibuf |> upload_bytes(device, instances)
    var idata = VkAccelerationStructureGeometryInstancesDataKHR()
    idata.data.deviceAddress = ibuf.address
    var geom : AccelerationStructureGeometryKHR
    geom.geometryType = VkGeometryTypeKHR.INSTANCES_KHR
    geom.geometry.instances = idata
    return <- build_accel_structure(device, phys, pool, queue, geom, VkAccelerationStructureTypeKHR.TOP_LEVEL_KHR,
        uint(length(instances)))
}

//! Write a top-level acceleration structure into a descriptor set — the pNext-chained
//! ``VkWriteDescriptorSetAccelerationStructureKHR`` dance in one call. Matches the
//! ``SpirvDescriptorKind.acceleration_structure`` binding vulkan_reflect lays out for the shader global.
def public write_descriptor_acceleration_structure(device : Device; dset : DescriptorSet; binding : uint; tlas : AccelStructure) {
    var raw_as = boost_value_to_vk(tlas.handle)
    var was = VkWriteDescriptorSetAccelerationStructureKHR()
    was.accelerationStructureCount = 1u
    var w = VkWriteDescriptorSet()
    w.dstSet = boost_value_to_vk(dset)
    w.dstBinding = binding
    w.descriptorCount = 1u
    w.descriptorType = VkDescriptorType.ACCELERATION_STRUCTURE_KHR
    unsafe {
        was.pAccelerationStructures = addr(raw_as)
        w.pNext = addr<void?>(was)
        vkUpdateDescriptorSets(boost_value_to_vk(device), 1u, addr(w), 0u, null)
    }
}
