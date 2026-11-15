#include <iostream>
#include <string>
#include <fstream>
#include <climits>
#include <limits>
using namespace std;

const int MAX_DELIVERIES = 100;
const int MAX_LOCATIONS = 6;

// ============================================================
// SHARED DELIVERY CLASS
// Used by Modules 1, 2 and 3
// ============================================================

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

// ============================================================
// MODULE 1: STORE ORDERING SYSTEM
// ============================================================

class Product {
private:
    int productID;
    string name;
    float price;
    int stock;

public:
    Product() {
        productID = 0;
        price = 0;
        stock = 0;
    }

    Product(int id, string n, float p, int s) {
        productID = id;
        name = n;
        price = p;
        stock = s;
    }

    int getID() const { return productID; }
    string getName() const { return name; }
    float getPrice() const { return price; }
    int getStock() const { return stock; }

    void reduceStock(int quantity) {
        if (quantity > 0 && quantity <= stock)
            stock -= quantity;
    }

    void display() const {
        cout << "\nProduct ID: " << productID;
        cout << "\nName: " << name;
        cout << "\nPrice: Rs. " << price;
        cout << "\nStock: " << stock << "\n";
    }
};

class Store {
private:
    int storeID;
    string name, location;
    Product products[10];
    int count;

public:
    Store() {
        storeID = 0;
        count = 0;
    }

    void setStore(int id, string n, string loc) {
        storeID = id;
        name = n;
        location = loc;
    }

    void addProduct(Product p) {
        if (count < 10)
            products[count++] = p;
    }

    int getStoreID() const { return storeID; }
    string getLocation() const { return location; }
    int getCount() const { return count; }

    Product getProduct(int i) const {
        return products[i];
    }

    void updateProduct(int i, Product p) {
        if (i >= 0 && i < count)
            products[i] = p;
    }

    int findProduct(int id) const {
        for (int i = 0; i < count; i++) {
            if (products[i].getID() == id)
                return i;
        }
        return -1;
    }

    void displayStore() const {
        cout << "\nStore ID: " << storeID;
        cout << "\nStore Name: " << name;
        cout << "\nLocation: " << location << "\n";
    }

    void displayProducts() const {
        cout << "\n--- AVAILABLE PRODUCTS ---\n";
        for (int i = 0; i < count; i++)
            products[i].display();
    }
};

class Customer {
private:
    int customerID;
    string name;

public:
    Customer() {
        customerID = 0;
    }

    void setCustomer(int id, string n) {
        customerID = id;
        name = n;
    }

    int getID() const { return customerID; }
    string getName() const { return name; }

    void display() const {
        cout << "\nCustomer ID: " << customerID;
        cout << "\nCustomer Name: " << name << "\n";
    }
};

class Cart {
private:
    int productIDs[20];
    string names[20];
    float prices[20];
    int quantities[20];
    int count;

public:
    Cart() {
        count = 0;
    }

    int getCount() const { return count; }

    int getIDAt(int i) const { return productIDs[i]; }
    int getQuantityAt(int i) const { return quantities[i]; }

    float calculateTotal() const {
        float total = 0;
        for (int i = 0; i < count; i++)
            total += prices[i] * quantities[i];
        return total;
    }

    void addProduct(Product p, int quantity) {
        if (quantity <= 0) {
            cout << "Invalid quantity.\n";
            return;
        }

        int pos = -1;

        for (int i = 0; i < count; i++) {
            if (productIDs[i] == p.getID()) {
                pos = i;
                break;
            }
        }

        int alreadyAdded = (pos == -1) ? 0 : quantities[pos];

        if (quantity > p.getStock() - alreadyAdded) {
            cout << "Insufficient stock.\n";
            return;
        }

        if (pos != -1) {
            quantities[pos] += quantity;
        }
        else if (count < 20) {
            productIDs[count] = p.getID();
            names[count] = p.getName();
            prices[count] = p.getPrice();
            quantities[count] = quantity;
            count++;
        }
        else {
            cout << "Cart is full.\n";
            return;
        }

        cout << "Product added to cart.\n";
    }

    void removeProduct(int id) {
        int pos = -1;

        for (int i = 0; i < count; i++) {
            if (productIDs[i] == id) {
                pos = i;
                break;
            }
        }

        if (pos == -1) {
            cout << "Product not found in cart.\n";
            return;
        }

        for (int i = pos; i < count - 1; i++) {
            productIDs[i] = productIDs[i + 1];
            names[i] = names[i + 1];
            prices[i] = prices[i + 1];
            quantities[i] = quantities[i + 1];
        }

        count--;
        cout << "Product removed.\n";
    }

    void display() const {
        if (count == 0) {
            cout << "Cart is empty.\n";
            return;
        }

        cout << "\n--- YOUR CART ---\n";

        for (int i = 0; i < count; i++) {
            cout << "\nID: " << productIDs[i];
            cout << "\nName: " << names[i];
            cout << "\nQuantity: " << quantities[i];
            cout << "\nSubtotal: Rs. "
                 << prices[i] * quantities[i] << "\n";
        }

        cout << "\nTotal: Rs. " << calculateTotal() << "\n";
    }

    void clear() {
        count = 0;
    }
};

class Order {
protected:
    int orderID;
    int customerID;
    string customerName;
    float total;
    int priority;

public:
    Order() {
        orderID = customerID = 0;
        total = 0;
        priority = 1;
    }

    void setOrder(int id, int cid, string name,
                  float amount, int p) {
        orderID = id;
        customerID = cid;
        customerName = name;
        total = amount;
        priority = p;
    }

    virtual void displayOrder() const {
        cout << "\nOrder ID: " << orderID;
        cout << "\nCustomer: " << customerName;
        cout << "\nTotal: Rs. " << total;
        cout << "\nPriority: " << priority << "\n";
    }

    virtual ~Order() {}
};

class StoreOrder : public Order {
private:
    int storeID;
    int itemIDs[20];
    int quantities[20];
    int itemCount;

public:
    StoreOrder() {
        storeID = 0;
        itemCount = 0;
    }

    void setStoreOrder(int sid, const Cart& cart) {
        storeID = sid;
        itemCount = cart.getCount();

        for (int i = 0; i < itemCount; i++) {
            itemIDs[i] = cart.getIDAt(i);
            quantities[i] = cart.getQuantityAt(i);
        }
    }

    void displayOrder() const override {
        Order::displayOrder();

        cout << "Store ID: " << storeID;
        cout << "\nOrdered Products:\n";

        for (int i = 0; i < itemCount; i++)
            cout << "Product ID: " << itemIDs[i]
                 << " Quantity: " << quantities[i] << "\n";
    }
};

// ============================================================
// MODULE 2: PACKAGE DELIVERY
// ============================================================

class User {
protected:
    int userID;
    string name;

public:
    User() {
        userID = 0;
    }

    User(int id, string n) {
        userID = id;
        name = n;
    }

    virtual void displayDetails() const {
        cout << "User ID: " << userID
             << " Name: " << name << "\n";
    }

    virtual ~User() {}
};

class Sender : public User {
public:
    Sender() : User() {}

    Sender(int id, string n) : User(id, n) {}

    void createRequest() const {
        cout << "\nSender " << name
             << " created a package delivery request.\n";
    }
};

class Receiver : public User {
public:
    Receiver() : User() {}

    Receiver(int id, string n) : User(id, n) {}

    void trackDelivery() const {
        cout << "\nReceiver " << name
             << " can track the delivery.\n";
    }
};

class PackageDelivery {
private:
    int deliveryID;
    string pickup, destination, item;

public:
    PackageDelivery() {
        deliveryID = 0;
    }

    void createPackage(int id, string p,
                       string d, string it) {
        deliveryID = id;
        pickup = p;
        destination = d;
        item = it;
    }

    int getID() const { return deliveryID; }
    string getPickup() const { return pickup; }
    string getDestination() const { return destination; }
    string getItem() const { return item; }

    void display() const {
        cout << "\nPackage ID: " << deliveryID;
        cout << "\nItem: " << item;
        cout << "\nPickup: " << pickup;
        cout << "\nDestination: " << destination << "\n";
    }
};

// ============================================================
// MODULE 4: ROUTE OPTIMIZATION
// ============================================================

const int MAX = 10;

class Location {
public:
    int id;
    char name;

    Location() {
        id = 0;
        name = 'A';
    }

    Location(int i, char n) {
        id = i;
        name = n;
    }
};

class Route {
public:
    int distance;
    int path[MAX];
    int count;

    Route() {
        distance = INT_MAX;
        count = 0;
    }

    bool operator<(const Route& r) const {
        return distance < r.distance;
    }
};

class RoutePlanner {
public:
    virtual void displayInfo() {
        cout << "Basic Route Planner\n";
    }

    virtual ~RoutePlanner() {}
};

class Graph : public RoutePlanner {
private:
    struct Node {
        int dest;
        int weight;
        Node* next;
    };

    Node* head[MAX];
    int n;

    Node* findRoad(int u, int v) {
        Node* p = head[u];

        while (p != NULL) {
            if (p->dest == v)
                return p;
            p = p->next;
        }

        return NULL;
    }

public:
    Graph(int vertices) {
        n = vertices;

        for (int i = 0; i < n; i++)
            head[i] = NULL;
    }

    void displayInfo() override {
        cout << "\nRoute Optimization System\n";
        cout << "Algorithm: Dijkstra's Algorithm\n";
    }

    void addRoad(int u, int v, int w) {
        if (u < 0 || v < 0 || u >= n || v >= n ||
            u == v || w < 0)
            return;

        Node* p = new Node;
        p->dest = v;
        p->weight = w;
        p->next = head[u];
        head[u] = p;

        Node* q = new Node;
        q->dest = u;
        q->weight = w;
        q->next = head[v];
        head[v] = q;
    }

    void blockRoad(int u, int v) {
        Node* p = findRoad(u, v);
        Node* q = findRoad(v, u);

        if (p == NULL || q == NULL) {
            cout << "Road does not exist.\n";
            return;
        }

        if (p->weight == INT_MAX) {
            cout << "Road is already blocked.\n";
            return;
        }

        p->weight = INT_MAX;
        q->weight = INT_MAX;

        cout << "Road blocked successfully.\n";
    }

    Route findShortestPath(int src, int dest) {
        Route r;

        if (src < 0 || src >= n || dest < 0 || dest >= n)
            return r;

        int dist[MAX], parent[MAX];
        bool visited[MAX] = {false};

        for (int i = 0; i < n; i++) {
            dist[i] = INT_MAX;
            parent[i] = -1;
        }

        dist[src] = 0;

        for (int i = 0; i < n; i++) {
            int u = -1;
            int minDist = INT_MAX;

            for (int j = 0; j < n; j++) {
                if (!visited[j] && dist[j] < minDist) {
                    minDist = dist[j];
                    u = j;
                }
            }

            if (u == -1)
                break;

            visited[u] = true;

            Node* p = head[u];

            while (p != NULL) {
                int v = p->dest;
                int w = p->weight;

                if (w != INT_MAX && !visited[v] &&
                    dist[u] != INT_MAX &&
                    dist[u] <= INT_MAX - w &&
                    dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                }

                p = p->next;
            }
        }

        r.distance = dist[dest];

        if (dist[dest] == INT_MAX)
            return r;

        int temp[MAX];
        int count = 0;

        for (int curr = dest; curr != -1; curr = parent[curr])
            temp[count++] = curr;

        for (int i = count - 1; i >= 0; i--)
            r.path[r.count++] = temp[i];

        return r;
    }

    void displayRoute(Route r) const {
        if (r.distance == INT_MAX) {
            cout << "No route available.\n";
            return;
        }

        cout << "\nShortest Route: ";

        for (int i = 0; i < r.count; i++) {
            cout << char('A' + r.path[i]);

            if (i < r.count - 1)
                cout << " -> ";
        }

        cout << "\nTotal Distance: "
             << r.distance << " km\n";
    }

    ~Graph() {
        for (int i = 0; i < n; i++) {
            Node* p = head[i];

            while (p != NULL) {
                Node* temp = p;
                p = p->next;
                delete temp;
            }
        }
    }
};

// Convert location names A-F into graph IDs.
int locationToID(string location) {
    if (location.length() != 1)
        return -1;

    char c = location[0];

    if (c >= 'a' && c <= 'f')
        c = char(c - 'a' + 'A');

    if (c >= 'A' && c <= 'F')
        return c - 'A';

    return -1;
}

// ============================================================
// MODULE 3: CARRIER ASSIGNMENT
// ============================================================

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

    int getID() const { return userID; }
    bool isAvailable() const { return available; }

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
            cout << "\nCurrent Delivery: " << currentDeliveryID;

        cout << "\n";
    }
};

// ---------------- MAX-HEAP PRIORITY QUEUE ----------------

class PriorityQueue {
private:
    struct Request {
        Delivery* delivery;
        int arrivalOrder;
    };

    Request heap[MAX_DELIVERIES];
    int size;
    int arrivalCounter;

    bool hasHigherPriority(Request a, Request b) {
        if (a.delivery->getPriority() != b.delivery->getPriority())
            return a.delivery->getPriority() >
                   b.delivery->getPriority();

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
            cout << "Priority queue is empty.\n";
            return;
        }

        cout << "\n--- PENDING DELIVERIES ---\n";

        for (int i = 0; i < size; i++) {
            cout << "\nDelivery ID: "
                 << heap[i].delivery->getID();

            cout << "\nPriority: "
                 << heap[i].delivery->getPriority();

            cout << "\n";
        }
    }
};

// ---------------- LINKED LIST OF CARRIERS ----------------

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

// ============================================================
// COMMON DELIVERY STORAGE AND FILE HANDLING
// ============================================================

Delivery* allDeliveries[MAX_DELIVERIES];
int deliveryCount = 0;
int nextDeliveryID = 1001;

Delivery* findDelivery(int id) {
    for (int i = 0; i < deliveryCount; i++) {
        if (allDeliveries[i]->getID() == id)
            return allDeliveries[i];
    }

    return NULL;
}

void saveDeliveriesToFile() {
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

void viewDeliveryFile() {
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

// ============================================================
// CONNECTION BETWEEN MODULE 3 AND MODULE 4
// ============================================================

void sendSourceDestinationToModule4(
    Delivery* d, Graph& graph) {

    int source = locationToID(d->getSource());
    int destination = locationToID(d->getDestination());

    cout << "\nSending source and destination to Module 4...\n";

    if (source == -1 || destination == -1) {
        cout << "Invalid location. Use A to F.\n";
        return;
    }

    Route r = graph.findShortestPath(source, destination);
    graph.displayRoute(r);
}

// ============================================================
// COMMON FUNCTION TO ADD A DELIVERY
// Called by both Module 1 and Module 2
// ============================================================

bool addDelivery(PriorityQueue& pending,
                 int priority, string type,
                 string item, string source,
                 string destination) {

    if (deliveryCount >= MAX_DELIVERIES) {
        cout << "Delivery storage is full.\n";
        return false;
    }

    if (priority < 1 || priority > 3) {
        cout << "Invalid priority.\n";
        return false;
    }

    if (locationToID(source) == -1 ||
        locationToID(destination) == -1) {
        cout << "Enter valid locations A-F.\n";
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

// ============================================================
// MODULE 3: ASSIGN DELIVERY TO CARRIER
// ============================================================

void assignNextDelivery(PriorityQueue& pending,
                        CarrierList& carriers,
                        Graph& graph) {

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

    // Call Module 4.
    sendSourceDestinationToModule4(d, graph);

    saveDeliveriesToFile();
}

void completeDelivery(int id, CarrierList& carriers) {
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

void trackDelivery(int id) {
    Delivery* d = findDelivery(id);

    if (d == NULL)
        cout << "Delivery not found.\n";
    else
        d->display();
}

void displayAllDeliveries() {
    if (deliveryCount == 0) {
        cout << "No deliveries created.\n";
        return;
    }

    for (int i = 0; i < deliveryCount; i++)
        allDeliveries[i]->display();
}

// ============================================================
// MAIN: INTEGRATION OF ALL FOUR MODULES
// ============================================================

int main() {
    // ---------------- MODULE 1 SETUP ----------------

    Store store;
    store.setStore(1, "General Store", "A");

    store.addProduct(Product(101, "Rice", 60, 50));
    store.addProduct(Product(102, "Milk", 30, 40));
    store.addProduct(Product(103, "Bread", 40, 30));
    store.addProduct(Product(104, "Sugar", 45, 25));

    Customer customer;
    customer.setCustomer(1, "Customer");

    Cart cart;

    int orderCounter = 1001;

    // ---------------- MODULE 3 SETUP ----------------

    PriorityQueue pendingDeliveries;
    CarrierList carriers;

    Carrier c1(201, "Aman", "A");
    Carrier c2(202, "Riya", "C");
    Carrier c3(203, "Rahul", "E");

    carriers.insertCarrier(&c1);
    carriers.insertCarrier(&c2);
    carriers.insertCarrier(&c3);

    // ---------------- MODULE 4 SETUP ----------------

    Graph graph(6);

    graph.addRoad(0, 1, 2);
    graph.addRoad(0, 2, 4);
    graph.addRoad(1, 2, 1);
    graph.addRoad(1, 3, 3);
    graph.addRoad(2, 4, 2);
    graph.addRoad(3, 5, 2);
    graph.addRoad(4, 5, 3);

    int choice;

    do {
        cout << "\n\n========== SMART DELIVERY MANAGEMENT SYSTEM ==========\n";

        cout << "\n1. Store Ordering (Module 1)";
        cout << "\n2. Send Something (Module 2)";
        cout << "\n3. Display Pending Priority Queue";
        cout << "\n4. Display Carriers";
        cout << "\n5. Assign Highest-Priority Delivery";
        cout << "\n6. Complete Delivery";
        cout << "\n7. Track Delivery";
        cout << "\n8. Display All Deliveries";
        cout << "\n9. Add Carrier";
        cout << "\n10. Remove Carrier";
        cout << "\n11. Block Road (Module 4)";
        cout << "\n12. View Saved Delivery File";
        cout << "\n13. Save Delivery Records";
        cout << "\n0. Exit";

        cout << "\n\nEnter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        // ====================================================
        // MODULE 1: STORE ORDERING
        // ====================================================

        if (choice == 1) {
            int ch;

            do {
                cout << "\n--- STORE ORDERING ---\n";
                cout << "1. Display Store\n";
                cout << "2. View Products\n";
                cout << "3. Add Product to Cart\n";
                cout << "4. Remove Product from Cart\n";
                cout << "5. View Cart\n";
                cout << "6. Place Order\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter choice: ";

                cin >> ch;

                if (ch == 1) {
                    store.displayStore();
                }
                else if (ch == 2) {
                    store.displayProducts();
                }
                else if (ch == 3) {
                    int id, quantity;

                    cout << "Enter Product ID: ";
                    cin >> id;

                    cout << "Enter Quantity: ";
                    cin >> quantity;

                    int index = store.findProduct(id);

                    if (index == -1) {
                        cout << "Product not found.\n";
                    }
                    else {
                        Product p = store.getProduct(index);
                        cart.addProduct(p, quantity);
                    }
                }
                else if (ch == 4) {
                    int id;
                    cout << "Enter Product ID: ";
                    cin >> id;
                    cart.removeProduct(id);
                }
                else if (ch == 5) {
                    cart.display();
                }
                else if (ch == 6) {
                    if (cart.getCount() == 0) {
                        cout << "Cart is empty.\n";
                        continue;
                    }

                    int priority;

                    cout << "1. Normal\n";
                    cout << "2. Medium\n";
                    cout << "3. Urgent\n";
                    cout << "Enter priority: ";
                    cin >> priority;

                    if (priority < 1 || priority > 3) {
                        cout << "Invalid priority.\n";
                        continue;
                    }

                    // Check stock before placing order.
                    bool valid = true;

                    for (int i = 0; i < cart.getCount(); i++) {
                        int index = store.findProduct(cart.getIDAt(i));

                        if (index == -1 ||
                            cart.getQuantityAt(i) >
                            store.getProduct(index).getStock()) {
                            valid = false;
                            break;
                        }
                    }

                    if (!valid) {
                        cout << "Insufficient stock.\n";
                        continue;
                    }

                    StoreOrder order;

                    order.setOrder(
                        orderCounter,
                        customer.getID(),
                        customer.getName(),
                        cart.calculateTotal(),
                        priority
                    );

                    order.setStoreOrder(store.getStoreID(), cart);
                    order.displayOrder();

                    // Create common delivery request.
                    string item = "Store Order";

                    bool added = addDelivery(
                        pendingDeliveries,
                        priority,
                        "StoreOrder",
                        item,
                        store.getLocation(),
                        "F"
                    );

                    if (added) {
                        // Reduce stock only after successful queue insertion.
                        for (int i = 0; i < cart.getCount(); i++) {
                            int index = store.findProduct(cart.getIDAt(i));

                            Product p = store.getProduct(index);
                            p.reduceStock(cart.getQuantityAt(i));
                            store.updateProduct(index, p);
                        }

                        cout << "Store order placed successfully.\n";
                        orderCounter++;
                        cart.clear();
                    }
                }

            } while (ch != 0);
        }

        // ====================================================
        // MODULE 2: PACKAGE DELIVERY
        // ====================================================

        else if (choice == 2) {
            int id, sid, rid, priority;
            string senderName, receiverName;
            string pickup, destination, item;

            cout << "\n--- SEND SOMETHING ---\n";

            cout << "Sender ID: ";
            cin >> sid;

            cout << "Sender Name: ";
            cin >> senderName;

            cout << "Receiver ID: ";
            cin >> rid;

            cout << "Receiver Name: ";
            cin >> receiverName;

            cout << "Pickup Location (A-F): ";
            cin >> pickup;

            cout << "Destination (A-F): ";
            cin >> destination;

            cout << "Item: ";
            cin >> item;

            cout << "Priority (1=Normal, 2=Medium, 3=Urgent): ";
            cin >> priority;

            Sender sender(sid, senderName);
            Receiver receiver(rid, receiverName);

            PackageDelivery package;

            package.createPackage(
                nextDeliveryID,
                pickup,
                destination,
                item
            );

            sender.createRequest();
            package.display();
            receiver.trackDelivery();

            // Convert package into the shared Delivery class.
            addDelivery(
                pendingDeliveries,
                priority,
                "PackageDelivery",
                item,
                pickup,
                destination
            );
        }

        // ====================================================
        // MODULE 3: DISPATCH AND CARRIER MANAGEMENT
        // ====================================================

        else if (choice == 3) {
            pendingDeliveries.displayQueue();
        }
        else if (choice == 4) {
            carriers.displayCarriers();
        }
        else if (choice == 5) {
            assignNextDelivery(
                pendingDeliveries,
                carriers,
                graph
            );
        }
        else if (choice == 6) {
            int id;

            cout << "Enter Delivery ID: ";
            cin >> id;

            completeDelivery(id, carriers);
        }
        else if (choice == 7) {
            int id;

            cout << "Enter Delivery ID: ";
            cin >> id;

            trackDelivery(id);
        }
        else if (choice == 8) {
            displayAllDeliveries();
        }
        else if (choice == 9) {
            int id;
            string name, location;

            cout << "Carrier ID: ";
            cin >> id;

            cout << "Carrier Name: ";
            cin >> name;

            cout << "Starting Location (A-F): ";
            cin >> location;

            if (locationToID(location) == -1) {
                cout << "Invalid location.\n";
            }
            else {
                Carrier* c = new Carrier(id, name, location);

                if (carriers.insertCarrier(c, true))
                    cout << "Carrier added successfully.\n";
                else {
                    delete c;
                    cout << "Carrier ID already exists.\n";
                }
            }
        }
        else if (choice == 10) {
            int id;

            cout << "Enter Carrier ID to remove: ";
            cin >> id;

            if (carriers.removeCarrier(id))
                cout << "Carrier removed.\n";
            else
                cout << "Carrier not found or is busy.\n";
        }

        // ====================================================
        // MODULE 4: ROAD BLOCKAGE
        // ====================================================

        else if (choice == 11) {
            string from, to;

            cout << "Enter road endpoints (A-F): ";
            cin >> from >> to;

            int a = locationToID(from);
            int b = locationToID(to);

            if (a == -1 || b == -1 || a == b) {
                cout << "Invalid road endpoints.\n";
            }
            else {
                graph.blockRoad(a, b);
            }
        }

        // ====================================================
        // FILE HANDLING
        // ====================================================

        else if (choice == 12) {
            viewDeliveryFile();
        }
        else if (choice == 13) {
            saveDeliveriesToFile();
        }
        else if (choice != 0) {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    saveDeliveriesToFile();

    for (int i = 0; i < deliveryCount; i++)
        delete allDeliveries[i];

    cout << "\nThank you for using Smart Delivery Management System.\n";

    return 0;
}