#include <iostream>
#include "biginteger/biginteger.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <number>" << std::endl;
        return 1;
    }
    int limit = std::stoi(argv[1]);
    BigInteger num = 1;
    for (int i = 1; i < limit; ++i) {
        num *= i;
    }
}
