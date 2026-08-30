#pragma once

namespace das {
    // shortest round-trip float printing, spelled byte-for-byte as fmt's default "{}";
    // returns the end of the written text, which is NOT null-terminated, and the buffer
    // must be at least 48 bytes - the emitter's fixed-length copies overshoot past the text
    DAS_API char * float2string ( char * buf, float value );
    DAS_API char * double2string ( char * buf, double value );
}
