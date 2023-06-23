package ynu.edu;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Logger {
    private BufferedWriter writer;

    public Logger(String filename) throws IOException {
        writer = new BufferedWriter(new FileWriter(filename));
        writer.write("iter,tourLength,time\n");
    }

    public void log(int iter, double tourLength, long time) throws IOException {
        writer.write(String.format("%d,%f,%d\n", iter, tourLength, time));
        writer.flush();
    }

    public void close() throws IOException {
        writer.close();
    }
}