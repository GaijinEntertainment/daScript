#pragma once
namespace das {
//todo: make aliased structure. if there is only one chunk (which is common case), we can alias this one Chunk data and array of chunks, and avoid one indirection

class BuddyAllocator
{
public:
    void        reset(uint32_t initial_size);
    bool        isHeapPtr ( const char * data ) const;
    char *      allocate ( uint32_t size );
    char *      allocateNonEmpty ( uint32_t size );//if buddy isn't empty
    bool        free ( char * data, uint32_t size );
    bool        reallocate ( char * data, uint32_t size, uint32_t newSize );
    uint32_t    calcUsed() const;
    uint32_t    getInitialSize() const;
    uint32_t    getChunksCount() const;
protected:
    struct Chunk//sizeof(Chunk) == (12) in 32 bit, and (16) in 64 bit.
    {
        char* __restrict data = nullptr;//don't use unique_ptr, so we can make restrict
        uint32_t capacity = 0;
        uint32_t used = 0;
        uint32_t getFree() const
        {
            assert(used <= capacity);
            return capacity - used;
        }
        Chunk(uint32_t size):capacity(size), data((char*)das_aligned_alloc16(size)){}
        __forceinline bool belongs(const char *__restrict ptr) const {return uintptr_t(ptr - data) < capacity;}
        __forceinline char *__restrict allocate(uint32_t sz)
        {
          char *__restrict ret = data + used;
          used += sz;
          return ret;
        }

        //Rule-of-5
        Chunk(const Chunk&) = delete;//non_copyable
        void swapChunk(Chunk&& a)
        {
            swap(data, a.data);
            swap(used, a.used);
            swap(capacity, a.capacity);
        }
        Chunk(Chunk&& a){swapChunk(move(a));}
        Chunk() = default;
        Chunk& operator = (Chunk&& a){swapChunk(move(a));return *this;}
        void reset(){if (data)das_aligned_free16(data); data = nullptr; used = capacity = 0;}
        ~Chunk(){reset();}
    };
    Chunk initial;
    vector<Chunk> chunks;//todo: we only need Chunk* and count.
	const Chunk *findBelongingChunk(const char * data) const;
	void allocateChunk(uint32_t size);
};

__forceinline const BuddyAllocator::Chunk *BuddyAllocator::findBelongingChunk(const char * data) const
{
    if (initial.belongs(data))
      return &initial;
    for (auto &c : chunks)
        if (c.belongs(data))
             return &c;
    return nullptr;
}

inline uint32_t BuddyAllocator::getInitialSize() const {return initial.capacity;}

inline uint32_t BuddyAllocator::calcUsed() const
{
    uint32_t used = initial.used;
    for (auto& c : chunks)
        used += c.used;
    return used;
}

inline void BuddyAllocator::reset(uint32_t initial_size)
{
    chunks.clear();chunks.shrink_to_fit();
    if (initial_size != initial.capacity)//avoid reallocation
        initial = Chunk(initial_size);
    else
        initial.used = 0;
}

__forceinline bool BuddyAllocator::isHeapPtr ( const char * data ) const
{
    if (initial.belongs(data))
      return true;
    for (auto &c : chunks)
        if (c.belongs(data))
          return true;
    return false;
}

inline bool BuddyAllocator::reallocate ( char * data, uint32_t size, uint32_t newSize )
{
    const Chunk *chunk = findBelongingChunk(data);
    if (!chunk)//doesn't belong to
        return false;
    const uintptr_t ofsInChunk = data - chunk->data;
    if (ofsInChunk + size == uintptr_t (chunk->used))//
    {
      if (ofsInChunk + newSize > uintptr_t (chunk->capacity))//no chance we can fit it anywhere
          return false;
        const_cast<Chunk*>(chunk)->used = chunk->used - size + newSize;//const cast just to save copy past in findBelonging non-cast version
        return true;
    }
    return false;
}

inline bool BuddyAllocator::free ( char * data, uint32_t size )
{
    return reallocate(data, size, 0);
}

inline void BuddyAllocator::allocateChunk(uint32_t size)
{
    chunks.emplace_back(size);
}

inline uint32_t BuddyAllocator::getChunksCount() const {return (uint32_t)chunks.size() + 1;}

inline char *BuddyAllocator::allocateNonEmpty ( uint32_t size )
{
    if (initial.getFree() >= size)
      return initial.allocate(size);
    if (!chunks.size() || chunks.back().getFree() < size)
    {
        const uint32_t nextSize = (chunks.size() ? chunks.back().capacity : initial.capacity)*2;
        allocateChunk(nextSize < size ? size : nextSize);
    }
    return chunks.back().allocate(size);
}

inline char *BuddyAllocator::allocate ( uint32_t size )
{
    if (!initial.capacity)
         initial = Chunk(size < 4096 ? 4096 : size);//4096 is small mem page
    return allocateNonEmpty(size);
}

};//namespace