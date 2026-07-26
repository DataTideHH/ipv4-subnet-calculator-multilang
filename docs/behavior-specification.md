---
title: Behavior Specification
description: Shared input, validation, calculation and output rules for all implementations
---

# Behavior Specification

This document defines the common observable behavior for the Java, C++ and Python implementations.

The specification is normative for the current project version. Language-specific implementation details may differ, but user-visible output, error classification and exit behavior must remain equivalent.

## 1. Supported scope

The calculator supports IPv4 addresses with CIDR prefixes from `/0` through `/32`.

The calculator does not support IPv6, dotted-decimal subnet-mask input, wildcard-mask input or multiple subnets in one input string.

## 2. Canonical input format

The accepted input format is:

```text
IPv4/CIDR
```

Examples:

```text
192.168.10.42/24
10.0.0.5/30
0.0.0.0/0
192.0.2.10/31
203.0.113.15/32
```

### 2.1 Outer whitespace

Only these six ASCII characters may be ignored at the beginning or end of the complete input:

| Character | Code point |
|---|---|
| space | U+0020 |
| horizontal tab | U+0009 |
| line feed | U+000A |
| carriage return | U+000D |
| form feed | U+000C |
| vertical tab | U+000B |

No other control character or Unicode whitespace character is trimmed. A non-breaking space (`U+00A0`), ideographic space (`U+3000`) or another non-listed character remains part of the token and is rejected by the ordinary IPv4/CIDR validation rules.

Whitespace inside the IPv4 address or CIDR prefix is invalid.

The same ASCII trimming rule applies to direct input, interactive input and interactive `q` / `Q` quit detection.

## 3. Program modes

### 3.1 Interactive mode

When no subnet argument is provided, the program starts an interactive session.

The session displays:

```text
IPv4 Subnet Calculator

Enter IPv4/CIDR or q to quit:
> 
```

After a successful calculation or validation error, the prompt is displayed again.

The session ends when the ASCII-trimmed input equals `q` or `Q`. Unicode whitespace around `q` is not ignored. An end-of-file condition also ends the session normally.

An empty line is not treated as a subnet. It displays a short validation message and returns to the prompt.

### 3.2 Direct command-line mode

When exactly one subnet argument is provided, the program performs one calculation and exits.

A successful calculation returns exit code `0`. Invalid input or incorrect usage returns exit code `1`.

### 3.3 Help mode

The following options display usage information and return exit code `0`:

```text
-h
--help
```

## 4. Structural validation and error priority

After outer ASCII whitespace is removed, validation proceeds in this order:

1. reject empty input
2. require exactly one `/` separator
3. reject an empty IPv4 component
4. reject an empty CIDR-prefix component
5. validate the IPv4 component in detail
6. validate the CIDR-prefix component in detail

This order is observable and therefore part of the contract. When multiple defects occur in one input, the earliest rule determines the error.

Examples:

```text
192.168.1.999/   -> CIDR prefix is empty.
1.2.3/           -> CIDR prefix is empty.
1.2.3.a/         -> CIDR prefix is empty.
192.168.001.1/   -> CIDR prefix is empty.
```

The empty prefix is reported before detailed IPv4 validation in all four examples.

## 5. IPv4 validation

A valid IPv4 address contains exactly four decimal octets separated by dots.

Each octet must:

- contain at least one digit
- contain only ASCII decimal digits `0-9`
- represent a value from `0` through `255`
- not contain a leading zero when it has more than one digit

Valid examples:

```text
0.0.0.0
10.0.0.5
192.168.10.42
255.255.255.255
```

Invalid examples:

```text
192.168.1
192.168.1.10.5
192..1.10
192.168.1.300
192.168.1.999999999999999999999
192.168.001.10
192.168.a.10
```

Digit-only octets are compared with the upper bound as decimal text before conversion. This prevents fixed-width parser overflow and ensures that Java, C++ and Python classify arbitrarily long numeric octets consistently as out of range.

## 6. CIDR validation

The IPv4 address and prefix must be separated by exactly one `/` character.

The prefix must:

- contain at least one digit
- contain only ASCII decimal digits `0-9`
- represent an integer from `0` through `32`
- not contain a leading zero when it has more than one digit

Valid examples:

```text
/0
/8
/24
/31
/32
```

Invalid examples:

```text
/
/-1
/24.0
/00
/08
/0032
/33
/999999999999999999999
/abc
```

A single `/0` is valid. Multi-digit forms such as `/00`, `/08` and `/0032` are rejected rather than normalized.

Digit-only prefixes are compared with the upper bound as decimal text before conversion. Oversized values therefore produce the same out-of-range category in every implementation.

## 7. Calculated values

For valid input, the calculator returns:

- input IPv4 address
- CIDR prefix
- subnet mask
- wildcard mask
- network address
- broadcast address
- total address count
- usable host count
- first usable host
- last usable host
- explanatory note

The calculations use 32-bit IPv4 values. Address counts use a representation capable of storing `2^32` so the complete `/0` space can be represented without overflow.

The `/0` result is a mathematical calculation over the complete 32-bit IPv4 address space. It does not claim that all reported addresses are globally assignable to hosts.

## 8. Standard subnet behavior

For prefixes `/0` through `/30`:

- the network address is not counted as a usable host
- the broadcast address is not counted as a usable host
- usable hosts equal total addresses minus two
- the first usable host is the network address plus one
- the last usable host is the broadcast address minus one

The note is:

```text
Standard subnet with network and broadcast addresses excluded.
```

## 9. `/31` behavior

For `/31`:

- total addresses equal `2`
- both addresses are treated as usable for point-to-point links
- the first usable host equals the network address
- the last usable host equals the broadcast address

The note is:

```text
/31 subnet: both addresses are usable for point-to-point links.
```

This behavior follows RFC 3021 for point-to-point links.

## 10. `/32` behavior

For `/32`:

- total addresses equal `1`
- usable hosts equal `1`
- network and broadcast addresses equal the input address
- the first and last usable hosts equal the input address

The note is:

```text
/32 host route: single usable address.
```

## 11. Output format

The field order is fixed:

```text
Input IP:
CIDR prefix:
Subnet mask:
Wildcard mask:
Network address:
Broadcast address:
Total addresses:
Usable hosts:
First usable host:
Last usable host:
Note:
```

The labels should be aligned for readable terminal output. Field names, values and order must remain consistent.

## 12. Error behavior

Errors identify the validation category. Shared contract cases verify exact messages, while the cross-language parity suite compares complete `stdout`, `stderr` and exit codes.

Examples include:

```text
Error: Missing '/' separator.
Error: Input must contain exactly one '/'.
Error: IPv4 must have exactly four octets.
Error: IPv4 octet is empty.
Error: IPv4 octet must contain only digits.
Error: IPv4 octet out of range (0-255): 300
Error: IPv4 octet must not have leading zeros: 001
Error: CIDR prefix is empty.
Error: CIDR prefix must contain only digits.
Error: CIDR prefix must not have leading zeros: 0032
Error: CIDR prefix out of range (0-32): 33
```

## 13. Implementation constraints

Each implementation should:

- separate calculation logic from terminal interaction
- return a structured calculation result
- avoid external runtime dependencies
- use the explicit six-character ASCII trim rule
- use ASCII-only digit validation
- preserve the shared validation order and categories
- keep comments limited to non-obvious behavior
- use English identifiers, comments and output
- remain small enough to explain without framework-specific knowledge

## 14. Automated verification

The executable data in [`tests/cases.tsv`](../tests/cases.tsv) contains seven valid and twenty-three invalid contract cases.

The invalid cases include combined-error inputs that verify validation priority and three leading-zero prefix forms. The separate [`tests/cross_language_parity.py`](../tests/cross_language_parity.py) suite adds control-character, Unicode-whitespace, direct-mode and interactive-mode probes and compares all three implementations at the process boundary.

## 15. Standards and learning references

- [RFC 3021: Using 31-Bit Prefixes on IPv4 Point-to-Point Links](https://www.rfc-editor.org/rfc/rfc3021.html)
- [RFC 4632: Classless Inter-domain Routing (CIDR)](https://www.rfc-editor.org/rfc/rfc4632.html)
- [Core Internet Standards and RFC Editor](https://github.com/DataTideHH/open-learning-resources/tree/main/resources/networking/core-internet-standards-rfc-editor) in `open-learning-resources`

The project remains a learning implementation rather than a replacement for a mature IP-address library.
