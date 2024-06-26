#pragma once

namespace das {

    class Context;
    struct LineInfo;

    #ifndef DAS_SMALL_BUFFER_SIZE
    #define DAS_SMALL_BUFFER_SIZE   4096
    #endif

    class SmallBufferPolicy {
    public:
        string str() const {            // todo: replace via stringview
            DAS_VERIFY(size <= DAS_SMALL_BUFFER_SIZE);
            return string(data, size);
        }
        int tellp() const {
            return size;
        }
    protected:
        __forceinline void append(const char * s, int l) {
            if ( size + l <= DAS_SMALL_BUFFER_SIZE ) {
                memcpy ( data+size, s, l );
                size += l;
            } else {
                DAS_FATAL_ERROR("DAS_SMALL_BUFFER_SIZE overflow");
            }
        }
        __forceinline char * allocate (int l) {
            if ( size + l <= DAS_SMALL_BUFFER_SIZE ) {
                char * res = data + size;
                size += l;
                return res;
            } else {
                DAS_FATAL_ERROR("DAS_SMALL_BUFFER_SIZE overflow");
                return nullptr;
            }
        }
        __forceinline void output() {}
    protected:
        char    data[DAS_SMALL_BUFFER_SIZE];
        int32_t size = 0;
    };

    #ifndef DAS_STRING_BUILDER_BUFFER_SIZE
    #define DAS_STRING_BUILDER_BUFFER_SIZE   256
    #endif

    class StringBuilderPolicy {
    public:
        virtual ~StringBuilderPolicy() {
            if ( largeBuffer != fixedBuffer ) {
                das_aligned_free16(largeBuffer);
            }
        }
        string str() const {            // todo: replace via stringview
            return string(largeBuffer, size);
        }
        uint64_t tellp() const {
            return uint64_t(size);
        }
        bool empty() const {
            return size == 0;
        }
        char * c_str();
        char * data() {
            return largeBuffer;
        }
        void clear() {
            size = 0;
        }
        virtual void output() {}
    protected:
        void append(const char * s, int l) {
            char * at = allocate(l);
            memcpy(at, s, l);
        }
        char * allocate (int l);
    protected:
        char    fixedBuffer[DAS_STRING_BUILDER_BUFFER_SIZE];
        char *  largeBuffer = fixedBuffer;
        int32_t size = 0;
        int32_t capacity = DAS_STRING_BUILDER_BUFFER_SIZE;
    };


    // todo: support hex
    struct StringWriterTag {};
    extern StringWriterTag HEX;
    extern StringWriterTag DEC;
    extern StringWriterTag FIXEDFP;
    extern StringWriterTag SCIENTIFIC;

    template <typename AllocationPolicy>
    class StringWriter : public AllocationPolicy {
    public:
        virtual ~StringWriter() {}
        template <typename TT>
        StringWriter & format(const char * format, TT value) {
            char buf[128];
            auto tail = fmt::format_to(buf, fmt::runtime(format), value);
            auto realL = tail - buf;
            if ( auto at = this->allocate(int(realL)) ) {
                memcpy(at, buf, realL);
                this->output();
            }
            return *this;
        }
        StringWriter & writeStr(const char * st, size_t len) {
            this->append(st, int(len));
            this->output();
            return *this;
        }
        StringWriter & writeChars(char ch, size_t len) {
            if ( auto at = this->allocate(int(len)) ) {
                memset(at, ch, len);
                this->output();
            }
            return *this;
        }
        StringWriter & write(const char * stst) {
            if ( stst ) {
                return writeStr(stst, strlen(stst));
            } else {
                return *this;
            }
        }
        StringWriter & operator << (const StringWriterTag & v ) {
            if (&v == &HEX) hex = true;
            else if (&v == &DEC) hex = false;
            else if (&v == &FIXEDFP) fixed = true;
            else if (&v == &SCIENTIFIC) fixed = false;
            return *this;
        }
        StringWriter & operator << (char v)                 { return format("{}", v); }
        StringWriter & operator << (unsigned char v)        { return format("{}", v); }
        StringWriter & operator << (bool v)                 { return write(v ? "true" : "false"); }
        StringWriter & operator << (int v)                  { return format(hex ? "{:x}" : "{}", v); }
        StringWriter & operator << (long v)                 { return format(hex ? "{:x}" : "{}", v); }
        StringWriter & operator << (long long v)            { return format(hex ? "{:x}" : "{}", v); }
        StringWriter & operator << (unsigned v)             { return format(hex ? "{:x}" : "{}", v); }
        StringWriter & operator << (unsigned long v)        { return format(hex ? "{:x}" : "{}", v); }
        StringWriter & operator << (unsigned long long v)   { return format(hex ? "{:x}" : "{}", v); }
        StringWriter & operator << (char * v)               { return write(v ? (const char*)v : ""); }
        StringWriter & operator << (const char * v)         { return write(v ? v : ""); }
        StringWriter & operator << (const string & v)       { return v.length() ? writeStr(v.c_str(), v.length()) : *this; }
        StringWriter & operator << (float v)                { return format(fixed ? "{:.9}" : "{}", v); }
        StringWriter & operator << (double v)               { return format(fixed ? "{:.17}" : "{}", v); }
    protected:
        bool hex = false;
        bool fixed = false;
    };

    typedef StringWriter<StringBuilderPolicy> TextWriter;

    typedef StringWriter<SmallBufferPolicy> FixedBufferTextWriter;

    class TextPrinter : public TextWriter {
    public:
        virtual void output() override;
    protected:
        uint64_t pos = 0;
        static mutex pmut;
    };




    enum LogLevel {

        // CRITICAL - application can no longer perform its functions, or further execution will cause data corruption.
        // For example: array overrun, os_debug_break, assertion failure inside compiler.
        critical    = 50000,

        // ERROR - Error events of considerable importance that will prevent normal program execution,
        // but might still allow the application to continue running.
        // For example: an error when opening a file, wrong settings for texture conversion, assertion failure in the script.
        error       = 40000,

        // WARNING - the error is handled locally, something may cause unexpected behavior of the application.
        // For example: not all characters have been extracted from keystroke buffer, MSAA level was too high.
        warning     = 30000,

        // INFO - highlight the progress of the application.
        // General system start/stop messages, device configuration information.
        info        = 20000,

        // DEBUG - information useful for application developers. (DEBUG is defaut level)
        debug       = 10000,

        // TRACE - information useful for developers of subsystems, libraries, daScript etc.
        // For example: application activation/deactivation, key pressed, texture loaded from a file, sound file loaded.
        trace       = 0,
    };

    const char * getLogMarker(int level);
    void logger ( int level, const char *marker, const char * text, Context * context, LineInfo * at );

    class LOG : public TextWriter {
    public:
        LOG ( int level = LogLevel::debug ) : logLevel(level) {}
        virtual void output() override {
            auto newPos = tellp();
            if (newPos != pos) {
                string st(data() + pos, newPos - pos);
                logger(logLevel, useMarker ? getLogMarker(logLevel) : "", st.c_str(), /*ctx*/nullptr, /*at*/nullptr);
                useMarker = false;
                clear();
                pos = newPos = 0;
            }
        }
    protected:
        uint64_t pos = 0;
        int logLevel = LogLevel::debug;
        bool useMarker = true;
    };
}
