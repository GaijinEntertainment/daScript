#! /bin/sh
# Run GCC preprocessor and delete empty lines
cpp -C -D URI_DOXYGEN -I ../include $1 | sed '/^$/d' | sed -r 's/COMMENT_HACK//g'
