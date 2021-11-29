#pragma once

#define URI_STATIC_BUILD
#include "uriparser/Uri.h"

namespace das {

    class Uri {
    public:
        Uri();
        Uri(const char *);
        Uri(const string &);
        Uri(const Uri &);
        Uri(Uri &&);
        Uri & operator = ( const Uri & );
        Uri & operator = ( Uri && );
        Uri & operator = ( const char * );
        Uri & operator = ( const string & );
        ~Uri();
        bool status() const { return lastOp==URI_SUCCESS; }
        const char * getErrorPos() const { return errorPos; }
        bool parse ( const char * );
        void reset();
        string str() const;
        int size() const;
        bool empty() const { return isEmpty; }
        bool normalize();
        string toUnixFileName() const;
        string toWindowsFileName() const;
        string toFileName() const;
        bool fromUnixFileName ( const string & );
        bool fromWindowsFileName ( const string & );
        bool fromFileName ( const string & );
        bool fromUnixFileName ( const char *, int len = -1 );
        bool fromWindowsFileName ( const char *, int len = -1 );
        bool fromFileName ( const char *, int len = -1 );
    protected:
        void clone ( const Uri & uri );
    protected:
        const char* errorPos = nullptr;
        mutable int lastOp = 0;
        bool    isEmpty = true;
    public: // note - this is public so that we can bind it easier
        UriUriA uri;
    };

}