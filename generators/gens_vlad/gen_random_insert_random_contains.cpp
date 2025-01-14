#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cstdint>
#include <utility>
#include <algorithm>

using i32 = int32_t;

int main(int argc, char** argv) {
  const i32 N = std::atoi(argv[1]);
  const i32 Q = std::atoi(argv[2]);
  const bool is_sorted = std::atoi(argv[3]);
  const i32 seed = std::atoi(argv[4]);

  std::mt19937 gen(seed);
  std::uniform_int_distribution<i32> dist(-1000000000, 1000000000);

  std::vector<i32> values(N);
  for (auto& x : values) {
    x = dist(gen);
  }

  if (is_sorted) {
    std::sort(values.begin(), values.end());
  }

  std::vector<i32> already_inserted;
  already_inserted.reserve(N);

  std::cout << Q << "\n";
  for (int i = 0; i < Q; i++) {
    auto op = std::abs(dist(gen)) % 2 + 1;
    if (op == 1 && values.empty()) {
      op = 2;
    }
    if (op == 2 && already_inserted.empty()) {
      op = 1;
    }

    if (op == 1) {
      const auto pos = std::abs(dist(gen)) % values.size();
      const auto value = values[pos];
      std::swap(values[pos], values.back());
      values.pop_back();
      std::cout << 1 << " " << value << "\n";
      already_inserted.push_back(value);
    } else {
      const auto pos = std::abs(dist(gen)) % already_inserted.size();
      const auto value = already_inserted[pos];
      std::cout << 3 << " " << value << "\n";
    }
  }
  return 0;
}