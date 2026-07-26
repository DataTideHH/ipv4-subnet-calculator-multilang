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

The shared cases cover:

- standard `/24`, `/30` and `/16` subnets
- the complete `/0` IPv4 address space
- `/31` point-to-point behavior
- `/32` host-route behavior
- surrounding-whitespace trimming
- malformed separators and octet counts
- empty, non-decimal, signed and out-of-range values
- leading zeros
- whitespace inside the token

Language-specific runners intentionally use only their standard libraries.
