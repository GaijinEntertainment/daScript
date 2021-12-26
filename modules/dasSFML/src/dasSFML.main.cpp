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

    void Module_dasSFML::initAotAlias () {
        addAlias(typeFactory<sf::Vector2f>::make(lib));
        addAlias(typeFactory<sf::Vector2i>::make(lib));
        addAlias(typeFactory<sf::Vector2u>::make(lib));
    }

	void Module_dasSFML::initMain () {
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
