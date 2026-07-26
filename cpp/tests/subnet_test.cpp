#include "subnet.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::vector<std::string> split_tabs(const std::string& line) {
    std::vector<std::string> columns;
    std::size_t start = 0;

    while (start <= line.size()) {
        const std::size_t end = line.find('\t', start);

        if (end == std::string::npos) {
            columns.push_back(line.substr(start));
            break;
        }

        columns.push_back(line.substr(start, end - start));
        start = end + 1;
    }

    return columns;
}

void assert_equal(std::string_view label, const std::string& expected, const std::string& actual) {
    if (expected != actual) {
        throw std::runtime_error(
            std::string(label) + " expected <" + expected + "> but was <" + actual + ">");
    }
}

void assert_equal(std::string_view label, std::uint64_t expected, std::uint64_t actual) {
    if (expected != actual) {
        throw std::runtime_error(
            std::string(label) + " expected <" + std::to_string(expected)
            + "> but was <" + std::to_string(actual) + ">");
    }
}

void verify_valid(const std::vector<std::string>& test_case) {
    const std::string& name = test_case[1];
    const auto result = subnet::calculate(test_case[2]);
    const std::string trimmed_input = test_case[2].substr(
        test_case[2].find_first_not_of(" \t\n\r\f\v"),
        test_case[2].find_last_not_of(" \t\n\r\f\v")
            - test_case[2].find_first_not_of(" \t\n\r\f\v") + 1);
    const auto separator = trimmed_input.find('/');

    assert_equal(name + " input IP", trimmed_input.substr(0, separator), result.input_ip);
    assert_equal(name + " prefix", std::stoull(trimmed_input.substr(separator + 1)),
                 static_cast<std::uint64_t>(result.prefix));
    assert_equal(name + " subnet mask", test_case[3], result.subnet_mask);
    assert_equal(name + " wildcard mask", test_case[4], result.wildcard_mask);
    assert_equal(name + " network", test_case[5], result.network_address);
    assert_equal(name + " broadcast", test_case[6], result.broadcast_address);
    assert_equal(name + " total", std::stoull(test_case[7]), result.total_addresses);
    assert_equal(name + " usable", std::stoull(test_case[8]), result.usable_hosts);
    assert_equal(name + " first", test_case[9], result.first_usable_host);
    assert_equal(name + " last", test_case[10], result.last_usable_host);
    assert_equal(name + " note", test_case[11], result.note);
}

void verify_invalid(const std::vector<std::string>& test_case) {
    try {
        static_cast<void>(subnet::calculate(test_case[2]));
        throw std::runtime_error(test_case[1] + " expected std::invalid_argument");
    } catch (const std::invalid_argument& error) {
        assert_equal(test_case[1] + " error", test_case[11], error.what());
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Expected path to tests/cases.tsv.\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Could not open shared test cases: " << argv[1] << '\n';
        return 1;
    }

    try {
        std::string line;
        int executed = 0;

        while (std::getline(input, line)) {
            if (line.empty() || line.front() == '#') {
                continue;
            }

            const auto columns = split_tabs(line);
            if (columns.size() != 12) {
                throw std::runtime_error(
                    "Expected 12 columns, found " + std::to_string(columns.size()) + ": " + line);
            }

            if (columns[0] == "valid") {
                verify_valid(columns);
            } else if (columns[0] == "invalid") {
                verify_invalid(columns);
            } else {
                throw std::runtime_error("Unknown case kind: " + columns[0]);
            }

            ++executed;
        }

        std::cout << "C++ contract tests passed: " << executed << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "C++ contract test failed: " << error.what() << '\n';
        return 1;
    }
}
