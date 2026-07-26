
# MyContainer

`MyContainer` is a C++17 template container assignment in the `ariel` namespace. It stores values in insertion order and exposes six read-only traversal orders without rearranging the stored data.

## Files

- `MyContainer.hpp` implements the container API, removal, output, and iterator factories.
- `Iterators.hpp` implements the shared iterator behavior and all six iterator types.
- `Main.cpp` is an original usage demonstration.
- `Test.cpp` contains doctest unit tests; `doctest.h` is the test framework header.
- `Makefile` provides the required build, test, memory-check, and cleanup targets.

## Container and type requirements

`ariel::MyContainer<T>` uses `std::vector<T>` and defaults to `T = int`. Adding requires `T` to be copyable, removal requires equality comparison, sorted traversals require `operator<`, and printing requires stream insertion (`operator<<`). Types such as `int`, `double`, and `std::string` meet these requirements.

`addElement`/`add` append a value. `removeElement`/`remove` erase every equal value while preserving the relative order of everything else, and throw `std::runtime_error` if no match exists.

## Traversal orders

- `AscendingOrder`: smallest to largest, with equal values kept in insertion-relative order.
- `DescendingOrder`: largest to smallest.
- `SideCrossOrder`: smallest, largest, second smallest, second largest, and so on.
- `ReverseOrder`: reverse insertion order.
- `Order`: original insertion order.
- `MiddleOutOrder`: begins at index `size / 2`, then visits left, right, left, right. For an even size, `size / 2` selects the upper middle index.

Each begin iterator builds a vector of indices describing its traversal. Values remain in the original `std::vector<T>`, so sorting a traversal never sorts or modifies the container itself. End iterators only store the logical end position and do not rebuild or sort an index vector.

Adding or removing elements invalidates all existing iterators because the underlying vector and its indices may change.

## Commands

```bash
make Main
make test
make valgrind
make clean
```

`make valgrind` requires Valgrind to be installed. The external demo file is only an API reference; it is not part of this project, its build, or its tests.
