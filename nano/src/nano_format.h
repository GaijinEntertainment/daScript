#pragma once

#include <stdarg.h>
#include <stddef.h>

namespace das {

    // vsnprintf for the panic path and the string writer, minus the parts nano
    // never asks for. Every format string that reaches it is one of ours, and
    // none of them formats a float - which is what keeps newlib's monolithic
    // formatter, and the 17K it drags in, out of the image.
    //
    // Always NUL-terminates when size is nonzero. Returns the number of
    // characters written, not the number that would have been.
    int nano_vformat ( char * buf, size_t size, const char * fmt, va_list args );
}
