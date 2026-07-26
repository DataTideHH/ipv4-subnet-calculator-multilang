---
title: Implementation Comparison
description: Comparison of the Java, C++ and planned Python calculator implementations
---

# Implementation Comparison

The Java and C++ implementations now follow the same observable behavior. The Python implementation will be added in Phase 3 using the same specification.

## Shared behavior

Both current implementations provide:

- interactive terminal mode without arguments
- direct command-line mode with one IPv4/CIDR argument
- `-h` and `--help`
- the same output fields and ordering
- the same `/0`, `/31` and `/32` behavior
- concrete validation errors
- exit code `0` for successful direct execution
- exit code `1` for invalid direct input or incorrect usage

## Current comparison

| Area | Java 21 | C++20 | Python 3.12 |
|---|---|---|---|
| Status | Implemented | Implemented | Planned |
| Result type | Record | Struct | Dataclass planned |
| Build | `javac` | CMake | None planned |
| Runtime | JVM | Native executable | Python interpreter |
| Dependencies | Java standard library | C++ standard library | Standard library planned |
| Interactive mode | Implemented | Implemented | Planned |
| Direct mode | Implemented | Implemented | Planned |
| Help options | `-h`, `--help` | `-h`, `--help` | Planned |
| Validation errors | `IllegalArgumentException` | `std::invalid_argument` | Planned |
| IPv4 storage | 64-bit `long` calculation values | `std::uint32_t` | Integer planned |
| Address count | 64-bit `long` | `std::uint64_t` | Integer planned |
| Automated tests | Phase 4 | Phase 4 | Phase 4 |

## Java design

The Java version keeps the complete beginner-readable implementation in one source file.

A private record stores the calculated values. Dedicated methods handle:

- program modes
- user input
- output formatting
- IPv4 validation
- CIDR validation
- subnet arithmetic
- IPv4 text conversion

The implementation uses a `long` for address counts so `/0` can represent all `4,294,967,296` IPv4 addresses.

## C++ design

The C++ version separates the terminal interface from the calculation module:

```text
cpp/src/main.cpp
cpp/src/subnet.cpp
cpp/include/subnet.h
```

The `Calculation` struct represents the shared result. IPv4 values use `std::uint32_t`, while total and usable address counts use `std::uint64_t`.

Earlier code returned only an empty `std::optional` for invalid input. The consolidated implementation throws `std::invalid_argument` with the specific validation reason, allowing the terminal layer to report errors equivalent to the Java version.

## Deliberate differences

The goal is equivalent behavior, not identical source code.

- Java uses exceptions that are conventional for invalid arguments in a small command-line program.
- C++ uses fixed-width unsigned integer types for explicit IPv4 arithmetic.
- Java remains in one file to avoid unnecessary structure for a small program.
- C++ keeps a header and source split because the calculation module and executable are separate compilation units.

These differences demonstrate language conventions while preserving the same user-facing contract.

## Validation status

The current implementations were built and manually compared for:

- standard subnet output
- `/0`
- `/31`
- `/32`
- all invalid examples in the shared specification
- interactive error recovery
- quit behavior
- direct-mode exit codes

Formal test suites and continuous integration remain intentionally reserved for Phase 4.
