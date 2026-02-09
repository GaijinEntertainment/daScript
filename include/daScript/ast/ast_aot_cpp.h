#ifndef DAS_AST_AOT_CPP_H
#define DAS_AST_AOT_CPP_H

#include "daScript/daScript.h"

namespace das {

    inline bool checkFileContentsIsTheSame ( const string_view & fname, const string_view & str ) {
        bool file_is_the_same = false;
        if ( FILE * f = fopen ( fname.data(), "rb" ) )
        {
            fseek ( f, 0, SEEK_END );
            if ( str.length() == ftell ( f ) )
            {
                if ( void * buf = malloc ( str.length() ) )
                {
                    fseek ( f, 0, SEEK_SET );
                    if ( fread ( buf, 1, str.length(), f ) == str.length() && memcmp ( str.data(), buf, str.length() ) == 0 )
                        file_is_the_same = true; // file is the same, don't write!
                    free ( buf );
                }
            }
            fclose ( f );
        }
        return file_is_the_same;
    }

    inline bool saveToFile ( TextPrinter &tout, const string_view & fname, const string_view & str, bool quiet = false ) {
        if ( checkFileContentsIsTheSame ( fname, str ) ) {
            if ( !quiet )
                tout << "contents is the same, skip writing to " << fname.data() << "\n";
            return true;
        }
        FILE * f = fopen ( fname.data(), "w" );
        if ( !f ) {
            tout << "can't open " << fname.data() << "\n";
            return false;
        }
        if ( !quiet )  {
            tout << "saving to " << fname.data() << "\n";
        }
        fwrite ( str.data(), str.length(), 1, f );
        fclose ( f );
        return true;
    }

    /**
     * Shortcut to wrap code into namespace
     */
    class NamespaceGuard {
    public:
        NamespaceGuard(TextWriter &tw, string namesp) : tw_(tw), namesp_(das::move(namesp)) {
            tw_ << "namespace " << namesp_ << " {\n";
        }
        ~NamespaceGuard() {
            tw_ << "} // namespace " << namesp_ << "\n";
        }
    private:
        TextWriter &tw_;
        string namesp_;
    };

    /**
     * Shortcut to wrap code into class
     */
    class ClassGuard {
    public:
        ClassGuard(TextWriter &tw, string class_name) : tw_(tw) {
            tw_ << "class " << class_name << " {\n";
        }
        ~ClassGuard() {
            tw_ << "};\n";
        }
    private:
        TextWriter &tw_;
    };

    struct StandaloneContextCfg {
        string context_name;
        string class_name;
        bool cross_platform = false;
    };

    /**
     * Enumerate all aot functions
     */
    void dumpRegisterAot(TextWriter& tw, ProgramPtr program, Context & context, bool allModules = false);

    /**
     * Create standalone context
     * @param program
     * @param cppOutputDir output directory where context will be created
     * @param standaloneContextName class name
     */
    DAS_API void runStandaloneVisitor(ProgramPtr program, const string& cppOutputDir, const StandaloneContextCfg &cfg);
}

#endif //DAS_AST_AOT_CPP_H
