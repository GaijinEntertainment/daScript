#pragma once

#include "ast_typefactory.h"

namespace das {
    // note:
    //  this binds any unspecified 'callback' thing to 'void *'
    //  generally this is included with generated headers
    template <typename ResT, typename ...Args>
    struct typeFactory<ResT (*)(Args...)> {
        static TypeDeclPtr make(const ModuleLibrary & library ) {
            return typeFactory<void *>::make(library);
        };
    };
}