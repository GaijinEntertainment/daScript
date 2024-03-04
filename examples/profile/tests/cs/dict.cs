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

    public static void MakeRandomSequence(ref string[] src)
    {
        int n = 500000;
        uint mod = (uint)n;
        src = new string[n]; // Resizing the array in C#
        for (int i = 0; i < n; i++)
        {
            uint num = (271828183u ^ (uint)(i * 119)) % mod;
            src[i] = num.ToString();
        }
    }

    public static int Dict(string[] src)
    {
        var tab = new Dictionary<string, int>();
        int maxOcc = 1;
        foreach (var s in src)
        {
            if ( tab.TryGetValue(s, out int val) )
            {
                val ++;
                maxOcc = Math.Max(val, maxOcc);
            }
            else
            {
                tab[s] = 1;
            }

        }
        return maxOcc;
    }

    static void Main() {
        int occ = 0;
        profile(20, "dictionary", () => {
            string[] sequence = null;
            MakeRandomSequence(ref sequence);
            int maxOccurrence = Dict(sequence);
            occ += maxOccurrence;
        });
        Debug.Assert(occ>13);
    }
}
