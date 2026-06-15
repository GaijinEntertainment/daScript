#include "daScript/misc/platform.h"

#include "daScript/simulate/debug_info.h"
#include "daScript/ast/ast.h"

namespace das {

    bool FileAccess::isSameFileName ( const string & a, const string & b ) const {
        if ( a.size() != b.size() ) return false;
        auto it_a = a.begin();
        auto it_b = b.begin();
        while ( it_a != a.end() ) {
            bool isSlahA = *it_a=='\\' || *it_a=='/';
            bool isSlahB = *it_b=='\\' || *it_b=='/';
            if ( isSlahA != isSlahB ) {
                return false;
            } else if ( !isSlahA && (tolower(*it_a) != tolower(*it_b)) ) {
                return false;
            }
            ++it_a;
            ++it_b;
        }
        return true;
    }

    FileInfoPtr FileAccess::letGoOfFileInfo ( const string & fileName ) {
        auto it = files.find(fileName);
        if ( it == files.end() ) return nullptr;
        return das::move(it->second);
    }

    FileInfo * FileAccess::setFileInfo ( const string & fileName, FileInfoPtr && info ) {
        files[fileName] = das::move(info);
        auto ins = files.find(fileName);
        ins->second->name = (char *) ins->first.c_str();
        return ins->second.get();
    }

    bool FileAccess::invalidateFileInfo ( const string & fileName ) {
        auto it = files.find(fileName);
        if ( it != files.end() ) {
            files.erase(it);
            return true;
        } else {
            return false;
        }
    }

    FileInfo * FileAccess::getFileInfo ( const string & fileName ) {
        auto it = files.find(fileName);
        if ( it != files.end() ) {
            return it->second.get();
        }
        auto ni = getNewFileInfo(fileName);
        if ( ni ) {
            ni->reserveProfileData();
        }
        return ni;
    }

    string FileAccess::getIncludeFileName ( const string & fileName, const string & incFileName ) const {
        auto np = fileName.find_last_of("\\/");
        if ( np != string::npos ) {
            return fileName.substr(0,np+1) + incFileName;
        } else {
            return incFileName;
        }
    }

    static string getModuleName ( const string & nameWithDots ) {
        auto idx = nameWithDots.find_last_of("./");
        if ( idx==string::npos ) return nameWithDots;
        return nameWithDots.substr(idx+1);
    }

    static string getModuleFileName ( const string & nameWithDots ) {
        auto fname = nameWithDots;
        replace ( fname.begin(), fname.end(), '.', '/' );
        return fname;
    }

    ModuleInfo FileAccess::getModuleInfo ( const string & req, const string & from ) const {
        auto mod = getModuleName(req);
        string modFName = getModuleFileName(req);
        string modFn = getIncludeFileName(from, modFName) + ".das";
        ModuleInfo info;
        info.moduleName = mod;
        info.fileName = modFn;
        return info;
    }

    void FileAccess::freeSourceData() {
        for ( auto & fp : files ) {
            fp.second->freeSourceData();
        }
    }
}
