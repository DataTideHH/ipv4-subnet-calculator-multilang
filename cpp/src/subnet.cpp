#include "subnet.h"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace subnet {

namespace {

std::string_view trim_ascii_whitespace(std::string_view text) {
    const auto first = text.find_first_not_of(" \t\n\r\f\v");

    if (first == std::string_view::npos) {
        return {};
    }

    const auto last = text.find_last_not_of(" \t\n\r\f\v");
    return text.substr(first, last - first + 1);
}

std::vector<std::string_view> split(std::string_view text, char delimiter) {
    std::vector<std::string_view> parts;
    std::size_t start = 0;

    while (start <= text.size()) {
        const std::size_t end = text.find(delimiter, start);

        if (end == std::string_view::npos) {
            parts.push_back(text.substr(start));
            break;
        }

        parts.push_back(text.substr(start, end - start));
        start = end + 1;
    }

    return parts;
}

bool is_all_digits(std::string_view text) {
    for (const char character : text) {
        if (character < '0' || character > '9') {
            return false;
        }
    }

    return true;
}

int parse_bounded_decimal(std::string_view text, std::string_view label, int maximum) {
    const auto first_non_zero = text.find_first_not_of('0');
    const std::string_view normalized = first_non_zero == std::string_view::npos
        ? text.substr(text.size() - 1)
        : text.substr(first_non_zero);
    const std::string maximum_text = std::to_string(maximum);

    const bool exceeds_maximum = normalized.size() > maximum_text.size()
        || (normalized.size() == maximum_text.size()
            && normalized.compare(maximum_text) > 0);

    if (exceeds_maximum) {
        throw std::invalid_argument(
            std::string(label) + " out of range (0-" + maximum_text + "): "
            + std::string(text));
    }

    int value = 0;

    for (const char character : normalized) {
        value = value * 10 + (character - '0');
    }

    return value;
}

int parse_prefix(std::string_view prefix_text) {
    if (prefix_text.empty()) {
        throw std::invalid_argument("CIDR prefix is empty.");
    }

    if (!is_all_digits(prefix_text)) {
        throw std::invalid_argument(
            "CIDR prefix must contain only digits: " + std::string(prefix_text));
    }

    if (prefix_text.size() > 1 && prefix_text.front() == '0') {
        throw std::invalid_argument(
            "CIDR prefix must not have leading zeros: " + std::string(prefix_text));
    }

    return parse_bounded_decimal(prefix_text, "CIDR prefix", 32);
}

std::uint32_t parse_ipv4(std::string_view ip_text) {
    const auto parts = split(ip_text, '.');

    if (parts.size() != 4) {
        throw std::invalid_argument(
            "IPv4 must have exactly four octets: " + std::string(ip_text));
    }

    std::uint32_t result = 0;

    for (const auto part : parts) {
        if (part.empty()) {
            throw std::invalid_argument(
                "IPv4 octet is empty in: " + std::string(ip_text));
        }

        if (!is_all_digits(part)) {
            throw std::invalid_argument(
                "IPv4 octet must contain only digits: " + std::string(part));
        }

        if (part.size() > 1 && part.front() == '0') {
            throw std::invalid_argument(
                "IPv4 octet must not have leading zeros: " + std::string(part));
        }

        const int octet = parse_bounded_decimal(part, "IPv4 octet", 255);
        result = (result << 8) | static_cast<std::uint32_t>(octet);
    }

    return result;
}

std::uint32_t prefix_to_mask(int prefix) {
    if (prefix == 0) {
        return 0;
    }

    return std::numeric_limits<std::uint32_t>::max() << (32 - prefix);
}

} // namespace

std::string to_ipv4(std::uint32_t value) {
    std::ostringstream output;

    output
        << ((value >> 24) & 0xFF) << '.'
        << ((value >> 16) & 0xFF) << '.'
        << ((value >> 8) & 0xFF) << '.'
        << (value & 0xFF);

    return output.str();
}

Calculation calculate(std::string_view cidr) {
    cidr = trim_ascii_whitespace(cidr);

    if (cidr.empty()) {
        throw std::invalid_argument("Input is empty.");
    }

    const auto first_slash = cidr.find('/');
    const auto last_slash = cidr.rfind('/');

    if (first_slash == std::string_view::npos) {
        throw std::invalid_argument("Missing '/' separator.");
    }

    if (first_slash != last_slash) {
        throw std::invalid_argument("Input must contain exactly one '/'.");
    }

    const auto ip_text = cidr.substr(0, first_slash);
    const auto prefix_text = cidr.substr(first_slash + 1);

    if (ip_text.empty()) {
        throw std::invalid_argument("IPv4 address is empty.");
    }

    if (prefix_text.empty()) {
        throw std::invalid_argument("CIDR prefix is empty.");
    }

    const std::uint32_t ip = parse_ipv4(ip_text);
    const int prefix = parse_prefix(prefix_text);
    const std::uint32_t mask = prefix_to_mask(prefix);
    const std::uint32_t wildcard = ~mask;
    const std::uint32_t network = ip & mask;
    const std::uint32_t broadcast = network | wildcard;

    // A 64-bit value is required for /0 because the address count is 2^32.
    const std::uint64_t total_addresses = 1ULL << (32 - prefix);

    std::uint64_t usable_hosts;
    std::uint32_t first_usable;
    std::uint32_t last_usable;
    std::string note;

    if (prefix <= 30) {
        usable_hosts = total_addresses - 2;
        first_usable = network + 1;
        last_usable = broadcast - 1;
        note = "Standard subnet with network and broadcast addresses excluded.";
    } else if (prefix == 31) {
        usable_hosts = 2;
        first_usable = network;
        last_usable = broadcast;
        note = "/31 subnet: both addresses are usable for point-to-point links.";
    } else {
        usable_hosts = 1;
        first_usable = network;
        last_usable = network;
        note = "/32 host route: single usable address.";
    }

    return Calculation{
        .input_ip = std::string(ip_text),
        .prefix = prefix,
        .subnet_mask = to_ipv4(mask),
        .wildcard_mask = to_ipv4(wildcard),
        .network_address = to_ipv4(network),
        .broadcast_address = to_ipv4(broadcast),
        .total_addresses = total_addresses,
        .usable_hosts = usable_hosts,
        .first_usable_host = to_ipv4(first_usable),
        .last_usable_host = to_ipv4(last_usable),
        .note = note
    };
}

} // namespace subnet
