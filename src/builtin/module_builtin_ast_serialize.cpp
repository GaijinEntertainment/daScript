#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    AstSerializer::AstSerializer ( const vector<uint8_t> & from ) {
        buffer = from;
        writing = false;
    }
    AstSerializer::AstSerializer ( void ) {
        writing = true;
    }

    void AstSerializer::write ( void * data, size_t size ) {
        auto oldSize = buffer.size();
        buffer.resize(oldSize+size);
        memcpy(buffer.data()+oldSize, data, size);
    }

    void AstSerializer::read  ( const void * data, size_t size ) {
        if ( readOffset + size > buffer.size() ) {
            DAS_FATAL_ERROR("ast serializer read overflow");
            return;
        }
    }

    void AstSerializer::serialize ( void * data, size_t size ) {
        if ( writing ) {
            write(data,size);
        } else {
            read(data,size);
        }
    }

    void AstSerializer::tag ( const char * name ) {
        uint64_t hash = hash64z(name);
        if ( writing ) {
            *this << hash;
        } else  {
            uint64_t hash2 = 0;
            *this << hash2;
            if ( hash != hash2 ) {
                DAS_FATAL_ERROR("ast serializer tag mismatch");
            }
        }
    }

    AstSerializer & AstSerializer::operator << ( Type & baseType ) {
        serialize_enum(baseType);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeDeclPtr & type ) {
        if ( !writing ) type = make_smart<TypeDecl>();
        type->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( string & str ) {
        if ( writing ) {
            uint32_t size = (uint32_t) str.size();
            *this << size;
            write((void *)str.data(), size);
        } else {
            uint32_t size = 0;
            * this << size;
            str.resize(size);
            read(&str[0], size);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( LineInfo & at ) {
        *this << at.fileInfo;
        *this << at.column << at.line;
        *this << at.last_column << at.last_line;
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( FileInfo * & info ) {
        // TODO: implement (likely add 'serialize' and 'factory' to FileAccess)
        if ( !writing ) {
            info = nullptr; // temporary!!!
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Module * & module ) {
        if ( writing ) {
            *this << module->name;
        } else {
            DAS_VERIFYF(moduleLibrary!=nullptr, "module library is not set");
            string name;
            *this << name;
            module = moduleLibrary->findModule(name);
            DAS_VERIFYF(module!=nullptr, "module '%s' is not found", name.c_str());
        }
        return *this;
    }
}
