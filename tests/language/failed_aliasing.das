//! the aliasing suite recompiled under `options no_aliasing` reports
//! every potential argument, result and global alias as an error
//! instead of turning the optimization off
expect 30104:18, 30105:5, 30151:3
options gen2
options no_aliasing

include uncategorized_no_aliasing_false.das

