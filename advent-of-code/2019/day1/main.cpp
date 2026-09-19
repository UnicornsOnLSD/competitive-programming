#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    std::string mass_str;
    int part1_result = 0;
    int part2_result = 0;

    while (std::getline(std::cin, mass_str)) {
        int mass = std::stoi(mass_str);
        int fuel_cost = mass / 3 - 2;

        part1_result += fuel_cost;
        part2_result += fuel_cost;

        while (fuel_cost > 0) {
            fuel_cost = fuel_cost / 3 - 2;

            if (fuel_cost > 0) {
                part2_result += fuel_cost;
            }
        }
    }

    std::cout << part1_result << '\n' << part2_result << '\n';
}
