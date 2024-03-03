using System;
using System.Diagnostics;
using System.Timers;
using System.Collections.Generic;

class HelloWorld {

    delegate void MyBlock();

    static void profile ( int count, string category, MyBlock f ) {
        double minT = 1e+06;
        for ( int i = 0; i < count; i++ ) {
            DateTime start = DateTime.Now;
            f();
            DateTime end = DateTime.Now;
            TimeSpan elapsed = end - start;
            double dt = ((double)elapsed.TotalMilliseconds) / 1000.0;
            minT = Math.Min(minT, dt);
        }
        Console.WriteLine("\"" + category + "\"," + minT + ", " + count);
    }

    // Check if a number is prime
    public static bool IsPrime(int n)
    {
        for (int i = 2; i < n; i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }
        return true;
    }

    // Count prime numbers up to n
    public static int Primes(int n)
    {
        int count = 0;
        for (int i = 2; i <= n; i++)
        {
            if (IsPrime(i))
            {
                count++;
            }
        }
        return count;
    }

    static void Main() {
        profile(20, "primes loop", () => {
            double f4 = Primes(14000);
            Debug.Assert(f4 == 1652, "The result is not correct.");
        });
    }
}
