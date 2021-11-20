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
#include "smmalloc.h"

namespace sm
{
	struct CacheWarmupLink
	{
		CacheWarmupLink* pNext;
	};


	namespace internal
	{


		void TlsPoolBucket::Init(uint32_t* pCacheStack, uint32_t maxElementsNum, CacheWarmupOptions warmupOptions, Allocator* alloc, size_t bucketIndex)
		{
			// assume thread_local variable always initialized with zeroes
			SM_ASSERT(numElementsL0 == 0);
			SM_ASSERT(numElementsL1 == 0);
			SM_ASSERT(pBucket == nullptr);
			SM_ASSERT(pBucketData == nullptr);
			SM_ASSERT(pStorageL1 == nullptr);
			SM_ASSERT(maxElementsCount == 0);

			Allocator::PoolBucket* poolBucket = alloc->GetBucketByIndex(bucketIndex);

			SM_ASSERT(maxElementsNum >= SMM_MAX_CACHE_ITEMS_COUNT + 2);
			pStorageL1 = pCacheStack;
			numElementsL1 = 0;
			numElementsL0 = 0;
			maxElementsCount = (maxElementsNum - SMM_MAX_CACHE_ITEMS_COUNT);
			pBucket = poolBucket;
			SM_ASSERT(pBucket);
			pBucketData = pBucket->pData;

			if (warmupOptions == CACHE_COLD)
			{
				return;
			}

			// warmup cache
			uint32_t elementSize = alloc->GetBucketElementSize(bucketIndex);
			uint32_t num = (warmupOptions == CACHE_WARM) ? (maxElementsCount / 2) : (maxElementsCount);

			CacheWarmupLink* pRoot = nullptr;

			for (uint32_t j = 0; j < num; j++)
			{
				//allocate from global (pThreadCache is still nullptr)
				void* p = alloc->Allocate<false>(elementSize, 16);
				if (p == nullptr)
				{
					break;
				}

				//do not steal allocations from different buckets or from generic allocator
				if (alloc->GetBucketIndex(p) != (int32_t)bucketIndex)
				{
					alloc->Free(p);
					break;
				}

				CacheWarmupLink* pItem = (CacheWarmupLink*)p;
				pItem->pNext = pRoot;
				pRoot = pItem;
			}


			//move nodes to cache but passtrhough L0 cache
			CacheWarmupLink* pCurrent = pRoot;
			while (pCurrent)
			{
				CacheWarmupLink* pNext = pCurrent->pNext;
				bool r = alloc->ReleaseToCache<false>(this, pCurrent);
				SMMALLOC_USED_IN_ASSERT(r);
				SM_ASSERT(r);
				pCurrent = pNext;
			}

			SM_ASSERT(GetElementsCount() == num);
		}

		uint32_t* TlsPoolBucket::Destroy()
		{
			// move cached allocations from L0 to L1 (we always has free space for L0 cache inside L1)
			for (uint32_t i = 0; i < numElementsL0; i++)
			{
				pStorageL1[numElementsL1] = storageL0[i];
				numElementsL1++;
			}

			//return all cached elements to master
			if (numElementsL1 > 0)
			{
				ReturnL1CacheToMaster(numElementsL1);
			}

			uint32_t* r = pStorageL1;
			pStorageL1 = nullptr;
			numElementsL0 = 0;
			numElementsL1 = 0;
			maxElementsCount = 0;
			pBucket = nullptr;
			pBucketData = nullptr;
			return r;
		}

	}

	void Allocator::CreateThreadCache(CacheWarmupOptions warmupOptions, std::initializer_list<uint32_t> options)
	{
		SM_ASSERT(bucketsCount >= options.size());

		size_t i = 0;
		for (uint32_t _elementsNum : options)
		{
			if (i >= bucketsCount)
			{
				break;
			}

			uint32_t elementsNum = _elementsNum + SMM_MAX_CACHE_ITEMS_COUNT;


			// allocate stack for cache
			uint32_t* localStack = (uint32_t*)GenericAllocator::Alloc(gAllocator, elementsNum * sizeof(uint32_t), 64);

			// initialize
			GetTlsBucket(i)->Init(localStack, elementsNum, warmupOptions, this, i);

			i++;
		}

	}

	void Allocator::DestroyThreadCache()
	{

		for (size_t i = 0; i < SMM_MAX_BUCKET_COUNT; i++)
		{
			uint32_t* p = GetTlsBucket(i)->Destroy();
			GenericAllocator::Free(gAllocator, p);
		}

	}



	void Allocator::PoolBucket::Create(size_t elementSize)
	{
		SM_ASSERT(elementSize >= 16 && "Invalid element size");

		// build inplace single linked list
		globalTag.store(0, std::memory_order_relaxed);
		uint8_t* node = pData;

		TaggedIndex headVal;
		headVal.p.tag = globalTag.load(std::memory_order_relaxed);
		headVal.p.offset = (uint32_t)(node - pData);
		head.store(headVal.u);

		while(true)
		{
			uint8_t* next = node + elementSize;
			if ((next + elementSize) <= pBufferEnd)
			{
				TaggedIndex nextVal;
				nextVal.p.tag = globalTag.load(std::memory_order_relaxed);;
				nextVal.p.offset = (uint32_t)(next - pData);
				*((TaggedIndex*)(node)) = nextVal;
			}
			else
			{
				//last node
				((TaggedIndex*)(node))->u = TaggedIndex::Invalid;
				break;
			}

			node = next;
			globalTag.fetch_add(1, std::memory_order_relaxed);
		}

	}



	Allocator::Allocator(GenericAllocator::TInstance allocator)
		: bucketsCount(0)
		, bucketSizeInBytes(0)
		, pBufferEnd(nullptr)
		, pBuffer(nullptr, GenericAllocator::Deleter(allocator))
		, gAllocator(allocator)
	{
#ifdef SMMALLOC_STATS_SUPPORT
		globalMissCount.store(0);
#endif
	}



	// Return the next power of 2 higher than the input
	// If the input is already a power of 2, the output will be the same as the input.
	// Got this from Brian Sharp's SWEng mailing list.
	inline int GetNextPow2(uint32_t n)
	{
		n -= 1;

		n |= n >> 16;
		n |= n >> 8;
		n |= n >> 4;
		n |= n >> 2;
		n |= n >> 1;

		return n + 1;
	}



	void Allocator::Init(uint32_t _bucketsCount, size_t _bucketSizeInBytes)
	{
		if (bucketsCount > 0)
		{
			//already initialized
			return;
		}

		SM_ASSERT(_bucketsCount > 0 && _bucketsCount <= 64);
		if (_bucketsCount == 0)
		{
			return;
		}


		bucketsCount = _bucketsCount;
		size_t alignmentMax = GetNextPow2((uint32_t)(16 * bucketsCount));
		bucketSizeInBytes = Align(_bucketSizeInBytes, alignmentMax);

		size_t i = 0;
		for (i = 0; i < bucketsDataBegin.size(); i++)
		{
			bucketsDataBegin[i] = nullptr;
		}


		size_t totalBytesCount = bucketSizeInBytes * bucketsCount;
		pBuffer.reset((uint8_t*)GenericAllocator::Alloc(gAllocator, totalBytesCount, alignmentMax));
		pBufferEnd = pBuffer.get() + totalBytesCount + 1;

		size_t elementSize = 16;
		for (i = 0; i < bucketsCount; i++)
		{
			PoolBucket& bucket = buckets[i];
			bucket.pData = pBuffer.get() + i * bucketSizeInBytes;
			SM_ASSERT(IsAligned((size_t)bucket.pData, GetNextPow2(elementSize)) && "Alignment failed");
			bucket.pBufferEnd = bucket.pData + bucketSizeInBytes;
			bucket.Create(elementSize);
			elementSize += 16;
			bucketsDataBegin[i] = bucket.pData;
		}
	}



}