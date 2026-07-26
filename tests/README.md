# Shared Contract Cases

`cases.tsv` is the language-neutral source of truth for automated subnet-calculator checks.

Each non-comment row contains twelve tab-separated columns:

1. case kind (`valid` or `invalid`)
2. case name
3. input
4. subnet mask
5. wildcard mask
6. network address
7. broadcast address
8. total address count
9. usable host count
10. first usable host
11. last usable host
12. note or exact validation message

Valid rows verify the complete structured result. Invalid rows verify that all implementations reject the same input for the same reason.

The current contract contains seven valid and sixteen invalid cases. It covers:

- standard `/24`, `/30` and `/16` subnets
- the complete `/0` IPv4 address space
- `/31` point-to-point behavior
- `/32` host-route behavior
- surrounding-whitespace trimming
- malformed separators and octet counts
- empty, non-decimal, signed and out-of-range values
- oversized numeric octets and prefixes that exceed fixed-width parser ranges
- leading zeros
- whitespace inside the token

The oversized numeric cases are important for cross-language consistency. Java and C++ use fixed-width integer types, while Python integers are arbitrary precision. All implementations therefore compare bounded decimal text before conversion and return the same out-of-range category.

Language-specific runners intentionally use only their standard libraries. Separate CLI smoke tests verify valid direct mode, invalid direct mode, help mode and incorrect usage.
