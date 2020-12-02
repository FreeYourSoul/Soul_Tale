// MIT License
//
// Copyright (c) 2020 Quentin Balland
// Repository : https://github.com/FreeYourSoul/FyS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
//         of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
//         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//         copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
//         copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <allegro_tmx/allegro_tmx.hh>
#include <catch2/catch.hpp>

TEST_CASE("t") {
  allegro_tmx::boundary_map<int, int> b{};
  b.insert(1, 1);
  b.insert(257, 2);
  b.insert(513, 3);
  b.insert(769, 4);
  b.insert(833, 5);
  b.insert(1089, 42);
  b.insert(1345, 8);
  b.insert(1601, 9);
  b.insert(1857, 10);

  CHECK(1 == b.get(3)->second);
  CHECK(4 == b.get(790)->second);

  CHECK(42 == b.get(1216)->second);
  CHECK(42 == b.get(1217)->second);
  CHECK(42 == b.get(1218)->second);
  CHECK(42 == b.get(1181)->second);

}// End TestCase :