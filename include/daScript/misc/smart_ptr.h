#pragma once

namespace das {

    template<typename T> 
    class smart_ptr {
    public:
        using element_type = T;
        __forceinline smart_ptr ( ) { 
            ptr = nullptr;
        }
        __forceinline smart_ptr ( const smart_ptr & p ) { 
            init(p.ptr);
        }
        template <typename Y>
        __forceinline smart_ptr ( const smart_ptr<Y> & p ) { 
            init(p.get());
        }
        __forceinline smart_ptr ( smart_ptr && p ) {
            ptr = p.ptr;
            p.ptr = nullptr;
        }
        __forceinline smart_ptr ( T * p ) { 
            init(p);
        }
        __forceinline ~smart_ptr() {
            reset();
        }
        __forceinline smart_ptr & operator = ( const smart_ptr & p ) {
            return set(p.ptr);
        }
        template <typename Y>
        __forceinline smart_ptr & operator = ( const smart_ptr<Y> & p ) {
            return set(p.get());
        }
        __forceinline smart_ptr & operator = ( smart_ptr && p ) {
            ptr = p.ptr;
            p.ptr = nullptr;
            return *this;
        }
        __forceinline smart_ptr & operator = ( T * p ) {
            return set(p);
        }
        __forceinline void reset() {
            T * t = ptr;
            ptr = nullptr;
            if ( t ) t->delRef();
        }
        __forceinline void swap ( smart_ptr & p ) {
            T * t = ptr;
            ptr = p.ptr;
            p.ptr = t;
        }
        __forceinline void reset ( T * p ) { 
            set(p); 
        }
        __forceinline T & operator * () const { 
            return *ptr; 
        }
        __forceinline T * operator -> () const { 
            return ptr; 
        }
        __forceinline T * get() const { 
            return ptr; 
        }
        __forceinline void addRef() { 
            if ( ptr ) ptr->addRef(); 
        }
        __forceinline void delRef() { 
            if ( ptr && ptr->delRef() ) ptr = nullptr;
        }
        __forceinline operator bool() const { 
            return ptr != nullptr; 
        }
        __forceinline bool operator == ( T * p ) const {
            return ptr == p;
        }
        template <typename Y>
        __forceinline bool operator == ( const smart_ptr<Y> & p ) const {
            return ptr == p.get();
        }
        __forceinline bool operator != ( T * p ) const {
            return ptr != p;
        }
        template <typename Y>
        __forceinline bool operator != ( const smart_ptr<Y> & p ) const {
            return ptr != p.get();
        }
        __forceinline bool operator >= ( T * p ) const {
            return ptr >= p;
        }
        template <typename Y>
        __forceinline bool operator >= ( const smart_ptr<Y> & p ) const {
            return ptr >= p.get();
        }
        __forceinline bool operator <= ( T * p ) const {
            return ptr <= p;
        }
        template <typename Y>
        __forceinline bool operator <= ( const smart_ptr<Y> & p ) const {
            return ptr <= p.get();
        }
        __forceinline bool operator > ( T * p ) const {
            return ptr > p;
        }
        template <typename Y>
        __forceinline bool operator > ( const smart_ptr<Y> & p ) const {
            return ptr > p.get();
        }
        __forceinline bool operator < ( T * p ) const {
            return ptr < p;
        }
        template <typename Y>
        __forceinline bool operator < ( const smart_ptr<Y> & p ) const {
            return ptr < p.get();
        }
    protected:
        __forceinline smart_ptr & set ( T * p )  { 
            T * t = ptr;
            ptr = p;
            addRef();
            if ( t ) t->delRef();
            return *this;
        }
        __forceinline void init ( T * p = nullptr )  { 
            ptr = p; 
            addRef(); 
        }
    protected:
        T * ptr;
    };

    template <class T, class U>
    __forceinline bool operator == (T * l, const smart_ptr<U> & r) {
        return l == r.get();
    }
    template <class T, class U>
    __forceinline bool operator != (T * l, const smart_ptr<U> & r) {
        return l != r.get();
    }
    template <class T, class U>
    __forceinline bool operator >= (T * l, const smart_ptr<U> & r) {
        return l >= r.get();
    }
    template <class T, class U>
    __forceinline bool operator <= (T * l, const smart_ptr<U> & r) {
        return l <= r.get();
    }
    template <class T, class U>
    __forceinline bool operator > (T * l, const smart_ptr<U> & r) {
        return l > r.get();
    }
    template <class T, class U>
    __forceinline bool operator < (T * l, const smart_ptr<U> & r) {
        return l < r.get();
    }

    template< class T, class... Args >
    inline smart_ptr<T> make_smart ( Args&&... args ) {
        T * p = new T(args...);
        return smart_ptr<T>(p);
    }

    template< class T, class U >
    inline smart_ptr<T> static_pointer_cast(const smart_ptr<U> & r) {
        return static_cast<T *>(r.get());
    }
    
    template< class T, class U >
    inline smart_ptr<T> dynamic_pointer_cast(const smart_ptr<U> & r) {
        return dynamic_cast<T *>(r.get());
    }

    template< class T, class U >
    inline smart_ptr<T> const_pointer_cast(const smart_ptr<U> & r) {
        return const_cast<T *>(r.get());
    }

    template< class T, class U >
    __forceinline smart_ptr<T> reinterpret_pointer_cast(const smart_ptr<U> & r) {
        return reinterpret_cast<T *>(r.get());
    }

    class ptr_ref_count {
        template <typename TT>
        friend class smart_ptr;
    public:
        __forceinline ptr_ref_count () {}
        __forceinline ptr_ref_count ( const ptr_ref_count &  ) {}
        __forceinline ptr_ref_count ( const ptr_ref_count && ) {}
        __forceinline ptr_ref_count & operator = ( const ptr_ref_count & ) { return *this;}
        __forceinline ptr_ref_count & operator = ( ptr_ref_count && ) { return *this; }
        virtual ~ptr_ref_count() {
            DAS_ASSERTF(ref_count == 0, "can only delete when ref_count==0");
        }
    protected:
        __forceinline void addRef() {
            ref_count ++;
            DAS_ASSERTF(ref_count, "ref_count overflow");
        }
        __forceinline bool delRef() {
            DAS_ASSERTF(ref_count, "deleting reference on the object with ref_count==0");
            if ( --ref_count==0 ) {
                delete this;
                return true;
            } else {
                return false;
            }
        }
    private:
        unsigned int ref_count = 0;
    };
}
