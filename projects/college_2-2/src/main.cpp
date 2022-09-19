#include <iostream>

bool isKleineletter(char c)
{
    return 1 << 5 & c;
}

bool isHoofdletter(char c)
{
    return !isKleineletter(c);
}

int main() {
    std::cout << isHoofdletter('A') << std::endl;
    std::cout << isKleineletter('a') << std::endl;
}