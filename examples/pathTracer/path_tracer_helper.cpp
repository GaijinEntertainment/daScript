#include "daScript/daScript.h"

using namespace std;
using namespace das;

uint8_t LinearToSRGB ( float x ) {
    x = max(x, 0.0f);
    x = max(1.055f * (float)pow(x, 0.416666667f) - 0.055f, 0.0f);
    return (uint8_t) min((unsigned int)(x * 255.9f), 255u);
}

void saveTga ( const char * fileName, Array & arr, int width, int height, Context * context ) {
    if ( arr.size != uint32_t(width*height) ) {
        context->throw_error_ex("back buffer size mismatch %ix%i vs %i", width, height, arr.size );
    }
    FILE * f = fopen ( fileName, "wb" );
    if ( !f ) {
        context->throw_error_ex("can't open TGA file %s", fileName);
    }
    uint8_t header[] = {
        0, // ID length
        0, // no color map
        2, // uncompressed, true color
        0, 0, 0, 0,
        0,
        0, 0, 0, 0, // x and y origin
        (uint8_t)(width & 0x00FF),
        (uint8_t)((width & 0xFF00) >> 8),
        (uint8_t)(height & 0x00FF),
        (uint8_t)((height & 0xFF00) >> 8),
        32, // 32 bit
        0 };
    fwrite ( header, 1, sizeof(header), f);
    unique_ptr<uint8_t[]> bytes ( new uint8_t[width * height * 4] );
    float3 * bb = (float3 *) arr.data;
    for ( int i=0; i != width*height; ++i ) {
        bytes[i*4+0] = LinearToSRGB(bb[i].z);
        bytes[i*4+1] = LinearToSRGB(bb[i].y);
        bytes[i*4+2] = LinearToSRGB(bb[i].x);
        bytes[i*4+3] = 255;
    }
    fwrite(bytes.get(), 1, width*height*4, f);
    fclose(f);
}

class Module_PathTracerHelper : public Module {
public:
    Module_PathTracerHelper() : Module("PathTracerHelper") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // TGA
        addExtern<DAS_BIND_FUN(saveTga)>(*this, lib, "saveTga",SideEffects::modifyExternal);
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"path_tracer_helper.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_MODULE(Module_PathTracerHelper);
