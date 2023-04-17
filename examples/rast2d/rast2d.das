module rast2d private

require math

struct public Bitmap8
    //! single surface
    size : int2
    [[do_not_delete]] data : uint8?


