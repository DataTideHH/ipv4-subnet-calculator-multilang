# IPv4 Subnet Calculator Multilang

**Java 21 · C++20 · Python 3.12 · IPv4 subnetting · terminal application · input validation · reproducible comparison**

A small terminal-based IPv4 subnet calculator implemented in three programming languages from one shared behavior specification.

The Java, C++ and Python implementations are complete for the current Phase 3 scope.

Project page: https://datatidehh.github.io/ipv4-subnet-calculator-multilang/

---

## Project Purpose

This repository compares how the same clearly bounded networking task can be implemented in multiple programming languages without adding unnecessary application complexity.

Each implementation follows the same contract:

- accept an IPv4 address with a CIDR prefix
- validate the complete input
- calculate the subnet information
- print the result in a consistent terminal format
- support interactive and direct command-line modes
- handle `/0`, `/31` and `/32` correctly
- use only the language standard library where practical

The project is deliberately limited to IPv4 subnet calculation. It is not intended to become a network-management platform, web application or general-purpose IP address library.

---

## Implementation Status

| Implementation | Version / Standard | Build or Runtime | Status |
|---|---|---|---|
| Java | Java 21 | `javac` / `java` | Implemented |
| C++ | C++20 | CMake | Implemented |
| Python | Python 3.12 | Python interpreter | Implemented |

All three versions use equivalent validation rules, output fields, special-case handling, terminal modes and exit behavior.

---

## Common Input

The canonical input format is:

```text
IPv4/CIDR
```

Examples:

```text
192.168.10.42/24
10.0.0.5/30
172.16.1.1/16
203.0.113.15/32
```

The project intentionally supports one clear input format instead of multiple alternative syntaxes.

---

## Terminal Modes

### Interactive mode

Starting an implementation without a subnet argument opens an interactive prompt:

```text
IPv4 Subnet Calculator

Enter IPv4/CIDR or q to quit:
> 192.168.10.42/24
```

After a valid result or validation error, the calculator accepts another input. Enter `q` or `Q` to quit.

### Direct command-line mode

A subnet can also be passed directly for scripting and repeatable manual checks:

```text
<program> 192.168.10.42/24
```

Successful direct execution returns exit code `0`. Invalid input or incorrect usage returns exit code `1`.

### Help

All implementations support:

```text
-h
--help
```

---

## Output

For a valid input, all implementations print the same fields in the same order:

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

The `/31` and `/32` cases use dedicated notes and host-range rules defined in the shared specification.

---

## Java 21

### Build

From the repository root:

```text
javac -d java/out java/src/SubnetCalculator.java
```

### Interactive mode

```text
java -cp java/out SubnetCalculator
```

### Direct mode

```text
java -cp java/out SubnetCalculator 192.168.10.42/24
```

See [java/README.md](java/README.md) for implementation notes.

---

## C++20

### Build

From the repository root:

```text
cmake -S cpp -B cpp/build
cmake --build cpp/build
```

For a release build with a multi-configuration generator:

```text
cmake --build cpp/build --config Release
```

### Interactive mode

Linux and macOS:

```text
./cpp/build/ipv4_subnet_calculator
```

Common Windows paths are:

```text
.\cpp\build\Debug\ipv4_subnet_calculator.exe
.\cpp\build\Release\ipv4_subnet_calculator.exe
```

### Direct mode

Linux and macOS:

```text
./cpp/build/ipv4_subnet_calculator 192.168.10.42/24
```

Windows release build:

```text
.\cpp\build\Release\ipv4_subnet_calculator.exe 192.168.10.42/24
```

See [cpp/README.md](cpp/README.md) for platform notes.

---

## Python 3.12

No build step or external package installation is required.

### Interactive mode

```text
python python/subnet_calculator.py
```

### Direct mode

```text
python python/subnet_calculator.py 192.168.10.42/24
```

On Windows, a specific interpreter can be selected with the Python launcher:

```text
py -3.12 python/subnet_calculator.py 192.168.10.42/24
```

See [python/README.md](python/README.md) for implementation notes.

---

## Validation Scope

The calculator rejects invalid input such as:

- a missing CIDR separator
- more than one `/` separator
- missing or additional IPv4 octets
- empty IPv4 octets
- non-decimal IPv4 octets
- octets outside `0-255`
- leading zeros in multi-digit octets
- a missing or non-decimal CIDR prefix
- prefixes outside `0-32`

Errors identify the validation category, for example:

```text
Error: Missing '/' separator.
Error: IPv4 octet out of range (0-255): 300
Error: IPv4 octet must not have leading zeros: 001
Error: CIDR prefix out of range (0-32): 33
```

Interactive mode reports the error and continues. Direct command-line mode reports the error and returns a non-zero exit code.

The normative behavior is documented in [docs/behavior-specification.md](docs/behavior-specification.md).

---

## Repository Structure

```text
ipv4-subnet-calculator-multilang/
├── README.md
├── LICENSE
├── CONTRIBUTING.md
├── java/
│   ├── README.md
│   └── src/SubnetCalculator.java
├── cpp/
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── include/subnet.h
│   └── src/
│       ├── main.cpp
│       └── subnet.cpp
├── python/
│   ├── README.md
│   └── subnet_calculator.py
├── docs/
│   ├── _config.yml
│   ├── index.md
│   ├── behavior-specification.md
│   ├── implementation-comparison.md
│   └── assets/css/style.scss
└── .github/
    └── pull_request_template.md
```

Automated tests and continuous integration are planned for Phase 4.

---

## Manual Validation Performed

The three implementations were manually checked for:

- standard `/24` output
- `/0` address count and host range
- `/31` point-to-point behavior
- `/32` single-host behavior
- all invalid-input examples from the shared specification
- interactive input, error recovery and quit behavior
- direct-mode success and error exit codes
- help and incorrect-usage behavior

Formal test suites will be added separately in Phase 4.

---

## Project Phases

| Phase | Scope | Status |
|---|---|---|
| 1 | Repository foundation, shared specification and project page | Complete |
| 2 | Java and C++ consolidation with interactive mode | Complete |
| 3 | Python implementation | Complete |
| 4 | Tests and GitHub Actions | Planned |
| 5 | Portfolio integration and archival of superseded repositories | Planned |

---

## Design Principles

- Keep the task small and understandable.
- Separate terminal interaction from calculation logic.
- Use a structured result object in every language.
- Prefer explicit validation over ambiguous parsing.
- Keep equivalent behavior across all implementations.
- Add comments only where the implementation is not self-explanatory.
- Use English for code, comments and technical documentation.
- Avoid claims that exceed the implemented and verified scope.

---

## Portfolio Context

The project supports a learning path in data and process analysis, Python, Java, C++, networking fundamentals and reproducible technical documentation.

Its main portfolio value is the controlled comparison of three implementations of the same technical specification. It also connects programming practice with IPv4 subnetting and CCNA-oriented networking fundamentals.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
