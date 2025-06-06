#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Product {
private:
    int id;
    string name;
    double price;
    int stock;

public:
    Product(int id, string name, double price, int stock)
        : id(id), name(name), price(price), stock(stock) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    void setStock(int s) { stock = s; }

    void display() const {
        cout << setw(4) << id << " | " << setw(25) << left << name << " | PHP "
             << setw(8) << fixed << setprecision(2) << price << " | Stock: " << stock << endl;
    }
};

class CartItem {
public:
    Product product;
    int quantity;

    CartItem(Product p, int q) : product(p), quantity(q) {}

    double getTotalPrice() const {
        return product.getPrice() * quantity;
    }

    void display() const {
        cout << product.getName() << " x" << quantity
             << " = PHP " << fixed << setprecision(2) << getTotalPrice() << endl;
    }
};

class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
};

class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid PHP " << fixed << setprecision(2) << amount << " using Cash." << endl;
    }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid PHP " << fixed << setprecision(2) << amount << " using Credit Card." << endl;
    }
};

class Cart {
private:
    vector<CartItem> items;

public:
    void addToCart(Product product, int quantity) {
        if (quantity <= 0 || quantity > product.getStock()) {
            cout << "Invalid quantity or not enough stock.\n";
            return;
        }
        for (auto& item : items) {
            if (item.product.getId() == product.getId()) {
                item.quantity += quantity;
                return;
            }
        }
        items.emplace_back(product, quantity);
    }

    void removeFromCart(int id) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->product.getId() == id) {
                items.erase(it);
                cout << "Item removed from cart.\n";
                return;
            }
        }
        cout << "Item not found in cart.\n";
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.getTotalPrice();
        }
        return total;
    }

    void checkout(PaymentStrategy* strategy) {
        if (items.empty()) {
            cout << "Cart is empty.\n";
            return;
        }
        double total = calculateTotal();
        strategy->pay(total);
        cout << "\n----- Receipt -----\n";
        for (const auto& item : items) {
            item.display();
        }
        cout << "Total: PHP " << fixed << setprecision(2) << total << endl;
        cout << "-------------------\n";
        items.clear();
    }

    void displayCart() const {
        if (items.empty()) {
            cout << "Cart is empty.\n";
            return;
        }
        cout << "\nItems in Cart:\n";
        for (const auto& item : items) {
            item.display();
        }
        cout << "Total: PHP " << fixed << setprecision(2) << calculateTotal() << "\n";
    }
};

int main() {
    vector<Product> products = {
        Product(1, "Wireless Mouse", 850.00, 20),
        Product(2, "Mechanical Keyboard", 3500.00, 15),
        Product(3, "USB-C Hub", 1200.00, 10),
        Product(4, "Bluetooth Speaker", 2750.00, 8),
        Product(5, "Noise Cancelling Headphones", 6800.00, 5)
    };

    Cart cart;
    while (true) {
        cout << "\n--- Online Shopping Cart ---\n";
        cout << "1. View Products\n2. Add to Cart\n3. Remove from Cart\n4. View Cart\n5. Checkout\n6. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            for (const auto& product : products) product.display();
        } else if (choice == 2) {
            int id, quantity;
            cout << "Enter product ID: "; cin >> id;
            cout << "Enter quantity: "; cin >> quantity;
            bool found = false;
            for (auto& product : products) {
                if (product.getId() == id) {
                    cart.addToCart(product, quantity);
                    if (quantity > 0 && quantity <= product.getStock()) {
                        product.setStock(product.getStock() - quantity);
                    }
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Product not found.\n";
        } else if (choice == 3) {
            int id;
            cout << "Enter product ID to remove: "; cin >> id;
            cart.removeFromCart(id);
        } else if (choice == 4) {
            cart.displayCart();
        } else if (choice == 5) {
            cout << "Choose payment method:\n1. Cash\n2. Card\nChoice: ";
            int ptype; cin >> ptype;
            PaymentStrategy* payment = nullptr;
            if (ptype == 1) payment = new CashPayment();
            else if (ptype == 2) payment = new CardPayment();
            else {
                cout << "Invalid payment type.\n";
                continue;
            }
            cart.checkout(payment);
            delete payment;
        } else if (choice == 6) {
            cout << "Thank you for shopping with us!\n";
            break;
        } else {
            cout << "Invalid menu option.\n";
        }
    }

    return 0;
}
