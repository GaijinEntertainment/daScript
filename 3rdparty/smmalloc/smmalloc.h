// The MIT License (MIT)
//
// 	Copyright (c) 2017-2018 Sergey Makeev
//
// 	Permission is hereby granted, free of charge, to any person obtaining a copy
// 	of this software and associated documentation files (the "Software"), to deal
// 	in the Software without restriction, including without limitation the rights
// 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// 	copies of the Software, and to permit persons to whom the Software is
// 	furnished to do so, subject to the following conditions:
//
//      The above copyright notice and this permission notice shall be included in
// 	all copies or substantial portions of the Software.
//
// 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// 	THE SOFTWARE.
#pragma once

#include <stdint.h>
#include <algorithm>
#include <array>
#include <memory>
#include <atomic>
#include <initializer_list>
#include <cstring>

//#define SMMALLOC_STATS_SUPPORT




#if __GNUC__ || __INTEL_COMPILER

#define SM_UNLIKELY(expr)						__builtin_expect(!!(expr),(0))
#define SM_LIKELY(expr)							__builtin_expect(!!(expr),(1))

#else

#define SM_UNLIKELY(expr)			(expr)
#define SM_LIKELY(expr)				(expr)

#endif


#ifdef _DEBUG
#define SMMALLOC_ENABLE_ASSERTS
#endif


#if defined(_M_X64) || _LP64
#define SMMMALLOC_X64
#define SMM_MAX_CACHE_ITEMS_COUNT (7)
#else
#define SMMMALLOC_X86
#define SMM_MAX_CACHE_ITEMS_COUNT (10)
#endif

#define SMM_CACHE_LINE_SIZE (64)
#define SMM_MAX_BUCKET_COUNT (8)


#define SMMALLOC_UNUSED(x) (void)(x)
#define SMMALLOC_USED_IN_ASSERT(x) (void)(x)

#ifdef _MSC_VER
#define INLINE __forceinline
//#define INLINE inline
#else
//#define INLINE inline __attribute__((always_inline))
#define INLINE inline
#endif

#ifdef _MSC_VER
#  define NOINLINE __declspec(noinline)
#else
#  define NOINLINE __attribute__((__noinline__))
#endif


#ifdef SMMALLOC_ENABLE_ASSERTS
#include <assert.h>
//#define SM_ASSERT(x) assert(x)
#define SM_ASSERT(cond) do { if (!(cond)) __debugbreak(); } while (0)
#else
#define SM_ASSERT(x)
#endif

namespace sm
{
#ifdef SMMALLOC_STATS_SUPPORT
	struct AllocatorStats
	{
		std::atomic<size_t> cacheHitCount;
		std::atomic<size_t> hitCount;
		std::atomic<size_t> missCount;
		std::atomic<size_t> freeCount;

		AllocatorStats()
		{
			cacheHitCount.store(0);
			hitCount.store(0);
			missCount.store(0);
			freeCount.store(0);
		}
	};

#endif

	enum CacheWarmupOptions
	{
		CACHE_COLD = 0,
		CACHE_WARM = 1,
		CACHE_HOT = 2,
	};

	
	namespace internal
	{
		struct TlsPoolBucket;
	}


	internal::TlsPoolBucket* __restrict GetTlsBucket(size_t index);

	INLINE bool IsAligned(size_t v, size_t alignment)
	{
		size_t lowBits = v & (alignment - 1);
		return (lowBits == 0);
	}

	INLINE size_t Align(size_t val, size_t alignment)
	{
		SM_ASSERT((alignment & (alignment - 1)) == 0 && "Invalid alignment. Must be power of two.");
		size_t r = (val + (alignment - 1)) & ~(alignment - 1);
		SM_ASSERT(IsAligned(r, alignment) && "Alignment failed.");
		return r;
	}



	INLINE size_t DetectAlignment(void* p)
	{
		uintptr_t v = (uintptr_t)p;

		size_t ptrBitsCount = sizeof(void*) * 8;
		size_t i;
		for (i = 0; i < ptrBitsCount; i++)
		{
			if (v & 1)
			{
				break;
			}
			v = v >> 1;
		}
		return (size_t(1) << i);
	}

	struct GenericAllocator
	{
		typedef void* TInstance;

		static TInstance Invalid();
		static bool IsValid(TInstance instance);

		static TInstance Create();
		static void Destroy(TInstance instance);

		static void* Alloc(TInstance instance, size_t bytesCount, size_t alignment);
		static void Free(TInstance instance, void* p);
		static void* Realloc(TInstance instance, void* p, size_t bytesCount, size_t alignment);
		static size_t GetUsableSpace(TInstance instance, void* p);

		struct Deleter
		{
			explicit Deleter(GenericAllocator::TInstance _instance)
				: instance(_instance)
			{
			}

			INLINE void operator()(uint8_t* p) { GenericAllocator::Free(instance, p); }
			GenericAllocator::TInstance instance;
		};

	};



	class Allocator
	{

	private:

		static const size_t MaxValidAlignment = 16384;

		friend struct internal::TlsPoolBucket;

		INLINE bool IsReadable(void* p) const
		{
			return (uintptr_t(p) > MaxValidAlignment);
		}

		struct PoolBucket
		{
			union TaggedIndex
			{
				struct
				{
					uint32_t tag;
					uint32_t offset;
				} p;
				uint64_t u;
				static const uint64_t Invalid = UINT64_MAX;
			};

			std::atomic<uint64_t> head;
			std::atomic<uint32_t> globalTag;
			uint8_t* pData;
			uint8_t* pBufferEnd;

#ifdef SMMALLOC_STATS_SUPPORT
			AllocatorStats stats;
#endif
			PoolBucket()
				: head(TaggedIndex::Invalid)
				, globalTag(0)
				, pData(nullptr)
				, pBufferEnd(nullptr)
			{
			}

			void Create(size_t elementSize);

			INLINE void* Alloc()
			{
				uint8_t* p = nullptr;
				TaggedIndex headValue;
				headValue.u = head.load();
				while (true)
				{
					// list is empty, exiting
					if (headValue.u == TaggedIndex::Invalid)
					{
						return nullptr;
					}

					// get head pointer
					p = (pData + headValue.p.offset);
					// read head->next value
					TaggedIndex nextValue = *((TaggedIndex*)(p));

					/*
						ABA problem here.

						pop = Alloc
						push = Free

						1. Thread 0 begins a pop and sees "A" as the top, followed by "B".
						2. Thread 1 begins and completes a pop, returning "A".
						3. Thread 1 begins and completes a push of "D".
						4. Thread 1 pushes "A" back onto the stack and completes.
						5. Thread 0 sees that "A" is on top and returns "A", setting the new top to "B".
						6. Node D is lost.

						Visual representation here
						http://15418.courses.cs.cmu.edu/spring2013/lecture/lockfree/slide_016

						Solive ABA problem using taggged indexes.
						Each time when thread push to the stack, unique 'tag' increased.
						And prevent invalid CAS operation.

					*/


					// try to swap head to head->next
					if (head.compare_exchange_strong(headValue.u, nextValue.u))
					{
						// success
						break;
					}
					// can't swap values, head is changed (now headValue has new head loaded) try again
				}

				// return memory block memory
				return p;
			}

			INLINE void FreeInterval(void* _pHead, void* _pTail)
			{
				uint8_t* pHead = (uint8_t*)_pHead;
				uint8_t* pTail = (uint8_t*)_pTail;


				// attach already linked list pHead->pTail to lock free list
				// replace head and link tail with old list head

				/*

				Need to use atomic increment here.
				Without atomic increment the operation++ works like this:
				1. uint32_t temp = load(addr);
				2. temp++;
				3. store(addr, temp);

				if one of the thread will superseded before step #3, it will actually roll back the counter (for all other threads) by writing the old value into globalTag.
				*/
				uint32_t tag = globalTag.fetch_add(1, std::memory_order_relaxed);

				TaggedIndex nodeValue;
				nodeValue.p.offset = (uint32_t)(pHead - pData);
				nodeValue.p.tag = tag;

				TaggedIndex headValue;
				headValue.u = head.load();

				while (true)
				{
					// store current head as node->next
					*((TaggedIndex*)(pTail)) = headValue;

					// try to swap node and head
					if (head.compare_exchange_strong(headValue.u, nodeValue.u))
					{
						// succes
						break;
					}
					// can't swap values, head is changed (now headValue has new head loaded) try again
				}
			}

			INLINE bool IsMyAlloc(void* p) const
			{
				return (p >= pData && p < pBufferEnd);
			}


		};

	public:


		void CreateThreadCache(CacheWarmupOptions warmupOptions, std::initializer_list<uint32_t> options);
		void DestroyThreadCache();


	private:

		size_t bucketsCount;
		size_t bucketSizeInBytes;
		uint8_t* pBufferEnd;
		std::array<uint8_t*, SMM_MAX_BUCKET_COUNT> bucketsDataBegin;
		std::array<PoolBucket, SMM_MAX_BUCKET_COUNT> buckets;
		std::unique_ptr<uint8_t, GenericAllocator::Deleter> pBuffer;
		GenericAllocator::TInstance gAllocator;

#ifdef SMMALLOC_STATS_SUPPORT
		std::atomic<size_t> globalMissCount;
#endif

		INLINE void* AllocFromCache(internal::TlsPoolBucket* __restrict _self) const;

		template<bool useCacheL0>
		INLINE bool ReleaseToCache(internal::TlsPoolBucket* __restrict _self, void* _p);


		INLINE size_t FindBucket(const void* p) const
		{
			// if p is our pointer it located inside pBuffer and pBufferEnd
			// pBuffer and bucketsDataBegin[0] the same pointers
			uintptr_t index = (uintptr_t)p - (uintptr_t)bucketsDataBegin[0];

			// if p is less than pBuffer we get very huge number due to overflow and this is valid result, since bucketIndex checked before use
			// if p is greater than pBufferEnd we get huge number and this is valid result too, since bucketIndex checked before use

			size_t r = (index / bucketSizeInBytes);
			return r;
		}


		INLINE PoolBucket* GetBucketByIndex(size_t bucketIndex)
		{
			if (bucketIndex >= bucketsCount)
			{
				return nullptr;
			}
			return &buckets[bucketIndex];
		}

		INLINE const PoolBucket* GetBucketByIndex(size_t bucketIndex) const
		{
			if (bucketIndex >= bucketsCount)
			{
				return nullptr;
			}
			return &buckets[bucketIndex];
		}


		template<bool enableStatistic>
		INLINE void* Allocate(size_t _bytesCount, size_t alignment)
		{
			SM_ASSERT(alignment <= MaxValidAlignment);

			// http://www.cplusplus.com/reference/cstdlib/malloc/
			// If size is zero, the return value depends on the particular library implementation (it may or may not be a null pointer),
			// but the returned pointer shall not be dereferenced.
			if (SM_UNLIKELY(_bytesCount == 0))
			{
				return (void*)alignment;
			}

			size_t bytesCount = (_bytesCount < alignment) ? alignment : _bytesCount;
			size_t bucketIndex = ((bytesCount - 1) >> 4);

			if (bucketIndex < bucketsCount)
			{
				// try to handle allocation using local thread cache
				void* pRes = AllocFromCache(GetTlsBucket(bucketIndex));
				if (pRes)
				{
#ifdef SMMALLOC_STATS_SUPPORT
					if (enableStatistic)
					{
						buckets[bucketIndex].stats.cacheHitCount.fetch_add(1, std::memory_order_relaxed);
					}
#endif
					return pRes;
				}
			}


			while (bucketIndex < bucketsCount)
			{
				void* pRes = buckets[bucketIndex].Alloc();
				if (pRes)
				{
#ifdef SMMALLOC_STATS_SUPPORT
					if (enableStatistic)
					{
						buckets[bucketIndex].stats.hitCount.fetch_add(1, std::memory_order_relaxed);
					}
#endif
					return pRes;
				}
				else
				{
#ifdef SMMALLOC_STATS_SUPPORT
					if (enableStatistic)
					{
						buckets[bucketIndex].stats.missCount.fetch_add(1, std::memory_order_relaxed);
					}
#endif
				}

				// try next bucket
				bucketIndex++;
			}


#ifdef SMMALLOC_STATS_SUPPORT
			if (enableStatistic)
			{
				globalMissCount.fetch_add(1, std::memory_order_relaxed);
			}
#endif
			// fallback to generic allocator
			return GenericAllocator::Alloc(gAllocator, _bytesCount, alignment);
		}

	public:

		Allocator(GenericAllocator::TInstance allocator);

		void Init(uint32_t bucketsCount, size_t bucketSizeInBytes);

		INLINE void* Alloc(size_t _bytesCount, size_t alignment)
		{
			return Allocate<true>(_bytesCount, alignment);
		}


		INLINE void Free(void* p)
		{
			// Assume that p is the pointer that is allocated by passing the zero size.
			if (SM_UNLIKELY(!IsReadable(p)))
			{
				return;
			}

			size_t bucketIndex = FindBucket(p);
			if (bucketIndex < bucketsCount)
			{
#ifdef SMMALLOC_STATS_SUPPORT
				buckets[bucketIndex].stats.freeCount.fetch_add(1, std::memory_order_relaxed);
#endif

				if (ReleaseToCache<true>(GetTlsBucket(bucketIndex), p))
				{
					return;
				}

				PoolBucket* bucket = &buckets[bucketIndex];
				bucket->FreeInterval(p, p);
				return;
			}

			//fallback to generic allocator
			GenericAllocator::Free(gAllocator, (uint8_t*)p);
		}

		INLINE void* Realloc(void* p, size_t bytesCount, size_t alignment)
		{
			if (!IsReadable(p))
			{
				return Alloc(bytesCount, alignment);
			}

			if (bytesCount == 0)
			{
				Free(p);
				return (void*)alignment;
			}

			size_t bucketIndex = FindBucket(p);
			if (bucketIndex < bucketsCount)
			{
				size_t elementSize = GetBucketElementSize(bucketIndex);
				if (bytesCount <= elementSize)
				{
					//reuse existing memory
					return p;
				}

				// alloc new memory block and move memory
				void* p2 = Alloc(bytesCount, alignment);

				// Assume that p is the pointer that is allocated by passing the zero size. No preserve memory conents is requried.
				if (IsReadable(p))
				{
					//move the memory block to the new location
					std::memmove(p2, p, elementSize);
				}

				Free(p);

				return p2;
			}

			if (bytesCount == 0)
			{
				// http://www.cplusplus.com/reference/cstdlib/realloc/
				// If size is zero, the return value depends on the particular library implementation (it may or may not be a null pointer),
				// but the returned pointer shall not be dereferenced.
				if (IsReadable(p))
				{
					GenericAllocator::Free(gAllocator, p);
				}

				return (void*)alignment;
			}

			// Assume that p is the pointer that is allocated by passing the zero size. So no real reallocation required.
			if (!IsReadable(p))
			{
				return GenericAllocator::Alloc(gAllocator, bytesCount, alignment);
			}

			return GenericAllocator::Realloc(gAllocator, p, bytesCount, alignment);
		}


		INLINE size_t GetUsableSize(void* p)
		{
			// Assume that p is the pointer that is allocated by passing the zero size.
			if (!IsReadable(p))
			{
				return 0;
			}

			size_t bucketIndex = FindBucket(p);
			if (bucketIndex < bucketsCount)
			{
				size_t elementSize = GetBucketElementSize(bucketIndex);
				return elementSize;
			}

			return GenericAllocator::GetUsableSpace(gAllocator, p);
		}

		INLINE int32_t GetBucketIndex(void* _p)
		{
			if (!IsMyAlloc(_p))
			{
				return -1;
			}

			size_t bucketIndex = FindBucket(_p);
			if (bucketIndex >= bucketsCount)
			{
				return -1;
			}

			return (int32_t)bucketIndex;
		}

		INLINE bool IsMyAlloc(const void* p) const
		{
			return(p >= pBuffer.get() && p < pBufferEnd);
		}

		INLINE size_t GetBucketsCount() const
		{
			return bucketsCount;
		}

		INLINE uint32_t GetBucketElementSize(size_t bucketIndex) const
		{
			return (uint32_t)((bucketIndex + 1) * 16);
		}

		INLINE uint32_t GetBucketElementsCount(size_t bucketIndex) const
		{
			if (bucketIndex >= bucketsCount)
			{
				return 0;
			}

			size_t oneElementSize = GetBucketElementSize(bucketIndex);
			return (uint32_t)(bucketSizeInBytes / oneElementSize);
		}


#ifdef SMMALLOC_STATS_SUPPORT

		size_t GetGlobalMissCount() const
		{
			return globalMissCount.load(std::memory_order_relaxed);
		}

		const AllocatorStats* GetBucketStats(size_t bucketIndex) const
		{
			const PoolBucket* bucket = GetBucketByIndex(bucketIndex);
			if (!bucket)
			{
				return nullptr;
			}
			return &bucket->stats;
		}

#endif

		GenericAllocator::TInstance GetGenericAllocatorInstance()
		{
			return gAllocator;
		}
};



namespace internal
{
	struct TlsPoolBucket
	{
		uint8_t* pBucketData;                     // 8 (4)
		uint32_t* pStorageL1;                     // 8 (4)
		Allocator::PoolBucket* pBucket;           // 8 (4)

		std::array<uint32_t, SMM_MAX_CACHE_ITEMS_COUNT> storageL0;         //

		uint32_t maxElementsCount;                // 4
		uint32_t numElementsL1;                   // 4
		uint8_t numElementsL0;                    // 1

		                                          // sizeof(storageL0) + 33 bytes

		INLINE uint32_t GetElementsCount() const
		{
			return numElementsL1 + numElementsL0;
		}

		void Init(uint32_t* pCacheStack, uint32_t maxElementsNum, CacheWarmupOptions warmupOptions, Allocator* alloc, size_t bucketIndex);
		uint32_t* Destroy();


		INLINE void ReturnL1CacheToMaster(uint32_t count)
		{
			if (count == 0)
			{
				return;
			}

			SM_ASSERT(pBucket != nullptr);
			if (numElementsL1 == 0)
			{
				return;
			}

			count = std::min(count, numElementsL1);

			uint32_t localTag = 0xFFFFFF;
			uint32_t firstElementToReturn = (numElementsL1 - count);
			uint32_t offset = pStorageL1[firstElementToReturn];
			uint8_t* pHead = pBucketData + offset;
			uint8_t* pPrevBlockMemory = pHead;

			for (uint32_t i = (firstElementToReturn + 1); i < numElementsL1; i++, localTag++)
			{
				offset = pStorageL1[i];

				Allocator::PoolBucket::TaggedIndex* pTag = (Allocator::PoolBucket::TaggedIndex*)pPrevBlockMemory;
				pTag->p.tag = localTag;
				pTag->p.offset = offset;

				uint8_t* pBlockMemory = pBucketData + offset;
				pPrevBlockMemory = pBlockMemory;
			}

			uint8_t* pTail = pPrevBlockMemory;
			pBucket->FreeInterval(pHead, pTail);

			numElementsL1 -= count;
		}
	};

	static_assert(std::is_pod<TlsPoolBucket>::value == true, "TlsPoolBucket must be POD type, stored in TLS");
	static_assert(sizeof(TlsPoolBucket) <= 64, "TlsPoolBucket sizeof must be less than CPU cache line");
}




INLINE void* Allocator::AllocFromCache(internal::TlsPoolBucket* __restrict _self) const
{
	if (_self->numElementsL0 > 0)
	{
		SM_ASSERT(_self->pBucketData != nullptr);
		_self->numElementsL0--;
		uint32_t offset = _self->storageL0[_self->numElementsL0];
		return _self->pBucketData + offset;
	}

	if (_self->numElementsL1 > 0)
	{
		SM_ASSERT(_self->pBucketData != nullptr);
		SM_ASSERT(_self->numElementsL0 == 0);
		_self->numElementsL1--;
		uint32_t offset = _self->pStorageL1[_self->numElementsL1];
		return _self->pBucketData + offset;
	}
	return nullptr;
}

template<bool useCacheL0>
INLINE bool Allocator::ReleaseToCache(internal::TlsPoolBucket* __restrict _self, void* _p)
{

	if (_self->maxElementsCount == 0)
	{
		//empty cache bucket
		return false;
	}

	SM_ASSERT(_self->pBucket != nullptr);
	SM_ASSERT(_self->pBucketData != nullptr);

	//get offset
	uint8_t* p = (uint8_t*)_p;
	SM_ASSERT(p >= _self->pBucketData && p < _self->pBucket->pBufferEnd);
	uint32_t offset = (uint32_t)(p - _self->pBucketData);

	if (useCacheL0)
	{
		// use L0 storage if available
		if (_self->numElementsL0 < SMM_MAX_CACHE_ITEMS_COUNT)
		{
			_self->storageL0[_self->numElementsL0] = offset;
			_self->numElementsL0++;
			return true;
		}
	}

	if (_self->numElementsL1 < _self->maxElementsCount)
	{
		//use L1 storage if available
		_self->pStorageL1[_self->numElementsL1] = offset;
		_self->numElementsL1++;
		return true;
	}

	//
	// too many elements in cache (return half of the cache to master)
	//    minimizing worst case scenario, cache is full and thread continues to Free a lot of blocks.
	//               and each Free() call leads to an operation with the global lock-free pool

	uint32_t halfOfElements = (_self->numElementsL1 >> 1);
	_self->ReturnL1CacheToMaster(halfOfElements);

	// use L1 storage
	_self->pStorageL1[_self->numElementsL1] = offset;
	_self->numElementsL1++;
	return true;
}





}



#define SMMALLOC_CSTYLE_FUNCS


#ifdef SMMALLOC_CSTYLE_FUNCS

#define SMMALLOC_DLL

#if defined(_WIN32) && defined(SMMALLOC_DLL)
    #define SMMALLOC_API __declspec(dllexport)
#else
    #define SMMALLOC_API extern
#endif

#ifdef __cplusplus
extern "C"
{
#endif


//
// C style api
//
////////////////////////////////////////////////////////////////////////////////

typedef sm::Allocator* sm_allocator;

SMMALLOC_API INLINE sm_allocator _sm_allocator_create(uint32_t bucketsCount, size_t bucketSizeInBytes)
{
	sm::GenericAllocator::TInstance instance = sm::GenericAllocator::Create();
	if (!sm::GenericAllocator::IsValid(instance))
	{
		return nullptr;
	}

	//allocate memory for an allocator
	size_t align = __alignof(sm::Allocator);
	align = sm::Align(align, SMM_CACHE_LINE_SIZE);

	void* pBuffer = sm::GenericAllocator::Alloc(instance, sizeof(sm::Allocator), align);

	//placement new
	sm::Allocator* allocator = new(pBuffer) sm::Allocator(instance);

	//initialize
	allocator->Init(bucketsCount, bucketSizeInBytes);

	return allocator;
}

SMMALLOC_API INLINE void _sm_allocator_destroy(sm_allocator allocator)
{
	if (allocator == nullptr)
	{
		return;
	}

	sm::GenericAllocator::TInstance instance = allocator->GetGenericAllocatorInstance();

	//call dtor
	allocator->~Allocator();

	//free memory
	sm::GenericAllocator::Free(instance, allocator);

	//destroy generic allocator
	sm::GenericAllocator::Destroy(instance);
}

SMMALLOC_API INLINE void _sm_allocator_thread_cache_create(sm_allocator allocator, sm::CacheWarmupOptions warmupOptions, std::initializer_list<uint32_t> options)
{
	if (allocator == nullptr)
	{
		return;
	}

	allocator->CreateThreadCache(warmupOptions, options);
}

SMMALLOC_API INLINE void _sm_allocator_thread_cache_destroy(sm_allocator allocator)
{
	if (allocator == nullptr)
	{
		return;
	}

	allocator->DestroyThreadCache();
}

SMMALLOC_API INLINE void* _sm_malloc(sm_allocator allocator, size_t bytesCount, size_t alignment)
{
	return allocator->Alloc(bytesCount, alignment);
}

SMMALLOC_API INLINE void _sm_free(sm_allocator allocator, void* p)
{
	return allocator->Free(p);
}

SMMALLOC_API INLINE void* _sm_realloc(sm_allocator allocator, void* p, size_t bytesCount, size_t alignment)
{
	return allocator->Realloc(p, bytesCount, alignment);
}

SMMALLOC_API INLINE size_t _sm_msize(sm_allocator allocator, void* p)
{
	return allocator->GetUsableSize(p);
}

SMMALLOC_API INLINE int32_t _sm_mbucket(sm_allocator allocator, void* p)
{
	return allocator->GetBucketIndex(p);
}


#ifdef __cplusplus
}
#endif


#endif
