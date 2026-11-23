#pragma once

namespace das {

    // src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-dag
    vector<Error> compileModules ( const vector<ModuleInfo> & modules, const FileAccessPtr & access, TextWriter & logs,
        ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, bool logDag = false,
        const ModuleCacheStream & stream = ModuleCacheStream() );
}
