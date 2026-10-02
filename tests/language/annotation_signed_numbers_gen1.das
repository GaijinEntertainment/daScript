options gen2 = false
[comment(min=-2, offset=-1.5, values=(-3, -1.5))]
struct SignedOld
    @min = -2
    value : int
[export]
def main
    verify(typeinfo struct_get_annotation_argument<comment; min>(type<SignedOld>) == -2)
    verify(typeinfo struct_get_annotation_argument<comment; offset>(type<SignedOld>) == -1.5)
