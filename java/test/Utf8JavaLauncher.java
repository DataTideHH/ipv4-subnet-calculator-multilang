import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;

public class Utf8JavaLauncher {
    public static void main(String[] args) {
        System.setOut(new PrintStream(
                new FileOutputStream(FileDescriptor.out),
                true,
                StandardCharsets.UTF_8));
        System.setErr(new PrintStream(
                new FileOutputStream(FileDescriptor.err),
                true,
                StandardCharsets.UTF_8));

        SubnetCalculator.main(args);
    }
}
