#include <iostream>

#include <bit>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>

using namespace std::chrono_literals;

namespace
{

float standard_calculator(float theta)
{
    return std::sin(theta);
}

float asm_calculator(float theta)
{
    float r;
    asm("fld %0\n"
        "fsin\n"
        "fstp %1"
        : "=r"(r)
        : "r"(theta));

    return r;
}

std::chrono::nanoseconds time_calculations(float (*calculator)(float))
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

}

int main()
{
    std::cout << time_calculations(standard_calculator).count() << "ns\n";

    return 0;
}