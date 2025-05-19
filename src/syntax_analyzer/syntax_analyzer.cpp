#include <iostream>
#include <format>
#include <variant>
#include "syntax_analyzer.hpp"
#include "ll1_analyzer.tpp"
#include "grammar.hpp"

constexpr lexeme syntax_analyzer::next_token() const
{
    return *it;
}

constexpr bool syntax_analyzer::pre_analysis(lexeme expected) const
{
    return expected == *it || expected == lexeme::UNDETERMINATED;
}

constexpr auto syntax_analyzer::match(lexeme expected) -> std::expected<void, error>
{
    if (expected == *it)
    {
        it++;
        return {};
    }
    return std::unexpected(error(std::format("Sintax error. Expected '{}' but '{}' encountred", (int)expected, (int)*it)));
}

constexpr auto syntax_analyzer::match(std::expected<void, error>(*)(syntax_analyzer&)) -> std::expected<void, error>
{
    return std::unexpected(error("Panic."));
}

constexpr auto syntax_analyzer::match(beta_type) -> std::expected<void, error>
{
        return std::unexpected(error("Panic."));
}

auto syntax_analyzer::start() -> std::expected<void, error>
{
    return rule<START>::run(*this);
}

