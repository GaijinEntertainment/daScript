#pragma once

#define DAS_FREE_LIST   0

#if DAS_FREE_LIST

namespace das {
    class FreeList {
    public:
        FreeList( uint32_t os, uint32_t gs = 64 ) {
            objectSize = os;
            growSpeed = gs;
        }
        ~FreeList() {
            clear();
        }
        void clear() {
            DAS_ASSERT(totalObjects == 0);
            for ( auto chunk : chunks ) {
                das_aligned_free16(chunk);
            }
            chunks.clear();
        }
        __forceinline char * allocate () {
            if ( !freeList ) grow();
            char * result = freeList;
            freeList = *(char **)freeList;
            totalObjects ++;
            return result;
        }
        __forceinline void free ( char * ptr ) {
            *(char **)ptr = freeList;
            freeList = ptr;
            DAS_ASSERT(totalObjects > 0);
            totalObjects --;
        }
        void grow() {
            char * chunk = (char *) das_aligned_alloc16(objectSize * growSpeed);
            chunks.push_back(chunk);
            char * head = chunk;
            char * last = chunk + (growSpeed-1) * objectSize;
            while ( head != last ) {
                *((char **)head) = head + objectSize;
                head += objectSize;
            }
            *((char **)head) = freeList;
            freeList = chunk;
        }
    protected:
        char *          freeList = nullptr;
        uint32_t        objectSize = 0;
        uint32_t        growSpeed = 64;
        uint64_t        totalObjects = 0;
        vector<char *>  chunks;
    };
}

#endif

#define DAS_MAX_REUSE_SIZE      256
#define DAS_MAX_BUCKET_COUNT    (DAS_MAX_REUSE_SIZE>>4)

namespace das {
    void * reuse_cache_allocate ( size_t size );
    void reuse_cache_free ( void * ptr, size_t size );
    void reuse_cache_free ( void * ptr );
    void reuse_cache_create();
    void reuse_cache_clear();
    void reuse_cache_destroy();
    void reuse_cache_push();
    void reuse_cache_pop();

    struct ReuseCacheGuard {
        ReuseCacheGuard() { reuse_cache_push(); }
        ~ReuseCacheGuard() { reuse_cache_pop(); }
    };

    template <typename TT>
    struct ReuseAllocator {
        void * operator new ( size_t size ) {
            return reuse_cache_allocate(size);
        }
        void * operator new[] ( size_t size ) {
            return reuse_cache_allocate(size);
        }
        void operator delete ( void * data ) {
            return reuse_cache_free(data, sizeof(TT));
        }
        void operator delete[] ( void * data ) {
            return reuse_cache_free(data);
        }
    };

    struct ReuseAllocatorAny {
        void * operator new ( size_t size ) {
            return reuse_cache_allocate(size);
        }
        void * operator new[] ( size_t size ) {
            return reuse_cache_allocate(size);
        }
        void operator delete ( void * data ) {
            return reuse_cache_free(data);
        }
        void operator delete[] ( void * data ) {
            return reuse_cache_free(data);
        }
    };

    template <typename T>
    class stl_reuse_allocator {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;
        stl_reuse_allocator(){}
        ~stl_reuse_allocator(){}
        template <class U> struct rebind { typedef stl_reuse_allocator<U> other; };
        template <class U> stl_reuse_allocator(const stl_reuse_allocator<U>&){}
        pointer address(reference x) const {return &x;}
        const_pointer address(const_reference x) const {return &x;}
        size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
        pointer allocate(size_type n, const void * = nullptr ) {
            return static_cast<pointer>(reuse_cache_allocate(n*sizeof(T)));
        }
        void deallocate(pointer p, size_type n) {
            reuse_cache_free(p,n);
        }
        void construct(pointer p, const T& val) {
            new(static_cast<void*>(p)) T(val);
        }
        void construct(pointer p) {
            new(static_cast<void*>(p)) T();
        }
        void destroy(pointer p) {
            p->~T();
        }
    };

   template <class T1, class T2>
   bool operator== (const stl_reuse_allocator<T1>&, const stl_reuse_allocator<T2>&) throw() {
       return true;
   }
   template <class T1, class T2>
   bool operator!= (const stl_reuse_allocator<T1>&, const stl_reuse_allocator<T2>&) throw() {
       return false;
   }
}
