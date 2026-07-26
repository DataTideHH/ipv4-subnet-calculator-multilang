import java.util.Scanner;

public class SubnetCalculator {
    private record Calculation(
            String inputIp,
            int prefix,
            String subnetMask,
            String wildcardMask,
            String networkAddress,
            String broadcastAddress,
            long totalAddresses,
            long usableHosts,
            String firstUsableHost,
            String lastUsableHost,
            String note
    ) {
    }

    public static void main(String[] args) {
        int exitCode = run(args);

        if (exitCode != 0) {
            System.exit(exitCode);
        }
    }

    private static int run(String[] args) {
        if (args.length == 0) {
            runInteractiveMode();
            return 0;
        }

        if (args.length != 1) {
            System.err.println("Error: Expected zero or one argument.");
            printUsage();
            return 1;
        }

        if (args[0].equals("-h") || args[0].equals("--help")) {
            printUsage();
            return 0;
        }

        return processInput(args[0], true);
    }

    private static void runInteractiveMode() {
        System.out.println("IPv4 Subnet Calculator");
        System.out.println();

        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("Enter IPv4/CIDR or q to quit:");
            System.out.print("> ");

            if (!scanner.hasNextLine()) {
                System.out.println();
                return;
            }

            String input = scanner.nextLine().trim();

            if (input.equalsIgnoreCase("q")) {
                return;
            }

            processInput(input, false);
            System.out.println();
        }
    }

    private static int processInput(String input, boolean showFormatHint) {
        try {
            Calculation result = calculate(input);
            printResult(result);
            return 0;
        } catch (IllegalArgumentException error) {
            System.err.println("Error: " + error.getMessage());

            if (showFormatHint) {
                System.err.println(
                        "Expected format: IPv4/CIDR, for example 192.168.10.42/24");
            }

            return 1;
        }
    }

    private static void printUsage() {
        System.out.println("IPv4 Subnet Calculator");
        System.out.println();
        System.out.println("Usage:");
        System.out.println("  java -cp out SubnetCalculator");
        System.out.println("  java -cp out SubnetCalculator <IPv4/CIDR>");
        System.out.println("  java -cp out SubnetCalculator --help");
        System.out.println();
        System.out.println("Without an argument, the calculator starts in interactive mode.");
        System.out.println();
        System.out.println("Examples:");
        System.out.println("  java -cp out SubnetCalculator 192.168.10.42/24");
        System.out.println("  java -cp out SubnetCalculator 10.0.0.5/30");
        System.out.println("  java -cp out SubnetCalculator 203.0.113.15/32");
    }

    private static void printResult(Calculation result) {
        System.out.println("Input IP:          " + result.inputIp());
        System.out.println("CIDR prefix:       /" + result.prefix());
        System.out.println("Subnet mask:       " + result.subnetMask());
        System.out.println("Wildcard mask:     " + result.wildcardMask());
        System.out.println("Network address:   " + result.networkAddress());
        System.out.println("Broadcast address: " + result.broadcastAddress());
        System.out.println("Total addresses:   " + result.totalAddresses());
        System.out.println("Usable hosts:      " + result.usableHosts());
        System.out.println("First usable host: " + result.firstUsableHost());
        System.out.println("Last usable host:  " + result.lastUsableHost());
        System.out.println("Note:              " + result.note());
    }

    private static Calculation calculate(String cidr) {
        if (cidr == null) {
            throw new IllegalArgumentException("Input is missing.");
        }

        String trimmed = cidr.trim();

        if (trimmed.isEmpty()) {
            throw new IllegalArgumentException("Input is empty.");
        }

        int firstSlash = trimmed.indexOf('/');
        int lastSlash = trimmed.lastIndexOf('/');

        if (firstSlash < 0) {
            throw new IllegalArgumentException("Missing '/' separator.");
        }

        if (firstSlash != lastSlash) {
            throw new IllegalArgumentException("Input must contain exactly one '/'.");
        }

        String ipText = trimmed.substring(0, firstSlash);
        String prefixText = trimmed.substring(firstSlash + 1);

        if (ipText.isEmpty()) {
            throw new IllegalArgumentException("IPv4 address is empty.");
        }

        if (prefixText.isEmpty()) {
            throw new IllegalArgumentException("CIDR prefix is empty.");
        }

        long ip = parseIpv4(ipText);
        int prefix = parsePrefix(prefixText);
        long mask = prefixToMask(prefix);
        long wildcard = (~mask) & 0xFFFFFFFFL;
        long network = ip & mask;
        long broadcast = network | wildcard;

        // A long is required for /0 because the IPv4 address space contains 2^32 values.
        long totalAddresses = 1L << (32 - prefix);

        long usableHosts;
        long firstUsable;
        long lastUsable;
        String note;

        if (prefix <= 30) {
            usableHosts = totalAddresses - 2;
            firstUsable = network + 1;
            lastUsable = broadcast - 1;
            note = "Standard subnet with network and broadcast addresses excluded.";
        } else if (prefix == 31) {
            usableHosts = 2;
            firstUsable = network;
            lastUsable = broadcast;
            note = "/31 subnet: both addresses are usable for point-to-point links.";
        } else {
            usableHosts = 1;
            firstUsable = network;
            lastUsable = network;
            note = "/32 host route: single usable address.";
        }

        return new Calculation(
                ipText,
                prefix,
                toIpv4(mask),
                toIpv4(wildcard),
                toIpv4(network),
                toIpv4(broadcast),
                totalAddresses,
                usableHosts,
                toIpv4(firstUsable),
                toIpv4(lastUsable),
                note
        );
    }

    private static int parsePrefix(String prefixText) {
        if (!isAllDigits(prefixText)) {
            throw new IllegalArgumentException(
                    "CIDR prefix must contain only digits: " + prefixText);
        }

        int prefix;

        try {
            prefix = Integer.parseInt(prefixText);
        } catch (NumberFormatException error) {
            throw new IllegalArgumentException(
                    "CIDR prefix is not a valid number: " + prefixText);
        }

        if (prefix < 0 || prefix > 32) {
            throw new IllegalArgumentException(
                    "CIDR prefix out of range (0-32): " + prefix);
        }

        return prefix;
    }

    private static long parseIpv4(String ipText) {
        String[] parts = ipText.split("\\.", -1);

        if (parts.length != 4) {
            throw new IllegalArgumentException(
                    "IPv4 must have exactly four octets: " + ipText);
        }

        long result = 0;

        for (String part : parts) {
            if (part.isEmpty()) {
                throw new IllegalArgumentException(
                        "IPv4 octet is empty in: " + ipText);
            }

            if (!isAllDigits(part)) {
                throw new IllegalArgumentException(
                        "IPv4 octet must contain only digits: " + part);
            }

            if (part.length() > 1 && part.charAt(0) == '0') {
                throw new IllegalArgumentException(
                        "IPv4 octet must not have leading zeros: " + part);
            }

            int octet;

            try {
                octet = Integer.parseInt(part);
            } catch (NumberFormatException error) {
                throw new IllegalArgumentException(
                        "IPv4 octet is not a valid number: " + part);
            }

            if (octet < 0 || octet > 255) {
                throw new IllegalArgumentException(
                        "IPv4 octet out of range (0-255): " + octet);
            }

            result = (result << 8) | octet;
        }

        return result;
    }

    private static boolean isAllDigits(String text) {
        for (int i = 0; i < text.length(); i++) {
            char character = text.charAt(i);

            if (character < '0' || character > '9') {
                return false;
            }
        }

        return true;
    }

    private static long prefixToMask(int prefix) {
        if (prefix == 0) {
            return 0;
        }

        return (0xFFFFFFFFL << (32 - prefix)) & 0xFFFFFFFFL;
    }

    private static String toIpv4(long value) {
        return ((value >> 24) & 0xFF) + "."
                + ((value >> 16) & 0xFF) + "."
                + ((value >> 8) & 0xFF) + "."
                + (value & 0xFF);
    }
}
