# Shared Contract and Parity Tests

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

The current contract contains seven valid and twenty-three invalid cases. It covers:

- standard `/24`, `/30` and `/16` subnets
- the complete `/0` IPv4 address space
- `/31` point-to-point behavior
- `/32` host-route behavior
- the explicit six-character ASCII outer-whitespace rule
- malformed separators and octet counts
- empty, non-decimal, signed and out-of-range values
- oversized numeric octets and prefixes
- leading zeros in IPv4 octets and CIDR prefixes
- whitespace inside the token
- combined-error cases that verify validation priority

The combined-error cases confirm that an empty prefix is reported before detailed IPv4 validation after the input has been split successfully.

`cross_language_parity.py` supplements the shared table with process-boundary probes that are awkward to represent safely in TSV, including:

- ASCII control characters that are not whitespace
- non-breaking and ideographic spaces
- allowed ASCII tab and carriage-return trimming
- interactive quit handling
- direct comparison of complete `stdout`, `stderr` and exit codes

The parity runner expects Java classes under `java/out` and the C++ executable under `cpp/build`. It uses the active Python interpreter for the Python implementation.

Language-specific runners intentionally use only their standard libraries. Separate CLI smoke tests verify valid direct mode, invalid direct mode, help mode and incorrect usage.
