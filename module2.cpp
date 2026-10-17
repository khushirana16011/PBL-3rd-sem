
#include <iostream>
#include <cstring>
using namespace std;

class User {
protected:
    int id;
    char name[30];

public:
    User(int i, const char n[]) {
        id = i;
        strcpy(name, n);
    }
};

class Sender : public User {
public:
    Sender(int i, const char n[]) : User(i, n) {}

    void createRequest() {
        cout << "Sender " << name << " created request.\n";
    }
};

class Receiver : public User {
public:
    Receiver(int i, const char n[]) : User(i, n) {}

    void trackDelivery() {
        cout << "Receiver " << name << " can track delivery.\n";
    }
};

class PackageDelivery {
private:
    int deliveryId;
    char pickup[40], destination[40], item[40];
    char status[30];

public:
    PackageDelivery() {
        deliveryId = 0;
        strcpy(pickup, "");
        strcpy(destination, "");
        strcpy(item, "");
        strcpy(status, "Created");
    }

    void createPackage(int id, const char p[],
                       const char d[], const char it[]) {
        deliveryId = id;
        strcpy(pickup, p);
        strcpy(destination, d);
        strcpy(item, it);
    }

    int getId() {
        return deliveryId;
    }

    void setStatus(const char s[]) {
        strcpy(status, s);
    }

    void display() {
        cout << "\nDelivery ID: " << deliveryId;
        cout << "\nPickup: " << pickup;
        cout << "\nDestination: " << destination;
        cout << "\nItem: " << item;
        cout << "\nStatus: " << status << endl;
    }

    void trackDelivery() {
        cout << "\nTracking delivery:";
        display();
    }
};

class DeliveryQueue {
private:
    PackageDelivery q[50];
    int front, rear;

public:
    DeliveryQueue() {
        front = 0;
        rear = -1;
    }

    void enqueue(PackageDelivery p) {
        if (rear == 49)
            cout << "Queue is full!\n";
        else {
            q[++rear] = p;
            cout << "Request added to queue.\n";
        }
    }

    void displayQueue() {
        if (front > rear) {
            cout << "Queue is empty!\n";
            return;
        }

        for (int i = front; i <= rear; i++)
            q[i].display();
    }

    void updateDelivery(int id) {
        for (int i = front; i <= rear; i++) {
            if (q[i].getId() == id) {
                int ch;

                cout << "\n1. Assigned";
                cout << "\n2. Picked Up";
                cout << "\n3. In Transit";
                cout << "\n4. Delivered";
                cout << "\nEnter choice: ";
                cin >> ch;

                if (ch == 1)
                    q[i].setStatus("Assigned");
                else if (ch == 2)
                    q[i].setStatus("Picked Up");
                else if (ch == 3)
                    q[i].setStatus("In Transit");
                else if (ch == 4)
                    q[i].setStatus("Delivered");
                else {
                    cout << "Invalid choice!\n";
                    return;
                }

                cout << "Status updated!\n";
                return;
            }
        }

        cout << "Delivery ID not found!\n";
    }

    void track(int id) {
        for (int i = front; i <= rear; i++) {
            if (q[i].getId() == id) {
                q[i].trackDelivery();
                return;
            }
        }

        cout << "Delivery not found!\n";
    }

    void processNext() {
        if (front > rear)
            cout << "No pending deliveries!\n";
        else {
            cout << "Processing next delivery:";
            q[front].display();
            front++;
        }
    }
};

int main() {
    DeliveryQueue dq;
    int ch, id, sid, rid;
    char sn[30], rn[30];
    char p[40], d[40], item[40];

    do {
        cout << "\n--- SMART DELIVERY SYSTEM ---";
        cout << "\n1. Create Request";
        cout << "\n2. Display Deliveries";
        cout << "\n3. Update Status";
        cout << "\n4. Track Delivery";
        cout << "\n5. Process Next";
        cout << "\n6. Exit";
        cout << "\nEnter choice: ";
        cin >> ch;

        if (ch == 1) {
            cout << "Delivery ID: ";
            cin >> id;
            cout << "Sender ID and name: ";
            cin >> sid >> sn;
            cout << "Receiver ID and name: ";
            cin >> rid >> rn;
            cout << "Pickup location: ";
            cin >> p;
            cout << "Destination: ";
            cin >> d;
            cout << "Item: ";
            cin >> item;

            Sender s(sid, sn);
            Receiver r(rid, rn);
            PackageDelivery obj;

            s.createRequest();
            obj.createPackage(id, p, d, item);
            dq.enqueue(obj);
            r.trackDelivery();
        }
        else if (ch == 2)
            dq.displayQueue();
        else if (ch == 3) {
            cout << "Delivery ID: ";
            cin >> id;
            dq.updateDelivery(id);
        }
        else if (ch == 4) {
            cout << "Delivery ID: ";
            cin >> id;
            dq.track(id);
        }
        else if (ch == 5)
            dq.processNext();
        else if (ch == 6)
            cout << "Exiting system.\n";
        else
            cout << "Invalid choice!\n";

    } while (ch != 6);

    return 0;
}

