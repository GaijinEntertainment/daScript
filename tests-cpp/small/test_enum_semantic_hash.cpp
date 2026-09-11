// the semantic hash of an enumeration type reads each entry's folded value: two enumerations
// that differ only in a value hash apart, two spelled the same hash alike

#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
using namespace das;

namespace {

Enumeration * makeEnum ( const string & name, std::initializer_list<pair<const char *, int32_t>> entries ) {
    auto e = new Enumeration(name);
    for ( auto & en : entries ) {
        e->add(en.first, new ExprConstInt(en.second), cppBindingLineInfo());
    }
    return e;
}

}

TEST_CASE("enum semantic hash reads the entry values") {
    gc_guard guard;
    TypeDeclPtr ab12 = new TypeDecl(makeEnum("E", {{"A",1},{"B",2}}));
    TypeDeclPtr ab13 = new TypeDecl(makeEnum("E", {{"A",1},{"B",3}}));
    TypeDeclPtr ab12again = new TypeDecl(makeEnum("E", {{"A",1},{"B",2}}));
    TypeDeclPtr ac12 = new TypeDecl(makeEnum("E", {{"A",1},{"C",2}}));
    CHECK_NE(ab12->getSemanticHash(), ab13->getSemanticHash());
    CHECK_NE(ab12->getSemanticHash(), ac12->getSemanticHash());
    CHECK_EQ(ab12->getSemanticHash(), ab12again->getSemanticHash());
    CHECK_NE(ab12->getOwnSemanticHash(), ab13->getOwnSemanticHash());
    CHECK_EQ(ab12->getOwnSemanticHash(), ab12again->getOwnSemanticHash());
}
