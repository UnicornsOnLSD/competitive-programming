#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string mass_str;
  int result = 0;

  while (std::getline(std::cin, mass_str)) {
      int mass = std::stoi(mass_str);
      result += mass / 3 - 2;
  }

  std::cout << result << '\n';
}
