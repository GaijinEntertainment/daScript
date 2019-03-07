#include "daScript/daScript.h"
#include "../common/fileAccess.h"

#include <thread>
#include <atomic>

using namespace std;
using namespace das;

uint8_t LinearToSRGB ( float x ) {
    x = max(x, 0.0f);
    x = max(1.055f * (float)pow(x, 0.416666667f) - 0.055f, 0.0f);
    return (uint8_t) min((unsigned int)(x * 255.9f), 255u);
}

void saveTga ( const char * fileName, Array * arr, int width, int height, Context * context ) {
    if ( arr->size != uint32_t(width*height) ) {
        context->throw_error_ex("back buffer size mismatch %ix%i vs %i", width, height, arr->size );
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
    float3 * bb = (float3 *) arr->data;
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
};

REGISTER_MODULE(Module_PathTracerHelper);

TextPrinter tout;


extern "C" int64_t ref_time_ticks();
extern "C" int get_time_usec(int64_t reft);

bool unit_test ( const string & fn ) {
	tout << fn << " ";
    auto access = make_shared<FsFileAccess>();
    ModuleGroup dummyGroup;
    if ( auto program = compileDaScript(fn, access, tout, dummyGroup) ) {
        if ( program->failed() ) {
			tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
				tout <<  reportError(err.at, err.what, err.cerr );
            }
            return false;
        } else {
            Context ctx;
            if ( !program->simulate(ctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.cerr );
                }
                return false;
            }
#if 1
            int width = 1280;
            int height = 720;
            int frameCount = 64;
            Array arr;
            memset(&arr, 0, sizeof(Array));
            array_resize(ctx, arr, width*height, sizeof(float3), true);
            int numCPU = max(thread::hardware_concurrency(), 1u);
            int chunk = height / numCPU;
            tout << "running on " << numCPU << ", " << chunk << " lines at a time\n";
            int magRayCount = 0;
            auto t0 = clock();
            for (int frame = 0; frame != frameCount; ++frame) {
                auto t0f = clock();
                vector<thread> threads;
                atomic<int> rayCount = 0;
                for (int c = 0; c != numCPU; ++c) {
                    auto thC = new Context(ctx);
                    threads.emplace_back(thread([&, thC]() {
                        auto fnJob = thC->findFunction("job");
                        thC->restart();
                        int yMin = c * chunk;
                        int yMax = min((c + 1)*chunk, height);
                        // def job(backbuffer:array<float3>;width,height,ymin,ymax:int)
                        vec4f args[6];
                        args[0] = cast<void *>::from((Array *)&arr);
                        args[1] = cast<int32_t>::from(frame);
                        args[2] = cast<int32_t>::from(width);
                        args[3] = cast<int32_t>::from(height);
                        args[4] = cast<int32_t>::from(yMin);
                        args[5] = cast<int32_t>::from(yMax);
                        int totalRays = cast<int>::to(thC->eval(fnJob, args));
                        rayCount += totalRays;
                        if (auto ex = thC->getException()) {
                            tout << "exception: " << ex << "\n";
                        }
                        delete thC;
                    }));
                }
                tout << "waiting for frame " << frame << "...";
                for (auto & th : threads) {
                    th.join();
                }
                auto t1 = clock();
                magRayCount += rayCount;
                tout << "done, " << int(rayCount) / double(1000000) << " mrays this frame, " << (double(t1 - t0f) / CLK_TCK) << " sec\n";
            }
            auto t2 = clock();
            tout << "took " << (double(t2 - t0) / CLK_TCK) << " sec, " << ((magRayCount/1000000.0) * CLK_TCK / (t2-t0)) << " mrays/s\n";
            saveTga("threaded.tga", &arr, width, height, &ctx);
            return true;
#else
            if ( auto fnTest = ctx.findFunction("test") ) {
                ctx.restart();
                bool result = cast<bool>::to(ctx.eval(fnTest, nullptr));
                if ( auto ex = ctx.getException() ) {
					tout << "exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
					tout << "failed\n";
                    return false;
                }
				tout << "ok\n";
                return true;
            } else {
				tout << "function 'test' not found\n";
                return false;
            }
#endif
        }
    } else {
        return false;
    }
}


int main() {
#ifdef _MSC_VER
#define    TEST_PATH "../"
#else
#define TEST_PATH "../../"
#endif
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_Random);
    NEED_MODULE(Module_PathTracerHelper);
    unit_test(TEST_PATH "examples/pathTracer/toy_path_tracer.das");
    Module::Shutdown();
    return 0;
}
