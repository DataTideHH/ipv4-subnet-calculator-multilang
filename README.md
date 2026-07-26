# IPv4 Subnet Calculator Multilang

[![CI](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/actions/workflows/ci.yml/badge.svg)](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/actions/workflows/ci.yml)

**Java 21 · C++20 · Python 3.12 · IPv4 subnetting · contract testing · GitHub Actions · reproducible comparison**

A focused terminal-based IPv4 subnet calculator implemented in three programming languages from one shared behavior specification and verified against one shared set of domain cases.

Project page: https://datatidehh.github.io/ipv4-subnet-calculator-multilang/

---

## Project Purpose

This repository compares how the same clearly bounded networking task can be implemented and tested in Java, C++ and Python without adding unnecessary application complexity.

Each implementation follows the same contract:

- accept one IPv4 address with a CIDR prefix
- validate the complete input explicitly
- calculate subnet, wildcard, network, broadcast and host-range values
- print the same fields in the same order
- support interactive, direct and help modes
- handle `/0`, `/31` and `/32` correctly
- return consistent exit codes
- use no external runtime dependency

The project deliberately excludes IPv6, VLSM planning, subnet splitting, a GUI, a web frontend, database storage and API endpoints.

---

## Implementation and Test Status

| Implementation | Version / Standard | Result type | Test runner | Status |
|---|---|---|---|---|
| Java | Java 21 | Record | Plain Java contract runner | Implemented and tested |
| C++ | C++20 | Struct | CTest executable | Implemented and tested |
| Python | Python 3.12 | Frozen, slotted dataclass | `unittest` | Implemented and tested |

All three versions use equivalent calculation rules, validation categories, output fields, special-case handling, terminal modes and exit behavior.

---

## Common Input

```text
IPv4/CIDR
```

Examples:

```text
192.168.10.42/24
10.0.0.5/30
172.16.1.1/16
192.0.2.10/31
203.0.113.15/32
```

Leading and trailing whitespace around the complete input is ignored. Whitespace inside the IPv4/CIDR token is rejected.

---

## Output

```text
Input IP:          192.168.10.42
CIDR prefix:       /24
Subnet mask:       255.255.255.0
Wildcard mask:     0.0.0.255
Network address:   192.168.10.0
Broadcast address: 192.168.10.255
Total addresses:   256
Usable hosts:      254
First usable host: 192.168.10.1
Last usable host:  192.168.10.254
Note:              Standard subnet with network and broadcast addresses excluded.
```

The `/31` and `/32` cases use the dedicated host-range rules documented in the [behavior specification](docs/behavior-specification.md).

---

## Terminal Modes

### Interactive mode

Starting an implementation without a subnet argument opens a reusable prompt. Valid results and errors are followed by another prompt. Enter `q` or `Q` to quit.

### Direct mode

Passing exactly one IPv4/CIDR argument performs one calculation and exits. Success returns exit code `0`; invalid input or incorrect usage returns exit code `1`.

### Help

Every implementation supports `-h` and `--help`.

---

## Run the Implementations

### Java 21

```text
javac -d java/out java/src/SubnetCalculator.java
java -cp java/out SubnetCalculator 192.168.10.42/24
```

Interactive mode:

```text
java -cp java/out SubnetCalculator
```

### C++20

```text
cmake -S cpp -B cpp/build
cmake --build cpp/build --config Release
```

Linux and macOS:

```text
./cpp/build/ipv4_subnet_calculator 192.168.10.42/24
```

A common Windows multi-configuration path is:

```text
.\cpp\build\Release\ipv4_subnet_calculator.exe 192.168.10.42/24
```

### Python 3.12

```text
python python/subnet_calculator.py 192.168.10.42/24
```

Windows Python launcher:

```text
py -3.12 python/subnet_calculator.py 192.168.10.42/24
```

---

## Shared Contract Tests

The language-neutral file [`tests/cases.tsv`](tests/cases.tsv) defines the expected results and validation failures used by every test runner.

The current contract contains:

- standard `/24`, `/30` and `/16` calculations
- the complete `/0` IPv4 address space
- `/31` point-to-point behavior
- `/32` host-route behavior
- surrounding-whitespace handling
- missing and repeated CIDR separators
- empty, malformed, signed and out-of-range values
- leading-zero rejection
- whitespace inside the token

Valid rows verify all result fields. Invalid rows verify the exact validation reason.

### Run Java tests

```text
javac -d java/out java/src/SubnetCalculator.java java/test/SubnetCalculatorTest.java
java -cp java/out SubnetCalculatorTest tests/cases.tsv
```

### Run C++ tests

```text
cmake -S cpp -B cpp/build -DBUILD_TESTING=ON
cmake --build cpp/build --config Release
ctest --test-dir cpp/build --build-config Release --output-on-failure
```

### Run Python tests

```text
python -m unittest discover -s python -p "test_*.py" -v
```

See [Testing and CI](docs/testing-and-ci.md) for the verification design.

---

## GitHub Actions

The `CI` workflow runs on pull requests, pushes to `main` and manual dispatches. It exposes three independent checks:

- `Java 21`
- `C++20`
- `Python 3.12`

The jobs use Java 21, a C++20 CMake build and Python 3.12 on GitHub-hosted Linux runners. No third-party test framework is required.

---

## Validation Scope

The calculator rejects:

- missing or multiple `/` separators
- missing or additional IPv4 octets
- empty IPv4 octets
- non-ASCII or non-decimal octets
- octets outside `0-255`
- leading zeros in multi-digit octets
- empty, signed or non-decimal prefixes
- prefixes outside `0-32`
- whitespace inside the token

Errors identify the concrete validation category rather than returning one generic invalid-input message.

---

## Repository Structure

```text
ipv4-subnet-calculator-multilang/
├── .github/workflows/ci.yml
├── README.md
├── CONTRIBUTING.md
├── LICENSE
├── tests/
│   ├── README.md
│   └── cases.tsv
├── java/
│   ├── README.md
│   ├── src/SubnetCalculator.java
│   └── test/SubnetCalculatorTest.java
├── cpp/
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── include/subnet.h
│   ├── src/
│   │   ├── main.cpp
│   │   └── subnet.cpp
│   └── tests/subnet_test.cpp
├── python/
│   ├── README.md
│   ├── subnet_calculator.py
│   └── test_subnet_calculator.py
└── docs/
    ├── index.md
    ├── behavior-specification.md
    ├── implementation-comparison.md
    └── testing-and-ci.md
```

---

## Project Phases

| Phase | Scope | Status |
|---|---|---|
| 1 | Repository foundation, shared specification and project page | Complete |
| 2 | Java and C++ consolidation with interactive mode | Complete |
| 3 | Python implementation | Complete |
| 4 | Shared tests, language-specific runners and GitHub Actions | Complete |
| 5 | Portfolio integration, related-project links and archival review | In progress |

---

## Related DataTideHH Projects

- [cisco-switching-lab](https://github.com/DataTideHH/cisco-switching-lab) provides the physical switching and CCNA-oriented networking context behind the subnetting concepts.
- [spring-boot-process-api-basics](https://github.com/DataTideHH/spring-boot-process-api-basics) shows the next Java progression from a focused command-line implementation to a layered REST API.

---

## Portfolio Context

The project is a supporting IT-foundations project within a broader Data/BI and process-analysis portfolio. Its value is not the size of the calculator, but the controlled implementation of one specification in three languages, explicit validation, shared domain tests, reproducible CI and clear technical documentation.

It demonstrates how networking knowledge, software structure, test design and documentation can reinforce one another without overstating the project as an enterprise application.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
