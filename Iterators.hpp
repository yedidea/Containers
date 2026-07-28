// Email: yedidea9@gmail.com
#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

namespace mycontainer {

// Distinguishes traversal types so different orders do not compare as equal.
enum class IteratorOrder {
    Ascending,
    Descending,
    SideCross,
    Reverse,
    Insertion,
    MiddleOut
};

// Holds the state and operators shared by all read-only iterators.
// Traversal indices change the visit order without moving the stored values.
template<typename T>
class IteratorBase {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    // Returns the current source value and rejects end iterators.
    reference operator*() const {
        if (position_ >= traversal_.size()) {
            throw std::out_of_range("cannot dereference an end iterator");
        }
        return (*elements_)[traversal_[position_]];
    }

    pointer operator->() const {
        return &operator*();
    }

    // Advances to the next planned index without moving past the end.
    IteratorBase& operator++() {
        if (position_ < traversal_.size()) {
            ++position_;
        }
        return *this;
    }

    IteratorBase operator++(int) {
        IteratorBase previous(*this);
        ++(*this);
        return previous;
    }

    // Equality requires the same container, traversal type, and position.
    bool operator==(const IteratorBase& other) const noexcept {
        return elements_ == other.elements_ &&
               order_ == other.order_ &&
               position_ == other.position_;
    }

    bool operator!=(const IteratorBase& other) const noexcept {
        return !(*this == other);
    }

protected:
    // Creates an unbound default iterator for one traversal type.
    explicit IteratorBase(IteratorOrder order) noexcept
        : elements_(nullptr), traversal_(), position_(0), order_(order) {}

    // Binds an iterator to its source values and planned visit order.
    IteratorBase(const std::vector<T>& elements,
                 std::vector<std::size_t> traversal,
                 std::size_t position,
                 IteratorOrder order)
        : elements_(&elements),
          traversal_(std::move(traversal)),
          position_(position),
          order_(order) {}

private:
    // Points to container storage, which must outlive the iterator.
    const std::vector<T>* elements_;
    // Stores source indices in the order they should be visited.
    std::vector<std::size_t> traversal_;
    // Tracks the current position inside the traversal list.
    std::size_t position_;
    // Records the traversal type for safe iterator comparisons.
    IteratorOrder order_;
};

namespace detail {

// Sorts source indices by value while keeping equal values in insertion order.
template<typename T>
std::vector<std::size_t> ascendingIndices(const std::vector<T>& elements) {
    std::vector<std::size_t> indices(elements.size());
    std::iota(indices.begin(), indices.end(), std::size_t{0});
    std::stable_sort(indices.begin(), indices.end(),
                     [&elements](std::size_t left, std::size_t right) {
                         return elements[left] < elements[right];
                     });
    return indices;
}

// Sorts source indices from largest to smallest with stable equal values.
template<typename T>
std::vector<std::size_t> descendingIndices(const std::vector<T>& elements) {
    std::vector<std::size_t> indices(elements.size());
    std::iota(indices.begin(), indices.end(), std::size_t{0});
    std::stable_sort(indices.begin(), indices.end(),
                     [&elements](std::size_t left, std::size_t right) {
                         return elements[right] < elements[left];
                     });
    return indices;
}

// Takes indices from both ends of the ascending order: low, high, low, high.
template<typename T>
std::vector<std::size_t> sideCrossIndices(const std::vector<T>& elements) {
    const std::vector<std::size_t> ascending = ascendingIndices(elements);
    std::vector<std::size_t> result;
    result.reserve(ascending.size());

    if (ascending.empty()) {
        return result;
    }

    std::size_t low = 0;
    std::size_t high = ascending.size() - 1;
    while (low < high) {
        result.push_back(ascending[low]);
        result.push_back(ascending[high]);
        ++low;
        --high;
    }
    if (low == high) {
        result.push_back(ascending[low]);
    }
    return result;
}

// Lists source indices in their original insertion order.
template<typename T>
std::vector<std::size_t> insertionIndices(const std::vector<T>& elements) {
    std::vector<std::size_t> indices(elements.size());
    std::iota(indices.begin(), indices.end(), std::size_t{0});
    return indices;
}

// Lists source indices from the last inserted value to the first.
template<typename T>
std::vector<std::size_t> reverseIndices(const std::vector<T>& elements) {
    std::vector<std::size_t> indices;
    indices.reserve(elements.size());
    for (std::size_t index = elements.size(); index > 0; --index) {
        indices.push_back(index - 1);
    }
    return indices;
}

// Starts at size / 2, then alternates left and right.
// For an even size, size / 2 selects the upper middle index.
template<typename T>
std::vector<std::size_t> middleOutIndices(const std::vector<T>& elements) {
    std::vector<std::size_t> indices;
    indices.reserve(elements.size());
    if (elements.empty()) {
        return indices;
    }

    const std::size_t middle = elements.size() / 2;
    indices.push_back(middle);
    for (std::size_t offset = 1; indices.size() < elements.size(); ++offset) {
        if (middle >= offset) {
            indices.push_back(middle - offset);
        }
        if (middle + offset < elements.size()) {
            indices.push_back(middle + offset);
        }
    }
    return indices;
}

}  // namespace detail

// End iterators skip the index list because comparison only needs the final position.

// Read-only iterator that visits values from smallest to largest.
template<typename T>
class AscendingOrder : public IteratorBase<T> {
public:
    AscendingOrder() noexcept : IteratorBase<T>(IteratorOrder::Ascending) {}

    explicit AscendingOrder(const std::vector<T>& elements, bool atEnd = false)
        : IteratorBase<T>(elements,
                          atEnd ? std::vector<std::size_t>{}
                                : detail::ascendingIndices(elements),
                          atEnd ? elements.size() : 0,
                          IteratorOrder::Ascending) {}

    AscendingOrder& operator++() {
        IteratorBase<T>::operator++();
        return *this;
    }

    AscendingOrder operator++(int) {
        AscendingOrder previous(*this);
        ++(*this);
        return previous;
    }
};

// Read-only iterator that visits values from largest to smallest.
template<typename T>
class DescendingOrder : public IteratorBase<T> {
public:
    DescendingOrder() noexcept : IteratorBase<T>(IteratorOrder::Descending) {}

    explicit DescendingOrder(const std::vector<T>& elements, bool atEnd = false)
        : IteratorBase<T>(elements,
                          atEnd ? std::vector<std::size_t>{}
                                : detail::descendingIndices(elements),
                          atEnd ? elements.size() : 0,
                          IteratorOrder::Descending) {}

    DescendingOrder& operator++() {
        IteratorBase<T>::operator++();
        return *this;
    }

    DescendingOrder operator++(int) {
        DescendingOrder previous(*this);
        ++(*this);
        return previous;
    }
};

// Read-only iterator that alternates between low and high sorted values.
template<typename T>
class SideCrossOrder : public IteratorBase<T> {
public:
    SideCrossOrder() noexcept : IteratorBase<T>(IteratorOrder::SideCross) {}

    explicit SideCrossOrder(const std::vector<T>& elements, bool atEnd = false)
        : IteratorBase<T>(elements,
                          atEnd ? std::vector<std::size_t>{}
                                : detail::sideCrossIndices(elements),
                          atEnd ? elements.size() : 0,
                          IteratorOrder::SideCross) {}

    SideCrossOrder& operator++() {
        IteratorBase<T>::operator++();
        return *this;
    }

    SideCrossOrder operator++(int) {
        SideCrossOrder previous(*this);
        ++(*this);
        return previous;
    }
};

// Read-only iterator that visits values in reverse insertion order.
template<typename T>
class ReverseOrder : public IteratorBase<T> {
public:
    ReverseOrder() noexcept : IteratorBase<T>(IteratorOrder::Reverse) {}

    explicit ReverseOrder(const std::vector<T>& elements, bool atEnd = false)
        : IteratorBase<T>(elements,
                          atEnd ? std::vector<std::size_t>{}
                                : detail::reverseIndices(elements),
                          atEnd ? elements.size() : 0,
                          IteratorOrder::Reverse) {}

    ReverseOrder& operator++() {
        IteratorBase<T>::operator++();
        return *this;
    }

    ReverseOrder operator++(int) {
        ReverseOrder previous(*this);
        ++(*this);
        return previous;
    }
};

// Read-only iterator that visits values in their original insertion order.
template<typename T>
class Order : public IteratorBase<T> {
public:
    Order() noexcept : IteratorBase<T>(IteratorOrder::Insertion) {}

    explicit Order(const std::vector<T>& elements, bool atEnd = false)
        : IteratorBase<T>(elements,
                          atEnd ? std::vector<std::size_t>{}
                                : detail::insertionIndices(elements),
                          atEnd ? elements.size() : 0,
                          IteratorOrder::Insertion) {}

    Order& operator++() {
        IteratorBase<T>::operator++();
        return *this;
    }

    Order operator++(int) {
        Order previous(*this);
        ++(*this);
        return previous;
    }
};

// Read-only iterator that starts in the middle, then alternates left and right.
template<typename T>
class MiddleOutOrder : public IteratorBase<T> {
public:
    MiddleOutOrder() noexcept : IteratorBase<T>(IteratorOrder::MiddleOut) {}

    explicit MiddleOutOrder(const std::vector<T>& elements, bool atEnd = false)
        : IteratorBase<T>(elements,
                          atEnd ? std::vector<std::size_t>{}
                                : detail::middleOutIndices(elements),
                          atEnd ? elements.size() : 0,
                          IteratorOrder::MiddleOut) {}

    MiddleOutOrder& operator++() {
        IteratorBase<T>::operator++();
        return *this;
    }

    MiddleOutOrder operator++(int) {
        MiddleOutOrder previous(*this);
        ++(*this);
        return previous;
    }
};

}  // namespace mycontainer

#endif
