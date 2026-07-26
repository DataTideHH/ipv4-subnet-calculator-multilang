# IPv4 Subnet Calculator Multilang

**Java 21 · C++20 · Python 3.12 · IPv4 subnetting · terminal application · input validation · reproducible comparison**

A small terminal-based IPv4 subnet calculator implemented in Java, C++ and Python.

All three implementations will follow the same input rules, calculation behavior and output structure. The project focuses on readable code, explicit validation and a practical connection between programming fundamentals and IPv4 subnetting.

Project page: https://datatidehh.github.io/ipv4-subnet-calculator-multilang/

---

## Project Purpose

This repository compares how the same clearly bounded networking task can be implemented in three programming languages without adding unnecessary application complexity.

Each implementation will:

- accept an IPv4 address with a CIDR prefix
- validate the complete input
- calculate the subnet information
- print the result in a consistent terminal format
- support interactive use and direct command-line use
- handle `/0`, `/31` and `/32` correctly
- use only the language standard library where practical

The project is deliberately limited to IPv4 subnet calculation. It is not intended to become a network-management platform, web application or general-purpose IP address library.

---

## Planned Implementations

| Implementation | Version / Standard | Build or Runtime | External dependencies |
|---|---|---|---|
| Java | Java 21 | `javac` / `java` | None |
| C++ | C++20 | CMake | None |
| Python | Python 3.12 | Python interpreter | None |

The implementations will be added incrementally in later project phases.

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

The first version will intentionally support one clear input format instead of multiple alternative syntaxes.

---

## Planned Terminal Usage

### Interactive mode

Starting an implementation without a subnet argument will open an interactive prompt:

```text
IPv4 Subnet Calculator

Enter IPv4/CIDR or q to quit:
> 192.168.10.42/24
```

The program will print the calculation and then accept another input.

### Direct command-line mode

A subnet can also be passed directly for scripting and repeatable tests:

```text
<program> 192.168.10.42/24
```

All implementations will also support:

```text
--help
-h
```

---

## Planned Output

For a valid input, each implementation will print:

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

The field names, ordering and special-case behavior will remain consistent across Java, C++ and Python.

---

## Validation Scope

The calculator will reject invalid input such as:

- a missing CIDR separator
- more than one `/` separator
- missing or additional IPv4 octets
- empty IPv4 octets
- non-decimal IPv4 octets
- octets outside `0-255`
- leading zeros in multi-digit octets
- a missing or non-decimal CIDR prefix
- prefixes outside `0-32`

Interactive mode will report the error and continue. Direct command-line mode will report the error and return a non-zero exit code.

The normative behavior is documented in [docs/behavior-specification.md](docs/behavior-specification.md).

---

## Repository Structure

```text
ipv4-subnet-calculator-multilang/
├── README.md
├── LICENSE
├── CONTRIBUTING.md
├── .editorconfig
├── .gitattributes
├── .gitignore
├── java/
│   └── README.md
├── cpp/
│   └── README.md
├── python/
│   └── README.md
├── docs/
│   ├── _config.yml
│   ├── index.md
│   ├── behavior-specification.md
│   ├── implementation-comparison.md
│   └── assets/css/style.scss
└── .github/
    └── pull_request_template.md
```

Source code, tests and continuous integration will be added in subsequent phases.

---

## Project Phases

| Phase | Scope | Status |
|---|---|---|
| 1 | Repository foundation, shared specification and project page | Complete |
| 2 | Java and C++ consolidation with interactive mode | Planned |
| 3 | Python implementation | Planned |
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

## Current Status

Phase 1 establishes the shared behavior, documentation structure, contribution workflow and GitHub Pages foundation. No calculator implementation is claimed as complete in this repository yet.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
