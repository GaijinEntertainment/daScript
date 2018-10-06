/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2018, Weijia Song <songweijia@gmail.com>
 * Copyright (C) 2018, Sebastian Pipping <sebastian@pipping.org>
 * All rights reserved.
 *
 * Redistribution  and use in source and binary forms, with or without
 * modification,  are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions   of  source  code  must  retain  the   above
 *       copyright  notice, this list of conditions and the  following
 *       disclaimer.
 *
 *     * Redistributions  in  binary  form must  reproduce  the  above
 *       copyright  notice, this list of conditions and the  following
 *       disclaimer   in  the  documentation  and/or  other  materials
 *       provided with the distribution.
 *
 *     * Neither  the name of the <ORGANIZATION> nor the names of  its
 *       contributors  may  be  used to endorse  or  promote  products
 *       derived  from  this software without specific  prior  written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT  NOT
 * LIMITED  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS
 * FOR  A  PARTICULAR  PURPOSE ARE DISCLAIMED. IN NO EVENT  SHALL  THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL,    SPECIAL,   EXEMPLARY,   OR   CONSEQUENTIAL   DAMAGES
 * (INCLUDING,  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES;  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT  LIABILITY,  OR  TORT (INCLUDING  NEGLIGENCE  OR  OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file UriMemory.c
 * Holds memory manager implementation.
 */

#include "../config.h"

#if HAVE_REALLOCARRAY
# define _GNU_SOURCE
#else
# include <errno.h>
#endif

#include <stdlib.h>



#ifndef URI_DOXYGEN
# include "UriMemory.h"
#endif



static void * uriDefaultMalloc(UriMemoryManager * URI_UNUSED(memory),
        size_t size) {
    return malloc(size);
}



static void * uriDefaultCalloc(UriMemoryManager * URI_UNUSED(memory),
        size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}



static void * uriDefaultRealloc(UriMemoryManager * URI_UNUSED(memory),
        void * ptr, size_t size) {
    return realloc(ptr, size);
}



static void * uriDefaultReallocarray(UriMemoryManager * URI_UNUSED(memory),
        void * ptr, size_t nmemb, size_t size) {
#if HAVE_REALLOCARRAY
    return reallocarray(ptr, nmemb, size);
#else
    const size_t total_size = nmemb * size;

    /* check for unsigned overflow */
    if ((nmemb != 0) && (total_size / nmemb != size)) {
        errno = ENOMEM;
        return NULL;
    }

    return realloc(ptr, total_size);
#endif
}



static void uriDefaultFree(UriMemoryManager * URI_UNUSED(memory),
        void * ptr) {
    free(ptr);
}



UriBool uriMemoryManagerIsComplete(const UriMemoryManager * memory) {
    return (memory
            && memory->malloc
            && memory->calloc
            && memory->realloc
            && memory->reallocarray
            && memory->free) ? URI_TRUE : URI_FALSE;
}



/*extern*/ UriMemoryManager defaultMemoryManager = {
    uriDefaultMalloc,
    uriDefaultCalloc,
    uriDefaultRealloc,
    uriDefaultReallocarray,
    uriDefaultFree,
    NULL  /* userData */
};
