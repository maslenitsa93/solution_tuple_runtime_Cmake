#include <iostream>

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

#include "tuple.h"

void print_tuple(const runtime::tuple & t)
{
  std::cout << "("<< t.get<int>(0) << ", "<< t.get<std::string>(1) << ", "<< t.get<double>(2) << ")" << std::endl;
}

template <typename V> 
bool try_get(const runtime::tuple& t,std::size_t index,V& value)
{
	try
	{
	  value = t.get<V>(index); // проверки второго метода
	  return true;
	}
	catch (const std::bad_cast& e)
	{
	  std::cerr << e.what() << std::endl;
	  return false;
	}
}

int main()
{
  /*
    Ограничения на типы в кортеже (runtime::tuple):
    1. Подставляемые типы должны иметь операторы сравнения < > == 
	2. Подставляемые типы не должны быть массивами.

	Исключения:
	1. При неправильном указании типа в методе get<T>(index)
	   выбрасывается исключение std::bad_cast()
    2. При сравнении кортежей с разным кол-вом элементов или же с разными
	   типами элементов выбрасывается исключение std::invalid_argument()

	Контейнер runtime::tuple содержит в себе элементы my::any
  */
  runtime::tuple t({ 2, std::string("zaz"), -0.1 });

  int n;
  std::string s;
  double d;

  // Проветка get<>
  assert(t.size() == 3);
  assert(try_get(t, 0, n) && n == 2);
  assert(try_get(t, 1, s) && s == "zaz");
  assert(try_get(t, 2, d) && d == -0.1);

  std::vector<runtime::tuple> v;
  v.push_back(t);
  v.push_back({ 1, std::string("foo"), 100.1 });
  v.push_back({ 3, std::string("baz"), -0.1 });
  v.push_back({ 2, std::string("baz"), -0.1 });
  v.push_back({ 1, std::string("foo"), 99.9 });

  std::cout << "Tuples:" << std::endl;

  // Проверка < >  !=
  assert(v[0] > v[1]);
  assert(v[1] < v[2]);
  assert(v[3] != v[4]);

  for (auto t : v)
	  print_tuple(t);

  std::sort(v.begin(), v.end());

  std::cout << "\nSorted tuples:" << std::endl;
  for (auto t : v)
	  print_tuple(t);

  getchar();
  return 0;
}

