creates a temporary array from the given data pointer and length
Important requirements are:
    * data pointer is valid and points to a memory block of at least lenA elements
    * each element follows the next one directly, with the stride equal to size of the element
    * data memory does not change within the lifetime of the returned array
