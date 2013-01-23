#include <iostream>

template<typename T>
struct base_type
{
  typedef T value;
};

template<typename T>
struct base_type<T*>
{
  typedef typename base_type<T>::value value;
};

template<typename T>
struct base_type<T&>
{
  typedef typename base_type<T>::value value;
};

template<typename T>
T deref(T arg)
{
  return arg;
}

template<typename T>
typename base_type<T>::value deref(T* arg)
{
  return deref(*arg);
}

int main()
{
  typename base_type<int***>::value my_int = 42;
  std::cout << my_int << std::endl;

  int i = 42;
  int* j = &i;
  int** k = &j;
  int*** l = &k;
  std::cout << deref(l) << std::endl;
}
