module rast2d private

struct public Bitmap
    w, h : int
    [[do_not_delete]] pixels : uint?
