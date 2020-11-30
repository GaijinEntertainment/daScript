#pragma once

// based on https://stackoverflow.com/a/38478032

#ifndef DAS_CALLABLE_BYTES
#define DAS_CALLABLE_BYTES  128
#endif

namespace das {

    template <typename T>
    class callable;

    template <typename R, typename... Args>
    class callable<R(Args...)> {
        typedef R (*invoke_fn_t)(char*, Args...);
        typedef void (*destroy_fn_t)(char*);
        template <typename Functor>
        static R invoke_fn(Functor* fn, Args... args) {
            return (*fn)(forward<Args>(args)...);
        }
        template <typename Functor>
        static void destroy_fn(Functor* f) {
            f->~Functor();
        }
        invoke_fn_t     invoke_f;
        destroy_fn_t    destroy_f;
        char *          data_ptr;
        vec4f           bytes[DAS_CALLABLE_BYTES/16];
    public:
        callable(callable && rhs) = delete;
        callable(callable const& rhs) = delete;
        callable() : data_ptr(nullptr) {}
        template <typename Functor>
        callable(Context * ctx, Functor f)
            : invoke_f(reinterpret_cast<invoke_fn_t>(invoke_fn<Functor>))
            , destroy_f(reinterpret_cast<destroy_fn_t>(destroy_fn<Functor>)) {
            auto data_size = sizeof(Functor);
            if ( data_size <= DAS_CALLABLE_BYTES ) {
                data_ptr = (char *) bytes;
            } else {
                data_ptr = (char *) das_aligned_alloc16(uint32_t(data_size));
            }
            new (data_ptr) Functor(f);
        }
        ~callable() {
            if ( data_ptr != nullptr ) {
                this->destroy_f(this->data_ptr);
                if ( data_ptr != (char *)bytes ) {
                    das_aligned_free16(data_ptr);
                }
            }
        }
        __forceinline R operator()(Args... args) {
            return this->invoke_f(this->data_ptr, forward<Args>(args)...);
        }
    };
}

