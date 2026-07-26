#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace subnet {

struct Calculation {
    std::string input_ip;
    int prefix;
    std::string subnet_mask;
    std::string wildcard_mask;
    std::string network_address;
    std::string broadcast_address;
    std::uint64_t total_addresses;
    std::uint64_t usable_hosts;
    std::string first_usable_host;
    std::string last_usable_host;
    std::string note;
};

Calculation calculate(std::string_view cidr);
std::string to_ipv4(std::uint32_t value);

} // namespace subnet
