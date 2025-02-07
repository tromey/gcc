// Copyright (C) 2020 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-options "-std=gnu++2a" }
// { dg-do run { target c++2a } }

#include <algorithm>
#include <ranges>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>

using __gnu_test::test_range;
using __gnu_test::random_access_iterator_wrapper;

namespace ranges = std::ranges;
namespace views = std::ranges::views;

void
test01()
{
  int x[] = {1,2,3,4,5};
  auto is_odd = [] (int i) { return i%2==1; };
  auto v = x | views::transform(is_odd);
  VERIFY( ranges::equal(v, (int[]){1,0,1,0,1}) );
  using R = decltype(v);
  static_assert(std::same_as<bool, decltype(*ranges::begin(v))>);
  static_assert(ranges::view<R>);
  static_assert(ranges::sized_range<R>);
  static_assert(ranges::random_access_range<R>);
}

struct X
{
  int i,j;
};

void
test02()
{
  X x[] = {{1,2},{3,4},{5,6},{7,8},{9,10}};
  test_range<X, random_access_iterator_wrapper> rx(x);
  auto v = rx | views::transform(&X::i);
  VERIFY( ranges::size(v) == 5 );
  VERIFY( ranges::distance(v.begin(), v.end()) == 5 );
  VERIFY( ranges::equal(v, (int[]){1,3,5,7,9}) );
  VERIFY( ranges::equal(v | views::reverse, (int[]){9,7,5,3,1}) );
  using R = decltype(v);
  static_assert(std::same_as<int&, decltype(*ranges::begin(v))>);
  static_assert(std::same_as<int, std::iter_value_t<ranges::iterator_t<R>>>);
  static_assert(ranges::view<R>);
  static_assert(ranges::sized_range<R>);
  static_assert(!ranges::common_range<R>);
  static_assert(ranges::random_access_range<R>);
}

void
test03()
{
  auto id = [] (int i) { return i; };
  auto v = views::iota(0) | (views::filter(id)
			     | views::transform(id)
			     | views::take(5));
  VERIFY( ranges::equal(v, (int[]){1,2,3,4,5}) );
}

int
main()
{
  test01();
  test02();
  test03();
}
