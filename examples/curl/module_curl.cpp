#include "daScript/daScript.h"

#include <curl/curl.h>

namespace das {

    struct Curl {
        CURL * curl;
        Context * context = nullptr;
        Lambda lambda;
        Curl() {
            curl = curl_easy_init();
        }
        ~Curl() {
            curl_easy_cleanup(curl);
            curl = nullptr;
        }
    };

    MAKE_TYPE_FACTORY(CURL, Curl);

    struct CurlAnnotation : ManagedStructureAnnotation<Curl> {
        CurlAnnotation(ModuleLibrary & ml) :  ManagedStructureAnnotation<Curl>("CURL",ml) {
        }
    };

    void curl_easy_setop_url ( Curl * curl, const char * url ) {
        curl_easy_setopt(curl->curl, CURLOPT_URL, url);
    }

    size_t writeFunction(void *ptr, size_t size, size_t nmemb, Curl * curl) {
        auto context = curl->context;
        int32_t * fnIndex = (int32_t *) curl->lambda.capture;
        if (!fnIndex) context->throw_error("invoke null lambda");
        SimFunction * simFunc = context->getFunction(*fnIndex-1);
        vec4f argValues[4] = {
            cast<Lambda>::from(curl->lambda),
            cast<char *>::from((char *)ptr),
            cast<uint32_t>::from(size),
            cast<uint32_t>::from(nmemb)
        };
        context->call(simFunc, argValues, 0);
        return size * nmemb;
    }

    void curl_easy_setopt_writedata ( Curl * curl, const Lambda & lambda, Context * __context__ ) {
        curl->context = __context__;
        curl->lambda = lambda;
        curl_easy_setopt(curl->curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl->curl, CURLOPT_WRITEDATA, curl);
    }

    void curl_perform ( Curl * curl ) {
        curl_easy_perform(curl->curl);
    }

    class Module_Curl : public Module {
    public:
        Module_Curl() : Module("libCurl") {
            // init curl
            curl_global_init(CURL_GLOBAL_DEFAULT);
            // module
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // annotation
            addAnnotation(make_shared<CurlAnnotation>(lib));
            // functions
            addExtern<DAS_BIND_FUN(curl_easy_setop_url)>(*this,lib,"curl_easy_setop_url",SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(curl_easy_setopt_writedata)>(*this,lib,"curl_easy_setopt_writedata",SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(curl_perform)>(*this,lib,"curl_perform",SideEffects::modifyExternal);
        }
        virtual ~Module_Curl() {
            curl_global_cleanup();
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Curl, das);

