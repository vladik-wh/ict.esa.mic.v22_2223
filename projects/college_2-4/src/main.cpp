#include <iostream>

int main() {
    uint8_t a = 'a';
    std::cout << a << std::endl;
    uint8_t b = a;
    b |= (1 << 1);            // maak letter b van letter a
    b &= ~(1 << 0);
    std::cout << b << std::endl;


    // Opgave a
    uint8_t n = b;
    n |= (0b11 << 2);
    std::cout << n << std::endl;


    //Opgave b
    uint8_t h = n;
    h &= ~(0b11 << 1);
    std::cout << h << std::endl;

}