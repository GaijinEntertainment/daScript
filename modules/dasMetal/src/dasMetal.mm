#include "daScript/misc/platform.h"

#include "dasMetal.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"

#import <Metal/Metal.h>
#import <Foundation/Foundation.h>

#include <atomic>
#include <mutex>
#include <unordered_map>
#include <map>

#if !__has_feature(objc_arc)
#error "dasMetal.mm must be compiled with ARC (-fobjc-arc)"
#endif

// MTLResidencySet is only declared in the macOS 15.0 / iOS 18.0 SDK and newer
#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 150000
#define DASMETAL_HAS_RESIDENCY_SETS 1
#elif defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && __IPHONE_OS_VERSION_MAX_ALLOWED >= 180000
#define DASMETAL_HAS_RESIDENCY_SETS 1
#endif

MAKE_TYPE_FACTORY(MetalDevice, MetalDevice)
MAKE_TYPE_FACTORY(MetalCommandQueue, MetalCommandQueue)
MAKE_TYPE_FACTORY(MetalLibrary, MetalLibrary)
MAKE_TYPE_FACTORY(MetalFunction, MetalFunction)
MAKE_TYPE_FACTORY(MetalComputePipeline, MetalComputePipeline)
MAKE_TYPE_FACTORY(MetalBuffer, MetalBuffer)
MAKE_TYPE_FACTORY(MetalCommandBuffer, MetalCommandBuffer)
MAKE_TYPE_FACTORY(MetalComputeEncoder, MetalComputeEncoder)
MAKE_TYPE_FACTORY(MetalSharedEvent, MetalSharedEvent)
MAKE_TYPE_FACTORY(MetalResidencySet, MetalResidencySet)

namespace das {

    // every handle handed to das is one __bridge_retained ref, counted here;
    // Metal objects are invisible to all six das leak detectors, so the shim counts
    static std::atomic<int64_t> g_metalLiveObjects{0};
    // lifetime dispatch-call counter (dispatchThreadgroups + dispatchThreads) — the dispatch-count
    // instrument: callers read deltas around a window (never reset; deltas compose across readers)
    static std::atomic<int64_t> g_metalDispatchCalls{0};

    // the leak-hunt registry behind the counter: live handle -> "type @ file:line" of the das
    // creation site. Volume is low (planes at load, cb/encoder per step), so the mutex is noise-free
    static std::mutex g_metalLiveTagsMx;
    static std::unordered_map<void *, std::string> g_metalLiveTags;

    static std::string handle_tag ( const char * type, LineInfoArg * at ) {
        if ( !at || !at->fileInfo ) return type;
        return std::string(type) + " @ " + at->fileInfo->name.c_str() + ":" + std::to_string(at->line);
    }

    // buffers add their byte size to the tag — pooled buffers all create at ONE das line (the
    // pool body), so the size bucket is what tells the leaked classes apart
    static std::string buffer_tag ( uint64_t bytes, LineInfoArg * at ) {
        return handle_tag(("MetalBuffer[" + std::to_string(bytes) + "]").c_str(), at);
    }

    template <typename HandleT>
    static HandleT * retain_handle_tagged ( id obj, const std::string & tag ) {
        if ( obj == nil ) return nullptr;
        g_metalLiveObjects.fetch_add(1, std::memory_order_relaxed);
        void * h = (__bridge_retained void *) obj;
        {
            std::lock_guard<std::mutex> guard(g_metalLiveTagsMx);
            g_metalLiveTags[h] = tag;
        }
        return (HandleT *) h;
    }

    template <typename HandleT>
    static HandleT * retain_handle ( id obj, const char * type, LineInfoArg * at ) {
        return retain_handle_tagged<HandleT>(obj, handle_tag(type, at));
    }

    static void release_handle ( void * h ) {
        if ( !h ) return;
        g_metalLiveObjects.fetch_sub(1, std::memory_order_relaxed);
        {
            std::lock_guard<std::mutex> guard(g_metalLiveTagsMx);
            g_metalLiveTags.erase(h);
        }
        id obj = (__bridge_transfer id) h;
        (void) obj;     // ARC releases at scope exit
    }

    static char * alloc_error_string ( NSError * err, const char * fallback, Context * ctx, LineInfoArg * at ) {
        const char * msg = err ? err.localizedDescription.UTF8String : nullptr;
        return ctx->allocateString(msg ? msg : fallback, at);
    }

    // ===== device =====

    MetalDevice * metal_create_system_default_device () {
        @autoreleasepool {
            return retain_handle<MetalDevice>(MTLCreateSystemDefaultDevice(), "MetalDevice", nullptr);
        }
    }

    char * metal_device_name ( MetalDevice * dev, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_device_name: null device");
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            const char * name = d.name.UTF8String;
            return ctx->allocateString(name ? name : "", at);
        }
    }

    bool metal_device_has_unified_memory ( MetalDevice * dev, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_device_has_unified_memory: null device");
        id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
        return d.hasUnifiedMemory;
    }

    // ===== compile =====

    MetalCommandQueue * metal_new_command_queue ( MetalDevice * dev, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_new_command_queue: null device");
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            return retain_handle<MetalCommandQueue>([d newCommandQueue], "MetalCommandQueue", at);
        }
    }

    MetalLibrary * metal_new_library_from_source ( MetalDevice * dev, const char * src, bool fastmath,
            char * & error, Context * ctx, LineInfoArg * at ) {
        error = nullptr;
        if ( !dev ) ctx->throw_error_at(at, "metal_new_library_from_source: null device");
        if ( !src || !*src ) {
            error = ctx->allocateString("metal_new_library_from_source: empty MSL source", at);
            return nullptr;
        }
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            MTLCompileOptions * opts = [MTLCompileOptions new];
#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 150000
            opts.mathMode = fastmath ? MTLMathModeFast : MTLMathModeSafe;
#else
            opts.fastMathEnabled = fastmath ? YES : NO;
#endif
            NSString * nsSrc = [NSString stringWithUTF8String:src];
            if ( nsSrc == nil ) {   // invalid UTF-8 — a nil source would raise an ObjC exception below
                error = ctx->allocateString("metal_new_library_from_source: MSL source is not valid UTF-8", at);
                return nullptr;
            }
            NSError * err = nil;
            id<MTLLibrary> lib = [d newLibraryWithSource:nsSrc
                                                 options:opts
                                                   error:&err];
            if ( lib == nil ) {
                error = alloc_error_string(err, "unknown MSL compile error", ctx, at);
                return nullptr;
            }
            return retain_handle<MetalLibrary>(lib, "MetalLibrary", at);
        }
    }

    MetalFunction * metal_new_function ( MetalLibrary * lib, const char * name, Context * ctx, LineInfoArg * at ) {
        if ( !lib ) ctx->throw_error_at(at, "metal_new_function: null library");
        if ( !name || !*name ) ctx->throw_error_at(at, "metal_new_function: empty function name");
        @autoreleasepool {
            id<MTLLibrary> l = (__bridge id<MTLLibrary>)(void *) lib;
            NSString * nsName = [NSString stringWithUTF8String:name];
            if ( nsName == nil ) ctx->throw_error_at(at, "metal_new_function: entry name is not valid UTF-8");
            return retain_handle<MetalFunction>([l newFunctionWithName:nsName], "MetalFunction", at);
        }
    }

    MetalComputePipeline * metal_new_compute_pipeline ( MetalDevice * dev, MetalFunction * fn,
            char * & error, Context * ctx, LineInfoArg * at ) {
        error = nullptr;
        if ( !dev ) ctx->throw_error_at(at, "metal_new_compute_pipeline: null device");
        if ( !fn ) ctx->throw_error_at(at, "metal_new_compute_pipeline: null function");
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            id<MTLFunction> f = (__bridge id<MTLFunction>)(void *) fn;
            NSError * err = nil;
            id<MTLComputePipelineState> pso = [d newComputePipelineStateWithFunction:f error:&err];
            if ( pso == nil ) {
                error = alloc_error_string(err, "unknown compute pipeline error", ctx, at);
                return nullptr;
            }
            return retain_handle<MetalComputePipeline>(pso, "MetalComputePipeline", at);
        }
    }

    // occupancy introspection: register-pressure-limited kernels report < 1024 here
    uint32_t metal_pipeline_max_total_threads ( MetalComputePipeline * pso, Context * ctx, LineInfoArg * at ) {
        if ( !pso ) ctx->throw_error_at(at, "metal_pipeline_max_total_threads: null pipeline");
        id<MTLComputePipelineState> p = (__bridge id<MTLComputePipelineState>)(void *) pso;
        return (uint32_t) p.maxTotalThreadsPerThreadgroup;
    }

    uint32_t metal_pipeline_thread_execution_width ( MetalComputePipeline * pso, Context * ctx, LineInfoArg * at ) {
        if ( !pso ) ctx->throw_error_at(at, "metal_pipeline_thread_execution_width: null pipeline");
        id<MTLComputePipelineState> p = (__bridge id<MTLComputePipelineState>)(void *) pso;
        return (uint32_t) p.threadExecutionWidth;
    }

    // ===== buffers =====

    MetalBuffer * metal_new_buffer ( MetalDevice * dev, uint64_t bytes, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_new_buffer: null device");
        if ( bytes == 0 ) ctx->throw_error_at(at, "metal_new_buffer: zero size");
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            return retain_handle_tagged<MetalBuffer>([d newBufferWithLength:bytes options:MTLResourceStorageModeShared], buffer_tag(bytes, at));
        }
    }

    // UNTRACKED buffer: opts out of the driver's per-command-buffer hazard/dependency analysis.
    // For GPU-read-only data written by the CPU before commit (weights) — commit-boundary
    // CPU->GPU coherency still holds for shared storage; the caller owns any GPU-GPU ordering.
    MetalBuffer * metal_new_buffer_untracked ( MetalDevice * dev, uint64_t bytes, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_new_buffer_untracked: null device");
        if ( bytes == 0 ) ctx->throw_error_at(at, "metal_new_buffer_untracked: zero size");
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            return retain_handle_tagged<MetalBuffer>([d newBufferWithLength:bytes
                options:MTLResourceStorageModeShared | MTLResourceHazardTrackingModeUntracked], buffer_tag(bytes, at));
        }
    }

    // ZERO-COPY buffer over caller-owned memory (the prepared-model-image mappings): wraps the
    // pages instead of copying — unified memory makes the same bytes GPU-visible. Both pointer
    // and length must be PAGE-ALIGNED (Metal's bytesNoCopy contract; 16KB on Apple Silicon —
    // the image format page-aligns every GPU-bound plane). No deallocator: the caller's
    // mapping must outlive the buffer (release the buffer before fmap_close). TRACKED, unlike
    // the weight buffers above: wrapped storage may carry GPU writes (the zero-copy logits
    // rail), which need cross-cb read/write hazards.
    MetalBuffer * metal_new_buffer_no_copy ( MetalDevice * dev, void * data, uint64_t bytes, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_new_buffer_no_copy: null device");
        if ( !data ) ctx->throw_error_at(at, "metal_new_buffer_no_copy: null data");
        if ( bytes == 0 ) ctx->throw_error_at(at, "metal_new_buffer_no_copy: zero size");
        uint64_t page = uint64_t(getpagesize());
        if ( (uintptr_t(data) % page) != 0 || (bytes % page) != 0 ) {
            ctx->throw_error_at(at, "metal_new_buffer_no_copy: pointer and length must be page-aligned (%llu)",
                (unsigned long long)page);
        }
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            id<MTLBuffer> b = [d newBufferWithBytesNoCopy:data length:bytes
                options:MTLResourceStorageModeShared
                deallocator:nil];
            if ( !b ) ctx->throw_error_at(at, "metal_new_buffer_no_copy: wrap failed (%llu bytes)", (unsigned long long)bytes);
            return retain_handle_tagged<MetalBuffer>(b, buffer_tag(bytes, at));
        }
    }

    // the untracked twin: zero-copy wrap of GPU-read-only pages (the blob-only model images) —
    // opts out of hazard tracking exactly like metal_new_buffer_untracked does for owned weights.
    // deallocator:nil — the wrapped memory must OUTLIVE the MetalBuffer (never wrap short-lived allocations)
    MetalBuffer * metal_new_buffer_no_copy_untracked ( MetalDevice * dev, void * data, uint64_t bytes, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_new_buffer_no_copy_untracked: null device");
        if ( !data ) ctx->throw_error_at(at, "metal_new_buffer_no_copy_untracked: null data");
        if ( bytes == 0 ) ctx->throw_error_at(at, "metal_new_buffer_no_copy_untracked: zero size");
        uint64_t page = uint64_t(getpagesize());
        if ( (uintptr_t(data) % page) != 0 || (bytes % page) != 0 ) {
            ctx->throw_error_at(at, "metal_new_buffer_no_copy_untracked: pointer and length must be page-aligned (%llu)",
                (unsigned long long)page);
        }
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            id<MTLBuffer> b = [d newBufferWithBytesNoCopy:data length:bytes
                options:MTLResourceStorageModeShared | MTLResourceHazardTrackingModeUntracked
                deallocator:nil];
            if ( !b ) ctx->throw_error_at(at, "metal_new_buffer_no_copy_untracked: wrap failed (%llu bytes)", (unsigned long long)bytes);
            return retain_handle_tagged<MetalBuffer>(b, buffer_tag(bytes, at));
        }
    }

    uint64_t metal_max_buffer_length ( MetalDevice * dev, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_max_buffer_length: null device");
        id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
        return (uint64_t) d.maxBufferLength;
    }

    void * metal_buffer_contents ( MetalBuffer * buf, Context * ctx, LineInfoArg * at ) {
        if ( !buf ) ctx->throw_error_at(at, "metal_buffer_contents: null buffer");
        id<MTLBuffer> b = (__bridge id<MTLBuffer>)(void *) buf;
        return b.contents;
    }

    // ===== dispatch =====

    MetalCommandBuffer * metal_new_command_buffer ( MetalCommandQueue * queue, Context * ctx, LineInfoArg * at ) {
        if ( !queue ) ctx->throw_error_at(at, "metal_new_command_buffer: null command queue");
        @autoreleasepool {
            id<MTLCommandQueue> q = (__bridge id<MTLCommandQueue>)(void *) queue;
            return retain_handle<MetalCommandBuffer>([q commandBuffer], "MetalCommandBuffer", at);
        }
    }

    // UNRETAINED command buffer: skips the per-dispatch retain/release of every referenced
    // resource at commit — the CALLER guarantees all buffers/pipelines outlive completion
    MetalCommandBuffer * metal_new_command_buffer_unretained ( MetalCommandQueue * queue, Context * ctx, LineInfoArg * at ) {
        if ( !queue ) ctx->throw_error_at(at, "metal_new_command_buffer_unretained: null command queue");
        @autoreleasepool {
            id<MTLCommandQueue> q = (__bridge id<MTLCommandQueue>)(void *) queue;
            return retain_handle<MetalCommandBuffer>([q commandBufferWithUnretainedReferences], "MetalCommandBuffer", at);
        }
    }

    MetalComputeEncoder * metal_new_compute_encoder ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_new_compute_encoder: null command buffer");
        @autoreleasepool {
            id<MTLCommandBuffer> c = (__bridge id<MTLCommandBuffer>)(void *) cb;
            return retain_handle<MetalComputeEncoder>([c computeCommandEncoder], "MetalComputeEncoder", at);
        }
    }

    // CONCURRENT-dispatch encoder: Metal does no per-dispatch hazard ordering inside the pass —
    // the CALLER orders dependent dispatches with metal_memory_barrier (the ggml-metal encode
    // model; measured against the serial encoder's per-dispatch launch gap in the framing lab)
    MetalComputeEncoder * metal_new_compute_encoder_concurrent ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_new_compute_encoder_concurrent: null command buffer");
        @autoreleasepool {
            id<MTLCommandBuffer> c = (__bridge id<MTLCommandBuffer>)(void *) cb;
            return retain_handle<MetalComputeEncoder>([c computeCommandEncoderWithDispatchType:MTLDispatchTypeConcurrent], "MetalComputeEncoder", at);
        }
    }

    // buffer-scope memory barrier: dispatches encoded after it observe writes of dispatches
    // encoded before it (only meaningful on a concurrent encoder)
    void metal_memory_barrier ( MetalComputeEncoder * enc, Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_memory_barrier: null encoder");
        [(__bridge id<MTLComputeCommandEncoder>)(void *) enc memoryBarrierWithScope:MTLBarrierScopeBuffers];
    }

    // shared event trio — the encode-ahead rail: a pre-encoded command buffer opens with
    // metal_cb_wait_for_event (encoded BEFORE any encoder on that cb) and commits early; the CPU
    // pokes the input buffers later and releases the GPU with metal_shared_event_signal. The
    // signaled value must be non-decreasing over the event's lifetime.
    MetalSharedEvent * metal_new_shared_event ( MetalDevice * dev, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_new_shared_event: null device");
        @autoreleasepool {
            id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
            return retain_handle<MetalSharedEvent>([d newSharedEvent], "MetalSharedEvent", at);
        }
    }

    void metal_cb_wait_for_event ( MetalCommandBuffer * cb, MetalSharedEvent * ev, uint64_t value,
            Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_cb_wait_for_event: null command buffer");
        if ( !ev ) ctx->throw_error_at(at, "metal_cb_wait_for_event: null event");
        id<MTLCommandBuffer> c = (__bridge id<MTLCommandBuffer>)(void *) cb;
        [c encodeWaitForEvent:(__bridge id<MTLSharedEvent>)(void *) ev value:value];
    }

    void metal_shared_event_signal ( MetalSharedEvent * ev, uint64_t value, Context * ctx, LineInfoArg * at ) {
        if ( !ev ) ctx->throw_error_at(at, "metal_shared_event_signal: null event");
        ((__bridge id<MTLSharedEvent>)(void *) ev).signaledValue = value;
    }

    // ===== residency set (macOS 15+) =====
    // ggml-metal's residency model: pin a working set globally + persistently — add the
    // allocations, commit, requestResidency. The set is NEVER bound to an encoder or command
    // buffer; a committed+requested set keeps its pages wired so per-command-buffer residency
    // (Metal's default, re-evaluated per commit over the whole referenced working set) is a no-op.
    MetalResidencySet * metal_new_residency_set ( MetalDevice * dev, Context * ctx, LineInfoArg * at ) {
        if ( !dev ) ctx->throw_error_at(at, "metal_new_residency_set: null device");
#if defined(DASMETAL_HAS_RESIDENCY_SETS)
        if ( @available(macOS 15.0, iOS 18.0, *) ) {
            @autoreleasepool {
                id<MTLDevice> d = (__bridge id<MTLDevice>)(void *) dev;
                MTLResidencySetDescriptor * desc = [MTLResidencySetDescriptor new];
                desc.label = @"dasllama";
                NSError * err = nil;
                id<MTLResidencySet> rset = [d newResidencySetWithDescriptor:desc error:&err];
                if ( rset == nil ) return nullptr;      // null == "unsupported, skip" on the das side
                return retain_handle<MetalResidencySet>(rset, "MetalResidencySet", at);
            }
        }
#endif
        return nullptr;
    }

    void metal_residency_set_add_buffer ( MetalResidencySet * rset, MetalBuffer * buf, Context * ctx, LineInfoArg * at ) {
        if ( !rset ) ctx->throw_error_at(at, "metal_residency_set_add_buffer: null residency set");
        if ( !buf ) ctx->throw_error_at(at, "metal_residency_set_add_buffer: null buffer");
#if defined(DASMETAL_HAS_RESIDENCY_SETS)
        if ( @available(macOS 15.0, iOS 18.0, *) ) {
            [(__bridge id<MTLResidencySet>)(void *) rset addAllocation:(__bridge id<MTLBuffer>)(void *) buf];
        }
#endif
    }

    void metal_residency_set_commit ( MetalResidencySet * rset, Context * ctx, LineInfoArg * at ) {
        if ( !rset ) ctx->throw_error_at(at, "metal_residency_set_commit: null residency set");
#if defined(DASMETAL_HAS_RESIDENCY_SETS)
        if ( @available(macOS 15.0, iOS 18.0, *) ) {
            [(__bridge id<MTLResidencySet>)(void *) rset commit];
        }
#endif
    }

    void metal_residency_set_request ( MetalResidencySet * rset, Context * ctx, LineInfoArg * at ) {
        if ( !rset ) ctx->throw_error_at(at, "metal_residency_set_request: null residency set");
#if defined(DASMETAL_HAS_RESIDENCY_SETS)
        if ( @available(macOS 15.0, iOS 18.0, *) ) {
            [(__bridge id<MTLResidencySet>)(void *) rset requestResidency];
        }
#endif
    }

    void metal_set_pipeline ( MetalComputeEncoder * enc, MetalComputePipeline * pso, Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_set_pipeline: null encoder");
        if ( !pso ) ctx->throw_error_at(at, "metal_set_pipeline: null pipeline");
        id<MTLComputeCommandEncoder> e = (__bridge id<MTLComputeCommandEncoder>)(void *) enc;
        [e setComputePipelineState:(__bridge id<MTLComputePipelineState>)(void *) pso];
    }

    void metal_set_buffer ( MetalComputeEncoder * enc, MetalBuffer * buf, uint64_t offset, int32_t index,
            Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_set_buffer: null encoder");
        if ( !buf ) ctx->throw_error_at(at, "metal_set_buffer: null buffer");
        if ( index < 0 ) ctx->throw_error_at(at, "metal_set_buffer: negative buffer index %i", index);
        id<MTLComputeCommandEncoder> e = (__bridge id<MTLComputeCommandEncoder>)(void *) enc;
        [e setBuffer:(__bridge id<MTLBuffer>)(void *) buf offset:offset atIndex:NSUInteger(index)];
    }

    // inline kargs: copies `len` bytes straight into the command stream (no MetalBuffer per
    // kargs — the ggml-metal model). Metal caps setBytes at 4KB; every LLM kargs struct is < 256B.
    void metal_set_bytes ( MetalComputeEncoder * enc, void * data, uint64_t len, int32_t index,
            Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_set_bytes: null encoder");
        if ( !data ) ctx->throw_error_at(at, "metal_set_bytes: null data");
        if ( index < 0 ) ctx->throw_error_at(at, "metal_set_bytes: negative buffer index %i", index);
        if ( len == 0 || len > 4096 ) {
            ctx->throw_error_at(at, "metal_set_bytes: length %llu out of range (1..4096)", (unsigned long long) len);
        }
        id<MTLComputeCommandEncoder> e = (__bridge id<MTLComputeCommandEncoder>)(void *) enc;
        [e setBytes:data length:NSUInteger(len) atIndex:NSUInteger(index)];
    }

    void metal_set_threadgroup_memory_length ( MetalComputeEncoder * enc, uint64_t length, int32_t index,
            Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_set_threadgroup_memory_length: null encoder");
        if ( index < 0 ) ctx->throw_error_at(at, "metal_set_threadgroup_memory_length: negative index %i", index);
        if ( length == 0 || (length % 16) != 0 ) {   // Metal requires a non-zero multiple of 16
            ctx->throw_error_at(at, "metal_set_threadgroup_memory_length: length %llu is not a non-zero multiple of 16",
                (unsigned long long) length);
        }
        id<MTLComputeCommandEncoder> e = (__bridge id<MTLComputeCommandEncoder>)(void *) enc;
        [e setThreadgroupMemoryLength:NSUInteger(length) atIndex:NSUInteger(index)];
    }

    static bool valid_grid ( uint3 g ) {
        return g.x > 0 && g.y > 0 && g.z > 0;
    }

    void metal_dispatch_threadgroups ( MetalComputeEncoder * enc, uint3 groups, uint3 threads_per_group,
            Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_dispatch_threadgroups: null encoder");
        if ( !valid_grid(groups) || !valid_grid(threads_per_group) ) {
            ctx->throw_error_at(at, "metal_dispatch_threadgroups: zero grid dimension (groups %ux%ux%u, threads %ux%ux%u)",
                groups.x, groups.y, groups.z, threads_per_group.x, threads_per_group.y, threads_per_group.z);
        }
        id<MTLComputeCommandEncoder> e = (__bridge id<MTLComputeCommandEncoder>)(void *) enc;
        g_metalDispatchCalls.fetch_add(1, std::memory_order_relaxed);
        [e dispatchThreadgroups:MTLSizeMake(groups.x, groups.y, groups.z)
            threadsPerThreadgroup:MTLSizeMake(threads_per_group.x, threads_per_group.y, threads_per_group.z)];
    }

    void metal_dispatch_threads ( MetalComputeEncoder * enc, uint3 threads, uint3 threads_per_group,
            Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_dispatch_threads: null encoder");
        if ( !valid_grid(threads) || !valid_grid(threads_per_group) ) {
            ctx->throw_error_at(at, "metal_dispatch_threads: zero grid dimension (threads %ux%ux%u, per group %ux%ux%u)",
                threads.x, threads.y, threads.z, threads_per_group.x, threads_per_group.y, threads_per_group.z);
        }
        id<MTLComputeCommandEncoder> e = (__bridge id<MTLComputeCommandEncoder>)(void *) enc;
        g_metalDispatchCalls.fetch_add(1, std::memory_order_relaxed);
        [e dispatchThreads:MTLSizeMake(threads.x, threads.y, threads.z)
            threadsPerThreadgroup:MTLSizeMake(threads_per_group.x, threads_per_group.y, threads_per_group.z)];
    }

    void metal_end_encoding ( MetalComputeEncoder * enc, Context * ctx, LineInfoArg * at ) {
        if ( !enc ) ctx->throw_error_at(at, "metal_end_encoding: null encoder");
        [(__bridge id<MTLComputeCommandEncoder>)(void *) enc endEncoding];
    }

    void metal_commit ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_commit: null command buffer");
        [(__bridge id<MTLCommandBuffer>)(void *) cb commit];
    }

    void metal_wait_until_completed ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_wait_until_completed: null command buffer");
        [(__bridge id<MTLCommandBuffer>)(void *) cb waitUntilCompleted];
    }

    char * metal_command_buffer_error ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_command_buffer_error: null command buffer");
        @autoreleasepool {
            id<MTLCommandBuffer> c = (__bridge id<MTLCommandBuffer>)(void *) cb;
            if ( c.status == MTLCommandBufferStatusError ) {
                return alloc_error_string(c.error, "unknown command buffer error", ctx, at);
            }
            return nullptr;     // no error — null IS the canonical daslang empty string
        }
    }

    double metal_command_buffer_gpu_start_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_command_buffer_gpu_start_time: null command buffer");
        return [(__bridge id<MTLCommandBuffer>)(void *) cb GPUStartTime];
    }

    double metal_command_buffer_gpu_end_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_command_buffer_gpu_end_time: null command buffer");
        return [(__bridge id<MTLCommandBuffer>)(void *) cb GPUEndTime];
    }

    double metal_command_buffer_kernel_start_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_command_buffer_kernel_start_time: null command buffer");
        return [(__bridge id<MTLCommandBuffer>)(void *) cb kernelStartTime];
    }

    double metal_command_buffer_kernel_end_time ( MetalCommandBuffer * cb, Context * ctx, LineInfoArg * at ) {
        if ( !cb ) ctx->throw_error_at(at, "metal_command_buffer_kernel_end_time: null command buffer");
        return [(__bridge id<MTLCommandBuffer>)(void *) cb kernelEndTime];
    }

    // ===== lifetime =====

    void metal_release_device ( MetalDevice * h ) { release_handle(h); }
    void metal_release_queue ( MetalCommandQueue * h ) { release_handle(h); }
    void metal_release_library ( MetalLibrary * h ) { release_handle(h); }
    void metal_release_function ( MetalFunction * h ) { release_handle(h); }
    void metal_release_pipeline ( MetalComputePipeline * h ) { release_handle(h); }
    void metal_release_buffer ( MetalBuffer * h ) { release_handle(h); }
    void metal_release_command_buffer ( MetalCommandBuffer * h ) { release_handle(h); }
    void metal_release_encoder ( MetalComputeEncoder * h ) { release_handle(h); }
    void metal_release_shared_event ( MetalSharedEvent * h ) { release_handle(h); }
    void metal_release_residency_set ( MetalResidencySet * h ) {
#if defined(DASMETAL_HAS_RESIDENCY_SETS)
        if ( @available(macOS 15.0, iOS 18.0, *) ) {
            if ( h ) [(__bridge id<MTLResidencySet>)(void *) h endResidency];
        }
#endif
        release_handle(h);
    }

    int64_t metal_live_object_count () {
        return g_metalLiveObjects.load(std::memory_order_relaxed);
    }

    // the leak-hunt witness: every live handle grouped by "type @ file:line" creation site,
    // one line per site with a count — empty string when nothing is live
    char * metal_live_object_report ( Context * ctx, LineInfoArg * at ) {
        std::map<std::string, int64_t> bySite;
        {
            std::lock_guard<std::mutex> guard(g_metalLiveTagsMx);
            for ( auto & kv : g_metalLiveTags ) bySite[kv.second]++;
        }
        std::string out;
        for ( auto & kv : bySite ) {
            out += kv.first;
            out += " x";
            out += std::to_string(kv.second);
            out += "\n";
        }
        return ctx->allocateString(out.c_str(), at);
    }

    int64_t metal_dispatch_call_count () {
        return g_metalDispatchCalls.load(std::memory_order_relaxed);
    }

    // ===== module =====

    class Module_DasMetal : public Module {
    public:
        Module_DasMetal() : Module("das_metal") {
            ModuleLibrary lib(this);
            lib.addBuiltInModule();

            addAnnotation(new DummyTypeAnnotation("MetalDevice", "MetalDevice", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalCommandQueue", "MetalCommandQueue", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalLibrary", "MetalLibrary", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalFunction", "MetalFunction", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalComputePipeline", "MetalComputePipeline", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalBuffer", "MetalBuffer", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalCommandBuffer", "MetalCommandBuffer", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalComputeEncoder", "MetalComputeEncoder", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalSharedEvent", "MetalSharedEvent", 1, 1));
            addAnnotation(new DummyTypeAnnotation("MetalResidencySet", "MetalResidencySet", 1, 1));

            addExtern<DAS_BIND_FUN(metal_create_system_default_device)>(*this, lib, "metal_create_system_default_device",
                SideEffects::modifyExternal, "metal_create_system_default_device");
            addExtern<DAS_BIND_FUN(metal_device_name)>(*this, lib, "metal_device_name",
                SideEffects::modifyExternal, "metal_device_name")
                    ->args({"device", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_device_has_unified_memory)>(*this, lib, "metal_device_has_unified_memory",
                SideEffects::modifyExternal, "metal_device_has_unified_memory")
                    ->args({"device", "context", "at"});

            addExtern<DAS_BIND_FUN(metal_new_command_queue)>(*this, lib, "metal_new_command_queue",
                SideEffects::modifyExternal, "metal_new_command_queue")
                    ->args({"device", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_library_from_source)>(*this, lib, "metal_new_library_from_source",
                SideEffects::modifyArgumentAndExternal, "metal_new_library_from_source")
                    ->args({"device", "source", "fastmath", "error", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_function)>(*this, lib, "metal_new_function",
                SideEffects::modifyExternal, "metal_new_function")
                    ->args({"library", "name", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_compute_pipeline)>(*this, lib, "metal_new_compute_pipeline",
                SideEffects::modifyArgumentAndExternal, "metal_new_compute_pipeline")
                    ->args({"device", "function", "error", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_pipeline_max_total_threads)>(*this, lib, "metal_pipeline_max_total_threads",
                SideEffects::none, "metal_pipeline_max_total_threads")
                    ->args({"pipeline", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_pipeline_thread_execution_width)>(*this, lib, "metal_pipeline_thread_execution_width",
                SideEffects::none, "metal_pipeline_thread_execution_width")
                    ->args({"pipeline", "context", "at"});

            addExtern<DAS_BIND_FUN(metal_new_buffer)>(*this, lib, "metal_new_buffer",
                SideEffects::modifyExternal, "metal_new_buffer")
                    ->args({"device", "bytes", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_buffer_untracked)>(*this, lib, "metal_new_buffer_untracked",
                SideEffects::modifyExternal, "metal_new_buffer_untracked")
                    ->args({"device", "bytes", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_buffer_no_copy)>(*this, lib, "metal_new_buffer_no_copy",
                SideEffects::modifyExternal, "metal_new_buffer_no_copy")
                    ->args({"device", "data", "bytes", "context", "at"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(metal_new_buffer_no_copy_untracked)>(*this, lib, "metal_new_buffer_no_copy_untracked",
                SideEffects::modifyExternal, "metal_new_buffer_no_copy_untracked")
                    ->args({"device", "data", "bytes", "context", "at"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(metal_max_buffer_length)>(*this, lib, "metal_max_buffer_length",
                SideEffects::accessExternal, "metal_max_buffer_length")
                    ->args({"device", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_buffer_contents)>(*this, lib, "metal_buffer_contents",
                SideEffects::modifyExternal, "metal_buffer_contents")
                    ->args({"buffer", "context", "at"});

            addExtern<DAS_BIND_FUN(metal_new_command_buffer)>(*this, lib, "metal_new_command_buffer",
                SideEffects::modifyExternal, "metal_new_command_buffer")
                    ->args({"queue", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_command_buffer_unretained)>(*this, lib, "metal_new_command_buffer_unretained",
                SideEffects::modifyExternal, "metal_new_command_buffer_unretained")
                    ->args({"queue", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_compute_encoder)>(*this, lib, "metal_new_compute_encoder",
                SideEffects::modifyExternal, "metal_new_compute_encoder")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_compute_encoder_concurrent)>(*this, lib, "metal_new_compute_encoder_concurrent",
                SideEffects::modifyExternal, "metal_new_compute_encoder_concurrent")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_memory_barrier)>(*this, lib, "metal_memory_barrier",
                SideEffects::modifyExternal, "metal_memory_barrier")
                    ->args({"encoder", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_new_shared_event)>(*this, lib, "metal_new_shared_event",
                SideEffects::modifyExternal, "metal_new_shared_event")
                    ->args({"device", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_cb_wait_for_event)>(*this, lib, "metal_cb_wait_for_event",
                SideEffects::modifyExternal, "metal_cb_wait_for_event")
                    ->args({"command_buffer", "event", "value", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_shared_event_signal)>(*this, lib, "metal_shared_event_signal",
                SideEffects::modifyExternal, "metal_shared_event_signal")
                    ->args({"event", "value", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_set_pipeline)>(*this, lib, "metal_set_pipeline",
                SideEffects::modifyExternal, "metal_set_pipeline")
                    ->args({"encoder", "pipeline", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_set_buffer)>(*this, lib, "metal_set_buffer",
                SideEffects::modifyExternal, "metal_set_buffer")
                    ->args({"encoder", "buffer", "offset", "index", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_set_bytes)>(*this, lib, "metal_set_bytes",
                SideEffects::modifyExternal, "metal_set_bytes")
                    ->args({"encoder", "data", "len", "index", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_set_threadgroup_memory_length)>(*this, lib, "metal_set_threadgroup_memory_length",
                SideEffects::modifyExternal, "metal_set_threadgroup_memory_length")
                    ->args({"encoder", "length", "index", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_dispatch_threadgroups)>(*this, lib, "metal_dispatch_threadgroups",
                SideEffects::modifyExternal, "metal_dispatch_threadgroups")
                    ->args({"encoder", "groups", "threads_per_group", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_dispatch_threads)>(*this, lib, "metal_dispatch_threads",
                SideEffects::modifyExternal, "metal_dispatch_threads")
                    ->args({"encoder", "threads", "threads_per_group", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_end_encoding)>(*this, lib, "metal_end_encoding",
                SideEffects::modifyExternal, "metal_end_encoding")
                    ->args({"encoder", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_commit)>(*this, lib, "metal_commit",
                SideEffects::modifyExternal, "metal_commit")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_wait_until_completed)>(*this, lib, "metal_wait_until_completed",
                SideEffects::modifyExternal, "metal_wait_until_completed")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_command_buffer_error)>(*this, lib, "metal_command_buffer_error",
                SideEffects::modifyExternal, "metal_command_buffer_error")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_command_buffer_gpu_start_time)>(*this, lib, "metal_command_buffer_gpu_start_time",
                SideEffects::modifyExternal, "metal_command_buffer_gpu_start_time")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_command_buffer_gpu_end_time)>(*this, lib, "metal_command_buffer_gpu_end_time",
                SideEffects::modifyExternal, "metal_command_buffer_gpu_end_time")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_command_buffer_kernel_start_time)>(*this, lib, "metal_command_buffer_kernel_start_time",
                SideEffects::modifyExternal, "metal_command_buffer_kernel_start_time")
                    ->args({"command_buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_command_buffer_kernel_end_time)>(*this, lib, "metal_command_buffer_kernel_end_time",
                SideEffects::modifyExternal, "metal_command_buffer_kernel_end_time")
                    ->args({"command_buffer", "context", "at"});

            addExtern<DAS_BIND_FUN(metal_new_residency_set)>(*this, lib, "metal_new_residency_set",
                SideEffects::modifyExternal, "metal_new_residency_set")
                    ->args({"device", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_residency_set_add_buffer)>(*this, lib, "metal_residency_set_add_buffer",
                SideEffects::modifyExternal, "metal_residency_set_add_buffer")
                    ->args({"residency_set", "buffer", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_residency_set_commit)>(*this, lib, "metal_residency_set_commit",
                SideEffects::modifyExternal, "metal_residency_set_commit")
                    ->args({"residency_set", "context", "at"});
            addExtern<DAS_BIND_FUN(metal_residency_set_request)>(*this, lib, "metal_residency_set_request",
                SideEffects::modifyExternal, "metal_residency_set_request")
                    ->args({"residency_set", "context", "at"});

            addExtern<DAS_BIND_FUN(metal_release_device)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_device")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_queue)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_queue")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_library)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_library")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_function)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_function")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_pipeline)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_pipeline")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_buffer)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_buffer")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_command_buffer)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_command_buffer")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_encoder)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_encoder")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_shared_event)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_shared_event")->args({"handle"});
            addExtern<DAS_BIND_FUN(metal_release_residency_set)>(*this, lib, "metal_release",
                SideEffects::modifyExternal, "metal_release_residency_set")->args({"handle"});

            addExtern<DAS_BIND_FUN(metal_live_object_count)>(*this, lib, "metal_live_object_count",
                SideEffects::modifyExternal, "metal_live_object_count");
            addExtern<DAS_BIND_FUN(metal_live_object_report)>(*this, lib, "metal_live_object_report",
                SideEffects::modifyExternal, "metal_live_object_report")
                    ->args({"context", "at"});
            addExtern<DAS_BIND_FUN(metal_dispatch_call_count)>(*this, lib, "metal_dispatch_call_count",
                SideEffects::modifyExternal, "metal_dispatch_call_count");
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"../modules/dasMetal/src/dasMetal.h\"\n";
            return ModuleAotType::cpp;
        }
    };

    REGISTER_DYN_MODULE(Module_DasMetal, Module_DasMetal);
}

REGISTER_MODULE_IN_NAMESPACE(Module_DasMetal, das);
