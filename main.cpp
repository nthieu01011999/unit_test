#include <iostream>
#include <memory>

class Sample {
public:
    Sample() { std::cout << "Sample Constructor\n"; }
    ~Sample() { std::cout << "Sample Destructor\n"; }
};

int main() {
    std::shared_ptr<Sample> sp1 = std::make_shared<Sample>(); // Constructor is called
    {
        std::shared_ptr<Sample> sp2 = sp1; // Reference count is now 2
        std::cout << "Inside the block\n";
    } // sp2 goes out of scope, Reference count goes back to 1
    std::cout << "Outside the block\n";
} // sp1 goes out of scope, Reference count is 0, Destructor is called
