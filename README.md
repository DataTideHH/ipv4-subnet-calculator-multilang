# IPv4 Subnet Calculator Multilang

[![CI](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/actions/workflows/ci.yml/badge.svg)](https://github.com/DataTideHH/ipv4-subnet-calculator-multilang/actions/workflows/ci.yml)

**Java 21 · C++20 · Python 3.12 · IPv4 subnetting · contract testing · CLI parity testing · GitHub Actions**

A focused terminal-based IPv4 subnet calculator implemented in three programming languages from one shared behavior specification and verified through shared domain cases plus process-level conformance tests.

Project page: https://datatidehh.github.io/ipv4-subnet-calculator-multilang/

---

## Project Purpose

This repository compares how the same clearly bounded networking task can be implemented and tested in Java, C++ and Python without adding unnecessary application complexity.

Each implementation follows the same contract:

- accept one IPv4 address with a CIDR prefix
- validate the complete input explicitly
- use the same structural validation priority
- ignore only the same six outer ASCII-whitespace characters
- reject multi-digit CIDR prefixes with leading zeros
- calculate subnet, wildcard, network, broadcast and host-range values
- print the same fields in the same order
- support interactive, direct and help modes
- handle `/0`, `/31` and `/32` correctly
- return consistent exit codes
- classify ordinary and oversized numeric range errors consistently
- use no external runtime dependency

The project deliberately excludes IPv6, VLSM planning, subnet splitting, a GUI, a web frontend, database storage and API endpoints.

---

## Implementation and Test Status

| Implementation | Version / Standard | Result type | Test runner | Status |
|---|---|---|---|---|
| Java | Java 21 | Record | Plain Java CLI contract runner | Implemented and tested |
| C++ | C++20 | Struct | CTest executable and CMake CLI script | Implemented and tested |
| Python | Python 3.12 | Frozen, slotted dataclass | `unittest` and subprocess checks | Implemented and tested |

All three versions use equivalent calculation rules, validation categories, output fields, special-case handling, terminal modes and exit behavior.

A separate cross-language runner executes all three real command-line programs and compares complete `stdout`, `stderr` and exit codes.

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

Only these characters are ignored around the complete input:

```text
space, tab, line feed, carriage return, form feed, vertical tab
```

Other control characters and Unicode whitespace remain part of the token and are rejected. Whitespace inside the IPv4/CIDR token is invalid.

CIDR prefixes use canonical decimal notation: `/0` is valid, while `/00`, `/08` and `/0032` are rejected.

---

## Validation Priority

After exactly one `/` separator is found, validation proceeds in this order:

1. empty IPv4 component
2. empty prefix component
3. detailed IPv4 validation
4. detailed prefix validation

This order is part of the observable contract. For example:

```text
192.168.1.999/
```

returns:

```text
Error: CIDR prefix is empty.
```

The empty prefix is reported before the invalid IPv4 octet in every implementation.

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

The `/0` output is a mathematical calculation over the complete 32-bit IPv4 address space. It does not imply that all reported addresses are globally assignable to hosts.

---

## Terminal Modes

### Interactive mode

Starting an implementation without a subnet argument opens a reusable prompt. Valid results and errors are followed by another prompt. ASCII-trimmed `q` or `Q` quits.

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

The language-neutral file [`tests/cases.tsv`](tests/cases.tsv) defines expected results and validation failures used by every language-specific runner.

The current contract contains seven valid and twenty-three invalid cases covering:

- standard `/24`, `/30` and `/16` calculations
- the complete `/0` IPv4 address space
- `/31` point-to-point behavior
- `/32` host-route behavior
- surrounding ASCII-whitespace handling
- missing and repeated CIDR separators
- empty, malformed, signed and out-of-range values
- oversized numeric octets and prefixes
- leading-zero rejection for octets and prefixes
- whitespace inside the token
- combined-error cases that verify validation priority

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

Every language also verifies valid direct execution, invalid direct execution, help mode and incorrect usage.

---

## Cross-Language Parity Tests

[`tests/cross_language_parity.py`](tests/cross_language_parity.py) executes the compiled Java and C++ programs together with the Python program.

It compares the full process observation:

```text
(return code, stdout, stderr)
```

The current suite contains nine direct and two interactive cases covering:

- mixed-invalid inputs and validation priority
- leading-zero prefixes
- a non-whitespace ASCII control character
- non-breaking and ideographic spaces
- allowed ASCII tab and carriage-return trimming
- interactive quit handling

After Java and C++ have been built:

```text
python tests/cross_language_parity.py
```

See [Testing and CI](docs/testing-and-ci.md) for the verification design.

---

## GitHub Actions

The `CI` workflow runs on pull requests, pushes to `main` and manual dispatches. It exposes four independent checks:

- `Java 21`
- `C++20`
- `Python 3.12`
- `Cross-language parity`

The jobs use read-only repository permissions, cancel superseded runs and apply per-job timeouts.

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
│   ├── cases.tsv
│   └── cross_language_parity.py
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
│   └── tests/
│       ├── subnet_test.cpp
│       └── cli_smoke_tests.cmake
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
| 5 | Portfolio integration, hardening and related-project links | Complete for current scope |
| 6 | Adversarial parsing review and process-level cross-language parity | Complete |

---

## Official References and Learning Resources

- [RFC 3021: Using 31-Bit Prefixes on IPv4 Point-to-Point Links](https://www.rfc-editor.org/rfc/rfc3021.html)
- [RFC 4632: Classless Inter-domain Routing (CIDR)](https://www.rfc-editor.org/rfc/rfc4632.html)
- [Core Internet Standards and RFC Editor](https://github.com/DataTideHH/open-learning-resources/tree/main/resources/networking/core-internet-standards-rfc-editor)
- [GitHub Actions Documentation](https://github.com/DataTideHH/open-learning-resources/tree/main/resources/git/github-actions-documentation)

This learning implementation is not intended to replace a mature IP-address library.

---

## Related DataTideHH Projects

- [cisco-switching-lab](https://github.com/DataTideHH/cisco-switching-lab) provides the physical switching and CCNA-oriented networking context behind the subnetting concepts.
- [spring-boot-process-api-basics](https://github.com/DataTideHH/spring-boot-process-api-basics) shows the next Java progression from a focused command-line implementation to a layered REST API.

---

## Portfolio Context

The project is a supporting IT-foundations project within a broader Data/BI and process-analysis portfolio. Its value is not the size of the calculator, but the controlled implementation of one specification in three languages, explicit validation, shared domain tests, adversarial review, reproducible CI and clear technical documentation.

It demonstrates how networking knowledge, software structure, test design and documentation can reinforce one another without overstating the project as an enterprise application.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
