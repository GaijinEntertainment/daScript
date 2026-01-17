options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module bool_array shared public

require strings

struct BoolArray {
    //! A dynamic array of booleans, stored as bits.
    private bits : array<uint>
    private real_size : uint
    def const operator [] (index : int) : bool {
        //! Get the boolean value at the given index.
        let uindex = uint(index)
        if (uindex >= real_size) panic("index out of range, {index}")
        let offs = uindex >> 5u
        let mask = (1u << (uindex & 31u))
        return (bits[offs] & mask) != 0u
    }
    def operator [] =(index : int, value : bool) {
        //! Set the boolean value at the given index.
        let uindex = uint(index)
        if (uindex >= real_size) panic("index out of range, {index}")
        let offs = uindex >> 5u
        let mask = (1u << (uindex & 31u))
        if (value) {
            bits[offs] |= mask
        } else {
            bits[offs] &= ~mask
        }
    }
    def static clear(var self : BoolArray) {
        //! Clear the BoolArray.
        with (self) {
            real_size = 0u
            bits.clear()
        }
    }
    def static reserve(var self : BoolArray, capacity : int) {
        //! Reserve capacity for the BoolArray.
        with (self) {
            assert(capacity >= 0)
            let needed_uints = (uint(capacity) + 31u) >> 5u
            bits.reserve(int(needed_uints))
        }
    }
    def static resize(var self : BoolArray, newSize : int) {
        //! Resize the BoolArray to the new size.
        with (self) {
            assert(newSize >= 0)
            let new_real_size = uint(newSize)
            if (new_real_size > real_size && (real_size & 31u) != 0u) {
                // fill remaining bits of the last uint with zeros
                let offs = real_size >> 5u
                let tail_mask = (1u << (real_size & 31u)) - 1u
                bits[offs] &= tail_mask
            }
            real_size = new_real_size
            let newBitsCount = (real_size + 31u) >> 5u
            bits.resize(int(newBitsCount))
        }
    }
    def static push(var self : BoolArray, value : bool) {
        //! Push a new boolean value to the end of the BoolArray.
        with (self) {
            if ((real_size & 31u) == 0u) {
                // need a new uint
                bits.push(0u)
            }
            let offs = real_size >> 5u
            let mask = (1u << (real_size & 31u))
            if (value) {
                bits[offs] |= mask
            } else {
                bits[offs] &= ~mask
            }
            real_size += 1u
        }
    }
    def static push(var self : BoolArray, value : bool, at : int) {
        //! Push a new boolean value at the given index in the BoolArray.
        if (at != int(self.real_size)) {
            self.insert(at, value)
        } else {
            self.push(value)
        }
    }
    def static pop(var self : BoolArray) : bool {
        //! Pop the last boolean value from the BoolArray and return it.
        with (self) {
            if (real_size == 0u) panic("pop from empty BoolArray")
            let old_size = real_size
            let value = self[ int(old_size - 1u) ]
            resize(self, int(real_size - 1u))
            return value
        }
    }
    def static length(self : BoolArray) : int {
        //! Get the length of the BoolArray.
        return int(self.real_size)
    }
    def static erase(var self : BoolArray, index : int) {
        //! Erase the boolean value at the given index from the BoolArray.
        with (self) {
            let uindex = uint(index)
            if (uindex >= real_size) panic("index out of range, {index}")
            // we will shift all bits after index to the left by one
            // we are going to do it uint by uint
            let total_uints = (real_size + 31u) >> 5u
            var bit_pos = uindex
            while (bit_pos < real_size - 1u) {
                let src_bit = bit_pos + 1u
                let src_offs = src_bit >> 5u
                let src_mask = (1u << (src_bit & 31u))
                let dst_offs = bit_pos >> 5u
                let dst_mask = (1u << (bit_pos & 31u))
                let bit_value = (bits[src_offs] & src_mask) != 0u
                if (bit_value) {
                    bits[dst_offs] |= dst_mask
                } else {
                    bits[dst_offs] &= ~dst_mask
                }
                bit_pos += 1u
            }
            resize(self, int(real_size - 1u))
        }
    }
    def static insert(var self : BoolArray, index : int, value : bool) {
        //! Insert a boolean value at the given index in the BoolArray.
        with (self) {
            let uindex = uint(index)
            if (uindex > real_size) panic("index out of range, {index}")
            // we will shift all bits from index to the right by one
            let old_size = real_size
            resize(self, int(real_size + 1u))
            var bit_pos = old_size
            while (bit_pos > uindex) {
                let src_bit = bit_pos - 1u
                let src_offs = src_bit >> 5u
                let src_mask = (1u << (src_bit & 31u))
                let dst_offs = bit_pos >> 5u
                let dst_mask = (1u << (bit_pos & 31u))
                let bit_value = (bits[src_offs] & src_mask) != 0u
                if (bit_value) {
                    bits[dst_offs] |= dst_mask
                } else {
                    bits[dst_offs] &= ~dst_mask
                }
                bit_pos -= 1u
            }
            self[ int(uindex) ] = value
        }
    }
    def static to_string(self : BoolArray) {
        //! Convert the BoolArray to a string representation.
        return build_string() <| $(wr) {
            wr.write("[")
            for (bit in 0 .. int(self.real_size)) {
                if (bit != 0) wr.write(", ")
                wr.write(self[bit] ? "true" : "false")
            }
            wr.write("]")
        }
    }
    [unsafe_operation]
    def static data_pointer(var self : BoolArray) : uint ? {
        //! Get the data pointer of the BoolArray.
        return unsafe(addr(self.bits[0]))
    }

    [unsafe_operation]
    def static data_pointer(self : BoolArray) : uint const ? {
        //! Get the data pointer of the BoolArray.
        return unsafe(addr(self.bits[0]))
    }
}

[unsafe_outside_of_for]
def each(self : BoolArray) : iterator<bool> {
    let pself = unsafe(addr(self))
    return <- generator<bool>() <| $ {
        let len = (*pself).length()
        let full_uints = uint(len) >> 5u
        let remaining_bits = uint(len) & 31u
        let data_ptr = unsafe((*pself).data_pointer())
        for (i in 0u .. full_uints) {
            var bits_value = unsafe(data_ptr[i])
            for (b in 0u .. 32u) {
                yield (bits_value & 1u) != 0u
                bits_value >>= 1u
            }
        }
        var tail = unsafe(data_ptr[full_uints])
        for (b in 0u .. remaining_bits) {
            yield (tail & 1u) != 0u
            tail >>= 1u
        }
        return false
    }
}
