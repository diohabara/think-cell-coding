#include <cassert>
#include <iostream>
#include <map>

template <typename K, typename V> class interval_map {
  friend void IntervalMapTest();
  V m_valBegin;
  std::map<K, V> m_map;

public:
  // constructor associates whole range of K with val
  interval_map(V const &val) : m_valBegin(val) {}

  void debug() {
    std::cout << "size: " << m_map.size() << std::endl;
    std::cout << "[";
    for (auto [k, v] : m_map) {
      std::cout << "(" << k << ": " << v << "),";
    }
    std::cout << "]\n";
  }

  void assign(K const &keyBegin, K const &keyEnd, V const &val) {
    // SOLUTION STARTS
    /**
     * Used `prev` instead of `--` in the example
     * Multiple O(log N) operations, which are not amortized O(log N), so it
     * should be OK
     * Debugged with this code and the result
      int main() {
        interval_map<int, char> imap{'A'};
        imap.assign(0, 2, 'A'); // Do nothing
        imap.assign(1, 3, 'B'); // [(1, B), (3, A)]
        imap.assign(3, 5, 'A'); // [(1, B), (3, A)]
        imap.assign(4, 0, 'C'); // Do nothing
        imap.assign(5, 7, 'A');  // Do nothing
        imap.assign(5, 7, 'B');  // [(1, B), (3, A), (5, B), (7, A)]
        imap.assign(4, 6, 'B');  // [(1, B), (3, A), (4, B), (7, A)]
        imap.assign(-1, 2, 'B'); // [(-1, B), (3, A), (4, B), (7, A)]
        return 0;
      }
     */

    // Empty Interval
    if (!(keyBegin < keyEnd)) {
      return;
    }

    auto itLower = m_map.lower_bound(keyBegin);
    auto itUpper = m_map.upper_bound(keyEnd);
    V valueBefore =
        (itLower == m_map.begin()) ? m_valBegin : std::prev(itLower)->second;

    // Duplicate Value, so do nothing
    if (valueBefore == val) {
      return;
    }
    m_map.erase(itLower, itUpper);

    // Add new start and end
    m_map.insert(
        std::make_pair(keyBegin, val)); // doesn't allow default-constructible
    if (itUpper == m_map.end() || itUpper->second != val) {
      V valueAfter = (itUpper == m_map.end()) ? m_valBegin : itUpper->second;
      m_map.insert(keyEnd, valueAfter);
    }

    // Merge same values
    // Remove the bigger one for the smallest edge
    // * is changed
    // [(1, B), (5, C), (7, A)] + [(-1, 3, B)]
    // -> [*(-1, B), (5, C), (7, A)]
    itLower = m_map.lower_bound(keyBegin);
    if (itLower != m_map.begin() &&
        std::prev(itLower)->second == itLower->second) {
      m_map.erase(std::prev(itLower));
    }

    // Remove the smaller one for the biggest edge
    // * is changed
    // [(1, A), (5, B), (7, A)] + [(4, 6, B)]
    // -> [(1, A), *(4, B), (7, A)]
    itUpper = m_map.lower_bound(keyEnd);
    if (itUpper != m_map.end() &&
        itUpper->second == std::next(itUpper)->second) {
      m_map.erase(itUpper);
    }
    // SOLUTION ENDS

    debug();
  }

  // look-up of the value associated with key
  V const &operator[](K const &key) const {
    auto it = m_map.upper_bound(key);
    if (it == m_map.begin()) {
      return m_valBegin;
    } else {
      return (--it)->second;
    }
  }
};

int main() {
  interval_map<int, char> imap{'A'};
  imap.assign(0, 2, 'A'); // Do nothing
  imap.assign(1, 3, 'B'); // [(1, B), (3, A)]
  imap.assign(3, 5, 'A'); // [(1, B), (3, A)]
  imap.assign(4, 0, 'C'); // Do nothing
  for (int i = -2; i <= 5; ++i) {
    auto mapped_value = imap[i];
    if (i <= 0 || 3 <= i) {
      assert(mapped_value == 'A');
    } else {
      assert(mapped_value == 'B');
    }
  }
  imap.assign(5, 7, 'A');  // Do nothing
  imap.assign(5, 7, 'B');  // [(1, B), (3, A), (5, B), (7, A)]
  imap.assign(4, 6, 'B');  // [(1, B), (3, A), (4, B), (7, A)]
  imap.assign(-1, 2, 'B'); // [(-1, B), (3, A), (4, B), (7, A)]
  return 0;
}
