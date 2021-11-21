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

#ifdef _MSC_VER
	#include <malloc.h>
#elif defined(__APPLE__)
	#include <malloc/malloc.h>
	#include <stdlib.h>
#elif defined(__linux__)
	#include <stdlib.h>
	#include <malloc.h>
#else
	#include <stdlib.h>
#endif

#include "smmalloc.h"


sm::GenericAllocator::TInstance sm::GenericAllocator::Invalid()
{
	return nullptr;
}

bool sm::GenericAllocator::IsValid(TInstance instance)
{
	SMMALLOC_UNUSED(instance);
	return true;
}

sm::GenericAllocator::TInstance sm::GenericAllocator::Create()
{
	return nullptr;
}

void sm::GenericAllocator::Destroy(sm::GenericAllocator::TInstance instance)
{
	SMMALLOC_UNUSED(instance);
}

void* sm::GenericAllocator::Alloc(sm::GenericAllocator::TInstance instance, size_t bytesCount, size_t alignment)
{
	SMMALLOC_UNUSED(instance);
	if (alignment < 16)
	{
		alignment = 16;
	}
	#ifdef _MSC_VER
		return _aligned_malloc(bytesCount, alignment);
	#else
		return malloc(bytesCount);	// todo: assume reasonable alignment?
	#endif
}

void sm::GenericAllocator::Free(sm::GenericAllocator::TInstance instance, void* p)
{
	SMMALLOC_UNUSED(instance);
	#ifdef _MSC_VER
		_aligned_free(p);
	#else
		free(p);
	#endif
}

void* sm::GenericAllocator::Realloc(sm::GenericAllocator::TInstance instance, void* p, size_t bytesCount, size_t alignment)
{
	SMMALLOC_UNUSED(instance);
	#ifdef _MSC_VER
		return _aligned_realloc(p, bytesCount, alignment);
	#else
		return realloc(p, bytesCount);
	#endif
}

size_t sm::GenericAllocator::GetUsableSpace(sm::GenericAllocator::TInstance instance, void* p)
{
	SMMALLOC_UNUSED(instance);
	size_t alignment = DetectAlignment(p);
	#ifdef _MSC_VER
		return _aligned_msize(p, alignment, 0);
	#elif defined(__APPLE__)
		return malloc_size(p);
	#elif defined(__linux__)
		return malloc_usable_size(p);
	#else
		return malloc_size(p);	// ???
	#endif
}
