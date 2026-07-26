---
title: Behavior Specification
description: Shared input, validation, calculation and output rules for all implementations
---

# Behavior Specification

This document defines the common observable behavior for the Java, C++ and Python implementations.

The specification is normative for the first project version. Language-specific implementation details may differ, but user-visible behavior should remain equivalent.

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

Leading and trailing whitespace around the complete input may be ignored.

Whitespace inside the IPv4 address or CIDR prefix is invalid.

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

The session ends when the user enters:

```text
q
```

The comparison should be case-insensitive, so `q` and `Q` both quit.

An end-of-file condition also ends the session normally.

An empty line should not be treated as a subnet. It should display a short validation message and return to the prompt.

### 3.2 Direct command-line mode

When exactly one subnet argument is provided, the program performs one calculation and exits.

A successful calculation returns exit code `0`.

Invalid input or incorrect usage returns exit code `1`.

### 3.3 Help mode

The following options display usage information and return exit code `0`:

```text
-h
--help
```

## 4. IPv4 validation

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
192.168.001.10
192.168.a.10
```

## 5. CIDR validation

The IPv4 address and prefix must be separated by exactly one `/` character.

The prefix must:

- contain at least one digit
- contain only ASCII decimal digits `0-9`
- represent an integer from `0` through `32`

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
/33
/abc
```

## 6. Calculated values

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

The calculations use 32-bit IPv4 values.

## 7. Standard subnet behavior

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

## 8. `/31` behavior

For `/31`:

- total addresses equal `2`
- both addresses are treated as usable for point-to-point links
- the first usable host equals the network address
- the last usable host equals the broadcast address

The note is:

```text
/31 subnet: both addresses are usable for point-to-point links.
```

## 9. `/32` behavior

For `/32`:

- total addresses equal `1`
- usable hosts equal `1`
- network and broadcast addresses equal the input address
- the first and last usable hosts equal the input address

The note is:

```text
/32 host route: single usable address.
```

## 10. Output format

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

The labels should be aligned for readable terminal output. Minor spacing differences caused by a platform are acceptable, but field names, values and order must remain consistent.

## 11. Error behavior

Errors should identify the validation category when practical.

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
Error: CIDR prefix out of range (0-32): 33
```

The exact punctuation may differ slightly between languages, but equivalent invalid inputs should be rejected for the same reason.

## 12. Implementation constraints

Each implementation should:

- separate calculation logic from terminal interaction
- return a structured calculation result
- avoid external runtime dependencies
- keep comments limited to non-obvious behavior
- use English identifiers, comments and output
- remain small enough to explain without framework-specific knowledge

## 13. Initial shared test cases

### Valid cases

| Input | Purpose |
|---|---|
| `192.168.10.42/24` | standard private subnet |
| `10.0.0.5/30` | small standard subnet |
| `172.16.1.1/16` | larger private subnet |
| `0.0.0.0/0` | complete IPv4 address space |
| `192.0.2.10/31` | point-to-point special case |
| `203.0.113.15/32` | single-host special case |

### Invalid cases

| Input | Expected category |
|---|---|
| `192.168.1.10` | missing CIDR separator |
| `192.168.1.10/24/1` | multiple separators |
| `192.168.1.10/` | empty prefix |
| `192.168.1.10/33` | prefix out of range |
| `192.168.1.300/24` | octet out of range |
| `192.168.001.10/24` | leading zero |
| `192.168.1/24` | incorrect octet count |
| `192..1.10/24` | empty octet |
| `abc/24` | invalid IPv4 address |
