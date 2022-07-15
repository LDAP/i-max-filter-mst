#pragma once

#include <cmath>
#include <vector>

#define RMQ_MIN 0

inline unsigned int ceil_log_2(unsigned long n) {
    return __builtin_popcountl(n) == 1 ? std::__lg(n) : std::__lg(n) + 1;
}

/**
 * @brief      This class allows range maximum queries. The values must be positive.
 *
 * @tparam     T     type of values
 */
template<typename T>
class RangeMaximumQuery {
  public:
    using Level = std::vector<T>;

    explicit RangeMaximumQuery(std::vector<T> &v) : n_levels(ceil_log_2(v.size())), level_size(1ull << n_levels) {

        levels.resize(n_levels, std::vector<T>(level_size, RMQ_MIN));

        // build levels

        // unroll case level = 0
        for(std::size_t i = 1; i < v.size(); i += 2) {
            levels[0][i] = v[i];
        }
        if(n_levels == 1)
            return;

        // resize here to eliminate special cases
        v.resize(level_size, RMQ_MIN);

        std::size_t block_count = level_size >> 2;

        // unroll case level = 1
        std::size_t l = 0;
        std::size_t r = 2;
        for(std::size_t i = 0; i < block_count; i++) {
            levels[1][l] = v[l + 1];

            levels[1][r] = v[r];
            levels[1][r + 1] = std::max(v[r], v[r + 1]);

            l += 4;
            r += 4;
        }
        if(n_levels == 2)
            return;

        block_count = block_count >> 1;

        // unroll case level = 2
        l = 3;
        r = 4;
        for(std::size_t i = 0; i < block_count; i++) {
            T l_max = RMQ_MIN;

            l_max = std::max(l_max, v[l]);
            l--;
            levels[2][l] = l_max;

            l_max = std::max(l_max, v[l]);
            l--;
            levels[2][l] = l_max;

            l_max = std::max(l_max, v[l]);
            l--;
            levels[2][l] = l_max;

            T r_max = RMQ_MIN;
            levels[2][r] = r_max = std::max(r_max, v[r]);
            r++;
            levels[2][r] = r_max = std::max(r_max, v[r]);
            r++;
            levels[2][r] = r_max = std::max(r_max, v[r]);
            r++;
            levels[2][r] = r_max = std::max(r_max, v[r]);

            l += 11;
            r += 5;
        }
        if(n_levels == 3)
            return;

        block_count = block_count >> 1;

        // general case
        std::size_t block_size = 8;
        for(uint j = 3; j < n_levels; j++) {
            for(std::size_t i = 0; i < block_count; i++) {
                const std::size_t shift = i * (block_size << 1);

                std::size_t l = block_size - 2 + shift;
                T l_max = RMQ_MIN;
                for(std::size_t k = 1; k < block_size; k++) {
                    levels[j][l] = l_max = std::max(l_max, v[l + 1]);
                    l--;
                }

                std::size_t r = block_size + shift;
                T r_max = RMQ_MIN;
                for(std::size_t k = 0; k < block_size; k++) {
                    levels[j][r] = r_max = std::max(r_max, v[r]);
                    r++;
                }
            }

            block_count = block_count >> 1;
            block_size = block_size << 1;
        }
    }

    /**
     * @brief      Returns max(min(i,j)...max(i,j))
     *
     * @param[in]  i     first bound
     * @param[in]  j     second bound
     *
     * @return     max(min(i,j)...max(i,j))
     */
    T query(std::size_t i, std::size_t j) const {
        const std::size_t level = std::__lg(i ^ j);
        return std::max(levels[level][i], levels[level][j]);
    }

    unsigned int get_n_levels() const {
        return n_levels;
    }

    std::size_t get_level_size() const {
        return level_size;
    }

    T get_level_value(unsigned int level, std::size_t i) const {
        return levels[level][i];
    }

  private:
    unsigned int n_levels;
    std::size_t level_size;
    std::vector<Level> levels;
};
