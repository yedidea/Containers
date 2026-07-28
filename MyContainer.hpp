// Email: yedidea9@gmail.com
#ifndef MYCONTAINER_HPP
#define MYCONTAINER_HPP

#include "Iterators.hpp"

#include <algorithm>
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace mycontainer {

// Stores values in insertion order.
// Its iterators provide different views without rearranging the stored values.
// Adding or removing values invalidates existing iterators.
template<typename T = int>
class MyContainer {
private:
    // Owns the values in the order they were added.
    std::vector<T> elements;

public:
    // Adds a new value after the existing elements.
    void addElement(const T& value) {
        elements.push_back(value);
    }

    // Removes every matching value and throws if no match exists.
    void removeElement(const T& value) {
        const auto firstRemoved = std::remove(elements.begin(), elements.end(), value);
        if (firstRemoved == elements.end()) {
            throw std::runtime_error("value does not exist in the container");
        }
        elements.erase(firstRemoved, elements.end());
    }

    std::size_t size() const noexcept {
        return elements.size();
    }

    void add(const T& value) {
        addElement(value);
    }

    void remove(const T& value) {
        removeElement(value);
    }

    // Begin iterators build a visit order; end iterators mark its final position.
    AscendingOrder<T> begin_ascending_order() const {
        return AscendingOrder<T>(elements);
    }

    AscendingOrder<T> end_ascending_order() const {
        return AscendingOrder<T>(elements, true);
    }

    DescendingOrder<T> begin_descending_order() const {
        return DescendingOrder<T>(elements);
    }

    DescendingOrder<T> end_descending_order() const {
        return DescendingOrder<T>(elements, true);
    }

    SideCrossOrder<T> begin_side_cross_order() const {
        return SideCrossOrder<T>(elements);
    }

    SideCrossOrder<T> end_side_cross_order() const {
        return SideCrossOrder<T>(elements, true);
    }

    ReverseOrder<T> begin_reverse_order() const {
        return ReverseOrder<T>(elements);
    }

    ReverseOrder<T> end_reverse_order() const {
        return ReverseOrder<T>(elements, true);
    }

    Order<T> begin_order() const {
        return Order<T>(elements);
    }

    Order<T> end_order() const {
        return Order<T>(elements, true);
    }

    MiddleOutOrder<T> begin_middle_out_order() const {
        return MiddleOutOrder<T>(elements);
    }

    MiddleOutOrder<T> end_middle_out_order() const {
        return MiddleOutOrder<T>(elements, true);
    }

    // Writes the values in insertion order using the format [a, b, c].
    friend std::ostream& operator<<(std::ostream& output,
                                    const MyContainer& container) {
        output << '[';
        for (std::size_t index = 0; index < container.elements.size(); ++index) {
            if (index != 0) {
                output << ", ";
            }
            output << container.elements[index];
        }
        return output << ']';
    }
};

}  // namespace mycontainer

#endif
