#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>
#include <chrono>

using namespace std;

struct Variables
{
    int a;
    int c;
    int m;
    int seed;
};

class VariableGenerator
{
public:
    void make_variables(Variables *variables)
    {
        // Time
        auto now = chrono::steady_clock::now();
        auto time_val = now.time_since_epoch().count() / 100;
        // Prime values from 12899 to 13337
        int prime_values[] = {
            11593, 11597, 11617, 11621, 11633, 11657, 11677, 11681, 11689, 11699,
            11701, 11717, 11719, 11731, 11743, 11777, 11779, 11783, 11789, 11801,
            11807, 11813, 11821, 11827, 11831, 11833, 11839, 11863, 11867, 11887,
            11897, 11903, 11909, 11923, 11927, 11933, 11939, 11941, 11953, 11959,
            11969, 11971, 11981, 11987, 12007, 12011, 12037, 12041, 12043, 12049,
            12071, 12073, 12097, 12101, 12107, 12109, 12113, 12119, 12143, 12149,
            12157, 12161, 12163, 12197, 12203, 12211, 12227, 12239, 12241, 12251,
            12253, 12263, 12269, 12277, 12281, 12289, 12301, 12323, 12329, 12343,
            12347, 12373, 12377, 12379, 12391, 12401, 12409, 12413, 12421, 12433,
            12437, 12451, 12457, 12473, 12479, 12487, 12491, 12497, 12503, 12511,
            12517, 12527, 12539, 12541, 12547, 12553, 12569, 12577, 12583, 12589,
            12601, 12611, 12613, 12619, 12637, 12641, 12647, 12653, 12659, 12671,
            12689, 12697, 12703, 12713, 12721, 12739, 12743, 12757, 12763, 12781,
            12791, 12799, 12809, 12821, 12823, 12829, 12841, 12853, 12889, 12893,
            12899, 12907, 12911, 12917, 12919, 12923, 12941, 12953, 12959, 12967,
            12973, 12979, 12983, 13001, 13003, 13007, 13009, 13033, 13037, 13043,
            13049, 13063, 13093, 13099, 13103, 13109, 13121, 13127, 13147, 13151,
            13159, 13163, 13171, 13177, 13183, 13187, 13217, 13219, 13229, 13241,
            13249, 13259, 13267, 13291, 13297, 13309, 13313, 13327, 13331, 13337};
        // How to get the index of m
        int index = *(&prime_values + 1) - prime_values;
        // a, c, m, seed variables
        int a = pow(10, (time_val % 3 + 2)) + 1;
        int c = ((time_val % 64) * 200) + 21;
        int m = prime_values[time_val % index];
        int seed = time_val % m;
        *variables = {a, c, m, seed};
    }

    // Time for 1000: 1975.02 ms
    bool check_full_period(Variables *variables)
    {
        int xn1 = (variables->a * variables->seed + variables->c) % variables->m;
        int xn1s[variables->m - 1] = {xn1};
        for (int i = 1; i < variables->m - 1; i++)
        {
            xn1 = (variables->a * xn1 + variables->c) % variables->m;
            xn1s[i] = xn1;
        }
        sort(xn1s, xn1s + variables->m - 1);
        for (int i = 0; i < variables->m - 2; i++)
        {
            if (xn1s[i] == xn1s[i + 1])
                return false;
        }
        return true;
    }
};

int main()
{
    VariableGenerator var_gen;
    Variables variables = {};
    int times = 1000;
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < times; i++)
    {
        var_gen.make_variables(&variables);
        var_gen.check_full_period(&variables);
    }
    auto stop = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
    cout << "Time for " << times << ": " << duration / 1e6 << " ms" << endl;
    return 0;
}