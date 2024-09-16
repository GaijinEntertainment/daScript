Indicates that function is unsafe, and will require `unsafe` keyword to be called::

    [unsafe_operation]
    def public temp_array ( var arr : auto implicit ==const )
        ...

    [export]
    def main
        ...
        temp_array(arr) // will not compile, need to use `unsafe(temp_array(arr))` instead

