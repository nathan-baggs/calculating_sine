#include <iostream>

#include <bit>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <numbers>
#include <tuple>

using namespace std::chrono_literals;

namespace
{

/**
 * Baseline implementation using std::sin.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float standard_calculator(float theta)
{
    return std::sin(theta);
}

/**
 * Implementation of sine using x87 instructions.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float asm_calculator(float theta)
{
    float result;

    asm(R"(flds %1
           fsin
           fstps %0)"
        : "=m"(result)
        : "m"(theta));

    return result;
}

/**
 * Implementation of sine with one expansion of Maclaurin series expansion.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float maclaurin_1_calculator(float theta)
{
    return theta;
}

/**
 * Implementation of sine with two expansions of Maclaurin series expansion.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float maclaurin_2_calculator(float theta)
{
    return theta - ((std::pow(theta, 3.0f) / 6.0f));
}

/**
 * Implementation of sine with three expansions of Maclaurin series expansion.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float maclaurin_3_calculator(float theta)
{
    return theta - (std::pow(theta, 3.0f) / 6.0f) + (std::pow(theta, 5.0f) / 120.0f);
}

/**
 * Implementation of sine with four expansions of Maclaurin series expansion.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float maclaurin_4_calculator(float theta)
{
    return theta - (std::pow(theta, 3.0f) / 6.0f) + (std::pow(theta, 5.0f) / 120.0f) -
           (std::pow(theta, 7.0f) / 5040.0f);
}

/**
 * This is the base case for the Chebyshev polynomial expansion, it just returns 1 so isn't really useful but is
 * included for completeness.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float chebyshev_0_calculator(float theta)
{
    return 1.0f;
}

/**
 * Implementation of sine with one expansion of Chebyshev polynomials.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float chebyshev_1_calculator(float theta)
{
    return theta;
}

/**
 * Implementation of sine with two expansions of Chebyshev polynomials.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float chebyshev_2_calculator(float theta)
{
    return (2.0f * std::pow(theta, 2.0f)) - 1.0f;
}

/**
 * Implementation of sine with three expansions of Chebyshev polynomials.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Sine of input value.
 */
float chebyshev_3_calculator(float theta)
{
    return (3.0f * std::pow(theta, 3.0f)) - (3.0f * theta);
}

/**
 * Base implementation of calculating sine and cosm, using std::sin and std::cos.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Tuple of sine and cos of input value.
 */
std::tuple<float, float> standard_sin_cos_calculator(float theta)
{
    return {std::sin(theta), std::cos(theta)};
}

/**
 * Implementation of sin and cos using one x87 instruction.
 *
 * @param theta
 *   Input value.
 *
 * @returns
 *   Tuple of sine and cos of input value.
 */
std::tuple<float, float> asm_sin_cos_calculator(float theta)
{
    float r1;
    float r2;

    asm(R"(flds %2
           fsincos
           fstps %0
           fstps %1)"
        : "=m"(r1), "=m"(r2)
        : "m"(theta));

    return {r1, r2};
}

/**
 * Time how long it takes for a function to calculate every possible float.
 *
 * @param calculator
 *   Function to time.
 *
 * @returns
 *   Number of nanoseconds spend executing function for all floats.
 */
template <class F>
std::chrono::nanoseconds time_calculations(F calculator)
{
    auto time = 0ns;

    for (std::size_t i = 0u; i < std::numeric_limits<std::uint32_t>::max(); ++i)
    {
        const auto n = static_cast<std::uint32_t>(i);
        const auto f = std::bit_cast<float>(n);

        const auto start = std::chrono::high_resolution_clock::now();
        const volatile auto r = calculator(f);
        const auto end = std::chrono::high_resolution_clock::now();

        time += end - start;
    }

    return time;
}

/**
 * Calculate the difference between the base line function and a supplied function, write each difference out to a
 * binary file.
 *
 * @param file_name
 *   Name of file to write diffs to.
 *
 * @param calculator
 *   Function to compare with baseline.
 */
void write_data(const std::string &file_name, float (*calculator)(float))
{
    std::ofstream out{file_name, std::ios::out | std::ios::binary};

    static const float interval = 0.00001f;
    auto f = 0.0f;

    do
    {
        const auto r = std::fabs(calculator(f) - standard_calculator(f));
        out.write(reinterpret_cast<const char *>(&r), sizeof(r));

        f += interval;
    } while (f <= 2.0f * std::numbers::pi_v<float>);

    std::cout << file_name << " written\n";
}

}

int main()
{
    std::cout << "starting accuracy tests\n";

    write_data("asm_accuracy", asm_calculator);
    write_data("maclaurin_0_accuracy", maclaurin_0_calculator);
    write_data("maclaurin_1_accuracy", maclaurin_1_calculator);
    write_data("maclaurin_2_accuracy", maclaurin_2_calculator);
    write_data("maclaurin_3_accuracy", maclaurin_3_calculator);
    write_data("chebyshev_0_accuracy", chebyshev_0_calculator);
    write_data("chebyshev_1_accuracy", chebyshev_1_calculator);
    write_data("chebyshev_2_accuracy", chebyshev_2_calculator);
    write_data("chebyshev_3_accuracy", chebyshev_3_calculator);

    std::cout << "accuracy tests done\n\n";

    std::cout << "starting performance tests\n";

    std::cout << "standard: " << time_calculations(standard_calculator).count() << "ns\n";
    std::cout << "asm: " << time_calculations(asm_calculator).count() << "ns\n";
    std::cout << "maclaruin_0: " << time_calculations(maclaurin_0_calculator).count() << "ns\n";
    std::cout << "maclaruin_1: " << time_calculations(maclaurin_1_calculator).count() << "ns\n";
    std::cout << "maclaruin_2: " << time_calculations(maclaurin_2_calculator).count() << "ns\n";
    std::cout << "maclaruin_3: " << time_calculations(maclaurin_3_calculator).count() << "ns\n";
    std::cout << "chebyshev_0: " << time_calculations(chebyshev_0_calculator).count() << "ns\n";
    std::cout << "chebyshev_1: " << time_calculations(chebyshev_1_calculator).count() << "ns\n";
    std::cout << "chebyshev_2: " << time_calculations(chebyshev_2_calculator).count() << "ns\n";
    std::cout << "chebyshev_3: " << time_calculations(chebyshev_3_calculator).count() << "ns\n";

    std::cout << "standard sincos: " << time_calculations(standard_sin_cos_calculator).count() << "ns\n";
    std::cout << "asm sincos: " << time_calculations(asm_sin_cos_calculator).count() << "ns\n";

    std::cout << "performance tests done\n\n";

    return 0;
}