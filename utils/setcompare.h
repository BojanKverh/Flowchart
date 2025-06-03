#pragma once

#include <algorithm>
#include <unordered_set>

namespace utils {

/**
 * @brief The SetCompare class This class is used to compare two sets
 */
class SetCompare {
public:
  /**
   * @brief equal Compares two sets, whether they contain exactly the same elements
   * @param set1 First set to compare
   * @param set2 Second set to compare
   * @return true, if the two sets contain exactly the same elements and false otherwise
   */
  template <class T>
  static bool equal(const std::unordered_set<T>& set1, const std::unordered_set<T>& set2) {
    return (set1.size() == set2.size())
        && (std::all_of(set1.begin(), set1.end(),
                        [set2](const auto& elem) { return set2.find(elem) != set2.end(); }));
  }
};

} // namespace utils
