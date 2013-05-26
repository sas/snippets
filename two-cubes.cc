/*
 * Compile with:
 * g++ -o two-cubes -Wall -Wextra -std=c++11 two-cubes.cc
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>

typedef unsigned int uint32;
typedef unsigned long long int uint64;

static std::map<uint32, uint32>
decompose_primes(uint64 num)
{
  std::map<uint32, uint32> res;
  uint32 divider = 2;

  while (num != 1) {
    uint32 count = 0;

    while (num % divider == 0) {
      count += 1;
      num /= divider;
    }

    if (count > 0)
      res[divider] = count;

    divider += 1;
  }

  return res;
}

int
main(int argc, char **argv)
{
  uint64 number;
  std::map<uint32, uint32> primes;

  if (argc != 2) {
    std::cerr
      << "Determine if a number is a product of two cubes." << std::endl
      << "usage: " << argv[0] << " NUMBER" << std::endl;
    return 1;
  }

  std::istringstream(argv[1]) >> number;
  primes = decompose_primes(number);

  std::cout << "1";
  for (const auto& e : primes) {
    std::cout << " x " << e.first << "^" << e.second;
  }
  std::cout
    << ": "
    << (std::all_of(primes.begin(), primes.end(),
                    [](const std::pair<uint32, uint32>& e){
                      return e.second % 3 == 0;
                    }) ? "yes" : "no")
    << "."
    << std::endl;

  return 0;
}
