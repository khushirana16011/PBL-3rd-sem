#include <iostream>
#include <string>
#include <climits>
using namespace std;

const int MAX_DELIVERIES = 100;
const int MAX_CARRIERS = 10;
const int MAX_LOCATIONS = 6;


// MODULE 1: STORE ORDERING

class Product {
private:
    int id;
    string name;
    int price;
    int stock;

public:
    Product() {
        id = 0;
        name = "";
        price = 0;
        stock = 0;
    }

    Product(int i, string n, int p, int s) {
        id = i;
        name = n;
        price = p;
        stock = s;
    }

    int getID() { return id; }
    string getName() { return name; }
    int getPrice() { return price; }
    int getStock() { return stock; }

    void display() {
        cout << id << ". " << name << " - Rs. " << price
             << " (Stock: " << stock << ")\n";
    }
};

class Store {
private:
    Product products[3];

public:
    Store() {
        products[0] = Product(101, "Rice", 60, 20);
        products[1] = Product(102, "Milk", 30, 15);
        products[2] = Product(103, "Bread", 40, 10);
    }

    void displayProducts() {
        cout << "\n--- Store Products ---\n";
        for (int i = 0; i < 3; i++)
            products[i].display();
    }

    int findProduct(int id) {
        for (int i = 0; i < 3; i++) {
            if (products[i].getID() == id)
                return i;
        }
        return -1;
    }

    Product getProduct(int index) {
        return products[index];
    }
};

class Cart {
private:
    string names[10];
    int prices[10];
    int quantities[10];
    int count;

public:
    Cart() {
        count = 0;
    }

    void addProduct(Product p, int quantity) {
        if (quantity <= 0) {
            cout << "Invalid quantity.\n";
            return;
        }
        if (quantity > p.getStock()) {
            cout << "Not enough stock.\n";
            return;
        }
        if (count == 10) {
            cout << "Cart is full.\n";
            return;
        }

        names[count] = p.getName();
        prices[count] = p.getPrice();
        quantities[count] = quantity;
        count++;

        cout << "Product added to cart.\n";
    }

    int getTotal() {
        int total = 0;
        for (int i = 0; i < count; i++)
            total += prices[i] * quantities[i];
        return total;
    }

    void displayCart() {
        if (count == 0) {
            cout << "Cart is empty.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            cout << names[i] << " x " << quantities[i]
                 << " = Rs. " << prices[i] * quantities[i] << "\n";
        }
        cout << "Total: Rs. " << getTotal() << "\n";
    }

    bool isEmpty() {
        return count == 0;
    }

    string getOrderName() {
        string items = "";
        for (int i = 0; i < count; i++) {
            if (i > 0)
                items += ", ";
            items += names[i];
        }
        return items;
    }

    void clearCart() {
        count = 0;
    }
};


// MODULE 2: SEND SOMETHING


class User {
protected:
    int id;
    string name;

public:
    User(int i, string n) {
        id = i;
        name = n;
    }

    virtual void displayDetails() {
        cout << "User ID: " << id << ", Name: " << name << "\n";
    }

    virtual ~User() {}
};

class Sender : public User {
public:
    Sender(int i, string n) : User(i, n) {}

    void createRequest() {
        cout << name << " created a package delivery request.\n";
    }
};

class Receiver : public User {
public:
    Receiver(int i, string n) : User(i, n) {}

    void trackDelivery() {
        cout << name << " can track the delivery using its ID.\n";
    }
};


// MODULE 3: CARRIER ASSIGNMENT AND SMART DISPATCH


class Carrier : public User {
private:
    string location;
    bool available;

public:
    Carrier(int i, string n, string loc) :User(i, n) {
        location = loc;
        available = true;
    }

    int getID() { return id; }
    string getLocation() { return location; }
    bool isAvailable() { return available; }

    void setAvailable(bool value) {
        available = value;
    }

    void displayDetails() override {
        cout << "Carrier ID: " << id
             << ", Name: " << name
             << ", Location: " << location
             << ", Status: " << (available ? "Available" : "Busy")
             << "\n";
    }
};

class Delivery {
private:
    int id;
    int priority;
    string type;
    string item;
    string pickup;
    string destination;
    string status;

public:
    Delivery() {
        id = 0;
        priority = 1;
        status = "Pending";
    }

    Delivery(int i, int p, string t, string it,
             string from, string to) {
        id = i;
        priority = p;
        type = t;
        item = it;
        pickup = from;
        destination = to;
        status = "Pending";
    }

    int getID() { return id; }
    int getPriority() { return priority; }
    string getType() { return type; }
    string getItem() { return item; }
    string getPickup() { return pickup; }
    string getDestination() { return destination; }
    string getStatus() { return status; }

    void setStatus(string s) {
        status = s;
    }

    void display() {
        cout << "\nDelivery ID: " << id
             << "\nType: " << type
             << "\nItem: " << item
             << "\nPickup: " << pickup
             << "\nDestination: " << destination
             << "\nPriority: " << priority
             << "\nStatus: " << status << "\n";
    }
};

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
            return a.delivery->getPriority() > b.delivery->getPriority();

        return a.arrivalOrder < b.arrivalOrder;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (hasHigherPriority(heap[parent], heap[index]))
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

            if (left < size && hasHigherPriority(heap[left], heap[largest]))
                largest = left;

            if (right < size && hasHigherPriority(heap[right], heap[largest]))
                largest = right;

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
        arrivalCounter = 0;
    }

    bool isEmpty() {
        return size == 0;
    }

    bool enqueue(Delivery* d) {
        if (size == MAX_DELIVERIES) {
            cout << "Priority queue is full.\n";
            return false;
        }

        heap[size].delivery = d;
        heap[size].arrivalOrder = arrivalCounter;
        arrivalCounter++;

        heapifyUp(size);
        size++;
        return true;
    }

    Delivery* dequeue() {
        if (size == 0)
            return NULL;

        Delivery* result = heap[0].delivery;
        heap[0] = heap[size - 1];
        size--;

        if (size > 0)
            heapifyDown(0);

        return result;
    }

    void displayQueue() {
        if (size == 0) {
            cout << "No pending deliveries.\n";
            return;
        }

        cout << "\n--- Pending Deliveries (heap order) ---\n";
        for (int i = 0; i < size; i++) {
            cout << "ID: " << heap[i].delivery->getID()
                 << " | Type: " << heap[i].delivery->getType()
                 << " | Priority: " << heap[i].delivery->getPriority()
                 << "\n";
        }
    }
};

class CarrierList {
private:
    struct Node {
        Carrier* carrier;
        Node* next;
    };

    Node* head;

public:
    CarrierList() {
        head = NULL;
    }

    ~CarrierList() {
        while (head != NULL) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insertCarrier(Carrier* c) {
        Node* newNode = new Node;
        newNode->carrier = c;
        newNode->next = head;
        head = newNode;
    }

    Carrier* findAvailableCarrier() {
        Node* current = head;
        while (current != NULL) {
            if (current->carrier->isAvailable())
                return current->carrier;
            current = current->next;
        }
        return NULL;
    }

    void displayCarriers() {
        if (head == NULL) {
            cout << "No carriers registered.\n";
            return;
        }

        Node* current = head;
        cout << "\n--- Carriers ---\n";
        while (current != NULL) {
            current->carrier->displayDetails();
            current = current->next;
        }
    }
};
// MODULE 4: ROUTE OPTIMIZATION USING DIJKSTRA

class RouteGraph {
private:
    struct Node {
        int destination;
        int distance;
        Node* next;
    };

    Node* head[MAX_LOCATIONS];

    Node* findRoad(int from, int to) {
        Node* current = head[from];
        while (current != NULL) {
            if (current->destination == to)
                return current;
            current = current->next;
        }
        return NULL;
    }

public:
    RouteGraph() {
        for (int i = 0; i < MAX_LOCATIONS; i++)
            head[i] = NULL;
    }

    void addRoad(int from, int to, int distance) {
        Node* a = new Node;
        a->destination = to;
        a->distance = distance;
        a->next = head[from];
        head[from] = a;

        Node* b = new Node;
        b->destination = from;
        b->distance = distance;
        b->next = head[to];
        head[to] = b;
    }

    void blockRoad(int from, int to) {
        Node* a = findRoad(from, to);
        Node* b = findRoad(to, from);

        if (a == NULL || b == NULL) {
            cout << "Road does not exist.\n";
            return;
        }

        a->distance = INT_MAX;
        b->distance = INT_MAX;
        cout << "Road blocked.\n";
    }

    void shortestRoute(int source, int destination) {
        int distance[MAX_LOCATIONS];
        int parent[MAX_LOCATIONS];
        bool visited[MAX_LOCATIONS] = {false};

        for (int i = 0; i < MAX_LOCATIONS; i++) {
            distance[i] = INT_MAX;
            parent[i] = -1;
        }

        distance[source] = 0;

        for (int i = 0; i < MAX_LOCATIONS; i++) {
            int u = -1;
            int best = INT_MAX;

            for (int j = 0; j < MAX_LOCATIONS; j++) {
                if (!visited[j] && distance[j] < best) {
                    best = distance[j];
                    u = j;
                }
            }

            if (u == -1)
                break;

            visited[u] = true;
            Node* current = head[u];

            while (current != NULL) {
                int v = current->destination;
                int w = current->distance;

                if (w != INT_MAX && !visited[v] &&
                    distance[u] != INT_MAX &&
                    distance[u] + w < distance[v]) {
                    distance[v] = distance[u] + w;
                    parent[v] = u;
                }

                current = current->next;
            }
        }

        if (distance[destination] == INT_MAX) {
            cout << "No route available.\n";
            return;
        }

        int path[MAX_LOCATIONS];
        int count = 0;
        int current = destination;

        while (current != -1) {
            path[count++] = current;
            current = parent[current];
        }

        cout << "Shortest route: ";
        for (int i = count - 1; i >= 0; i--) {
            cout << char('A' + path[i]);
            if (i > 0)
                cout << " -> ";
        }
        cout << "\nTotal distance: " << distance[destination] << " km\n";
    }

    ~RouteGraph() {
        for (int i = 0; i < MAX_LOCATIONS; i++) {
            Node* current = head[i];
            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};
// MAIN: CONNECTS ALL FOUR MODULES

int main() {
    Store store;
    Cart cart;
    PriorityQueue pendingDeliveries;
    CarrierList carriers;
    RouteGraph graph;

    Delivery* allDeliveries[MAX_DELIVERIES];
    int deliveryCount = 0;
    int nextDeliveryID = 1001;

    Carrier c1(201, "Aman", "College Gate");
    Carrier c2(202, "Riya", "Main Market");
    Carrier c3(203, "Rahul", "Library");

    carriers.insertCarrier(&c1);
    carriers.insertCarrier(&c2);
    carriers.insertCarrier(&c3);

    // Sample roads between locations A-F.
    graph.addRoad(0, 1, 2);
    graph.addRoad(0, 2, 4);
    graph.addRoad(1, 2, 1);
    graph.addRoad(1, 3, 3);
    graph.addRoad(2, 4, 2);
    graph.addRoad(3, 5, 2);
    graph.addRoad(4, 5, 3);

    int choice;

    do {
        cout << "\n===== SMART DELIVERY MANAGEMENT SYSTEM =====\n";
        cout << "1. Module 1 - View Store Products\n";
        cout << "2. Module 1 - Add Product to Cart\n";
        cout << "3. Module 1 - View Cart\n";
        cout << "4. Module 1 - Place Store Order\n";
        cout << "5. Module 2 - Create Package Delivery\n";
        cout << "6. Module 3 - Display Pending Deliveries\n";
        cout << "7. Module 3 - Display Carriers\n";
        cout << "8. Module 3 - Process Highest-Priority Request\n";
        cout << "9. Module 3 - Display All Deliveries\n";
        cout << "10. Module 4 - Find Shortest Route\n";
        cout << "11. Module 4 - Block a Road\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            store.displayProducts();
        }
        else if (choice == 2) {
            int id, quantity;
            store.displayProducts();
            cout << "Enter product ID: ";
            cin >> id;
            cout << "Enter quantity: ";
            cin >> quantity;

            int index = store.findProduct(id);
            if (index == -1)
                cout << "Product not found.\n";
            else
                cart.addProduct(store.getProduct(index), quantity);
        }
        else if (choice == 3) {
            cart.displayCart();
        }
        else if (choice == 4) {
            if (cart.isEmpty()) {
                cout << "Add a product to the cart first.\n";
                continue;
            }
            if (deliveryCount == MAX_DELIVERIES) {
                cout << "Delivery storage is full.\n";
                continue;
            }

            string destination;
            cout << "Enter delivery destination (one word): ";
            cin >> destination;

            Delivery* d = new Delivery(
                nextDeliveryID, 2, "Store Order",
                cart.getOrderName(), "General Store", destination
            );

            if (pendingDeliveries.enqueue(d)) {
                allDeliveries[deliveryCount++] = d;
                cout << "Store order placed. Delivery ID: "
                     << nextDeliveryID << "\n";
                cout << "Cart total: Rs. " << cart.getTotal() << "\n";
                nextDeliveryID++;
                cart.clearCart();
            } else {
                delete d;
            }
        }
        else if (choice == 5) {
            if (deliveryCount == MAX_DELIVERIES) {
                cout << "Delivery storage is full.\n";
                continue;
            }

            int senderID, receiverID, priority;
            string senderName, receiverName, item, pickup, destination;

            cout << "Sender ID and name: ";
            cin >> senderID >> senderName;
            cout << "Receiver ID and name: ";
            cin >> receiverID >> receiverName;
            cout << "Item to send: ";
            cin >> item;
            cout << "Pickup location: ";
            cin >> pickup;
            cout << "Destination: ";
            cin >> destination;
            cout << "Priority (1=normal, 2=medium, 3=urgent): ";
            cin >> priority;

            if (priority < 1 || priority > 3) {
                cout << "Priority must be from 1 to 3.\n";
                continue;
            }

            Sender sender(senderID, senderName);
            Receiver receiver(receiverID, receiverName);
            sender.createRequest();
            receiver.trackDelivery();

            Delivery* d = new Delivery(
                nextDeliveryID, priority, "Package Delivery",
                item, pickup, destination
            );

            if (pendingDeliveries.enqueue(d)) {
                allDeliveries[deliveryCount++] = d;
                cout << "Package request added. Delivery ID: "
                     << nextDeliveryID << "\n";
                nextDeliveryID++;
            } else {
                delete d;
            }
        }
        else if (choice == 6) {
            pendingDeliveries.displayQueue();
        }
        else if (choice == 7) {
            carriers.displayCarriers();
        }
        else if (choice == 8) {
            if (pendingDeliveries.isEmpty()) {
                cout << "No pending deliveries.\n";
                continue;
            }

            Carrier* carrier = carriers.findAvailableCarrier();
            if (carrier == NULL) {
                cout << "No available carriers right now.\n";
                continue;
            }

            Delivery* d = pendingDeliveries.dequeue();
            if (d != NULL) {
                d->setStatus("Assigned");
                carrier->setAvailable(false);

                cout << "Highest-priority delivery assigned.\n";
                d->display();
                cout << "Assigned carrier:\n";
                carrier->displayDetails();

                // This is a basic Phase 2 assignment demonstration.
                // Full accept/reject/completion workflow is still pending.
            }
        }
        else if (choice == 9) {
            if (deliveryCount == 0) {
                cout << "No deliveries created yet.\n";
            } else {
                for (int i = 0; i < deliveryCount; i++)
                    allDeliveries[i]->display();
            }
        }
        else if (choice == 10) {
            int source, destination;
            cout << "Enter source (0=A, 1=B, 2=C, 3=D, 4=E, 5=F): ";
            cin >> source;
            cout << "Enter destination (0=A, 1=B, 2=C, 3=D, 4=E, 5=F): ";
            cin >> destination;

            if (source < 0 || source >= MAX_LOCATIONS ||
                destination < 0 || destination >= MAX_LOCATIONS) {
                cout << "Invalid location.\n";
            } else {
                graph.shortestRoute(source, destination);
            }
        }
        else if (choice == 11) {
            int from, to;
            cout << "Enter road endpoints (0=A, 1=B, 2=C, 3=D, 4=E, 5=F): ";
            cin >> from >> to;

            if (from < 0 || from >= MAX_LOCATIONS ||
                to < 0 || to >= MAX_LOCATIONS || from == to) {
                cout << "Invalid road.\n";
            } else {
                graph.blockRoad(from, to);
            }
        }
        else if (choice != 0) {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    for (int i = 0; i < deliveryCount; i++)
        delete allDeliveries[i];

    cout << "Exiting system.\n";
    return 0;
}
