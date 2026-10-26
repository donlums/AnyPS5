#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace AgcDriver::Graphics {

inline bool CurrentWriteBackCompletion(std::span<const std::uint64_t> tickets, std::size_t unit, std::uint64_t ticket) noexcept {
    return ticket != 0 && unit < tickets.size() && tickets[unit] == ticket;
}

}
