#include <iostream>
#include <vector>
#include <cstdint>
#include <random>

using i32 = std::int32_t;

enum class Operations {
  INSERT = 0, 
  ERASE, 
  CONTAINS, 
  UPPER_BOUND, 
  LOWER_BOUND, 
  RANGE_SUM
};

int main(int argc, char** argv) {
  const i32 N = std::atoi(argv[1]); // N > Q works best in this case
  i32 Q = std::atoi(argv[2]);
  const bool insert_first = std::atoi(argv[3]); 
  const i32 seed = std::atoi(argv[4]);

  std::mt19937 gen(seed);
  std::uniform_int_distribution<i32> dist(-1'000'000'000, 1'000'000'000);

  std::vector<i32> values(N);
  for (auto& x : values) {
    x = dist(gen);
  }

  std::cout << Q << "\n";
  if (insert_first) {
    for (const auto x : values) {
      std::cout << 1 << " " << x << "\n";
    }
    Q -= N;
  }
  for (int i = 0; i < Q; i++) {
    i32 pos, value, low, high;
    auto operation = static_cast<Operations>(std::abs(dist(gen)) % 6);
    while (insert_first && operation == Operations::INSERT) {
      operation = static_cast<Operations>(std::abs(dist(gen)) % 6);
    }

    switch (operation) {
    case Operations::INSERT: 
      pos = std::abs(dist(gen)) % values.size();
      std::cout << 1 << " " << values[pos] << "\n";
      break;
    case Operations::ERASE:
      pos = std::abs(dist(gen)) % values.size();
      std::cout << 2 << " " << values[pos] << "\n";
      break;
    case Operations::CONTAINS:
      pos = std::abs(dist(gen)) % values.size();
      std::cout << 3 << " " << values[pos] << "\n";
      break;
    case Operations::UPPER_BOUND:
      value = dist(gen); 
      std::cout << 4 << " " << value << "\n";
      break;
    case Operations::LOWER_BOUND: 
      value = dist(gen);
      std::cout << 5 << " " << value << "\n";
      break;
    case Operations::RANGE_SUM: 
      low = dist(gen);
      high = std::min(1'000'000'000, low + std::abs(dist(gen)) % (2'000'000));
      std::cout << 6 << " " << low << " " << high << "\n";
      break;
    }
  }
  
  return 0;
}