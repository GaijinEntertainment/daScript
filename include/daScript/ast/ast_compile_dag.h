#pragma once

namespace das {

    // src/ast/ARCHITECTURE_DAG.md#compile-dag
    DAS_CC_API ProgramPtr compileModules ( const vector<ModuleInfo> & modules, const FileAccessPtr & access, TextWriter & logs,
        ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, bool logDag = false,
        const ModuleCacheStream & stream = ModuleCacheStream() );
}
