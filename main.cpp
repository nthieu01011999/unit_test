#include <iostream>
#include <memory>

class Sample {
public:
    Sample() { std::cout << "Sample Constructor\n"; }
    ~Sample() { std::cout << "Sample Destructor\n"; }
};

int main() {
    std::shared_ptr<Sample> sp1 = std::make_shared<Sample>(); // Constructor is called
    std::weak_ptr<Sample> wp1 = sp1; // wp1 is a weak pointer observing sp1

    {
        std::shared_ptr<Sample> sp2 = sp1; // Reference count is now 2
        std::cout << "Inside the block\n";
        std::cout << "sp2 use count: " << sp2.use_count() << "\n"; // Shows 2
    } // sp2 goes out of scope, Reference count goes back to 1
    std::cout << "Outside the block\n";
    std::cout << "sp1 use count: " << sp1.use_count() << "\n"; // Shows 1

    // Check if wp1 still points to an active object
    if (auto spt = wp1.lock()) { // Converts wp1 to shared_ptr if the object still exists
        std::cout << "The object is still alive, sp1 use count: " << spt.use_count() << "\n"; // Shows 1
        // Do something with spt
    } else {
        std::cout << "The object is no longer alive.\n";
    }

    sp1.reset(); // Manually clear sp1, object is destroyed here

    if (auto spt = wp1.lock()) {
        std::cout << "The object is still alive, sp1 use count: " << spt.use_count() << "\n";
    } else {
        std::cout << "The object is no longer alive.\n"; // This will be displayed
    }
}
