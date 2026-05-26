import java.util.Scanner;
public class GoBackN {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        //Inputs
        System.out.print("Enter total number of packets: ");
        int totalPackets = sc.nextInt();
        System.out.print("Enter Window Size: ");
        int windowSize = sc.nextInt();
        System.out.print("How many frames will be lost? ");
        int numLost = sc.nextInt();
        int[] lostFrames = new int[numLost];
        for (int i = 0; i < numLost; i++) {
            System.out.print("Enter sequence number of lost frame " + (i + 1) + ": ");
            lostFrames[i] = sc.nextInt();
        }
        int sent = 0;
        int ackReceived = 0;
        System.out.println("\nTransmission Started");
        while (ackReceived < totalPackets) {
            for (int i = sent; i < (ackReceived + windowSize) && i < totalPackets; i++) {
                System.out.println("Sender: Sending Packet " + i);
                sent++;
            }
            boolean isLost = false;
            for (int j = 0; j < numLost; j++) {
                if (lostFrames[j] == ackReceived) {
                    isLost = true;
                    lostFrames[j] = -1; 
                    break;
                }
            }
            if (isLost) {
                System.out.println("!!! Frame " + ackReceived + " was LOST/CORRUPTED.");
                System.out.println("Sender: Timeout detected. Going back to Packet " + ackReceived);
                sent = ackReceived; 
            } else {
                System.out.println("Receiver: Packet " + ackReceived + " received. Sending ACK.");
                ackReceived++;
            }
            System.out.println("\n");
        }
        System.out.println("SUCCESS: All " + totalPackets + " packets acknowledged.");
        sc.close();
    }
}