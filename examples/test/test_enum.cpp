#include "daScript/misc/platform.h"

#include "unitTest.h"
#include "module_unitTest.h"

using namespace das;

DAS_BASE_BIND_ENUM(SomeEnum, SomeEnum, zero, one, two)

SomeEnum efn_takeOne_giveTwo ( SomeEnum one) {
    return (one == SomeEnum::one) ? SomeEnum::two : SomeEnum::zero;
}

DAS_BASE_BIND_ENUM_98(SomeEnum98, SomeEnum98, SomeEnum98_zero, SomeEnum98_one, SomeEnum98_two)

SomeEnum98 efn_takeOne_giveTwo_98 ( SomeEnum98 one ) {
    return (one == SomeEnum98_one) ? SomeEnum98_two : SomeEnum98_zero;
}

SomeEnum98_DasProxy efn_takeOne_giveTwo_98_DasProxy ( SomeEnum98_DasProxy two) {
    return (SomeEnum98_DasProxy) efn_takeOne_giveTwo_98( (SomeEnum98)two);
}

void Module_UnitTest::addEnumTest(ModuleLibrary &lib)
{
    // enum
    addEnumeration(make_shared<EnumerationSomeEnum>());
    addExtern<DAS_BIND_FUN(efn_takeOne_giveTwo)>(*this, lib, "efn_takeOne_giveTwo", SideEffects::none);
    // enum98
    addEnumeration(make_shared<EnumerationSomeEnum98>());
    addExtern<DAS_BIND_FUN(efn_takeOne_giveTwo_98_DasProxy)>(*this, lib, "efn_takeOne_giveTwo_98", SideEffects::none);
};

