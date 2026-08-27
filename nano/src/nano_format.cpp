#include "daScript/misc/platform.h"

#include "nano_format.h"

#include <stdint.h>

// Conversions: %s %d %i %u %p %x %%, each with an optional l or ll width.
// No flags, no field width, no precision, no floats - a census of all 161
// throw_error / fatal call sites in nano's reach uses none of them.

namespace das {

    namespace {

        struct Sink {
            char * buf;
            size_t size;    // capacity including the NUL
            size_t len;     // characters written so far

            void put ( char c ) {
                if ( len + 1 < size ) buf[len] = c;
                len ++;
            }

            void puts ( const char * s ) {
                while ( *s ) put(*s++);
            }

            void put_range ( const char * s, const char * e ) {
                while ( s != e ) put(*s++);
            }
        };

        // Writes backwards into the tail of tmp and returns the first digit, so
        // the digits come out in order without a second reversal pass.
        char * utoa_back ( char * end, uint64_t v, unsigned base ) {
            static const char digits[] = "0123456789abcdef";
            do {
                *--end = digits[v % base];
                v /= base;
            } while ( v );
            return end;
        }

        void put_unsigned ( Sink & out, uint64_t v, unsigned base ) {
            char tmp[24];
            char * end = tmp + sizeof(tmp);
            out.put_range(utoa_back(end, v, base), end);
        }
    }

    int nano_vformat ( char * buf, size_t size, const char * fmt, va_list args ) {
        Sink out{ buf, size, 0 };
        for ( ; *fmt; ++fmt ) {
            if ( *fmt != '%' ) { out.put(*fmt); continue; }
            ++fmt;
            if ( *fmt == 0 ) break;
            if ( *fmt == '%' ) { out.put('%'); continue; }
            // width matters: va_arg must read exactly the type the caller pushed.
            // long and long long are distinct sizes on ILP32 targets (cortex-m4),
            // so %l and %ll cannot share a va_arg the way a first draft had it.
            int longs = 0;
            while ( *fmt == 'l' ) { ++longs; ++fmt; }
            switch ( *fmt ) {
            case 'd':
            case 'i': {
                int64_t v = longs == 0 ? (int64_t) va_arg(args, int)
                          : longs == 1 ? (int64_t) va_arg(args, long)
                                       : (int64_t) va_arg(args, long long);
                uint64_t mag;
                if ( v < 0 ) { out.put('-'); mag = (uint64_t)(-(v + 1)) + 1u; }
                else         { mag = (uint64_t) v; }
                put_unsigned(out, mag, 10);
                break;
            }
            case 'u': {
                uint64_t v = longs == 0 ? (uint64_t) va_arg(args, unsigned)
                           : longs == 1 ? (uint64_t) va_arg(args, unsigned long)
                                        : (uint64_t) va_arg(args, unsigned long long);
                put_unsigned(out, v, 10);
                break;
            }
            case 'x':
                put_unsigned(out, longs == 0 ? (uint64_t) va_arg(args, unsigned)
                           : longs == 1 ? (uint64_t) va_arg(args, unsigned long)
                                        : (uint64_t) va_arg(args, unsigned long long), 16);
                break;
            case 'p':
                out.puts("0x");
                put_unsigned(out, (uint64_t)(uintptr_t) va_arg(args, void *), 16);
                break;
            case 's': {
                const char * s = va_arg(args, const char *);
                out.puts(s ? s : "(null)");
                break;
            }
            default:
                // an unsupported conversion is reported rather than swallowed:
                // a panic message that silently drops its detail is worse than
                // one that says the formatter does not know this letter
                out.put('%');
                out.put(*fmt);
                break;
            }
        }
        if ( size ) buf[out.len < size ? out.len : size - 1] = 0;
        return int(out.len < size ? out.len : (size ? size - 1 : 0));
    }
}
