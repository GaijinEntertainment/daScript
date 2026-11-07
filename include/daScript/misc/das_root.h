#pragma once

#include <cctype>
#include <cstdio>
#include <cstring>

#if defined(_WIN32)
    #include <filesystem>
    #include <string>
#else
    #include <sys/stat.h>
#endif

// the helpers the daslang stub and the runtime share, over whichever string the caller holds -
// std::string in the stub, das::string (eastl under a Dagor config) in the runtime
namespace das {

    template <typename String>
    inline String dasPathPrefix ( const String & path ) {
        auto np = path.find_last_of("\\/");
        return np==String::npos ? path : path.substr(0, np);
    }

    template <typename String>
    inline String dasPathSuffix ( const String & path ) {
        auto np = path.find_last_of("\\/");
        return np==String::npos ? path : path.substr(np + 1);
    }

    //! the binary's name without its folder or `.exe`, lowercased on Windows, where names are case-blind
    template <typename String>
    inline String dasBinaryStem ( const String & path ) {
        String stem = dasPathSuffix(path);
#if defined(_WIN32)
        for ( auto & ch : stem ) ch = char(tolower((unsigned char)ch));
#endif
        if ( stem.size() > 4 && stem.compare(stem.size() - 4, 4, ".exe")==0 ) stem.resize(stem.size() - 4);
        return stem;
    }

    template <typename String>
    struct DasBinLayout {
        bool    underBin = false;
        String  root;
        String  bin;
        String  config;
    };

    //! `<root>/bin/<exeDir>` or `<root>/bin/<config>/<exeDir>`: root and the config folder ("" for none); underBin false for any other folder
    template <typename String>
    inline DasBinLayout<String> dasBinLayoutOf ( const String & exeDir ) {
        DasBinLayout<String> layout;
        String bin = exeDir;
        if ( dasPathSuffix(bin)!="bin" ) {
            layout.config = dasPathSuffix(bin);
            bin = dasPathPrefix(bin);
        }
        if ( dasPathSuffix(bin)!="bin" ) return DasBinLayout<String>();
        layout.underBin = true;
        layout.bin = bin;
        layout.root = dasPathPrefix(bin);
        return layout;
    }

#if defined(_WIN32)
    //! the UTF-8 `path` as a filesystem path; false when the bytes are not UTF-8 (a sidecar saved in
    //! the ANSI codepage), which the conversion would otherwise throw on
    inline bool dasWidePath ( const char * path, std::filesystem::path & out ) {
        try {
            out = std::filesystem::u8path(path);
            return true;
        } catch ( ... ) {
            return false;
        }
    }
#endif

    inline bool dasIsDirectory ( const char * path ) {
#if defined(_WIN32)
        std::filesystem::path p;
        std::error_code ec;
        return dasWidePath(path, p) && std::filesystem::is_directory(p, ec);
#else
        struct stat st;
        return stat(path, &st)==0 && (st.st_mode & S_IFDIR)!=0;
#endif
    }

    inline bool dasIsFile ( const char * path ) {
#if defined(_WIN32)
        std::filesystem::path p;
        std::error_code ec;
        return dasWidePath(path, p) && std::filesystem::is_regular_file(p, ec);
#else
        struct stat st;
        return stat(path, &st)==0 && (st.st_mode & S_IFREG)!=0;
#endif
    }

    inline FILE * dasOpenForReading ( const char * path ) {
#if defined(_WIN32)
        std::filesystem::path p;
        return dasWidePath(path, p) ? _wfopen(p.c_str(), L"rb") : nullptr;
#else
        return fopen(path, "rb");
#endif
    }

    inline const char * dasRootSidecarName () { return "daslang.root"; }

    //! `line` with its UTF-8 byte order mark (first line only), spaces, tabs and line end trimmed; the trimmed length
    inline size_t dasTrimSidecarLine ( char * line, bool first ) {
        size_t b = 0, e = strlen(line);
        if ( first && e >= 3 && memcmp(line, "\xEF\xBB\xBF", 3)==0 ) b = 3;
        while ( b < e && (line[b]==' ' || line[b]=='\t') ) b++;
        while ( e > b && (line[e-1]==' ' || line[e-1]=='\t' || line[e-1]=='\r' || line[e-1]=='\n') ) e--;
        memmove(line, line + b, e - b);
        line[e - b] = 0;
        return e - b;
    }

    template <typename String>
    inline String dasRootSidecarCandidate ( const String & exeDir, const char * line ) {
        bool absolute = line[0]=='/' || line[0]=='\\' || (line[1] && line[1]==':');
        return absolute ? String(line) : exeDir + "/" + line;
    }

    // src/misc/ARCHITECTURE_DAS_ROOT.md#root-sidecar
    //! the first folder the sidecar beside `exeDir`'s binary names that exists, "" when it is absent or names none
    template <typename String>
    inline String readDasRootSidecar ( const String & exeDir ) {
        String found;
        FILE * f = dasOpenForReading((exeDir + "/" + dasRootSidecarName()).c_str());
        if ( !f ) return found;
        char line[4096];
        bool first = true;
        while ( fgets(line, int(sizeof(line)), f) ) {
            size_t n = dasTrimSidecarLine(line, first);
            first = false;
            if ( !n ) continue;
            String candidate = dasRootSidecarCandidate(exeDir, line);
            if ( dasIsDirectory(candidate.c_str()) ) { found = candidate; break; }
        }
        fclose(f);
        return found;
    }

}
