import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class SubnetCalculatorTest {
    private record ProcessResult(int exitCode, String output) {
    }

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            throw new IllegalArgumentException("Expected path to tests/cases.tsv.");
        }

        List<String> lines = Files.readAllLines(Path.of(args[0]), StandardCharsets.UTF_8);
        int executed = 0;

        for (String line : lines) {
            if (line.isBlank() || line.startsWith("#")) {
                continue;
            }

            String[] columns = line.split("\t", -1);
            if (columns.length != 12) {
                throw new AssertionError("Expected 12 columns, found " + columns.length + ": " + line);
            }

            if (columns[0].equals("valid")) {
                verifyValid(columns);
            } else if (columns[0].equals("invalid")) {
                verifyInvalid(columns);
            } else {
                throw new AssertionError("Unknown case kind: " + columns[0]);
            }

            executed++;
        }

        System.out.println("Java contract tests passed: " + executed);
    }

    private static void verifyValid(String[] testCase) throws Exception {
        String name = testCase[1];
        String input = testCase[2];
        ProcessResult result = runCalculator(input);

        assertEquals(name + " exit code", 0, result.exitCode());

        String trimmedInput = input.trim();
        int separator = trimmedInput.indexOf('/');
        String inputIp = trimmedInput.substring(0, separator);
        String prefix = trimmedInput.substring(separator);

        Map<String, String> expectedFields = new LinkedHashMap<>();
        expectedFields.put("Input IP", inputIp);
        expectedFields.put("CIDR prefix", prefix);
        expectedFields.put("Subnet mask", testCase[3]);
        expectedFields.put("Wildcard mask", testCase[4]);
        expectedFields.put("Network address", testCase[5]);
        expectedFields.put("Broadcast address", testCase[6]);
        expectedFields.put("Total addresses", testCase[7]);
        expectedFields.put("Usable hosts", testCase[8]);
        expectedFields.put("First usable host", testCase[9]);
        expectedFields.put("Last usable host", testCase[10]);
        expectedFields.put("Note", testCase[11]);

        Map<String, String> actualFields = parseFields(result.output());
        assertEquals(name + " fields", expectedFields, actualFields);
    }

    private static void verifyInvalid(String[] testCase) throws Exception {
        String name = testCase[1];
        ProcessResult result = runCalculator(testCase[2]);

        assertEquals(name + " exit code", 1, result.exitCode());
        assertContains(name + " error", result.output(), "Error: " + testCase[11]);
    }

    private static ProcessResult runCalculator(String input) throws IOException, InterruptedException {
        String executable = Path.of(
                System.getProperty("java.home"),
                "bin",
                System.getProperty("os.name").toLowerCase().contains("win") ? "java.exe" : "java"
        ).toString();

        Process process = new ProcessBuilder(
                executable,
                "-cp",
                System.getProperty("java.class.path"),
                "SubnetCalculator",
                input
        ).redirectErrorStream(true).start();

        String output = new String(process.getInputStream().readAllBytes(), StandardCharsets.UTF_8);
        int exitCode = process.waitFor();
        return new ProcessResult(exitCode, output);
    }

    private static Map<String, String> parseFields(String output) {
        Map<String, String> fields = new LinkedHashMap<>();

        for (String line : output.lines().toList()) {
            int separator = line.indexOf(':');
            if (separator < 0) {
                continue;
            }

            String label = line.substring(0, separator).trim();
            String value = line.substring(separator + 1).trim();
            fields.put(label, value);
        }

        return fields;
    }

    private static void assertContains(String label, String actual, String expectedFragment) {
        if (!actual.contains(expectedFragment)) {
            throw new AssertionError(label + " expected fragment <" + expectedFragment
                    + "> but output was:\n" + actual);
        }
    }

    private static void assertEquals(String label, Object expected, Object actual) {
        if (!expected.equals(actual)) {
            throw new AssertionError(label + " expected <" + expected + "> but was <" + actual + ">");
        }
    }
}
