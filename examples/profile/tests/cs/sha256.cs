using System;
using System.Diagnostics;
using System.Timers;
using System.Collections.Generic;
using System.Text;
using System.Linq;

class HelloWorld
{

    delegate void MyBlock();

    static void profile(int count, string category, MyBlock f)
    {
        double minT = 1e+06;
        for (int i = 0; i < count; i++)
        {
            DateTime start = DateTime.Now;
            f();
            DateTime end = DateTime.Now;
            TimeSpan elapsed = end - start;
            double dt = ((double)elapsed.TotalMilliseconds) / 1000.0;
            minT = Math.Min(minT, dt);
        }
        Console.WriteLine("\"" + category + "\"," + minT + ", " + count);
    }

    private static readonly uint[] primes =
    {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
    };

    private static uint RightRotate(uint value, int bits)
    {
        return (value >> bits) | (value << (32 - bits));
    }

    private static uint Ch(uint x, uint y, uint z)
    {
        return (x & y) ^ (~x & z);
    }

    private static uint Maj(uint x, uint y, uint z)
    {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    private static uint Sigma0(uint x)
    {
        return RightRotate(x, 2) ^ RightRotate(x, 13) ^ RightRotate(x, 22);
    }

    private static uint Sigma1(uint x)
    {
        return RightRotate(x, 6) ^ RightRotate(x, 11) ^ RightRotate(x, 25);
    }

    private static uint Omega0(uint x)
    {
        return RightRotate(x, 7) ^ RightRotate(x, 18) ^ (x >> 3);
    }

    private static uint Omega1(uint x)
    {
        return RightRotate(x, 17) ^ RightRotate(x, 19) ^ (x >> 10);
    }

    public static string ComputeHash(string input)
    {
        byte[] bytes = Encoding.UTF8.GetBytes(input);

        // Initialize hash values.
        uint[] hash = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19, };


        // Process the message in successive 512-bit chunks:
        for (int i = 0; i < bytes.Length; i += 64)
        {
            uint[] w = new uint[64];
            for (int t = 0; t < 16; t++)
            {
                w[t] = BitConverter.ToUInt32(bytes.Skip(i + t * 4).Take(4).Reverse().ToArray(), 0);
            }

            for (int t = 16; t < 64; t++)
            {
                w[t] = Omega1(w[t - 2]) + w[t - 7] + Omega0(w[t - 15]) + w[t - 16];
            }

            uint a = hash[0];
            uint b = hash[1];
            uint c = hash[2];
            uint d = hash[3];
            uint e = hash[4];
            uint f = hash[5];
            uint g = hash[6];
            uint h = hash[7];

            for (int t = 0; t < 64; t++)
            {
                uint T1 = h + Sigma1(e) + Ch(e, f, g) + primes[t] + w[t];
                uint T2 = Sigma0(a) + Maj(a, b, c);
                h = g;
                g = f;
                f = e;
                e = d + T1;
                d = c;
                c = b;
                b = a;
                a = T1 + T2;
            }

            hash[0] += a;
            hash[1] += b;
            hash[2] += c;
            hash[3] += d;
            hash[4] += e;
            hash[5] += f;
            hash[6] += g;
            hash[7] += h;
        }

        StringBuilder result = new StringBuilder();
        foreach (uint part in hash)
        {
            result.Append(part.ToString("x8"));
        }

        return result.ToString();
    }

    static void Main()
    {
        string res = "";
        DateTime start = DateTime.Now;
        string input = new string('.', 1024);
        profile(1, "sha256", () => {
            res = ComputeHash(input);
        });
        DateTime end = DateTime.Now;
        TimeSpan elapsed = end - start;
        double dt = ((double)elapsed.TotalMilliseconds) / 1000.0;
        Console.WriteLine($"\t{1 / dt} mb/sec");
        Debug.Assert(res == "8adcaee60bb05a9964a1df12d2f007adcb8f3fa20ff7d1ecfde0a2ac301ff412");

    }
}
