#pragma once

namespace das {

    vector<RequireRecord> getAllRequire ( FileInfo * fi, string & modName, vector<FileInfo *> & chain, const FileAccessPtr & access );
    ProgramPtr parseDaScriptEx ( const string & fileName, const string & moduleName, const FileAccessPtr & access,
        TextWriter & logs, ModuleGroup & libGroup, bool exportAll, bool isDep, CodeOfPolicies policies, bool inferAfterParse,
        const ModuleCacheStream & stream = ModuleCacheStream() );
    bool aotModuleHasName ( ProgramPtr program, const ModuleInfo & mod );
    bool canShareModule ( ProgramPtr program );
    void addNewModules ( ModuleGroup & libGroup, ProgramPtr program );
}
