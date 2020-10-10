// daScript requires built-in memory allocators to be 16 bytes aligned
// msvc in 32-bit mode often produce 8 bytes aligned allocations, hence the workaround

#if !defined(DAS_GLOBAL_NEW) && defined(_MSC_VER) && !defined(_WIN64)

void * operator new(std::size_t n) throw(std::bad_alloc)
{
    return das_aligned_alloc16(n);
}
void operator delete(void * p) throw()
{
    das_aligned_free16(p);
}

#endif
