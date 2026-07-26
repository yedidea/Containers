// Email: yedidea9@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "MyContainer.hpp"

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using ariel::MyContainer;

template<typename Iterator>
std::vector<typename Iterator::value_type> collect(Iterator current,
                                                   const Iterator& end) {
    std::vector<typename Iterator::value_type> values;
    while (current != end) {
        values.push_back(*current);
        ++current;
    }
    return values;
}

template<typename T>
std::string printed(const MyContainer<T>& container) {
    std::ostringstream output;
    output << container;
    return output.str();
}

TEST_CASE("containers start empty and support both API naming styles") {
    MyContainer<> defaultContainer;
    CHECK(defaultContainer.size() == 0);
    CHECK(printed(defaultContainer) == "[]");

    defaultContainer.addElement(7);
    CHECK(defaultContainer.size() == 1);
    defaultContainer.add(15);
    defaultContainer.addElement(6);
    CHECK(defaultContainer.size() == 3);
    CHECK(printed(defaultContainer) == "[7, 15, 6]");

    defaultContainer.removeElement(15);
    defaultContainer.remove(7);
    CHECK(defaultContainer.size() == 1);
    CHECK(printed(defaultContainer) == "[6]");
}

TEST_CASE("removal erases every duplicate and preserves remaining order") {
    MyContainer<int> container;
    for (int value : {4, 2, 4, 3, 4, 1}) {
        container.addElement(value);
    }
    CHECK(container.size() == 6);

    container.removeElement(4);
    CHECK(container.size() == 3);
    CHECK(printed(container) == "[2, 3, 1]");
    CHECK_THROWS_AS(container.removeElement(9), std::runtime_error);
    CHECK(container.size() == 3);
}

TEST_CASE("container supports int double and string") {
    MyContainer<int> integers;
    integers.add(3);
    CHECK(printed(integers) == "[3]");

    MyContainer<double> doubles;
    doubles.addElement(1.5);
    doubles.addElement(-2.25);
    CHECK(doubles.size() == 2);
    CHECK(printed(doubles) == "[1.5, -2.25]");

    MyContainer<std::string> words;
    words.add("pear");
    words.add("apple");
    words.remove("pear");
    CHECK(printed(words) == "[apple]");
}

TEST_CASE("all six required traversal orders are exact") {
    MyContainer<int> container;
    for (int value : {7, 15, 6, 1, 2}) {
        container.addElement(value);
    }

    CHECK(collect(container.begin_ascending_order(), container.end_ascending_order()) ==
          std::vector<int>{1, 2, 6, 7, 15});
    CHECK(collect(container.begin_descending_order(), container.end_descending_order()) ==
          std::vector<int>{15, 7, 6, 2, 1});
    CHECK(collect(container.begin_side_cross_order(), container.end_side_cross_order()) ==
          std::vector<int>{1, 15, 2, 7, 6});
    CHECK(collect(container.begin_reverse_order(), container.end_reverse_order()) ==
          std::vector<int>{2, 1, 6, 15, 7});
    CHECK(collect(container.begin_order(), container.end_order()) ==
          std::vector<int>{7, 15, 6, 1, 2});
    CHECK(collect(container.begin_middle_out_order(), container.end_middle_out_order()) ==
          std::vector<int>{6, 15, 1, 7, 2});
}

TEST_CASE("empty and singleton containers work with every order") {
    MyContainer<int> empty;
    CHECK(collect(empty.begin_ascending_order(), empty.end_ascending_order()).empty());
    CHECK(collect(empty.begin_descending_order(), empty.end_descending_order()).empty());
    CHECK(collect(empty.begin_side_cross_order(), empty.end_side_cross_order()).empty());
    CHECK(collect(empty.begin_reverse_order(), empty.end_reverse_order()).empty());
    CHECK(collect(empty.begin_order(), empty.end_order()).empty());
    CHECK(collect(empty.begin_middle_out_order(), empty.end_middle_out_order()).empty());

    MyContainer<int> one;
    one.addElement(42);
    const std::vector<int> expected{42};
    CHECK(collect(one.begin_ascending_order(), one.end_ascending_order()) == expected);
    CHECK(collect(one.begin_descending_order(), one.end_descending_order()) == expected);
    CHECK(collect(one.begin_side_cross_order(), one.end_side_cross_order()) == expected);
    CHECK(collect(one.begin_reverse_order(), one.end_reverse_order()) == expected);
    CHECK(collect(one.begin_order(), one.end_order()) == expected);
    CHECK(collect(one.begin_middle_out_order(), one.end_middle_out_order()) == expected);
}

TEST_CASE("middle-out handles even and odd sizes") {
    MyContainer<int> even;
    for (int value : {10, 20, 30, 40}) {
        even.addElement(value);
    }
    CHECK(collect(even.begin_middle_out_order(), even.end_middle_out_order()) ==
          std::vector<int>{30, 20, 40, 10});

    MyContainer<int> odd;
    for (int value : {10, 20, 30, 40, 50}) {
        odd.addElement(value);
    }
    CHECK(collect(odd.begin_middle_out_order(), odd.end_middle_out_order()) ==
          std::vector<int>{30, 20, 40, 10, 50});
}

TEST_CASE("duplicates are stable and negative values are ordered") {
    MyContainer<int> duplicates;
    for (int value : {2, 1, 2, 1}) {
        duplicates.addElement(value);
    }
    CHECK(collect(duplicates.begin_ascending_order(), duplicates.end_ascending_order()) ==
          std::vector<int>{1, 1, 2, 2});
    CHECK(collect(duplicates.begin_descending_order(), duplicates.end_descending_order()) ==
          std::vector<int>{2, 2, 1, 1});
    CHECK(collect(duplicates.begin_side_cross_order(), duplicates.end_side_cross_order()) ==
          std::vector<int>{1, 2, 1, 2});

    MyContainer<int> negatives;
    for (int value : {-3, 5, -10, 0}) {
        negatives.addElement(value);
    }
    CHECK(collect(negatives.begin_ascending_order(), negatives.end_ascending_order()) ==
          std::vector<int>{-10, -3, 0, 5});
    CHECK(collect(negatives.begin_descending_order(), negatives.end_descending_order()) ==
          std::vector<int>{5, 0, -3, -10});
}

TEST_CASE("string iterators support dereference and arrow") {
    MyContainer<std::string> words;
    words.addElement("pear");
    words.addElement("apple");
    words.addElement("banana");

    CHECK(collect(words.begin_ascending_order(), words.end_ascending_order()) ==
          std::vector<std::string>{"apple", "banana", "pear"});
    auto iterator = words.begin_ascending_order();
    CHECK(iterator->size() == 5);
    CHECK(*iterator == "apple");
}

TEST_CASE("pre-increment post-increment and end comparisons work") {
    MyContainer<int> container;
    container.addElement(7);
    container.addElement(15);

    auto iterator = container.begin_order();
    auto end = container.end_order();
    CHECK(iterator != end);
    auto previous = iterator++;
    CHECK(*previous == 7);
    CHECK(*iterator == 15);
    CHECK(&++iterator == &iterator);
    CHECK(iterator == end);
    CHECK_THROWS_AS(*iterator, std::out_of_range);
}

TEST_CASE("every end iterator rejects dereference") {
    MyContainer<int> container;
    container.addElement(1);
    CHECK_THROWS_AS(*container.end_ascending_order(), std::out_of_range);
    CHECK_THROWS_AS(*container.end_descending_order(), std::out_of_range);
    CHECK_THROWS_AS(*container.end_side_cross_order(), std::out_of_range);
    CHECK_THROWS_AS(*container.end_reverse_order(), std::out_of_range);
    CHECK_THROWS_AS(*container.end_order(), std::out_of_range);
    CHECK_THROWS_AS(*container.end_middle_out_order(), std::out_of_range);
}

TEST_CASE("iterator comparison includes container and traversal kind") {
    MyContainer<int> first;
    MyContainer<int> second;
    first.addElement(1);
    second.addElement(1);

    CHECK(first.begin_order() != second.begin_order());
    CHECK(first.begin_ascending_order() != first.begin_descending_order());
    CHECK(first.begin_order() == first.begin_order());
}

TEST_CASE("sorted traversal does not modify insertion order") {
    MyContainer<int> container;
    for (int value : {7, 15, 6, 1, 2}) {
        container.addElement(value);
    }
    const std::string before = printed(container);
    static_cast<void>(collect(container.begin_ascending_order(),
                              container.end_ascending_order()));
    static_cast<void>(collect(container.begin_descending_order(),
                              container.end_descending_order()));
    CHECK(printed(container) == before);
    CHECK(collect(container.begin_order(), container.end_order()) ==
          std::vector<int>{7, 15, 6, 1, 2});
}
