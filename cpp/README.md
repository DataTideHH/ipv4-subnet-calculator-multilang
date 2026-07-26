# C++ Implementation

C++20 implementation of the shared IPv4 subnet calculator specification.

## Build

From the repository root:

```text
cmake -S cpp -B cpp/build
cmake --build cpp/build
```

## Interactive mode

On Linux and macOS:

```text
./cpp/build/ipv4_subnet_calculator
```

With a multi-configuration generator on Windows, the executable is usually located in a configuration subdirectory, for example:

```text
.\cpp\build\Debug\ipv4_subnet_calculator.exe
```

Enter IPv4/CIDR values repeatedly and use `q` or `Q` to quit.

## Direct command-line mode

Linux and macOS:

```text
./cpp/build/ipv4_subnet_calculator 192.168.10.42/24
```

Windows example:

```text
.\cpp\build\Debug\ipv4_subnet_calculator.exe 192.168.10.42/24
```

## Help

```text
<executable> --help
```

## Implementation notes

- uses a C++ struct for the structured calculation result
- separates calculation, validation and terminal interaction
- reports concrete validation errors through `std::invalid_argument`
- uses `std::uint32_t` for IPv4 values and `std::uint64_t` for address counts
- has no external runtime dependencies
- follows `docs/behavior-specification.md`
