import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) throws Exception {

        Socket socket = new Socket("127.0.0.1", 9090);

        BufferedReader userInput = new BufferedReader(
                new InputStreamReader(System.in));
        BufferedReader in = new BufferedReader(
                new InputStreamReader(socket.getInputStream()));
        PrintWriter out = new PrintWriter(
                socket.getOutputStream(), true);

        String word;

        while (true) {
            System.out.print("Enter word (type 'exit' to quit): ");
            word = userInput.readLine();

            out.println(word);

            if (word.equalsIgnoreCase("exit")) {
                break;
            }

            String response = in.readLine();
            System.out.println("Definition: " + response);
        }

        socket.close();
    }
}