using System;
using System.Diagnostics;
using System.Timers;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

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
        Console.WriteLine($"\"{category}\", {minT}, {count}");
    }

    static int N = 8;

    // check whether position (n,c) is free from attacks
    static bool isplaceok ( int[] a, int n, int c ) {
        for ( int i = 0; i < n; i++ ) {
            if ( a[i] == c || a[i] - i == c - n || a[i] + i == c + n ) {
                return false;
            }
        }
        return true;
    }

    static List<string> solutions = new List<string>();

    static void printsolution ( int[] a ) {
        string result = "";
        for ( int i = 0; i < N; i++ ) {
            for ( int j = 0; j < N; j++ ) {
                result += a[i] == j ? "X" : "-";
                result += " ";
            }
            result += "\n";
        }
        result += "\n";
        solutions.Add(result);
    }

    static void addqueen ( int[] a, int n ) {
        if ( n == N ) {
            printsolution(a);
        } else {
            for ( int c = 0; c < N; c++ ) {
                if ( isplaceok(a, n, c) ) {
                    a[n] = c;
                    addqueen(a, n + 1);
                }
            }
        }
    }

    [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
    static void test () {
        solutions.Clear();
        int[] a = new int[N];
        addqueen(a, 0);
    }

    static void Main() {
        profile(1, "queen", () => {
            test();
            Debug.Assert(solutions.Count == 92, "The result is not correct.");
        });
    }
}
