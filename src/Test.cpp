#include "VariableGenerator.h"
#include "Variables.h"
#include "Test.h"
#include <iostream>
#include <deque>
#include <math.h>
#include <cmath>

int calc_xn1(Variables var, int xn)
{
    return (var.a * xn + var.c) % var.m;
}

// alpha == 5%
bool mean_test(float random_samples[], int sample_size)
{
    float EXPECTED_MEAN = 0.5;
    float Z_VALUE = 1.96;
    float sum, observed_mean, z0;
    for (int i = 0; i < sample_size; i++)
    {
        sum += random_samples[i];
    }
    observed_mean = sum / sample_size;
    z0 = (observed_mean - EXPECTED_MEAN) * sqrt(sample_size) / sqrt(1 / 12.0);
    return abs(z0) < Z_VALUE;
}

// alpha == 5%
bool frequency_test(float random_samples[], int sample_size, int n)
{
    // From n = 2 to n = 11
    float chi_vals[] = {3.841, 5.991, 7.815, 9.488, 11.070, 12.592, 14.067, 15.507, 16.919, 18.307};
    float val = 1.0 / n;
    float FE = (float) sample_size / n;
    float fo[n] = {};
    int interval = {};
    float chi2_0 = {}, chi2_alpha_lib = chi_vals[n - 2];
    for (int i = 0; i < sample_size; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (val * j <= random_samples[i] && random_samples[i] < val * (j + 1))
            {
                fo[j]++;
                break;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        chi2_0 += pow(fo[i] - FE, 2) / FE;   
    }
    return chi2_0 < chi2_alpha_lib;
}

std::deque<Variables> Test::apply_tests(int n_variables, int sample_size)
{
    std::deque<Variables> var_container;
    VariableGenerator var_gen;
    Variables vars;
    float random_samples[sample_size];
    float xn, xn1;
    for (int i = 0; i < n_variables; i++)
    {
        // Generate new complete variables
        vars = var_gen.get_complete_variables();
        if (vars.m < sample_size)
            continue;

        // Create the random samples for the tests
        for (int j = 0; j < sample_size; j++)
        {
            // The first time xn == seed
            xn = j == 0 ? vars.seed : xn1;
            xn1 = calc_xn1(vars, xn);
            random_samples[j] = xn1 / vars.m;
        }

        // Put in practice each test
        if (!mean_test(random_samples, sample_size))
            continue;
        if (!frequency_test(random_samples, sample_size, 10))
            continue;
        var_container.push_front(vars);
    }
    return var_container;
}
