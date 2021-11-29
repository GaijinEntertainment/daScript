#include "daScript/misc/platform.h"

#include "daScript/misc/uric.h"


namespace das {

    Uri::Uri(UriUriA && uriA) {
        uri = uriA;
        memset(&uriA, 0, sizeof(UriUriA));
        isEmpty = false;
        lastOp = 0;
    }

    Uri::Uri() {
        memset(&uri, 0, sizeof(UriUriA));
        isEmpty = true;
    }

    Uri::Uri(const char * uriString) {
        memset(&uri, 0, sizeof(UriUriA));
        parse(uriString);
    }
    Uri::Uri(const string & uriS) {
        memset(&uri, 0, sizeof(UriUriA));
        parse(uriS.c_str());
    }

    Uri::Uri(const Uri & uriA) {
        memset(&uri, 0, sizeof(UriUriA));
        clone(uriA);
    }

    Uri::Uri(Uri && uriA) {
        memset(&uri, 0, sizeof(UriUriA));
        if ( !uriA.isEmpty ) {
            memcpy(&uri,&uriA.uri,sizeof(UriUriA));
            uriA.isEmpty = true;
            lastOp = uriA.lastOp;
            errorPos = uriA.errorPos;
            isEmpty = false;
        }
    }

    Uri & Uri::operator = ( const Uri & uriA ) {
        reset();
        clone(uriA);
        return *this;
    }

    Uri & Uri::operator = ( Uri && uriA ) {
        reset();
        if ( !uriA.isEmpty ) {
            memcpy(&uri,&uriA.uri,sizeof(UriUriA));
            uriA.isEmpty = true;
            lastOp = uriA.lastOp;
            errorPos = uriA.errorPos;
            isEmpty = false;
        }
        return * this;
    }

    Uri & Uri::operator = ( const char * uriString ) {
        reset();
        parse(uriString);
        return *this;
    }

    Uri & Uri::operator = ( const string & uriS ) {
        reset();
        parse(uriS.c_str());
        return *this;
    }

    Uri::~Uri() {
        reset();
    }

    Uri Uri::addBaseUri ( const Uri & base ) const {
        UriUriA absoluteDest;
        auto lOp = uriAddBaseUriA(&absoluteDest, &uri, &base.uri);
        if ( lOp != URI_SUCCESS ) {
            uriFreeUriMembersA(&absoluteDest);
            Uri failed;
            failed.lastOp = lOp;
            return failed;
        }
        return Uri(move(absoluteDest));
    }

    bool Uri::normalize() {
        const unsigned int dirtyParts = uriNormalizeSyntaxMaskRequiredA(&uri);
        lastOp = uriNormalizeSyntaxExA(&uri, dirtyParts);
        return lastOp == URI_SUCCESS;
    }

    void Uri::reset() {
        if ( !isEmpty ) {
            uriFreeUriMembersA(&uri);
            isEmpty = true;
        }
        errorPos = 0;
        lastOp = URI_SUCCESS;
    }

    bool Uri::parse ( const char * uriString ) {
        if ( uriString==nullptr ) uriString = "";
        reset();
        lastOp = uriParseSingleUriA(&uri, uriString, &errorPos);
        if ( lastOp != URI_SUCCESS ) return false;
        uriMakeOwnerA(&uri);
        isEmpty = false;
        lastOp = URI_SUCCESS;
        return true;
    }

    void Uri::clone ( const Uri & uriS ) {
        reset();
        if ( !uriS.isEmpty ) parse(uriS.str().c_str());
    }

    int Uri::size() const {
        if ( isEmpty ) return 0;
        int charsRequired;
        lastOp = uriToStringCharsRequiredA(&uri, &charsRequired);
        return (lastOp != URI_SUCCESS) ? 0 : charsRequired;
    }

    string Uri::str() const {
        int charsRequired = size();
        if (lastOp != URI_SUCCESS) return "";
        charsRequired++;
        vector<char> result;
        result.reserve(charsRequired);
        lastOp = uriToStringA(result.data(), &uri, charsRequired, nullptr);
        return result.data();
    }

    string Uri::toUnixFileName() const {
        auto uriS = str();
        vector<char> result;
        result.reserve(uriS.length()+1);
        lastOp = uriUriStringToUnixFilenameA(uriS.c_str(), result.data());
        return result.data();
    }

    string Uri::toWindowsFileName() const {
        auto uriS = str();
        vector<char> result;
        result.reserve(uriS.length()+1);
        lastOp = uriUriStringToWindowsFilenameA(uriS.c_str(), result.data());
        return result.data();
    }

    string Uri::toFileName() const {
        #ifdef _WIN32
            return toWindowsFileName();
        #else
            return toUnixFileName();
        #endif
    }

    bool Uri::fromUnixFileName ( const char * fileName, int len ) {
        vector<char> result;
        if ( len==-1 ) len = int(strlen(fileName));
        result.reserve(3*len + 1);
        lastOp = uriUnixFilenameToUriStringA(fileName, result.data());
        if ( lastOp != URI_SUCCESS) return false;
        return parse(result.data());

    }

    bool Uri::fromWindowsFileName ( const char * fileName, int len ) {
        vector<char> result;
        if ( len==-1 ) len = int(strlen(fileName));
        result.reserve(8 + 3*len + 1);
        lastOp = uriWindowsFilenameToUriStringA(fileName, result.data());
        if ( lastOp != URI_SUCCESS) return false;
        return parse(result.data());
    }

    bool Uri::fromFileName ( const char * fileName, int len ) {
        #ifdef _WIN32
            return fromWindowsFileName(fileName,len);
        #else
            return fromUnixFileName(fileName,len);
        #endif
    }

    bool Uri::fromUnixFileName ( const string & fileName ) {
        return fromUnixFileName(fileName.c_str(),int(fileName.length()));

    }

    bool Uri::fromWindowsFileName ( const string & fileName ) {
        return fromWindowsFileName(fileName.c_str(),int(fileName.length()));
    }

    bool Uri::fromFileName ( const string & fileName ) {
        return fromFileName(fileName.c_str(),int(fileName.length()));
    }
}
