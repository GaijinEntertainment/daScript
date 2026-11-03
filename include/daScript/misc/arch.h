#pragma once

#if defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
    #define DAS_ARCH_ARM64 1
#elif defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
    #define DAS_ARCH_X64 1
#elif defined(__i386__) || defined(__i386) || defined(_M_IX86)
    #define DAS_ARCH_X86 1
#endif
