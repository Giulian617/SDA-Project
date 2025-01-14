#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>

using i32 = int32_t;

int main(int argc, char** argv) {
  const int N = std::atoi(argv[1]);
  const int Q = std::atoi(argv[2]);
  const int MAX_RANGE_LENGTH = std::atoi(argv[3]);
  const int seed = std::atoi(argv[4]);

  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> dist(-1000000000, 1000000000);

  std::vector<i32> values(N);
  for (auto& x : values) {
    x = dist(gen);
  }

  std::cout << Q << "\n";
  for (const auto value : values) {
    std::cout << 1 << " " << value << "\n";
  }

  std::sort(values.begin(), values.end());

  for (int i = 0; i < Q - N; i++) {
    int left = std::abs(dist(gen)) % N;
    int right = left + std::abs(dist(gen)) % std::min(N - left, MAX_RANGE_LENGTH);
    if (left > right) {
      std::swap(left, right);
    }
    std::cout << 6 << " " << values[left] << " " << values[right] << "\n";
  }
  return 0;
}