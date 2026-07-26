#include "subnet.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

void print_usage(std::string_view program_name) {
    std::cout
        << "IPv4 Subnet Calculator\n\n"
        << "Usage:\n"
        << "  " << program_name << '\n'
        << "  " << program_name << " <IPv4/CIDR>\n"
        << "  " << program_name << " --help\n\n"
        << "Without an argument, the calculator starts in interactive mode.\n\n"
        << "Examples:\n"
        << "  " << program_name << " 192.168.10.42/24\n"
        << "  " << program_name << " 10.0.0.5/30\n"
        << "  " << program_name << " 203.0.113.15/32\n";
}

void print_result(const subnet::Calculation& result) {
    std::cout
        << "Input IP:          " << result.input_ip << '\n'
        << "CIDR prefix:       /" << result.prefix << '\n'
        << "Subnet mask:       " << result.subnet_mask << '\n'
        << "Wildcard mask:     " << result.wildcard_mask << '\n'
        << "Network address:   " << result.network_address << '\n'
        << "Broadcast address: " << result.broadcast_address << '\n'
        << "Total addresses:   " << result.total_addresses << '\n'
        << "Usable hosts:      " << result.usable_hosts << '\n'
        << "First usable host: " << result.first_usable_host << '\n'
        << "Last usable host:  " << result.last_usable_host << '\n'
        << "Note:              " << result.note << '\n';
}

int process_input(std::string_view input, bool show_format_hint) {
    try {
        print_result(subnet::calculate(input));
        return 0;
    } catch (const std::invalid_argument& error) {
        std::cerr << "Error: " << error.what() << '\n';

        if (show_format_hint) {
            std::cerr
                << "Expected format: IPv4/CIDR, for example 192.168.10.42/24\n";
        }

        return 1;
    }
}

bool is_quit_command(std::string_view input) {
    const auto first = input.find_first_not_of(" \t\n\r\f\v");

    if (first == std::string_view::npos) {
        return false;
    }

    const auto last = input.find_last_not_of(" \t\n\r\f\v");
    const auto command = input.substr(first, last - first + 1);
    return command == "q" || command == "Q";
}

void run_interactive_mode() {
    std::cout << "IPv4 Subnet Calculator\n\n";
    std::string input;

    while (true) {
        std::cout << "Enter IPv4/CIDR or q to quit:\n> " << std::flush;

        if (!std::getline(std::cin, input)) {
            std::cout << '\n';
            return;
        }

        if (is_quit_command(input)) {
            return;
        }

        process_input(input, false);
        std::cout << '\n';
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc == 1) {
        run_interactive_mode();
        return 0;
    }

    if (argc != 2) {
        std::cerr << "Error: Expected zero or one argument.\n";
        print_usage(argv[0]);
        return 1;
    }

    const std::string_view argument = argv[1];

    if (argument == "-h" || argument == "--help") {
        print_usage(argv[0]);
        return 0;
    }

    return process_input(argument, true);
}
