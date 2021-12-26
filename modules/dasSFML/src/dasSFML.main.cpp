#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasSFML.h"
#include "need_dasSFML.h"
#include "aot_dasSFML.h"

namespace das {

    void sfml_window_close ( sf::Window & win ) { win.close(); }

    void Module_dasSFML::initAotAlias () {
        addAlias(typeFactory<sf::Vector2f>::make(lib));
        addAlias(typeFactory<sf::Vector2i>::make(lib));
        addAlias(typeFactory<sf::Vector2u>::make(lib));
    }

	void Module_dasSFML::initMain () {
        // sf::ContextSettings
        addCtorAndUsing<sf::ContextSettings>(*this,lib,"ContextSettings","sf::ContextSettings");
        // sf::VideoMode
        addCtorAndUsing<sf::VideoMode>(*this,lib,"VideoMode","sf::VideoMode");
        addCtor<sf::VideoMode,uint32_t,uint32_t,uint32_t>(*this,lib,"VideoMode","sf::VideoMode")
            ->arg_init(2,make_smart<ExprConstUInt>(24));
        // sf::Window
        addCtorAndUsing<sf::Window>(*this,lib,"Window","sf::Window");
        addCtorAndUsing<sf::Window,sf::VideoMode,char *,uint32_t,const sf::ContextSettings &>(*this,lib,"Window","sf::Window");
        addExtern<DAS_BIND_FUN(sfml_window_close)>(*this,lib,"close",
            SideEffects::worstDefault,"sfml_window_close");
        // time to fix-up const & Vector2i, Vector2u, Vector2f, and String
        for ( auto & pfn : this->functions.each() ) {
            for ( auto & arg : pfn->arguments ) {
                if ( arg->type->constant && arg->type->ref && arg->type->dim.size()==0 ) {
                    if ( arg->type->baseType==Type::tFloat2 || arg->type->baseType==Type::tInt2
                        || arg->type->baseType==Type::tUInt2 || arg->type->baseType==Type::tString ) {
                        arg->type->ref = false;
                    }
                }
            }
        }
        // fixup anything 'string' to need a string cast
        for ( auto & pfn : this->functions.each() ) {
            bool anyString = false;
            for ( auto & arg : pfn->arguments ) {
                if ( arg->type->isString() && !arg->type->ref ) {
                    anyString = true;
                }
            }
            if ( anyString ) {
                pfn->needStringCast = true;
            }
        }
    }

	ModuleAotType Module_dasSFML::aotRequire ( TextWriter & tw ) const {
        // add your stuff here
        tw << "#include \"../modules/dasGlfw/src/sfml_stub.h\"\n";
        tw << "#include \"../modules/dasGlfw/src/aot_dasSFML.h\"\n";
        tw << "#include \"daScript/simulate/bind_enum.h\"\n";
        tw << "#include \"../modules/dasGlfw/src/dasSFML.enum.decl.cast.inc\"\n";
        return ModuleAotType::cpp;
    }

}
