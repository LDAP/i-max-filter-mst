#pragma once

#include <random>

// source: https://stackoverflow.com/questions/35358501/what-is-performance-wise-the-best-way-to-generate-random-bools
class BiasedXORShift128 {

  public:
    std::size_t xorshift128plus(void) noexcept {
        std::size_t x = s[0];
        std::size_t const y = s[1];
        s[0] = y;
        x ^= x << 23;                         // a
        s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
        return s[1] + y;
    }

  private:
    std::random_device rd;
    /* The state must be seeded so that it is not everywhere zero. */
    std::size_t s[2] = {(std::size_t(rd()) << 32) ^ (rd()), (std::size_t(rd()) << 32) ^ (rd())};
    std::size_t last_rand = xorshift128plus();
};

inline std::size_t uniform_int() noexcept {
    static thread_local BiasedXORShift128 generator;
    return generator.xorshift128plus();
}
