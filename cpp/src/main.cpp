#include "subnet.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

std::string_view trim_ascii_whitespace(std::string_view text) {
    const auto first = text.find_first_not_of(" \t\n\r\f\v");

    if (first == std::string_view::npos) {
        return {};
    }

    const auto last = text.find_last_not_of(" \t\n\r\f\v");
    return text.substr(first, last - first + 1);
}

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
    const auto command = trim_ascii_whitespace(input);
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

int run_program(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        run_interactive_mode();
        return 0;
    }

    if (arguments.size() != 2) {
        std::cerr << "Error: Expected zero or one argument.\n";
        print_usage(arguments.front());
        return 1;
    }

    const std::string_view argument = arguments[1];

    if (argument == "-h" || argument == "--help") {
        print_usage(arguments.front());
        return 0;
    }

    return process_input(argument, true);
}

#ifdef _WIN32
std::string wide_to_utf8(std::wstring_view text) {
    if (text.empty()) {
        return {};
    }

    const int required_size = WideCharToMultiByte(
        CP_UTF8,
        WC_ERR_INVALID_CHARS,
        text.data(),
        static_cast<int>(text.size()),
        nullptr,
        0,
        nullptr,
        nullptr);

    if (required_size <= 0) {
        throw std::runtime_error("Could not convert the Windows command line to UTF-8.");
    }

    std::string result(static_cast<std::size_t>(required_size), '\0');
    const int written_size = WideCharToMultiByte(
        CP_UTF8,
        WC_ERR_INVALID_CHARS,
        text.data(),
        static_cast<int>(text.size()),
        result.data(),
        required_size,
        nullptr,
        nullptr);

    if (written_size != required_size) {
        throw std::runtime_error("Could not convert the Windows command line to UTF-8.");
    }

    return result;
}
#endif

} // namespace

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[]) {
    try {
        std::vector<std::string> arguments;
        arguments.reserve(static_cast<std::size_t>(argc));

        for (int index = 0; index < argc; ++index) {
            arguments.push_back(wide_to_utf8(argv[index]));
        }

        return run_program(arguments);
    } catch (const std::runtime_error& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
#else
int main(int argc, char* argv[]) {
    const std::vector<std::string> arguments(argv, argv + argc);
    return run_program(arguments);
}
#endif
