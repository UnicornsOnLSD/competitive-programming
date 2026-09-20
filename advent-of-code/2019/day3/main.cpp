// this solution is terrible and buggy, I promise it would look and run better
// had I written it in Rust ;)

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct Vector {
    Direction direction;
    int magnitude;
};

// what a language
struct PairHash {
    size_t operator()(const std::pair<int, int> &p) const noexcept {
        return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
    }
};

std::unordered_set<std::pair<int, int>, PairHash>
wire_path(std::vector<Vector> &wire) {
    int x = 0;
    int y = 0;

    std::unordered_set<std::pair<int, int>, PairHash> out;

    for (auto vector : wire) {
        for (int i = 0; i < vector.magnitude; i++) {
            switch (vector.direction) {
            case UP:
                y -= 1;
                break;
            case DOWN:
                y += 1;
                break;
            case LEFT:
                x -= 1;
                break;
            case RIGHT:
                x += 1;
                break;
            }

            out.insert(std::pair(x, y));
        }
    }

    return out;
}

int main() {
    std::vector<std::vector<Vector>> wires;

    std::string wire_str;

    while (std::getline(std::cin, wire_str)) {
        std::vector<Vector> wire;

        for (auto vector_str : std::views::split(wire_str, ',')) {
            Vector vector;

            switch (vector_str[0]) {
            case 'U':
                vector.direction = UP;
                break;
            case 'D':
                vector.direction = DOWN;
                break;
            case 'L':
                vector.direction = LEFT;
                break;
            case 'R':
                vector.direction = RIGHT;
                break;
            default:
                throw std::invalid_argument(
                    std::format("Invalid direction %s", vector_str[0]));
            }

            auto magnitude_str = vector_str | std::views::drop(1);

            auto [ptr, ec] = std::from_chars(
                magnitude_str.data(),
                magnitude_str.data() + magnitude_str.size(), vector.magnitude);

            if (ec != std::errc{}) {
                throw std::system_error(std::make_error_code(ec));
            }

            wire.push_back(vector);
        }

        wires.push_back(wire);
    }

    std::vector<std::unordered_set<std::pair<int, int>, PairHash>> paths;

    for (auto wire : wires) {
        paths.push_back(wire_path(wire));
    }

    auto intersections = std::accumulate(
        std::next(paths.begin()), paths.end(), paths.front(),
        [](std::unordered_set<std::pair<int, int>, PairHash> acc,
           std::unordered_set<std::pair<int, int>, PairHash> &s) {
            std::unordered_set<std::pair<int, int>, PairHash> out;

            for (const auto &point : acc) {
                if (s.find(point) != s.end()) {
                    out.insert(point);
                }
            }

            return out;
        });

    int part1_answer = std::numeric_limits<int>::max();

    for (auto intersection : intersections) {
        part1_answer =
            std::min(part1_answer, std::abs(intersection.first) +
                                       std::abs(intersection.second));
    }

    std::cout << part1_answer << '\n';
}
