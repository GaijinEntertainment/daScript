#pragma once

#include "daScript/misc/platform.h"
#include "daScript/misc/vectypes.h"

namespace das {

    class Context;
    struct LineInfoArg;

    // opaque handles — each wraps exactly one retained Metal object (__bridge_retained);
    // metal_release is the only way to drop the retain
    struct MetalDevice;
    struct MetalCommandQueue;
    struct MetalLibrary;
    struct MetalFunction;
    struct MetalComputePipeline;
    struct MetalBuffer;
    struct MetalCommandBuffer;
    struct MetalComputeEncoder;
    struct MetalSharedEvent;
    struct MetalResidencySet;

    DAS_MOD_API MetalDevice * metal_create_system_default_device ();
    DAS_MOD_API char * metal_device_name ( MetalDevice * dev, Context * ctx, LineInfoArg * at );
    DAS_MOD_API bool metal_device_has_unified_memory ( MetalDevice * dev, Context * ctx, LineInfoArg * at );

    DAS_MOD_API MetalCommandQueue * metal_new_command_queue ( MetalDevice * dev, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalLibrary * metal_new_library_from_source ( MetalDevice * dev, const char * src, bool fastmath,
        char * & error, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalFunction * metal_new_function ( MetalLibrary * lib, const char * name, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalComputePipeline * metal_new_compute_pipeline ( MetalDevice * dev, MetalFunction * fn,
        char * & error, Context * ctx, LineInfoArg * at );
    DAS_MOD_API uint32_t metal_pipeline_max_total_threads ( MetalComputePipeline * pso, Context * ctx, LineInfoArg * at );
    DAS_MOD_API uint32_t metal_pipeline_thread_execution_width ( MetalComputePipeline * pso, Context * ctx, LineInfoArg * at );

    DAS_MOD_API MetalBuffer * metal_new_buffer ( MetalDevice * dev, uint64_t bytes, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalBuffer * metal_new_buffer_untracked ( MetalDevice * dev, uint64_t bytes, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalBuffer * metal_new_buffer_no_copy ( MetalDevice * dev, void * data, uint64_t bytes, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalBuffer * metal_new_buffer_no_copy_untracked ( MetalDevice * dev, void * data, uint64_t bytes, Context * ctx, LineInfoArg * at );
    DAS_MOD_API uint64_t metal_max_buffer_length ( MetalDevice * dev, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void * metal_buffer_contents ( MetalBuffer * buf, Context * ctx, LineInfoArg * at );

    DAS_MOD_API MetalCommandBuffer * metal_new_command_buffer ( MetalCommandQueue * queue, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalCommandBuffer * metal_new_command_buffer_unretained ( MetalCommandQueue * queue, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalComputeEncoder * metal_new_compute_encoder ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalComputeEncoder * metal_new_compute_encoder_concurrent ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_memory_barrier ( MetalComputeEncoder * enc, Context * ctx, LineInfoArg * at );
    DAS_MOD_API MetalSharedEvent * metal_new_shared_event ( MetalDevice * dev, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_cb_wait_for_event ( MetalCommandBuffer * cb, MetalSharedEvent * ev, uint64_t value,
        Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_shared_event_signal ( MetalSharedEvent * ev, uint64_t value, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_set_pipeline ( MetalComputeEncoder * enc, MetalComputePipeline * pso, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_set_buffer ( MetalComputeEncoder * enc, MetalBuffer * buf, uint64_t offset, int32_t index,
        Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_set_bytes ( MetalComputeEncoder * enc, void * data, uint64_t len, int32_t index,
        Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_set_threadgroup_memory_length ( MetalComputeEncoder * enc, uint64_t length, int32_t index,
        Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_dispatch_threadgroups ( MetalComputeEncoder * enc, uint3 groups, uint3 threads_per_group,
        Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_dispatch_threads ( MetalComputeEncoder * enc, uint3 threads, uint3 threads_per_group,
        Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_end_encoding ( MetalComputeEncoder * enc, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_commit ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_wait_until_completed ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API char * metal_command_buffer_error ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API bool metal_command_buffer_failed ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API double metal_command_buffer_gpu_start_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API double metal_command_buffer_gpu_end_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API double metal_command_buffer_kernel_start_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );
    DAS_MOD_API double metal_command_buffer_kernel_end_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at );

    // MTLResidencySet (macOS 15+) — pin a working set resident globally + persistently. Never bound
    // to an encoder/command buffer (ggml-metal's model): add allocations, commit, requestResidency
    // once. metal_new_residency_set returns null on older OS — callers treat null as "skip".
    DAS_MOD_API MetalResidencySet * metal_new_residency_set ( MetalDevice * dev, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_residency_set_add_buffer ( MetalResidencySet * rset, MetalBuffer * buf, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_residency_set_commit ( MetalResidencySet * rset, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void metal_residency_set_request ( MetalResidencySet * rset, Context * ctx, LineInfoArg * at );

    // each registers as a das-side `metal_release` overload; C++ names differ
    // because DAS_BIND_FUN cannot take an overload set
    DAS_MOD_API void metal_release_device ( MetalDevice * h );
    DAS_MOD_API void metal_release_queue ( MetalCommandQueue * h );
    DAS_MOD_API void metal_release_library ( MetalLibrary * h );
    DAS_MOD_API void metal_release_function ( MetalFunction * h );
    DAS_MOD_API void metal_release_pipeline ( MetalComputePipeline * h );
    DAS_MOD_API void metal_release_buffer ( MetalBuffer * h );
    DAS_MOD_API void metal_release_command_buffer ( MetalCommandBuffer * h );
    DAS_MOD_API void metal_release_encoder ( MetalComputeEncoder * h );
    DAS_MOD_API void metal_release_shared_event ( MetalSharedEvent * h );
    DAS_MOD_API void metal_release_residency_set ( MetalResidencySet * h );

    DAS_MOD_API int64_t metal_live_object_count ();
    DAS_MOD_API char * metal_live_object_report ( Context * ctx, LineInfoArg * at );
    DAS_MOD_API int64_t metal_dispatch_call_count ();
}
