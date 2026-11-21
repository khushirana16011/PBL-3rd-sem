
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_DELIVERIES = 100;

// =====================================================
// USER CLASS
// =====================================================

class User {
protected:
    int userID;
    string name;

public:
    User() : userID(0), name("") {}

    User(int id, string n) {
        userID = id;
        name = n;
    }

    virtual void displayDetails() const {
        cout << "User ID: " << userID
             << "\nName: " << name << "\n";
    }

    virtual ~User() {}
};

// =====================================================
// DELIVERY CLASS
// =====================================================

class Delivery {
private:
    int deliveryID;
    int priority;
    string type, item, source, destination, status;
    int carrierID;

public:
    Delivery() {
        deliveryID = 0;
        priority = 1;
        carrierID = -1;
        type = item = source = destination = "";
        status = "Pending";
    }

    Delivery(int id, int p, string t, string it,
             string from, string to) {
        deliveryID = id;
        priority = p;
        type = t;
        item = it;
        source = from;
        destination = to;
        status = "Pending";
        carrierID = -1;
    }

    int getID() const { return deliveryID; }
    int getPriority() const { return priority; }
    int getCarrierID() const { return carrierID; }

    string getSource() const { return source; }
    string getDestination() const { return destination; }
    string getStatus() const { return status; }

    void setStatus(string s) { status = s; }
    void setCarrier(int id) { carrierID = id; }

    void display() const {
        cout << "\nDelivery ID: " << deliveryID;
        cout << "\nType: " << type;
        cout << "\nItem: " << item;
        cout << "\nSource: " << source;
        cout << "\nDestination: " << destination;
        cout << "\nPriority: " << priority;
        cout << "\nStatus: " << status;

        if (carrierID != -1)
            cout << "\nCarrier ID: " << carrierID;

        cout << "\n";
    }

    void saveToFile(ofstream& file) const {
        file << "Delivery ID: " << deliveryID << '\n';
        file << "Type: " << type << '\n';
        file << "Item: " << item << '\n';
        file << "Source: " << source << '\n';
        file << "Destination: " << destination << '\n';
        file << "Priority: " << priority << '\n';
        file << "Status: " << status << '\n';
        file << "Carrier ID: " << carrierID << '\n';
        file << "----------------------------------\n";
    }
};

// =====================================================
// MODULE 3: CARRIER CLASS
// =====================================================

class Carrier : public User {
private:
    string location;
    bool available;
    int currentDeliveryID;

public:
    Carrier() : User() {
        location = "A";
        available = true;
        currentDeliveryID = -1;
    }

    Carrier(int id, string n, string loc)
        : User(id, n) {
        location = loc;
        available = true;
        currentDeliveryID = -1;
    }

    int getID() const {
        return userID;
    }

    bool isAvailable() const {
        return available;
    }

    void assignDelivery(int id) {
        available = false;
        currentDeliveryID = id;
    }

    void releaseCarrier() {
        available = true;
        currentDeliveryID = -1;
    }

    void displayDetails() const override {
        cout << "\nCarrier ID: " << userID;
        cout << "\nName: " << name;
        cout << "\nLocation: " << location;
        cout << "\nStatus: "
             << (available ? "Available" : "Busy");

        if (!available)
            cout << "\nCurrent Delivery: "
                 << currentDeliveryID;

        cout << "\n";
    }
};

// =====================================================
// MAX-HEAP PRIORITY QUEUE
// =====================================================

class PriorityQueue {
private:
    struct Request {
        Delivery* delivery;
        int arrivalOrder;
    };

    Request heap[MAX_DELIVERIES];
    int size;
    int arrivalCounter;

    bool hasHigherPriority(Request a, Request b) const {
        if (a.delivery->getPriority() !=
            b.delivery->getPriority()) {
            return a.delivery->getPriority() >
                   b.delivery->getPriority();
        }

        return a.arrivalOrder < b.arrivalOrder;
    }

    void swapRequests(int a, int b) {
        Request temp = heap[a];
        heap[a] = heap[b];
        heap[b] = temp;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (hasHigherPriority(heap[parent], heap[index]))
                break;

            swapRequests(parent, index);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int highest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size &&
                hasHigherPriority(heap[left], heap[highest]))
                highest = left;

            if (right < size &&
                hasHigherPriority(heap[right], heap[highest]))
                highest = right;

            if (highest == index)
                break;

            swapRequests(index, highest);
            index = highest;
        }
    }

public:
    PriorityQueue() {
        size = 0;
        arrivalCounter = 0;
    }

    bool isEmpty() const {
        return size == 0;
    }

    bool enqueue(Delivery* d) {
        if (d == NULL || size == MAX_DELIVERIES)
            return false;

        heap[size].delivery = d;
        heap[size].arrivalOrder = arrivalCounter++;

        heapifyUp(size);
        size++;

        return true;
    }

    Delivery* peek() const {
        if (size == 0)
            return NULL;

        return heap[0].delivery;
    }

    Delivery* dequeue() {
        if (size == 0)
            return NULL;

        Delivery* result = heap[0].delivery;
        size--;

        if (size > 0) {
            heap[0] = heap[size];
            heapifyDown(0);
        }

        return result;
    }

    void displayQueue() const {
        if (size == 0) {
            cout << "\nPriority queue is empty.\n";
            return;
        }

        cout << "\n--- PENDING DELIVERIES ---\n";

        for (int i = 0; i < size; i++) {
            cout << "\nDelivery ID: "
                 << heap[i].delivery->getID();

            cout << "\nPriority: "
                 << heap[i].delivery->getPriority() << "\n";
        }
    }
};

// =====================================================
// LINKED LIST OF CARRIERS
// =====================================================

class CarrierList {
private:
    struct Node {
        Carrier* carrier;
        bool ownsCarrier;
        Node* next;
    };

    Node* head;

public:
    CarrierList() {
        head = NULL;
    }

    bool insertCarrier(Carrier* c, bool takeOwnership = false) {
        if (c == NULL || findCarrier(c->getID()) != NULL)
            return false;

        Node* n = new Node;
        n->carrier = c;
        n->ownsCarrier = takeOwnership;
        n->next = head;
        head = n;

        return true;
    }

    Carrier* findCarrier(int id) const {
        Node* p = head;

        while (p != NULL) {
            if (p->carrier->getID() == id)
                return p->carrier;

            p = p->next;
        }

        return NULL;
    }

    Carrier* findAvailableCarrier() const {
        Node* p = head;

        while (p != NULL) {
            if (p->carrier->isAvailable())
                return p->carrier;

            p = p->next;
        }

        return NULL;
    }

    bool removeCarrier(int id) {
        Node* p = head;
        Node* prev = NULL;

        while (p != NULL) {
            if (p->carrier->getID() == id) {
                if (!p->carrier->isAvailable()) {
                    cout << "Cannot remove a busy carrier.\n";
                    return false;
                }

                if (prev == NULL)
                    head = p->next;
                else
                    prev->next = p->next;

                if (p->ownsCarrier)
                    delete p->carrier;

                delete p;
                return true;
            }

            prev = p;
            p = p->next;
        }

        return false;
    }

    void displayCarriers() const {
        if (head == NULL) {
            cout << "No carriers registered.\n";
            return;
        }

        Node* p = head;

        while (p != NULL) {
            p->carrier->displayDetails();
            p = p->next;
        }
    }

    ~CarrierList() {
        while (head != NULL) {
            Node* temp = head;
            head = head->next;

            if (temp->ownsCarrier)
                delete temp->carrier;

            delete temp;
        }
    }
};

// =====================================================
// MODULE 3: DELIVERY MANAGEMENT SYSTEM
// =====================================================

class Module3System {
private:
    PriorityQueue pending;
    CarrierList carriers;

    Delivery* allDeliveries[MAX_DELIVERIES];
    int deliveryCount;
    int nextDeliveryID;

    Delivery* findDelivery(int id) const {
        for (int i = 0; i < deliveryCount; i++) {
            if (allDeliveries[i]->getID() == id)
                return allDeliveries[i];
        }

        return NULL;
    }

    void saveDeliveriesToFile() const {
        ofstream file("delivery_records.txt");

        if (!file) {
            cout << "Unable to open file.\n";
            return;
        }

        file << "SMART DELIVERY MANAGEMENT SYSTEM\n";
        file << "DELIVERY RECORDS\n\n";

        for (int i = 0; i < deliveryCount; i++)
            allDeliveries[i]->saveToFile(file);

        file.close();
        cout << "Delivery records saved successfully.\n";
    }

public:
    Module3System() {
        deliveryCount = 0;
        nextDeliveryID = 1001;
    }

    ~Module3System() {
        for (int i = 0; i < deliveryCount; i++)
            delete allDeliveries[i];
    }

    bool addDelivery(int priority, string type,
                     string item, string source,
                     string destination) {
        if (deliveryCount >= MAX_DELIVERIES) {
            cout << "Delivery storage is full.\n";
            return false;
        }

        if (priority < 1 || priority > 3) {
            cout << "Invalid priority. Use 1, 2 or 3.\n";
            return false;
        }

        Delivery* d = new Delivery(
            nextDeliveryID, priority, type,
            item, source, destination
        );

        if (!pending.enqueue(d)) {
            delete d;
            cout << "Priority queue is full.\n";
            return false;
        }

        allDeliveries[deliveryCount++] = d;

        cout << "\nDelivery request added successfully.";
        cout << "\nDelivery ID: " << nextDeliveryID << "\n";

        nextDeliveryID++;
        saveDeliveriesToFile();

        return true;
    }

    bool addCarrier(int id, string name, string location) {
        Carrier* c = new Carrier(id, name, location);

        if (!carriers.insertCarrier(c, true)) {
            delete c;
            cout << "Carrier ID already exists.\n";
            return false;
        }

        cout << "Carrier added successfully.\n";
        return true;
    }

    void removeCarrier(int id) {
        if (carriers.removeCarrier(id))
            cout << "Carrier removed successfully.\n";
        else
            cout << "Carrier not found or is busy.\n";
    }

    void assignNextDelivery() {
        if (pending.isEmpty()) {
            cout << "No pending deliveries.\n";
            return;
        }

        Carrier* carrier = carriers.findAvailableCarrier();

        if (carrier == NULL) {
            cout << "No available carrier. Request remains pending.\n";
            return;
        }

        Delivery* d = pending.dequeue();

        carrier->assignDelivery(d->getID());
        d->setCarrier(carrier->getID());
        d->setStatus("In Transit");

        cout << "\nHighest-priority delivery assigned.";
        cout << "\nCarrier automatically accepts the delivery.\n";

        d->display();
        carrier->displayDetails();

        saveDeliveriesToFile();
    }

    void completeDelivery(int id) {
        Delivery* d = findDelivery(id);

        if (d == NULL) {
            cout << "Delivery not found.\n";
            return;
        }

        if (d->getStatus() != "In Transit") {
            cout << "Delivery is not currently in transit.\n";
            return;
        }

        Carrier* c = carriers.findCarrier(d->getCarrierID());

        if (c == NULL) {
            cout << "Carrier not found.\n";
            return;
        }

        d->setStatus("Delivered");
        c->releaseCarrier();

        cout << "Delivery completed successfully.\n";
        cout << "Carrier is now available.\n";

        saveDeliveriesToFile();
    }

    void trackDelivery(int id) const {
        Delivery* d = findDelivery(id);

        if (d == NULL)
            cout << "Delivery not found.\n";
        else
            d->display();
    }

    void displayAllDeliveries() const {
        if (deliveryCount == 0) {
            cout << "No deliveries created.\n";
            return;
        }

        for (int i = 0; i < deliveryCount; i++)
            allDeliveries[i]->display();
    }

    void displayPendingQueue() const {
        pending.displayQueue();
    }

    void displayCarriers() const {
        carriers.displayCarriers();
    }

    void saveRecords() const {
        saveDeliveriesToFile();
    }

    void viewSavedRecords() const {
        ifstream file("delivery_records.txt");

        if (!file) {
            cout << "No saved delivery file found.\n";
            return;
        }

        string line;

        cout << "\n--- SAVED DELIVERY RECORDS ---\n";

        while (getline(file, line))
            cout << line << '\n';

        file.close();
    }
};

// =====================================================
// MAIN FUNCTION
// =====================================================

int main() {
    Module3System system;

    // Initial carriers
    system.addCarrier(201, "Aman", "A");
    system.addCarrier(202, "Riya", "C");
    system.addCarrier(203, "Rahul", "E");

    int choice;

    do {
        cout << "\n\n========== MODULE 3: CARRIER MANAGEMENT ==========\n";

        cout << "\n1. Add Delivery";
        cout << "\n2. Display Pending Priority Queue";
        cout << "\n3. Display Carriers";
        cout << "\n4. Assign Highest-Priority Delivery";
        cout << "\n5. Complete Delivery";
        cout << "\n6. Track Delivery";
        cout << "\n7. Display All Deliveries";
        cout << "\n8. Add Carrier";
        cout << "\n9. Remove Carrier";
        cout << "\n10. View Saved Delivery Records";
        cout << "\n11. Save Delivery Records";
        cout << "\n0. Exit";

        cout << "\n\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            int priority;
            string type, item, source, destination;

            cout << "Delivery Type: ";
            cin >> type;

            cout << "Item: ";
            cin >> item;

            cout << "Source Location: ";
            cin >> source;

            cout << "Destination: ";
            cin >> destination;

            cout << "Priority (1=Normal, 2=Medium, 3=Urgent): ";
            cin >> priority;

            system.addDelivery(
                priority, type, item, source, destination
            );
        }

        else if (choice == 2) {
            system.displayPendingQueue();
        }

        else if (choice == 3) {
            system.displayCarriers();
        }

        else if (choice == 4) {
            system.assignNextDelivery();
        }

        else if (choice == 5) {
            int id;

            cout << "Enter Delivery ID: ";
            cin >> id;

            system.completeDelivery(id);
        }

        else if (choice == 6) {
            int id;

            cout << "Enter Delivery ID: ";
            cin >> id;

            system.trackDelivery(id);
        }

        else if (choice == 7) {
            system.displayAllDeliveries();
        }

        else if (choice == 8) {
            int id;
            string name, location;

            cout << "Carrier ID: ";
            cin >> id;

            cout << "Carrier Name: ";
            cin >> name;

            cout << "Starting Location: ";
            cin >> location;

            system.addCarrier(id, name, location);
        }

        else if (choice == 9) {
            int id;

            cout << "Enter Carrier ID to remove: ";
            cin >> id;

            system.removeCarrier(id);
        }

        else if (choice == 10) {
            system.viewSavedRecords();
        }

        else if (choice == 11) {
            system.saveRecords();
        }

        else if (choice != 0) {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    system.saveRecords();

    cout << "\nThank you for using Module 3.\n";

    return 0;
}
