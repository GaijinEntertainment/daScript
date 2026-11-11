options gen2

module _struct_alias_facade public

require _struct_alias_helper
require _struct_alias_unrequired
require math

typedef AliasedBox = _struct_alias_helper::AliasedBox
typedef AliasedHue = _struct_alias_helper::AliasedHue
typedef AliasedNum = int
typedef SharedAl = _struct_alias_helper::SharedBox
typedef UnrequiredAl = _struct_alias_unrequired::UnrequiredBox
typedef float4x4 = math::float4x4
