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
#include <malloc.h>
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
	return _aligned_malloc(bytesCount, alignment);
}

void sm::GenericAllocator::Free(sm::GenericAllocator::TInstance instance, void* p)
{
	SMMALLOC_UNUSED(instance);
	_aligned_free(p);
}

void* sm::GenericAllocator::Realloc(sm::GenericAllocator::TInstance instance, void* p, size_t bytesCount, size_t alignment)
{
	SMMALLOC_UNUSED(instance);
	return _aligned_realloc(p, bytesCount, alignment);
}

size_t sm::GenericAllocator::GetUsableSpace(sm::GenericAllocator::TInstance instance, void* p)
{
	SMMALLOC_UNUSED(instance);
	size_t alignment = DetectAlignment(p);
	#ifdef __GNUC__
		if (alignment < sizeof(void*))
			alignment = sizeof(void*);

		return _msize(p) - alignment - sizeof(void*);
	#else
		return _aligned_msize(p, alignment, 0);
	#endif
}
