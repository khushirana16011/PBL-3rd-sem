#include <iostream>
using namespace std;
//select 2 to view products
//select 3 to add products
//select 5 to view cart
//select 6 to place an order
//select 7 to view the cart
//select 8 tp process a delivery
//select 0 to exit



// SERIAL 1: PRODUCT CLASS
class Product
{
private:
    int productID;
    char productName[50];
    float price;
    int stock;

public:
    Product()
    {
        productID = 0;
        price = 0;
        stock = 0;
        productName[0] = '\0';
    }

    void setProduct(int id, const char name[], float p, int s)
    {
        productID = id;
        price = p;
        stock = s;

        int i = 0;

        while (name[i] != '\0' && i < 49)
        {
            productName[i] = name[i];
            i++;
        }

        productName[i] = '\0';
    }

    int getID()
    {
        return productID;
    }

    float getPrice()
    {
        return price;
    }

    int getStock()
    {
        return stock;
    }

    const char* getName()
    {
        return productName;
    }

    void reduceStock(int quantity)
    {
        if (quantity > 0 && quantity <= stock)
        {
            stock = stock - quantity;
        }
    }

    void displayProduct()
    {
        cout << "\nProduct ID: " << productID;
        cout << "\nProduct Name: " << productName;
        cout << "\nPrice: Rs. " << price;
        cout << "\nAvailable Stock: " << stock;
        cout << "\n";
    }
};


// ==================================================
// SERIAL 2: STORE CLASS
// ==================================================

class Store
{
private:
    int storeID;
    char storeName[50];
    char location[50];

    Product products[10];
    int productCount;

public:
    Store()
    {
        storeID = 0;
        productCount = 0;

        storeName[0] = '\0';
        location[0] = '\0';
    }

    void setStore(int id, const char name[], const char loc[])
    {
        storeID = id;

        int i = 0;

        while (name[i] != '\0' && i < 49)
        {
            storeName[i] = name[i];
            i++;
        }

        storeName[i] = '\0';

        i = 0;

        while (loc[i] != '\0' && i < 49)
        {
            location[i] = loc[i];
            i++;
        }

        location[i] = '\0';
    }

    void addProduct(Product p)
    {
        if (productCount < 10)
        {
            products[productCount] = p;
            productCount++;
        }
    }

    int getStoreID()
    {
        return storeID;
    }

    int getProductCount()
    {
        return productCount;
    }

    Product getProduct(int index)
    {
        return products[index];
    }

    void updateProduct(int index, Product p)
    {
        if (index >= 0 && index < productCount)
        {
            products[index] = p;
        }
    }

    int findProduct(int id)
    {
        for (int i = 0; i < productCount; i++)
        {
            if (products[i].getID() == id)
            {
                return i;
            }
        }

        return -1;
    }

    void displayStore()
    {
        cout << "\nStore ID: " << storeID;
        cout << "\nStore Name: " << storeName;
        cout << "\nLocation: " << location;
        cout << "\n";
    }

    void displayProducts()
    {
        cout << "\n========== AVAILABLE PRODUCTS ==========\n";

        for (int i = 0; i < productCount; i++)
        {
            products[i].displayProduct();
        }
    }
};


// ==================================================
// SERIAL 3: CUSTOMER CLASS
// ==================================================

class Customer
{
private:
    int customerID;
    char customerName[50];

public:
    Customer()
    {
        customerID = 0;
        customerName[0] = '\0';
    }

    void setCustomer(int id, const char name[])
    {
        customerID = id;

        int i = 0;

        while (name[i] != '\0' && i < 49)
        {
            customerName[i] = name[i];
            i++;
        }

        customerName[i] = '\0';
    }

    int getCustomerID()
    {
        return customerID;
    }

    const char* getName()
    {
        return customerName;
    }

    void displayCustomer()
    {
        cout << "\nCustomer ID: " << customerID;
        cout << "\nCustomer Name: " << customerName;
        cout << "\n";
    }
};


// ==================================================
// SERIAL 4: CART CLASS
// ==================================================

class Cart
{
private:
    int productIDs[20];
    char productNames[20][50];

    float prices[20];
    int quantities[20];

    int count;

public:
    Cart()
    {
        count = 0;
    }

    int getCount()
    {
        return count;
    }

    int getQuantity(int id)
    {
        for (int i = 0; i < count; i++)
        {
            if (productIDs[i] == id)
            {
                return quantities[i];
            }
        }

        return 0;
    }

    int getQuantityAt(int index)
    {
        return quantities[index];
    }

    int getIDAt(int index)
    {
        return productIDs[index];
    }

    float calculateTotal()
    {
        float total = 0;

        for (int i = 0; i < count; i++)
        {
            total = total + prices[i] * quantities[i];
        }

        return total;
    }

    void addProduct(int id, const char name[],
                    float price, int quantity, int availableStock)
    {
        if (quantity <= 0)
        {
            cout << "\nQuantity must be greater than zero.\n";
            return;
        }

        int position = -1;

        for (int i = 0; i < count; i++)
        {
            if (productIDs[i] == id)
            {
                position = i;
                break;
            }
        }

        int alreadyInCart = 0;

        if (position != -1)
        {
            alreadyInCart = quantities[position];
        }

        if (quantity > availableStock - alreadyInCart)
        {
            cout << "\nInsufficient stock for this quantity.\n";
            return;
        }

        if (position != -1)
        {
            quantities[position] =
                quantities[position] + quantity;

            cout << "\nProduct quantity increased in cart.\n";
            return;
        }

        if (count >= 20)
        {
            cout << "\nCart is full.\n";
            return;
        }

        productIDs[count] = id;
        prices[count] = price;
        quantities[count] = quantity;

        int i = 0;

        while (name[i] != '\0' && i < 49)
        {
            productNames[count][i] = name[i];
            i++;
        }

        productNames[count][i] = '\0';

        count++;

        cout << "\nProduct added successfully.\n";
    }

    void removeProduct(int id)
    {
        int position = -1;

        for (int i = 0; i < count; i++)
        {
            if (productIDs[i] == id)
            {
                position = i;
                break;
            }
        }

        if (position == -1)
        {
            cout << "\nProduct not found in cart.\n";
            return;
        }

        for (int i = position; i < count - 1; i++)
        {
            productIDs[i] = productIDs[i + 1];

            prices[i] = prices[i + 1];

            quantities[i] = quantities[i + 1];

            int j = 0;

            while (productNames[i + 1][j] != '\0')
            {
                productNames[i][j] = productNames[i + 1][j];
                j++;
            }

            productNames[i][j] = '\0';
        }

        count--;

        cout << "\nProduct removed successfully.\n";
    }

    void displayCart()
    {
        if (count == 0)
        {
            cout << "\nYour cart is empty.\n";
            return;
        }

        cout << "\n========== YOUR CART ==========\n";

        for (int i = 0; i < count; i++)
        {
            cout << "\nProduct ID: " << productIDs[i];

            cout << "\nProduct Name: "
                 << productNames[i];

            cout << "\nPrice: Rs. " << prices[i];

            cout << "\nQuantity: " << quantities[i];

            cout << "\nSubtotal: Rs. "
                 << prices[i] * quantities[i];

            cout << "\n";
        }

        cout << "\nTotal Amount: Rs. "
             << calculateTotal() << "\n";
    }

    void clearCart()
    {
        count = 0;
    }
};


// ==================================================
// SERIAL 5: ORDER BASE CLASS
// ==================================================

class Order
{
protected:
    int orderID;
    int customerID;

    char customerName[50];

    float totalAmount;

    int priority;

    char status[30];

public:
    Order()
    {
        orderID = 0;
        customerID = 0;
        totalAmount = 0;
        priority = 1;

        customerName[0] = '\0';

        status[0] = 'P';
        status[1] = 'e';
        status[2] = 'n';
        status[3] = 'd';
        status[4] = 'i';
        status[5] = 'n';
        status[6] = 'g';
        status[7] = '\0';
    }

    void setOrder(int id, int cid, const char name[],
                  float amount, int p)
    {
        orderID = id;
        customerID = cid;
        totalAmount = amount;
        priority = p;

        int i = 0;

        while (name[i] != '\0' && i < 49)
        {
            customerName[i] = name[i];
            i++;
        }

        customerName[i] = '\0';
    }

    int getOrderID()
    {
        return orderID;
    }

    int getPriority()
    {
        return priority;
    }

    float getTotalAmount()
    {
        return totalAmount;
    }

    void setStatus(const char newStatus[])
    {
        int i = 0;

        while (newStatus[i] != '\0' && i < 29)
        {
            status[i] = newStatus[i];
            i++;
        }

        status[i] = '\0';
    }

    virtual void displayOrder()
    {
        cout << "\nOrder ID: " << orderID;
        cout << "\nCustomer ID: " << customerID;
        cout << "\nCustomer Name: " << customerName;

        cout << "\nTotal Amount: Rs. "
             << totalAmount;

        cout << "\nPriority: " << priority;
        cout << "\nStatus: " << status << "\n";
    }

    virtual ~Order()
    {
    }
};


// ==================================================
// SERIAL 6: STORE ORDER CLASS
// ==================================================

class StoreOrder : public Order
{
private:
    int storeID;
    int itemCount;

    int itemIDs[20];
    int itemQuantities[20];

public:
    StoreOrder()
    {
        storeID = 0;
        itemCount = 0;
    }

    void setStoreOrder(int sid, Cart &cart)
    {
        storeID = sid;

        itemCount = cart.getCount();

        for (int i = 0; i < itemCount; i++)
        {
            itemIDs[i] = cart.getIDAt(i);

            itemQuantities[i] = cart.getQuantityAt(i);
        }
    }

    void displayOrder()
    {
        cout << "\n========== STORE ORDER ==========\n";

        Order::displayOrder();

        cout << "Store ID: " << storeID;

        cout << "\nOrdered Items:\n";

        for (int i = 0; i < itemCount; i++)
        {
            cout << "Product ID: " << itemIDs[i];

            cout << ", Quantity: "
                 << itemQuantities[i] << "\n";
        }
    }
};


// ==================================================
// SERIAL 7: SENDER CLASS
// ==================================================

class Sender
{
private:
    char senderName[50];

public:
    Sender()
    {
        senderName[0] = '\0';
    }

    void setSender(const char name[])
    {
        int i = 0;

        while (name[i] != '\0' && i < 49)
        {
            senderName[i] = name[i];
            i++;
        }

        senderName[i] = '\0';
    }

    const char* getName()
    {
        return senderName;
    }
};


// ==================================================
// SERIAL 8: RECEIVER CLASS
// ==================================================

class Receiver
{
private:
    char receiverName[50];

public:
    Receiver()
    {
        receiverName[0] = '\0';
    }

    void setReceiver(const char name[])
    {
        int i = 0;

        while (name[i] != '\0' && i < 49)
        {
            receiverName[i] = name[i];
            i++;
        }

        receiverName[i] = '\0';
    }

    const char* getName()
    {
        return receiverName;
    }
};


// ==================================================
// SERIAL 9: DELIVERY REQUEST CLASS
// ==================================================

class DeliveryRequest
{
private:
    int requestID;
    int orderID;
    int priority;

    char senderName[50];
    char receiverName[50];

    float amount;

    char deliveryStatus[30];

public:
    DeliveryRequest()
    {
        requestID = 0;
        orderID = 0;
        priority = 1;
        amount = 0;

        senderName[0] = '\0';
        receiverName[0] = '\0';

        deliveryStatus[0] = 'W';
        deliveryStatus[1] = 'a';
        deliveryStatus[2] = 'i';
        deliveryStatus[3] = 't';
        deliveryStatus[4] = 'i';
        deliveryStatus[5] = 'n';
        deliveryStatus[6] = 'g';
        deliveryStatus[7] = '\0';
    }

    void setRequest(int rid, int oid, int p, float total,
                    const char sender[], const char receiver[])
    {
        requestID = rid;
        orderID = oid;
        priority = p;
        amount = total;

        int i = 0;

        while (sender[i] != '\0' && i < 49)
        {
            senderName[i] = sender[i];
            i++;
        }

        senderName[i] = '\0';

        i = 0;

        while (receiver[i] != '\0' && i < 49)
        {
            receiverName[i] = receiver[i];
            i++;
        }

        receiverName[i] = '\0';
    }

    int getPriority()
    {
        return priority;
    }

    int getOrderID()
    {
        return orderID;
    }

    void setDeliveryStatus(const char newStatus[])
    {
        int i = 0;

        while (newStatus[i] != '\0' && i < 29)
        {
            deliveryStatus[i] = newStatus[i];
            i++;
        }

        deliveryStatus[i] = '\0';
    }

    void displayRequest()
    {
        cout << "\nRequest ID: " << requestID;

        cout << "\nOrder ID: " << orderID;

        cout << "\nPriority: ";

        if (priority == 2)
        {
            cout << "High";
        }
        else
        {
            cout << "Normal";
        }

        cout << "\nSender: " << senderName;

        cout << "\nReceiver: " << receiverName;

        cout << "\nAmount: Rs. " << amount;

        cout << "\nDelivery Status: "
             << deliveryStatus << "\n";
    }
};


// ==================================================
// SERIAL 10: PRIORITY-AWARE DELIVERY QUEUE
// ==================================================

class DeliveryQueue
{
private:
    DeliveryRequest requests[50];

    int count;

public:
    DeliveryQueue()
    {
        count = 0;
    }

    bool isEmpty()
    {
        return count == 0;
    }

    void enqueue(DeliveryRequest request)
    {
        if (count >= 50)
        {
            cout << "\nDelivery queue is full.\n";
            return;
        }

        int position = count;

        // Higher priority requests come first.
        // Equal priorities retain arrival order.

        while (position > 0 &&
               requests[position - 1].getPriority()
                    < request.getPriority())
        {
            requests[position] = requests[position - 1];

            position--;
        }

        requests[position] = request;

        count++;

        cout << "\nDelivery request added successfully.\n";
    }

    void dequeue()
    {
        if (count == 0)
        {
            cout << "\nNo delivery requests available.\n";
            return;
        }

        cout << "\n========== PROCESSING DELIVERY ==========\n";

        requests[0].displayRequest();

        requests[0].setDeliveryStatus("Delivered");

        cout << "\nDelivery processed successfully.\n";

        for (int i = 0; i < count - 1; i++)
        {
            requests[i] = requests[i + 1];
        }

        count--;
    }

    void displayQueue()
    {
        if (count == 0)
        {
            cout << "\nDelivery queue is empty.\n";
            return;
        }

        cout << "\n========== DELIVERY QUEUE ==========\n";

        for (int i = 0; i < count; i++)
        {
            cout << "\nPosition: " << i + 1;

            requests[i].displayRequest();
        }
    }
};


// ==================================================
// SERIAL 11: MAIN FUNCTION
// ==================================================

int main()
{
    Store store;

    Product p1, p2, p3, p4;

    p1.setProduct(101, "Rice", 60, 50);
    p2.setProduct(102, "Milk", 30, 40);
    p3.setProduct(103, "Bread", 40, 30);
    p4.setProduct(104, "Sugar", 45, 25);

    store.setStore(1, "General Store", "Dehradun");

    store.addProduct(p1);
    store.addProduct(p2);
    store.addProduct(p3);
    store.addProduct(p4);

    Customer customer;

    char customerName[50];

    cout << "========== STORE ORDERING SYSTEM ==========\n";

    cout << "\nEnter Customer Name: ";

    cin.getline(customerName, 50);

    customer.setCustomer(1, customerName);

    Cart cart;

    DeliveryQueue deliveryQueue;

    int choice;

    int orderCounter = 1001;

    Sender sender;

    sender.setSender("General Store");

    Receiver receiver;

    receiver.setReceiver(customerName);

    do
    {
        cout << "\n\n========== MAIN MENU ==========\n";

        cout << "1. Display Store\n";
        cout << "2. View Products\n";
        cout << "3. Add Product to Cart\n";
        cout << "4. Remove Product from Cart\n";
        cout << "5. View Cart\n";
        cout << "6. Place Order\n";
        cout << "7. View Delivery Queue\n";
        cout << "8. Process Delivery\n";
        cout << "9. View Customer Details\n";
        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";

        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                store.displayStore();

                break;
            }

            case 2:
            {
                store.displayProducts();

                break;
            }

            case 3:
            {
                int id;
                int quantity;

                cout << "\nEnter Product ID: ";
                cin >> id;

                cout << "Enter Quantity: ";
                cin >> quantity;

                int index = store.findProduct(id);

                if (index == -1)
                {
                    cout << "\nProduct not found.\n";
                    break;
                }

                Product p = store.getProduct(index);

                int available = p.getStock();

                cart.addProduct(
                    p.getID(),
                    p.getName(),
                    p.getPrice(),
                    quantity,
                    available
                );

                break;
            }

            case 4:
            {
                int id;

                cout << "\nEnter Product ID to remove: ";

                cin >> id;

                cart.removeProduct(id);

                break;
            }

            case 5:
            {
                cart.displayCart();

                break;
            }

            case 6:
            {
                if (cart.getCount() == 0)
                {
                    cout << "\nYour cart is empty.\n";

                    break;
                }

                int priority;

                cout << "\nSelect Delivery Priority:\n";

                cout << "1. Normal\n";
                cout << "2. High\n";

                cout << "Enter priority: ";

                cin >> priority;

                if (priority != 1 && priority != 2)
                {
                    cout << "\nInvalid priority.\n";

                    break;
                }

                // Check every product before changing stock.

                int valid = 1;

                for (int i = 0; i < cart.getCount(); i++)
                {
                    int index = store.findProduct(
                        cart.getIDAt(i)
                    );

                    if (index == -1)
                    {
                        valid = 0;
                        break;
                    }

                    Product p = store.getProduct(index);

                    if (cart.getQuantityAt(i) > p.getStock())
                    {
                        valid = 0;
                        break;
                    }
                }

                if (!valid)
                {
                    cout << "\nOrder cannot be placed because "
                         << "stock is insufficient.\n";

                    break;
                }

                // Create the order.

                StoreOrder order;

                order.setOrder(
                    orderCounter,
                    customer.getCustomerID(),
                    customer.getName(),
                    cart.calculateTotal(),
                    priority
                );

                order.setStoreOrder(
                    store.getStoreID(),
                    cart
                );

                order.displayOrder();

                // Create the delivery request.

                DeliveryRequest request;

                request.setRequest(
                    orderCounter,
                    orderCounter,
                    priority,
                    cart.calculateTotal(),
                    sender.getName(),
                    receiver.getName()
                );

                // Add the delivery request.

                deliveryQueue.enqueue(request);

                // Deduct stock after successful queue insertion.

                // The queue can reject an insertion if full.
                // Therefore, check queue capacity before ordering
                // in a real system. This example uses the queue
                // display and the fixed-capacity design below.

                // Update inventory.

                for (int i = 0; i < cart.getCount(); i++)
                {
                    int index = store.findProduct(
                        cart.getIDAt(i)
                    );

                    Product p = store.getProduct(index);

                    p.reduceStock(cart.getQuantityAt(i));

                    store.updateProduct(index, p);
                }

                cout << "\nOrder placed successfully!";

                cout << "\nOrder ID: " << orderCounter;

                cout << "\nTotal Amount: Rs. "
                     << cart.calculateTotal() << "\n";

                orderCounter++;

                cart.clearCart();

                break;
            }

            case 7:
            {
                deliveryQueue.displayQueue();

                break;
            }

            case 8:
            {
                deliveryQueue.dequeue();

                break;
            }

            case 9:
            {
                customer.displayCustomer();

                break;
            }

            case 0:
            {
                cout << "\nThank you for using "
                     << "the Store Ordering System!\n";

                break;
            }

            default:
            {
                cout << "\nInvalid choice. Please try again.\n";
            }
        }

    } while (choice != 0);

    return 0;
}