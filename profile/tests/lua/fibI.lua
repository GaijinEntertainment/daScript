function fibR(n)

    if (n < 2) then return n end
    return (fibR(n-2) + fibR(n-1))
end


function fibI(n)

    local last = 0
    local cur = 1
    n = n - 1
    while (n > 0)
    do
        n = n - 1
        local tmp = cur
        cur = last + cur
        last = tmp
    end
    return cur
end



local start = os.clock()
--N = 34 --Should return 433494437
fibI(11111134)
--print("fib: " .. fibR(N) .. " = " .. fibI(N))
io.write(string.format("elapsed: %.8f\n", os.clock() - start))