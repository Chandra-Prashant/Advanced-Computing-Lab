#include <iostream>
#include <vector>
using namespace std;
int main() {
    int totalPackets, windowSize, numLost;
    cout << "Enter total number of packets: ";
    cin >> totalPackets;
    cout << "Enter window size: ";
    cin >> windowSize;
    cout << "How many frames will be lost? ";
    cin >> numLost;
    vector<int> lostFrames(numLost);
    for (int i = 0; i < numLost; i++) {
        cout << "Enter sequence number of lost frame " << i + 1 << ": ";
        cin >> lostFrames[i];
    }
    vector<bool> received(totalPackets, false);
    int base = 0;
    cout << "\n--- Selective Repeat Simulation Start ---\n";
    while (base < totalPackets) {
        for (int i = base; i < base + windowSize && i < totalPackets; i++) {
            if (!received[i]) {
                cout << "Sender: Sending Frame " << i << endl;
            }
        }
        for (int i = base; i < base + windowSize && i < totalPackets; i++) {
            if (received[i]) continue;
            bool isLost = false;
            for (int j = 0; j < numLost; j++) {
                if (lostFrames[j] == i) {
                    isLost = true;
                    lostFrames[j] = -1;
                    break;
                }
            }
            if (isLost) {
                cout << "Frame " << i << " LOST → Will retransmit later\n";
            } else {
                cout << "Receiver: Frame " << i << " received. ACK " << i << " sent\n";
                received[i] = true;
            }
        }
        while (base < totalPackets && received[base]) {
            base++;
        }
        cout << endl;
    }
    cout << "SUCCESS: All packets received using Selective Repeat.\n";
    return 0;
}