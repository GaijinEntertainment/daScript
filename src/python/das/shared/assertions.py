def assert_equal(a, b):
    if a != b:
        raise AssertionError('{} != {}'.format(repr(a), repr(b)))

def assert_not_equal(a, b):
    if a == b:
        raise AssertionError('{} == {}'.format(repr(a), repr(b)))

def assert_is_instance(x, cls):
    if not isinstance(x, cls):
        raise AssertionError('{} is not an instance of {}'.format(
            repr(x), repr(cls)))
