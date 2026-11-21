#include <iostream>
using namespace std;

// 1. PRODUCT CLASS
class Product
{
private:
    int productID;
    char productName[30];
    float price;
    int stock;

public:
    void setProduct(int id, const char name[],
                    float p, int s)
    {
        productID = id;
        price = p;
        stock = s;

        int i = 0;

        while (name[i] != '\0' && i < 29)
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

    void displayProduct()
    {
        cout << "\nProduct ID: " << productID;
        cout << "\nName: " << productName;
        cout << "\nPrice: Rs. " << price;
        cout << "\nStock: " << stock << "\n";
    }
};


// 2. STORE CLASS
class Store
{
private:
    int storeID;
    char storeName[30];
    Product products[10];
    int productCount;

public:
    Store()
    {
        productCount = 0;
    }

    void setStore(int id, const char name[])
    {
        storeID = id;

        int i = 0;

        while (name[i] != '\0' && i < 29)
        {
            storeName[i] = name[i];
            i++;
        }

        storeName[i] = '\0';
    }

    void addProduct(Product p)
    {
        if (productCount < 10)
        {
            products[productCount] = p;
            productCount++;
        }
    }

    void displayStore()
    {
        cout << "\nStore ID: " << storeID;
        cout << "\nStore Name: " << storeName << "\n";
    }

    void displayProducts()
    {
        for (int i = 0; i < productCount; i++)
        {
            products[i].displayProduct();
        }
    }

    int findProduct(int id)
    {
        for (int i = 0; i < productCount; i++)
        {
            if (products[i].getID() == id)
                return i;
        }

        return -1;
    }

    Product getProduct(int index)
    {
        return products[index];
    }
};


// 3. CART CLASS
class Cart
{
private:
    int productIDs[10];
    char productNames[10][30];
    float prices[10];
    int quantities[10];
    int count;

public:
    Cart()
    {
        count = 0;
    }

    void addProduct(Product p, int quantity)
    {
        if (quantity <= 0)
        {
            cout << "\nInvalid quantity.\n";
            return;
        }

        if (quantity > p.getStock())
        {
            cout << "\nInsufficient stock.\n";
            return;
        }

        if (count >= 10)
        {
            cout << "\nCart is full.\n";
            return;
        }

        productIDs[count] = p.getID();
        prices[count] = p.getPrice();
        quantities[count] = quantity;

        int i = 0;

        while (p.getName()[i] != '\0' && i < 29)
        {
            productNames[count][i] = p.getName()[i];
            i++;
        }

        productNames[count][i] = '\0';

        count++;

        cout << "\nProduct added to cart.\n";
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

    void displayCart()
    {
        if (count == 0)
        {
            cout << "\nCart is empty.\n";
            return;
        }

        for (int i = 0; i < count; i++)
        {
            cout << "\nProduct: " << productNames[i];
            cout << "\nQuantity: " << quantities[i];
            cout << "\nSubtotal: Rs. "
                 << prices[i] * quantities[i] << "\n";
        }

        cout << "\nTotal: Rs. " << calculateTotal() << "\n";
    }
};


// 4. ORDER CLASS — BASIC STRUCTURE ONLY
class Order
{
protected:
    int orderID;
    float totalAmount;
    char status[20];

public:
    Order()
    {
        orderID = 0;
        totalAmount = 0;

        status[0] = 'P';
        status[1] = 'e';
        status[2] = 'n';
        status[3] = 'd';
        status[4] = 'i';
        status[5] = 'n';
        status[6] = 'g';
        status[7] = '\0';
    }

    void displayOrder()
    {
        cout << "\nOrder ID: " << orderID;
        cout << "\nAmount: Rs. " << totalAmount;
        cout << "\nStatus: " << status << "\n";
    }
};


// 5. MAIN FUNCTION
int main()
{
    Store store;

    Product p1, p2, p3;

    p1.setProduct(101, "Rice", 60, 20);
    p2.setProduct(102, "Milk", 30, 15);
    p3.setProduct(103, "Bread", 40, 10);

    store.setStore(1, "General Store");

    store.addProduct(p1);
    store.addProduct(p2);
    store.addProduct(p3);

    Cart cart;

    int choice;

    do
    {
        cout << "\n===== STORE ORDERING SYSTEM =====\n";
        cout << "1. Display Store\n";
        cout << "2. View Products\n";
        cout << "3. Add Product to Cart\n";
        cout << "4. View Cart\n";
        cout << "0. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                store.displayStore();
                break;

            case 2:
                store.displayProducts();
                break;

            case 3:
            {
                int id, quantity;

                cout << "Enter Product ID: ";
                cin >> id;

                cout << "Enter Quantity: ";
                cin >> quantity;

                int index = store.findProduct(id);

                if (index == -1)
                {
                    cout << "\nProduct not found.\n";
                }
                else
                {
                    Product p = store.getProduct(index);
                    cart.addProduct(p, quantity);
                }

                break;
            }

            case 4:
                cart.displayCart();
                break;

            case 0:
                cout << "\nExiting system.\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}