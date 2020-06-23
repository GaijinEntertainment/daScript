#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"

#include "path_tracer_helper.h"

#include <thread>
#include <atomic>

using namespace std;
using namespace das;

#if !defined(DAS_GLOBAL_NEW) && defined(_MSC_VER) && !defined(_WIN64)

void * operator new(std::size_t n) throw(std::bad_alloc)
{
    return das_aligned_alloc16(n);
}
void operator delete(void * p) throw()
{
    das_aligned_free16(p);
}

#endif

#define USE_AOT 1

TextPrinter tout;

extern "C" int64_t ref_time_ticks();
extern "C" int get_time_usec(int64_t reft);

double get_time_sec(uint64_t t) {
    return get_time_usec(t) / 1000000.0;
}

bool unit_test ( const string & fn ) {
    tout << fn << " ";
    auto access = make_smart<FsFileAccess>();
    ModuleGroup dummyGroup;
    if ( auto program = compileDaScript(fn, access, tout, dummyGroup) ) {
        if ( program->failed() ) {
            tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                tout <<  reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
            }
            return false;
        } else {
            Context ctx(program->getContextStackSize());
            if ( !program->simulate(ctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
                }
                return false;
            }
#if USE_AOT
            // now, what we get to do is to link AOT
            AotLibrary aotLib;
            AotListBase::registerAot(aotLib);
            program->linkCppAot(ctx, aotLib, tout);
#endif
#if 0
            int width = 1280;
            int height = 720;
            int frameCount = 16;
            Array arr;
            memset(&arr, 0, sizeof(Array));
            array_resize(ctx, arr, width*height, sizeof(float3), true);
            int numCPU = max(thread::hardware_concurrency(), 1u);
            int chunk = height / numCPU;
            tout << "running on " << numCPU << " threads, " << chunk << " lines at a time\n";
            int64_t magRayCount = 0;
            auto t0 = ref_time_ticks();
            vector<unique_ptr<Context>> ctxU;
            for ( int c=0; c!=numCPU; ++c ) {
                ctxU.emplace_back(make_unique<Context>(ctx));
            }
            auto fnJob = ctx.findFunction("job");
            if ( !verifyCall<int32_t,Array,int32_t,int32_t,int32_t,int32_t,int32_t>(fnJob->debugInfo, dummyGroup) ) {
                tout << "function 'job', call arguments do not match\n";
                return false;
            }
            for (int frame = 0; frame != frameCount; ++frame) {
                auto t0f = ref_time_ticks();
                vector<thread> threads;
                atomic<int> rayCount;
                rayCount = 0;
                for (int c = 0; c != numCPU; ++c) {
                    auto thC = ctxU[c].get();
                    threads.emplace_back(thread([&,thC,c]() {
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
                    }));
                }
                tout << "waiting for frame " << frame << "...";
                for (auto & th : threads) {
                    th.join();
                }
                auto dtf = get_time_sec(t0f);
                magRayCount += rayCount;
                tout << "done, " << int(rayCount) / double(1000000.0) << " mrays this frame, " << dtf << " sec\n";
            }
            auto dta = get_time_sec(t0);
            auto mrays = magRayCount / 1000000.0;
            tout
                << "took " << dta << " sec, "
                << "total " << mrays << " mrays, "
                << (mrays / dta) << " mrays/s\n";
            saveTga("path_tracer.tga", arr, width, height, &ctx);
            return true;
#else
            if ( auto fnTest = ctx.findFunction("test") ) {
                if ( !verifyCall<bool>(fnTest->debugInfo, dummyGroup) ) {
                    tout << "function 'test', call arguments do not match\n";
                    return false;
                }
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
  _mm_setcsr((_mm_getcsr()&~_MM_ROUND_MASK) | _MM_FLUSH_ZERO_MASK | _MM_ROUND_NEAREST | 0x40);//0x40
#ifdef _MSC_VER
#define    TEST_PATH "../"
#else
#define TEST_PATH "../../"
#endif
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_Strings);
    NEED_MODULE(Module_Random);
    NEED_MODULE(Module_PathTracerHelper);
    unit_test(TEST_PATH "examples/pathTracer/toy_path_tracer.das");
    Module::Shutdown();
    return 0;
}
