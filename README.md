<p align="center">
  <img src="assets/logo.png" alt="CXXStateTree Logo" width="200"/>
</p>

<h1 align="center">CXXStateTree</h1>
<p align="center"><i>Modern Hierarchical State Machine for C++20</i></p>

## 🚀 Features

* 🔧 Fluent builder API with lambda-based DSL
* ⚡ Fast runtime performance with zero heap allocation
* 🛡️ Optional guards and actions for transitions
* 🔄 Event-based state transitions
* 🧪 Google Test integration
* 📈 Code coverage with Codecov
* 🌳 Designed for extensibility: nested states, DOT export coming soon

---

## 🛠️ Quick Example

```cpp
#include <iostream>
#include "CXXStateTree/StateTree.hpp"

using namespace CXXStateTree;

int main() {
    auto machine = Builder()
        .initial("Idle")
        .state("Idle", [](State& s) {
            s.on("Start", "Running", nullptr, []() {
                std::cout << "Idle -> Running" << std::endl;
            });
        })
        .state("Running", [](State& s) {
            s.on("Stop", "Idle", nullptr, []() {
                std::cout << "Running -> Idle" << std::endl;
            });
        })
        .build();

    machine.send("Start");
    machine.send("Stop");
}
```

---

## 🧪 Running Tests

```bash
cmake -S . -B build -DENABLE_COVERAGE=ON
cmake --build build
cd build && ctest
```

---

## 📦 Dependencies

* C++20 compiler (GCC >= 10, Clang >= 11, MSVC >= 2019)
* [GoogleTest](https://github.com/google/googletest) (auto-downloaded via CMake)

---

## 📈 Code Coverage

[![codecov](https://codecov.io/gh/ZigRazor/CXXStateTree/graph/badge.svg?token=A1RP2ZDGI6)](https://codecov.io/gh/ZigRazor/CXXStateTree)

---

## 📂 Directory Structure

```
CXXStateTree/
├── include/CXXStateTree/     # Public header-only API
├── examples/                 # Usage examples
├── tests/                    # Google Test suite
├── CMakeLists.txt            # Project build
└── .github/workflows/ci.yml  # GitHub Actions CI
```

---

## 📋 License

MPL2.0 License — see [LICENSE](LICENSE) for details.

---

## 🌟 Coming Soon

* Nested (hierarchical) state support
* DOT/Graphviz state diagram export
* Transitions with context/parameters

| Completed | Milestone   | Features                                                           |
| :-: | :---------   | ------------------------------------------------------------------ |
| :heavy_check_mark: | v0.1.0      | Basic state machine with `send()`, transitions, and state tracking |
| :heavy_check_mark: | v0.2.0      | Guards and actions                                                 |
| :heavy_check_mark: | v0.3.0      | Nested (hierarchical) states                                       |
| :heavy_check_mark: | v0.4.0      | Graphviz export                                                    |
| :memo: | v0.5.0      | Coroutine/async support (optional)                                 |
| :memo: | v1.0.0      | Full unit test coverage, benchmarks, and docs                      |

---

## 👋 Contributions Welcome

Issues, feature suggestions, and PRs are welcome!
