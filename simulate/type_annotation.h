#pragma once

namespace yzg {
    
    struct TypeDecl;
    class Context;
    struct LineInfo;
    class Module;
    class ModuleLibrary;
    class Structure;
    struct TypeAnnotation;
    
    typedef shared_ptr<TypeAnnotation> TypeAnnotationPtr;
    
    struct TypeAnnotation : enable_shared_from_this<TypeAnnotation> {
        TypeAnnotation ( const string & n ) : name(n) {}
        virtual ~TypeAnnotation() {}
        virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const {
            assert(p && "can only clone real type");
            p->name = name;
            return p;
        }
        virtual bool canMove() const { return true; }
        virtual bool canCopy() const { return true; }
        virtual bool isPod() const { return true; }
        virtual bool isRefType() const { return false; }
        virtual bool isLocal() const { return false; }
        virtual bool isNewable() const { return false; }
        virtual bool isIndexable ( TypeDecl * indexType ) const { return false; }
        virtual bool isIterable ( ) const { return false; }
        virtual bool isStructureAnnotation() const { return false; }
        virtual size_t getSizeOf() const { return sizeof(void *); }
        virtual TypeDecl * getField ( const string & ) const { return nullptr; }
        virtual TypeDecl * getIndex ( TypeDecl * ) const { return nullptr; }
        virtual TypeDecl * getIterator () const { return nullptr; }
        virtual SimNode * simulateCopy ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const { return nullptr; }
        virtual SimNode * simulateRef2Value ( Context & context, const LineInfo & at, SimNode * l ) const { return nullptr; }
        virtual SimNode * simulateGetField ( const string & name, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateSafeGetField ( const string & name, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateSafeGetFieldPtr ( const string & name, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetNew ( Context &, const LineInfo & ) const { return nullptr; }
        virtual SimNode * simulateGetAt ( Context &, const LineInfo &, SimNode *, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetIterator ( Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual void debug ( stringstream & ss, void * data ) const { ss << "handle<" << name << ">"; }
        string      name;
        Module *    module = nullptr;
    };
    
    // annotated structure
    //  needs to override
    //      create,clone, simulateGetField, SafeGetField, and SafeGetFieldPtr
    struct StructureTypeAnnotation : TypeAnnotation {
        StructureTypeAnnotation ( const string & n ) : TypeAnnotation(n) {}
        virtual bool isStructureAnnotation() const override { return true; }
        virtual bool canCopy() const override { return false; }
        virtual bool isPod() const override { return false; }
        virtual bool isRefType() const override { return false; }
        virtual bool create ( const shared_ptr<Structure> & st, string & /* err */ ) {
            structureType = st;
            return true;
        }
        virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const override {
            shared_ptr<StructureTypeAnnotation> cp =  p ? static_pointer_cast<StructureTypeAnnotation>(p) : make_shared<StructureTypeAnnotation>(name);
            cp->structureType = structureType;
            return TypeAnnotation::clone(cp);
        }
        shared_ptr<Structure>   structureType;
    };
    
}
