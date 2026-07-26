from dataclasses import dataclass
import sys


@dataclass(frozen=True, slots=True)
class Calculation:
    input_ip: str
    prefix: int
    subnet_mask: str
    wildcard_mask: str
    network_address: str
    broadcast_address: str
    total_addresses: int
    usable_hosts: int
    first_usable_host: str
    last_usable_host: str
    note: str


def main(args: list[str] | None = None) -> int:
    arguments = sys.argv[1:] if args is None else args

    if not arguments:
        run_interactive_mode()
        return 0

    if len(arguments) != 1:
        print("Error: Expected zero or one argument.", file=sys.stderr)
        print_usage()
        return 1

    if arguments[0] in {"-h", "--help"}:
        print_usage()
        return 0

    return process_input(arguments[0], show_format_hint=True)


def run_interactive_mode() -> None:
    print("IPv4 Subnet Calculator")
    print()

    while True:
        print("Enter IPv4/CIDR or q to quit:")
        try:
            user_input = input("> ").strip()
        except EOFError:
            print()
            return

        if user_input.lower() == "q":
            return

        process_input(user_input, show_format_hint=False)
        print()


def process_input(cidr: str, show_format_hint: bool) -> int:
    try:
        result = calculate(cidr)
    except ValueError as error:
        print(f"Error: {error}", file=sys.stderr)

        if show_format_hint:
            print(
                "Expected format: IPv4/CIDR, for example 192.168.10.42/24",
                file=sys.stderr,
            )

        return 1

    print_result(result)
    return 0


def print_usage() -> None:
    print("IPv4 Subnet Calculator")
    print()
    print("Usage:")
    print("  python python/subnet_calculator.py")
    print("  python python/subnet_calculator.py <IPv4/CIDR>")
    print("  python python/subnet_calculator.py --help")
    print()
    print("Without an argument, the calculator starts in interactive mode.")
    print()
    print("Examples:")
    print("  python python/subnet_calculator.py 192.168.10.42/24")
    print("  python python/subnet_calculator.py 10.0.0.5/30")
    print("  python python/subnet_calculator.py 203.0.113.15/32")


def print_result(result: Calculation) -> None:
    print(f"Input IP:          {result.input_ip}")
    print(f"CIDR prefix:       /{result.prefix}")
    print(f"Subnet mask:       {result.subnet_mask}")
    print(f"Wildcard mask:     {result.wildcard_mask}")
    print(f"Network address:   {result.network_address}")
    print(f"Broadcast address: {result.broadcast_address}")
    print(f"Total addresses:   {result.total_addresses}")
    print(f"Usable hosts:      {result.usable_hosts}")
    print(f"First usable host: {result.first_usable_host}")
    print(f"Last usable host:  {result.last_usable_host}")
    print(f"Note:              {result.note}")


def calculate(cidr: str) -> Calculation:
    if cidr is None:
        raise ValueError("Input is missing.")

    trimmed = cidr.strip()

    if not trimmed:
        raise ValueError("Input is empty.")

    slash_count = trimmed.count("/")

    if slash_count == 0:
        raise ValueError("Missing '/' separator.")

    if slash_count != 1:
        raise ValueError("Input must contain exactly one '/'.")

    ip_text, prefix_text = trimmed.split("/", maxsplit=1)

    if not ip_text:
        raise ValueError("IPv4 address is empty.")

    if not prefix_text:
        raise ValueError("CIDR prefix is empty.")

    ip_value = parse_ipv4(ip_text)
    prefix = parse_prefix(prefix_text)
    mask = prefix_to_mask(prefix)
    wildcard = (~mask) & 0xFFFFFFFF
    network = ip_value & mask
    broadcast = network | wildcard
    total_addresses = 1 << (32 - prefix)

    if prefix <= 30:
        usable_hosts = total_addresses - 2
        first_usable = network + 1
        last_usable = broadcast - 1
        note = "Standard subnet with network and broadcast addresses excluded."
    elif prefix == 31:
        usable_hosts = 2
        first_usable = network
        last_usable = broadcast
        note = "/31 subnet: both addresses are usable for point-to-point links."
    else:
        usable_hosts = 1
        first_usable = network
        last_usable = network
        note = "/32 host route: single usable address."

    return Calculation(
        input_ip=ip_text,
        prefix=prefix,
        subnet_mask=to_ipv4(mask),
        wildcard_mask=to_ipv4(wildcard),
        network_address=to_ipv4(network),
        broadcast_address=to_ipv4(broadcast),
        total_addresses=total_addresses,
        usable_hosts=usable_hosts,
        first_usable_host=to_ipv4(first_usable),
        last_usable_host=to_ipv4(last_usable),
        note=note,
    )


def parse_prefix(prefix_text: str) -> int:
    if not is_ascii_digits(prefix_text):
        raise ValueError(f"CIDR prefix must contain only digits: {prefix_text}")

    prefix = int(prefix_text)

    if prefix < 0 or prefix > 32:
        raise ValueError(f"CIDR prefix out of range (0-32): {prefix}")

    return prefix


def parse_ipv4(ip_text: str) -> int:
    parts = ip_text.split(".")

    if len(parts) != 4:
        raise ValueError(f"IPv4 must have exactly four octets: {ip_text}")

    result = 0

    for part in parts:
        if not part:
            raise ValueError(f"IPv4 octet is empty in: {ip_text}")

        if not is_ascii_digits(part):
            raise ValueError(f"IPv4 octet must contain only digits: {part}")

        if len(part) > 1 and part[0] == "0":
            raise ValueError(f"IPv4 octet must not have leading zeros: {part}")

        octet = int(part)

        if octet < 0 or octet > 255:
            raise ValueError(f"IPv4 octet out of range (0-255): {octet}")

        result = (result << 8) | octet

    return result


def is_ascii_digits(text: str) -> bool:
    return bool(text) and all("0" <= character <= "9" for character in text)


def prefix_to_mask(prefix: int) -> int:
    if prefix == 0:
        return 0

    return (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF


def to_ipv4(value: int) -> str:
    return ".".join(str((value >> shift) & 0xFF) for shift in (24, 16, 8, 0))


if __name__ == "__main__":
    raise SystemExit(main())
