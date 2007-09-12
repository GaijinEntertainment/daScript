/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
 * Copyright (C) 2007, Sebastian Pipping <webmaster@hartwork.org>
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

/* What encodings are enabled? */
#include <uriparser/UriDefsConfig.h>
#if (!defined(URI_PASS_ANSI) && !defined(URI_PASS_UNICODE))
/* Include SELF twice */
# define URI_PASS_ANSI 1
# include "UriShorten.c"
# undef URI_PASS_ANSI
# define URI_PASS_UNICODE 1
# include "UriShorten.c"
# undef URI_PASS_UNICODE
#else
# ifdef URI_PASS_ANSI
#  include <uriparser/UriDefsAnsi.h>
# else
#  include <uriparser/UriDefsUnicode.h>
#  include <wchar.h>
# endif



#ifndef URI_DOXYGEN
# include <uriparser/Uri.h>
# include "UriCommon.h"
#endif



static URI_INLINE UriBool URI_FUNC(AppendSegment)(URI_TYPE(Uri) * uri,
		const URI_CHAR * first, const URI_CHAR * afterLast) {
	/* Create segment */
	URI_TYPE(PathSegment) * segment = malloc(1 * sizeof(URI_TYPE(PathSegment)));
	if (segment == NULL) {
		return URI_FALSE; /* Raises malloc error */
	}
	segment->next = NULL;
	segment->text.first = first;
	segment->text.afterLast = afterLast;

	/* Put into chain */
	if (uri->pathTail == NULL) {
		uri->pathHead = segment;
	} else {
		uri->pathTail->next = segment;
	}
	uri->pathTail = segment;

	return URI_TRUE;
}



static URI_INLINE UriBool URI_FUNC(EqualsAuthority)(const URI_TYPE(Uri) * first,
		const URI_TYPE(Uri) * second) {
	/* IPv4 */
	if (first->hostData.ip4 != NULL) {
		return ((second->hostData.ip4 != NULL)
				&& !memcmp(first->hostData.ip4->data,
					second->hostData.ip4->data, 4)) ? URI_TRUE : URI_FALSE;
	}

	/* IPv6 */
	if (first->hostData.ip6 != NULL) {
		return ((second->hostData.ip6 != NULL)
				&& !memcmp(first->hostData.ip6->data,
					second->hostData.ip6->data, 16)) ? URI_TRUE : URI_FALSE;
	}

	/* IPvFuture */
	if (first->hostData.ipFuture.first != NULL) {
		return ((second->hostData.ipFuture.first != NULL)
				&& !URI_STRNCMP(first->hostData.ipFuture.first,
					second->hostData.ipFuture.first,
					first->hostData.ipFuture.afterLast
					- first->hostData.ipFuture.first))
						? URI_TRUE : URI_FALSE;
	}

	if (first->hostText.first != NULL) {
		return ((second->hostText.first != NULL)
				&& !URI_STRNCMP(first->hostText.first,
					second->hostText.first,
					first->hostText.afterLast
					- first->hostText.first)) ? URI_TRUE : URI_FALSE;
	}

	return (second->hostText.first == NULL);
}



int URI_FUNC(RemoveBaseUri)(URI_TYPE(Uri) * dest,
		const URI_TYPE(Uri) * absSource,
		const URI_TYPE(Uri) * absBase,
		UriBool domainRootMode) {
	if ((dest == NULL) || (absSource == NULL) || (absBase == NULL)) {
		return URI_ERROR_NULL;
	}

	/* absBase absolute? */
	if (absBase->scheme.first == NULL) {
		return URI_ERROR_REMOVEBASE_REL_BASE;
	}

	/* absSource absolute? */
	if (absSource->scheme.first == NULL) {
		return URI_ERROR_REMOVEBASE_REL_SOURCE;
	}

	URI_FUNC(ResetUri)(dest);

	/* [XX/XX]	if (A.scheme != Base.scheme) then */
				if (URI_STRNCMP(absSource->scheme.first, absBase->scheme.first,
						absSource->scheme.afterLast - absSource->scheme.first)) {
	/* [XX/XX]	   T.scheme    = A.scheme; */
					dest->scheme = absSource->scheme;
	/* [XX/XX]	   T.authority = A.authority; */
					if (!URI_FUNC(CopyAuthority)(dest, absSource)) {
						return URI_ERROR_MALLOC;
					}
	/* [XX/XX]	   T.path      = A.path; */
					if (!URI_FUNC(CopyPath)(dest, absSource)) {
						return URI_ERROR_MALLOC;
					}
	/* [XX/XX]	else */
				} else {
	/* [XX/XX]	   undef(T.scheme); */
					/* NOOP */
	/* [XX/XX]	   if (A.authority != Base.authority) then */
					if (!URI_FUNC(EqualsAuthority)(absSource, absBase)) {
	/* [XX/XX]	      T.authority = A.authority; */
						if (!URI_FUNC(CopyAuthority)(dest, absSource)) {
							return URI_ERROR_MALLOC;
						}
	/* [XX/XX]	      T.path      = A.path; */
						if (!URI_FUNC(CopyPath)(dest, absSource)) {
							return URI_ERROR_MALLOC;
						}
	/* [XX/XX]	   else */
					} else {
	/* [XX/XX]	      if domainRootMode then */
						if (domainRootMode == URI_TRUE) {
	/* [XX/XX]	         undef(T.authority); */
							/* NOOP */
	/* [XX/XX]	         if (first(A.path) == "") then */
							/* GROUPED */
	/* [XX/XX]	            T.path   = "/." + A.path; */
								/* GROUPED */
	/* [XX/XX]	         else */
								/* GROUPED */
	/* [XX/XX]	            T.path   = A.path; */
								/* GROUPED */
	/* [XX/XX]	         endif; */
							if (!URI_FUNC(CopyPath)(dest, absSource)) {
								return URI_ERROR_MALLOC;
							}
							dest->absolutePath = URI_TRUE;

							if (!URI_FUNC(FixAmbiguity)(dest)) {
								return URI_ERROR_MALLOC;
							}
	/* [XX/XX]	      else */
						} else {
							const URI_TYPE(PathSegment) * sourceSeg = absSource->pathHead;
							const URI_TYPE(PathSegment) * baseSeg = absBase->pathHead;
	/* [XX/XX]	         bool pathNaked = true; */
							UriBool pathNaked = URI_TRUE;
	/* [XX/XX]	         undef(last(Base.path)); */
							/* NOOP */
	/* [XX/XX]	         T.path = ""; */
							dest->absolutePath = URI_FALSE;
	/* [XX/XX]	         while (first(A.path) == first(Base.path)) do */
							while ((sourceSeg != NULL) && (baseSeg != NULL)
									&& !URI_STRNCMP(sourceSeg->text.first, baseSeg->text.first,
									sourceSeg->text.afterLast - sourceSeg->text.first)
									&& !((sourceSeg->text.first == sourceSeg->text.afterLast)
										&& ((sourceSeg->next == NULL) != (baseSeg->next == NULL)))) {
	/* [XX/XX]	            A.path++; */
								sourceSeg = sourceSeg->next;
	/* [XX/XX]	            Base.path++; */
								baseSeg = baseSeg->next;
	/* [XX/XX]	         endwhile; */
							}
	/* [XX/XX]	         while defined(first(Base.path)) do */
							while ((baseSeg != NULL) && (baseSeg->next != NULL)) {
	/* [XX/XX]	            Base.path++; */
								baseSeg = baseSeg->next;
	/* [XX/XX]	            T.path += "../"; */
								if (!URI_FUNC(AppendSegment)(dest, URI_FUNC(ConstParent),
										URI_FUNC(ConstParent) + 2)) {
									return URI_ERROR_MALLOC;
								}
	/* [XX/XX]	            pathNaked = false; */
								pathNaked = URI_FALSE;
	/* [XX/XX]	         endwhile; */
							}
	/* [XX/XX]	         while defined(first(A.path)) do */
							while (sourceSeg != NULL) {
	/* [XX/XX]	            if pathNaked then */
								if (pathNaked == URI_TRUE) {
	/* [XX/XX]	               if (first(A.path) contains ":") then */
									UriBool containsColon = URI_FALSE;
									const URI_CHAR * ch = sourceSeg->text.first;
									for (; ch < sourceSeg->text.afterLast; ch++) {
										if (*ch == _UT(':')) {
											containsColon = URI_TRUE;
											break;
										}
									}

									if (containsColon) {
	/* [XX/XX]	                  T.path += "./"; */
										if (!URI_FUNC(AppendSegment)(dest, URI_FUNC(ConstPwd),
												URI_FUNC(ConstPwd) + 1)) {
											return URI_ERROR_MALLOC;
										}
	/* [XX/XX]	               elseif (first(A.path) == "") then */
									} else if (sourceSeg->text.first == sourceSeg->text.afterLast) {
	/* [XX/XX]	                  T.path += "/."; */
										if (!URI_FUNC(AppendSegment)(dest, URI_FUNC(ConstPwd),
												URI_FUNC(ConstPwd) + 1)) {
											return URI_ERROR_MALLOC;
										}
	/* [XX/XX]	               endif; */
									}
	/* [XX/XX]	            endif; */
								}
	/* [XX/XX]	            T.path += first(A.path); */
								if (!URI_FUNC(AppendSegment)(dest, sourceSeg->text.first,
										sourceSeg->text.afterLast)) {
									return URI_ERROR_MALLOC;
								}
	/* [XX/XX]	            pathNaked = false; */
								pathNaked = URI_FALSE;
	/* [XX/XX]	            A.path++; */
								sourceSeg = sourceSeg->next;
	/* [XX/XX]	            if defined(first(A.path)) then */
								/* NOOP */
	/* [XX/XX]	               T.path += + "/"; */
								/* NOOP */
	/* [XX/XX]	            endif; */
								/* NOOP */
	/* [XX/XX]	         endwhile; */
							}
	/* [XX/XX]	      endif; */
						}
	/* [XX/XX]	   endif; */
					}
	/* [XX/XX]	endif; */
				}
	/* [XX/XX]	T.query     = A.query; */
				dest->query = absSource->query;
	/* [XX/XX]	T.fragment  = A.fragment; */
				dest->fragment = absSource->fragment;

	return URI_SUCCESS;
}



#endif
