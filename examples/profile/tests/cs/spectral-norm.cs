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
        Console.WriteLine($"\"{category}\", {minT}, {count}");
    }

    static int A(int i, int j) {
        return ((i + j) * (i + j + 1) / 2 + i + 1);
    }

    static double dot(double [] v, double [] u, int n) {
        int i;
        double sum = 0;
        for (i = 0; i < n; i++)
            sum += v[i] * u[i];
        return sum;
    }

    static void mult_Av(double [] v, double [] output, int n) {
        int i, j;
        double sum;
        for (i = 0; i < n; i++) {
            for (sum = j = 0; j < n; j++)
                sum += v[j] / A(i, j);
            output[i] = sum;
        }
    }

    static void mult_Atv(double [] v, double [] output, int n) {
        int i, j;
        double sum;
        for (i = 0; i < n; i++) {
            for (sum = j = 0; j < n; j++)
                sum += v[j] / A(j, i);
            output[i] = sum;
        }
    }

    static double [] tmp;
    static void mult_AtAv(double [] v, double [] output, int n) {
        mult_Av(v, tmp, n);
        mult_Atv(tmp, output, n);
    }

    static double testSnorm(int n) {
        double[] u = new double[n];
        double[] v = new double[n];
        tmp = new double[n];
        int i;
        for (i = 0; i < n; i++)
            u[i] = 1;
        for (i = 0; i < 10; i++) {
            mult_AtAv(u, v, n);
            mult_AtAv(v, u, n);
        }
        double result = Math.Sqrt(dot(u, v, n) / dot(v, v, n));
        tmp = null;
        u = null;
        v = null;
        return result;
    }


    static void Main() {
        profile(10, "spectral norm", () => {
            testSnorm(500);
        });
    }
}
