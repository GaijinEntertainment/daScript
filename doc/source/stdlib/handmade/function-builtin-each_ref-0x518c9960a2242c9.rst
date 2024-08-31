Iterates over values, which are supplied via lambda.
When lambda returns false - no more values are iterated.
This is similar to 'each' function, but it takes pointer to the value instead of reference.
That way by-value types can be iterated as well.
:param lam: lambda which takes pointer to the value we iterate over, and returns bool
:returns: iterator


