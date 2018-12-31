function isprime(n)
    for i = 2, (n - 1) do
        if (n % i == 0) then
            return false
        end
    end
    return true
end


function primes(n)
    local count = 0

    for i = 2, n do
        if (isprime(i)) then
            count = count + 1
        end
    end
    return count
end


N = 50000
print("primes: " .. primes(N))
