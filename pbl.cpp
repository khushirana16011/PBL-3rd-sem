#include <iostream>
#include <string>
using namespace std;
// 1. USER CLASS
class User {
protected:
    int userId;
    string name;

public:
    User(int id, string n) {
        userId = id;
        name = n;
    }

    virtual void displayDetails() {
        cout << "User ID: " << userId << endl;
        cout << "Name: " << name << endl;
    }

    virtual ~User() {}
};
// 2. CARRIER CLASS
class Carrier : public User {
private:
    string location;
    bool available;
    int currentDeliveryId;

public:
    Carrier(int id, string n, string loc):User(id, n)
     {
        location = loc;
        available = true;
        currentDeliveryId = -1;
    }

    int getId() {
        return userId;
    }

    string getLocation() {
        return location;
    }

    bool isAvailable() {
        return available;
    }

    int getCurrentDeliveryId() {
        return currentDeliveryId;
    }

    void assignDelivery(int deliveryId) {
        available = false;
        currentDeliveryId = deliveryId;
    }

    void acceptDelivery() {
        cout << "Delivery accepted by " << name << endl;
    }

    void rejectDelivery() {
        available = true;
        currentDeliveryId = -1;
    }

    void completeDelivery() {
        available = true;
        currentDeliveryId = -1;
    }

    void displayDetails() override {
        cout << "\nCarrier ID: " << userId << endl;
        cout << "Name: " << name << endl;
        cout << "Location: " << location << endl;
        cout << "Availability: "
             << (available ? "Available" : "Busy") << endl;
    }
};
// 3. DELIVERY CLASS
class Delivery {
private:
    int deliveryId;
    int priority;
    string type;
    string item;
    string pickup;
    string destination;
    string status;

public:
    Delivery(int id, int p, string t, string itemName,
             string source, string dest) {
        deliveryId = id;
        priority = p;
        type = t;
        item = itemName;
        pickup = source;
        destination = dest;
        status = "Pending";
    }

    int getId() {
        return deliveryId;
    }

    int getPriority() {
        return priority;
    }

    string getPickup() {
        return pickup;
    }

    string getDestination() {
        return destination;
    }

    string getStatus() {
        return status;
    }

    string getType() {
        return type;
    }

    string getItem() {
        return item;
    }

    void setStatus(string s) {
        status = s;
    }

    void displayDetails() {
        cout << "\nDelivery ID: " << deliveryId << endl;
        cout << "Type: " << type << endl;
        cout << "Item: " << item << endl;
        cout << "Pickup: " << pickup << endl;
        cout << "Destination: " << destination << endl;
        cout << "Priority: " << priority << endl;
        cout << "Status: " << status << endl;
    }
};
// 4. PRIORITY QUEUE USING MAX-HEAP
class PriorityQueue {
private:
    struct Request {
        Delivery* delivery;
        int arrivalOrder;
    };

    Request heap[100];
    int size;
    int counter;

    bool higherPriority(Request a, Request b) {
        if (a.delivery->getPriority() !=
            b.delivery->getPriority()) {
            return a.delivery->getPriority() >
                   b.delivery->getPriority();
        }

        // Earlier request gets preference for equal priority.
        return a.arrivalOrder < b.arrivalOrder;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (higherPriority(heap[parent], heap[index]))
                break;

            Request temp = heap[parent];
            heap[parent] = heap[index];
            heap[index] = temp;

            index = parent;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int largest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size &&
                higherPriority(heap[left], heap[largest])) {
                largest = left;
            }

            if (right < size &&
                higherPriority(heap[right], heap[largest])) {
                largest = right;
            }

            if (largest == index)
                break;

            Request temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;

            index = largest;
        }
    }

public:
    PriorityQueue() {
        size = 0;
        counter = 0;
    }

    bool isEmpty() {
        return size == 0;
    }

    bool enqueue(Delivery* d) {
        if (size >= 100) {
            cout << "Priority Queue is full!" << endl;
            return false;
        }

        Request newRequest;
        newRequest.delivery = d;
        newRequest.arrivalOrder = counter++;

        heap[size] = newRequest;
        heapifyUp(size);
        size++;

        return true;
    }

    Delivery* dequeue() {
        if (size == 0)
            return nullptr;

        Delivery* result = heap[0].delivery;

        heap[0] = heap[size - 1];
        size--;

        if (size > 0)
            heapifyDown(0);

        return result;
    }

    void displayQueue() {
        cout << "\n--- Pending Deliveries ---" << endl;

        if (size == 0) {
            cout << "No pending deliveries." << endl;
            return;
        }

        for (int i = 0; i < size; i++) {
            cout << "Delivery ID: "
                 << heap[i].delivery->getId()
                 << " | Type: "
                 << heap[i].delivery->getType()
                 << " | Priority: "
                 << heap[i].delivery->getPriority()
                 << endl;
        }
    }
};
// 5. LINKED LIST FOR AVAILABLE CARRIERS

class CarrierList {
private:
    struct Node {
        Carrier* carrier;
        Node* next;
    };

    Node* head;

public:
    CarrierList() {
        head = nullptr;
    }

    ~CarrierList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insertCarrier(Carrier* c) {
        if (findCarrier(c->getId()) != nullptr)
            return;

        Node* newNode = new Node;
        newNode->carrier = c;
        newNode->next = head;
        head = newNode;
    }

    Carrier* findCarrier(int id) {
        Node* current = head;

        while (current != nullptr) {
            if (current->carrier->getId() == id)
                return current->carrier;

            current = current->next;
        }

        return nullptr;
    }

    Carrier* removeCarrier() {
        if (head == nullptr)
            return nullptr;

        Node* temp = head;
        Carrier* c = temp->carrier;

        head = head->next;
        delete temp;

        return c;
    }

    void displayCarriers() {
        cout << "\n--- Available Carriers ---" << endl;

        if (head == nullptr) {
            cout << "No carriers available." << endl;
            return;
        }

        Node* current = head;

        while (current != nullptr) {
            current->carrier->displayDetails();
            current = current->next;
        }
    }
};

// 6. MAIN FUNCTION

int main() {
    // Data structures
    PriorityQueue pq;
    CarrierList availableCarriers;

    // Store all deliveries for later viewing.
    Delivery* deliveries[100];
    int deliveryCount = 0;
    int nextDeliveryId = 1001;

    // Register sample carriers.
    Carrier c1(101, "Aman", "College Gate");
    Carrier c2(102, "Riya", "Main Market");
    Carrier c3(103, "Rahul", "Library");

    availableCarriers.insertCarrier(&c1);
    availableCarriers.insertCarrier(&c2);
    availableCarriers.insertCarrier(&c3);

    int choice;

    do {
        cout << "\n========== SMART DELIVERY SYSTEM ==========";
        cout << "\n1. Module 1 - Store Ordering";
        cout << "\n2. Module 2 - Send Something";
        cout << "\n3. Display Pending Queue";
        cout << "\n4. Display Available Carriers";
        cout << "\n5. Display All Deliveries";
        cout << "\n0. Exit";
        cout << "\nEnter choice: ";

        cin >> choice;
        cin.ignore(1000, '\n');

        // MODULE 1: STORE ORDERING
        if (choice == 1) {
            if (deliveryCount >= 100) {
                cout << "Delivery storage is full." << endl;
                continue;
            }

            string product, store, destination;
            int priority;

            cout << "\n--- Store Ordering ---" << endl;

            cout << "Enter product name: ";
            getline(cin, product);

            cout << "Enter store name: ";
            getline(cin, store);

            cout << "Enter destination: ";
            getline(cin, destination);

            cout << "Enter priority (1-3): ";
            cin >> priority;
            cin.ignore(1000, '\n');

            if (priority < 1 || priority > 3) {
                cout << "Priority must be 1, 2 or 3." << endl;
                continue;
            }

            Delivery* d = new Delivery(
                nextDeliveryId, priority, "Store Order",
                product, store, destination
            );

            if (pq.enqueue(d)) {
                deliveries[deliveryCount++] = d;

                cout << "Store order added to the shared queue."
                     << endl;
                cout << "Delivery ID: " << nextDeliveryId << endl;

                nextDeliveryId++;
            } else {
                delete d;
            }
        }

        // MODULE 2: SEND SOMETHING
        else if (choice == 2) {
            if (deliveryCount >= 100) {
                cout << "Delivery storage is full." << endl;
                continue;
            }

            string item, pickup, destination;
            int priority;

            cout << "\n--- Send Something ---" << endl;

            cout << "Enter item to send: ";
            getline(cin, item);

            cout << "Enter pickup location: ";
            getline(cin, pickup);

            cout << "Enter destination: ";
            getline(cin, destination);

            cout << "Enter priority (1-3): ";
            cin >> priority;
            cin.ignore(1000, '\n');

            if (priority < 1 || priority > 3) {
                cout << "Priority must be 1, 2 or 3." << endl;
                continue;
            }

            Delivery* d = new Delivery(
                nextDeliveryId, priority, "Package Delivery",
                item, pickup, destination
            );

            if (pq.enqueue(d)) {
                deliveries[deliveryCount++] = d;

                cout << "Package request added to the shared queue."
                     << endl;
                cout << "Delivery ID: " << nextDeliveryId << endl;

                nextDeliveryId++;
            } else {
                delete d;
            }
        }

        // DISPLAY PENDING QUEUE
        else if (choice == 3) {
            pq.displayQueue();
        }

        // DISPLAY AVAILABLE CARRIERS
        else if (choice == 4) {
            availableCarriers.displayCarriers();
        }

        // DISPLAY ALL DELIVERIES
        else if (choice == 5) {
            cout << "\n--- All Deliveries ---" << endl;

            if (deliveryCount == 0) {
                cout << "No deliveries created yet." << endl;
            } else {
                for (int i = 0; i < deliveryCount; i++) {
                    deliveries[i]->displayDetails();
                }
            }
        }

        else if (choice != 0) {
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 0);

    // Free allocated delivery objects.
    for (int i = 0; i < deliveryCount; i++) {
        delete deliveries[i];
    }

    return 0;
}