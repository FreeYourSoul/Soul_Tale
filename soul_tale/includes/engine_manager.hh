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

#ifndef SOUL_TALE_SOUL_TALE_SRC_DISPLAY_MANAGER_HH
#define SOUL_TALE_SOUL_TALE_SRC_DISPLAY_MANAGER_HH

#include <memory>

// forward declarations
namespace fys::st {
class network_manager;
}
// !forward declarations

namespace fys::st {

class engine_manager {
  struct internal;

public:
  ~engine_manager();
  engine_manager();
  engine_manager(engine_manager&& other) noexcept = default;
  engine_manager& operator=(engine_manager&& other) noexcept = default;

  void run_launcher();

  /**
   * Run the engine for the given character (need to be on an authenticated network manager)
   */
  void run(const std::string& user_name, std::shared_ptr<network_manager> net);

  engine_manager(const engine_manager&) = delete;
  engine_manager& operator=(const engine_manager&) = delete;

private:
  std::unique_ptr<internal> _internal;
};

}// namespace fys::st

#endif//SOUL_TALE_SOUL_TALE_SRC_DISPLAY_MANAGER_HH
