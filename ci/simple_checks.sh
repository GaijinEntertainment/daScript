# check for tabs
! grep -rn $'\t' --exclude=*lexer.cpp \
                --exclude=MacOSXBundleInfo.plist.in\
                --exclude=*.yy.h\
                utils src include || {
 echo "tabs found!"; exit 1
}
# check for trailing spaces
! grep -rn '[ ]$' --exclude=*lexer.cpp\
                 --exclude=*.yy.h\
                 --exclude=fast_float.h\
                 utils src include || {
 echo "trailing spaces found!";
 exit 1
}
# check for std::
! grep -rn --include='*.cpp'\
           --include='*.hpp'\
           --include='*.h'\
           --exclude-dir=modules\
           --exclude-dir=web\
           --exclude-dir='3rdparty'\
           --exclude='*lexer.cpp'\
           --exclude={'fast_float.h','flat_hash_map.hpp','das_config.h','callable.h','format.cpp','unitTest.h'}\
           'std::' || {
  echo "do not use std::!"; exit 1
}
