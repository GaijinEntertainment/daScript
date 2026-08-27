#include "daScript/misc/platform.h"
#include "daScript/misc/string_writer.h"
#include "daScript/nano_print.h"
#include "nano_format.h"

#include <stdarg.h>
#include <stdio.h>

// nano's implementation of daScript/misc/string_writer.h: the same header,
// numbers through snprintf instead of fmt, and one sink instead of the full
// runtime's re-pointable printer. nano/ARCHITECTURE.md says what that changes.

namespace das {

    DAS_API StringWriterTag HEX;
    DAS_API StringWriterTag DEC;
    DAS_API StringWriterTag FIXEDFP;
    DAS_API StringWriterTag SCIENTIFIC;

    mutex TextPrinter::pmut;

    static StringWriter & writeFormatted ( StringWriter & w, const char * fmt, ... ) {
        char buf[128];
        va_list args;
        va_start(args, fmt);
        int n = nano_vformat(buf, sizeof(buf), fmt, args);
        va_end(args);
        if ( n < 0 ) return w;
        size_t len = size_t(n) < sizeof(buf) ? size_t(n) : sizeof(buf) - 1;
        return w.writeStr(buf, len);
    }

    StringWriter & StringWriter::writeStr(const char * st, size_t len) {
        this->append(st, int(len));
        this->output();
        return *this;
    }
    StringWriter & StringWriter::writeChars(char ch, size_t len) {
        if ( auto at = this->allocate(int(len)) ) {
            memset(at, ch, len);
            this->output();
        }
        return *this;
    }
    StringWriter & StringWriter::write(const char * stst) {
        if ( stst ) {
            return writeStr(stst, strlen(stst));
        } else {
            return *this;
        }
    }
    StringWriter & StringWriter::operator << (const StringWriterTag & v ) {
        if (&v == &HEX) hex = true;
        else if (&v == &DEC) hex = false;
        else if (&v == &FIXEDFP) fixed = true;
        else if (&v == &SCIENTIFIC) fixed = false;
        return *this;
    }
    StringWriter & StringWriter::operator << (char v)                 { return writeStr(&v, 1); }
    StringWriter & StringWriter::operator << (unsigned char v)        { return writeFormatted(*this, "%u", unsigned(v)); }
    StringWriter & StringWriter::operator << (bool v)                 { return write(v ? "true" : "false"); }
    StringWriter & StringWriter::operator << (int v)                  { return writeFormatted(*this, hex ? "%x" : "%d", v); }
    StringWriter & StringWriter::operator << (long v)                 { return writeFormatted(*this, hex ? "%lx" : "%ld", v); }
    StringWriter & StringWriter::operator << (long long v)            { return writeFormatted(*this, hex ? "%llx" : "%lld", v); }
    StringWriter & StringWriter::operator << (unsigned v)             { return writeFormatted(*this, hex ? "%x" : "%u", v); }
    StringWriter & StringWriter::operator << (unsigned long v)        { return writeFormatted(*this, hex ? "%lx" : "%lu", v); }
    StringWriter & StringWriter::operator << (unsigned long long v)   { return writeFormatted(*this, hex ? "%llx" : "%llu", v); }
    StringWriter & StringWriter::operator << (char * v)               { return write(v ? (const char*)v : ""); }
    StringWriter & StringWriter::operator << (const char * v)         { return write(v ? v : ""); }
    StringWriter & StringWriter::operator << (const string & v)       { return v.length() ? writeStr(v.c_str(), v.length()) : *this; }
    // floats stay on snprintf: nano_vformat is integer-only, and --gc-sections
    // keeps the printf machinery out of any image that never prints a float
    // snprintf returns what it WOULD have written; a truncated result must be
    // clamped to the buffer or writeStr reads past it
    StringWriter & StringWriter::operator << (float v) {
        char fb[48];
        int n = snprintf(fb, sizeof(fb), fixed ? "%.9g" : "%g", double(v));
        if ( n >= int(sizeof(fb)) ) n = int(sizeof(fb)) - 1;
        return n > 0 ? writeStr(fb, uint32_t(n)) : *this;
    }
    StringWriter & StringWriter::operator << (double v) {
        char fb[48];
        int n = snprintf(fb, sizeof(fb), fixed ? "%.17g" : "%g", v);
        if ( n >= int(sizeof(fb)) ) n = int(sizeof(fb)) - 1;
        return n > 0 ? writeStr(fb, uint32_t(n)) : *this;
    }

    string FixedBufferTextWriter::str() const {
        DAS_VERIFY(size <= DAS_SMALL_BUFFER_SIZE);
        return string(data, size);
    }

    uint64_t FixedBufferTextWriter::tellp() const {
        return uint64_t(size);
    }

    void FixedBufferTextWriter::append(const char * s, int l) {
        if ( size + l <= DAS_SMALL_BUFFER_SIZE ) {
            memcpy ( data+size, s, l );
            size += l;
        } else {
            DAS_FATAL_ERROR("DAS_SMALL_BUFFER_SIZE overflow");
        }
    }

    char * FixedBufferTextWriter::allocate (int l) {
        if ( size + l <= DAS_SMALL_BUFFER_SIZE ) {
            char * res = data + size;
            size += l;
            return res;
        } else {
            DAS_FATAL_ERROR("DAS_SMALL_BUFFER_SIZE overflow");
            return nullptr;
        }
    }

    void FixedBufferTextWriter::output() {
    }

    TextWriter::~TextWriter() {
        if ( largeBuffer != fixedBuffer ) {
            das_aligned_free16(largeBuffer);
        }
    }

    string TextWriter::str() const {
        return string(largeBuffer, size);
    }

    uint64_t TextWriter::tellp() const {
        return uint64_t(size);
    }

    bool TextWriter::empty() const {
        return size == 0;
    }

    char * TextWriter::data() {
        return largeBuffer;
    }

    void TextWriter::clear() {
        size = 0;
    }

    void TextWriter::output() {
    }

    void TextWriter::append(const char * s, int l) {
        char * at = allocate(l);
        memcpy(at, s, l);
    }

    char * TextWriter::c_str() {
        if ( size < capacity ) {
            largeBuffer[size] = 0;
            return largeBuffer;
        } else {
            char * newBuffer = (char *) das_aligned_alloc16(size + 1);
            memcpy(newBuffer, largeBuffer, size);
            newBuffer[size] = 0;
            if ( largeBuffer != fixedBuffer ) {
                das_aligned_free16(largeBuffer);
            }
            largeBuffer = newBuffer;
            capacity = size + 1;
            return largeBuffer;
        }
    }

    char * TextWriter::allocate (int l) {
        if ( size + l <= capacity ) {
            char * res = largeBuffer + size;
            size += l;
            return res;
        } else {
            int32_t newCapacity = capacity * 2;
            if ( newCapacity < size + l ) {
                newCapacity = size + l;
            }
            char * newBuffer = (char *) das_aligned_alloc16(newCapacity);
            if ( largeBuffer != fixedBuffer ) {
                memcpy(newBuffer, largeBuffer, size);
                das_aligned_free16(largeBuffer);
            } else {
                memcpy(newBuffer, fixedBuffer, size);
            }
            largeBuffer = newBuffer;
            capacity = newCapacity;
            char * res = largeBuffer + size;
            size += l;
            return res;
        }
    }

    void TextPrinter::output() {
        lock_guard<mutex> guard(pmut);
        uint64_t newPos = tellp();
        if (newPos != pos) {
            string st(data() + pos, size_t(newPos - pos));
            das_nano_write(LogLevel::defaultPrint, "", st.c_str());
            pos = newPos;
        }
    }

    const char * getLogMarker(int level)
    {
        if ( level >= LogLevel::error )
            return "[E] ";
        else if ( level >= LogLevel::warning )
            return "[W] ";
        else if ( level >= LogLevel::info )
            return "[I] ";
        else
            return "";
    }

    void LOG::output() {
        auto newPos = tellp();
        if (newPos != pos) {
            string st(data() + pos, size_t(newPos - pos));
            das_to_stdout_level_prefix_text(logLevel, useMarker ? getLogMarker(logLevel) : "", st.c_str());
            useMarker = false;
            clear();
            pos = newPos = 0;
        }
    }

    void setTextPrinterSink ( TextPrinterSink sink ) {
        das_nano_set_print(sink);
    }

    void textPrinterToStderr() {
    }

    bool textPrinterToFile ( const char * ) {
        return false;
    }
}

void das_fatal_log ( const char * format, ... ) {
    char buf[512];
    va_list args;
    va_start(args, format);
    das::nano_vformat(buf, sizeof(buf), format, args);
    va_end(args);
    buf[sizeof(buf) - 1] = 0;
    das::das_nano_write(das::LogLevel::error, "", buf);
}
