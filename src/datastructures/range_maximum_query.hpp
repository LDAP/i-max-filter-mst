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
    explicit RangeMaximumQuery(std::vector<T> &v) : n_levels(ceil_log_2(v.size())), level_size(1ull << n_levels) {

        levels.resize(n_levels << n_levels, RMQ_MIN);

        // resize to eliminate special cases
        v.resize(level_size, RMQ_MIN);

        // build levels

        std::size_t block_count = level_size >> 1;
        std::size_t shift = 0;

        // general case
        std::size_t block_size = 1;
        for (uint j = 0; j < n_levels; j++) {
            for (std::size_t i = 0; i < block_count; i++) {
                const std::size_t block_shift = i * (block_size << 1);

                std::size_t l = block_size - 2 + block_shift;
                T l_max = RMQ_MIN;
                for (std::size_t k = 1; k < block_size; k++) {
                    levels[shift + l] = l_max = std::max(l_max, v[l + 1]);
                    l--;
                }

                std::size_t r = block_size + block_shift;
                T r_max = RMQ_MIN;
                for (std::size_t k = 0; k < block_size; k++) {
                    levels[shift + r] = r_max = std::max(r_max, v[r]);
                    r++;
                }
            }

            block_count = block_count >> 1;
            block_size = block_size << 1;
            shift += level_size;
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
        const unsigned int level = std::__lg(i ^ j);
        return std::max(levels[(level << n_levels) + i], levels[(level << n_levels) + j]);
    }

    unsigned int get_n_levels() const {
        return n_levels;
    }

    std::size_t get_level_size() const {
        return level_size;
    }

    T get_level_value(unsigned int level, std::size_t i) const {
        return levels[(level << n_levels) + i];
    }

  private:
    const unsigned int n_levels;
    const std::size_t level_size;
    std::vector<T> levels;
};
