#pragma once

#include <functional>

class semantic_rule
{
    std::function<void(void)> function;
};