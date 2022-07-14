#pragma once

#include <vector>

template<typename T>
class RangeMaximumQuery {
  public:
    explicit RangeMaximumQuery(std::vector<T> &&v);

    /**
     * @brief      Returns max(min(i,j) + 1...max(i,j))
     *
     * @param[in]  i     first bound
     * @param[in]  j     second bound
     *
     * @return     max(min(i,j) + 1...max(i,j))
     */
    T query(std::size_t i, std::size_t j) const;

  private:
    using Level = std::vector<T>;

    std::vector<Level> levels;
};
