/*
 * Compile with:
 * g++ -o rotated_max -Wall -Wextra -std=c++11 rotated_max.cc
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

typedef unsigned int uint32;
typedef unsigned long long int uint64;

int
main(int argc, char **argv)
{
  std::vector<uint64> array;
  uint32 bound_low, bound_up;

  if (argc < 2) {
    std::cerr
      << "Find the max element of a rotated array" << std::endl
      << "usage: " << argv[0] << " NUMBER..." << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    uint64 number;

    std::istringstream(argv[i]) >> number;
    array.push_back(number);
  }

  std::cout << "[ ";
  for (const auto& e : array) {
    std::cout << e << " ";
  }
  std::cout << "]" << std::endl;

  bound_low = 0;
  bound_up = array.size() - 1;

  while (bound_low < bound_up) {
    uint32 pivot = bound_low + (bound_up - bound_low) / 2;

    if (array[pivot] > array[bound_low])
      bound_low = pivot;
    else
      bound_up = pivot;
  }

  std::cout << "Max element is " << array[bound_low] << "." << std::endl;

  return 0;
}
