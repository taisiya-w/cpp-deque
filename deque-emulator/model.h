#pragma once

#include <deque>
#include <string>
#include <algorithm>
#include <random>

struct Model {
    using Deque = std::deque<std::string>;
    Deque items;
    Deque::iterator iterator = items.begin();
    std::mt19937 random_gen{std::random_device{}()};
};
