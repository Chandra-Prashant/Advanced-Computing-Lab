import java.io.*;
import java.net.*;
import java.util.*;

public class Server {

    static Map<String, String> dict = new HashMap<>();

    public static void main(String[] args) throws Exception {

        // Load dictionary
        BufferedReader file = new BufferedReader(new FileReader("dict.txt"));
        String line;
        while ((line = file.readLine()) != null) {
            String[] parts = line.split(":", 2);
            if (parts.length == 2) {
                dict.put(parts[0].toLowerCase(), parts[1]);
            }
        }
        file.close();

        ServerSocket server = new ServerSocket(9090);
        System.out.println("Server running on port 9090...");

        while (true) {
            Socket client = server.accept();
            new ClientHandler(client).start();  // multithread
        }
    }
}

// Thread class
class ClientHandler extends Thread {
    private Socket socket;

    ClientHandler(Socket socket) {
        this.socket = socket;
    }

    public void run() {
        try {
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(
                    socket.getOutputStream(), true);

            String word;

            while (true) {
                word = in.readLine();

                if (word == null || word.equalsIgnoreCase("exit")) {
                    break;  // client exits
                }

                word = word.toLowerCase();

                String response = Server.dict.getOrDefault(word, "Word not found");
                out.println(response);
            }

            socket.close();
            System.out.println("Client disconnected");

        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}