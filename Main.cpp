// Email: yedidea9@gmail.com
#include "MyContainer.hpp"

#include <iostream>

/** Prints every value produced by a traversal. */
template<typename Iterator>
void printTraversal(const char* label, Iterator current, const Iterator& end) {
    std::cout << label << ": ";
    bool first = true;
    while (current != end) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << *current;
        first = false;
        ++current;
    }
    std::cout << '\n';
}

/** Demonstrates the container API and all traversal orders. */
int main() {
    ariel::MyContainer<> defaultContainer;
    defaultContainer.add(10);
    defaultContainer.addElement(20);
    defaultContainer.remove(10);
    std::cout << "Default container after add/remove: " << defaultContainer << '\n';

    ariel::MyContainer<int> numbers;
    for (int value : {1, 2, 3, 4, 5}) {
        numbers.addElement(value);
    }

    std::cout << "Container: " << numbers << '\n';
    std::cout << "Size: " << numbers.size() << '\n';
    printTraversal("AscendingOrder", numbers.begin_ascending_order(),
                   numbers.end_ascending_order());
    printTraversal("DescendingOrder", numbers.begin_descending_order(),
                   numbers.end_descending_order());
    printTraversal("SideCrossOrder", numbers.begin_side_cross_order(),
                   numbers.end_side_cross_order());
    printTraversal("ReverseOrder", numbers.begin_reverse_order(),
                   numbers.end_reverse_order());
    printTraversal("Order", numbers.begin_order(), numbers.end_order());
    printTraversal("MiddleOutOrder", numbers.begin_middle_out_order(),
                   numbers.end_middle_out_order());

    return 0;
}
