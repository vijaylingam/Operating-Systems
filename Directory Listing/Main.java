import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.lang.String;
import java.util.stream.Stream;
import java.io.FileNotFoundException;
import java.nio.file.*;
import java.util.*;
import java.util.stream.StreamSupport;
import java.util.concurrent.*;

public class Main {


    private static void listRecursive(File dir) throws IOException {
        Stream<Path> files = Files.list(dir.toPath()).parallel();
        files.forEach(System.out::println);
    }

    public static void main(String[] args) throws IOException {
        long starttime1 = System.currentTimeMillis();
        String filepath = "/Users/vijaychandra/Desktop/os/";
        File dir = new File(filepath);
        System.out.println(dir);
        listRecursive(dir);
    }
}
