//! the aliasing suite recompiled under `options no_aliasing` reports
//! every potential argument, result and global alias as an error
//! instead of turning the optimization off
expect 30104:22, 30105:5, 30151:35
options gen2
options no_aliasing

include in_place_build_aliasing.das

