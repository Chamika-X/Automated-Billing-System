#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Define user credentials
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin";

// Define a struct to hold company details
struct CompanyDetails {
    string companyName;
    string companyAddress;
    string contactEmail;
    string contactPhone;
};

struct BakeryItem {
    string name;
    double price;
    int quantity;
};

struct SalesRecord {
    string itemName;
    int quantitySold;
    double totalAmount;
};

class Bakery {
private:
    vector<BakeryItem> items;
    vector<SalesRecord> salesRecords;
    bool loggedIn;  // Track user login status
    CompanyDetails companyInfo; // Store company details

public:
    Bakery() : loggedIn(false) {
        // Initialize companyInfo with default values
        companyInfo.companyName = "Amber Bakery";
        companyInfo.companyAddress = "123 Bakery Street";
        companyInfo.contactEmail = "contact@amberbakery.com";
        companyInfo.contactPhone = "123-456-7890";

        // Load bakery items, sales records, and company details from files when the program starts
        loadItemsFromFile();
        loadSalesFromFile();
        loadCompanyDetailsFromFile();
        system("cls");
    }

    // User login function
    bool login() {
        string username, password;
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
            cout << "Login successful." << endl;
            return true;
        } else {
            cout << "Login failed. Please try again." << endl;
            return false;
        }
    }

    // Function to view and update company details
    void manageCompanyDetails() {
        cout << "Company Details:" << endl;
        cout << "1. View Company Details" << endl;
        cout << "2. Update Company Details" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Company Name: " << companyInfo.companyName << endl;
                cout << "Company Address: " << companyInfo.companyAddress << endl;
                cout << "Contact Email: " << companyInfo.contactEmail << endl;
                cout << "Contact Phone: " << companyInfo.contactPhone << endl;
                break;
            case 2:
                cout << "Enter the new company name: ";
                cin.ignore();
                getline(cin, companyInfo.companyName);
                cout << "Enter the new company address: ";
                getline(cin, companyInfo.companyAddress);
                cout << "Enter the new contact email: ";
                getline(cin, companyInfo.contactEmail);
                cout << "Enter the new contact phone: ";
                getline(cin, companyInfo.contactPhone);
                cout << "Company details updated successfully." << endl;
                // Save updated company details to file
                saveCompanyDetailsToFile();
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
                break;
        }
    }

    // Function to add a bakery item
    void addItem() {
        BakeryItem newItem;
        cout << "Enter the item name: ";
        cin.ignore();
        getline(cin, newItem.name);
        cout << "Enter the item price: ";
        cin >> newItem.price;
        cout << "Enter the item quantity: ";
        cin >> newItem.quantity;
        items.push_back(newItem);
        cout << "Item added successfully." << endl;

        // Save bakery items to file after adding a new item
        saveItemsToFile();
    }

    // Function to update a bakery item
    void updateItem() {
        string updateName;
        cout << "Enter the name of the item to update: ";
        cin.ignore();
        getline(cin, updateName);

        for (BakeryItem &item : items) {
            if (item.name == updateName) {
                cout << "Enter the updated price: ";
                cin >> item.price;
                cout << "Enter the updated quantity: ";
                cin >> item.quantity;
                cout << "Item updated successfully." << endl;

                // Save bakery items to file after updating an item
                saveItemsToFile();
                return;
            }
        }
        cout << "Item not found." << endl;
    }

    // Function to delete a bakery item
    void deleteItem() {
        string deleteName;
        cout << "Enter the name of the item to delete: ";
        cin.ignore();
        getline(cin, deleteName);

        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->name == deleteName) {
                items.erase(it);
                cout << "Item deleted successfully." << endl;

                // Save bakery items to file after deleting an item
                saveItemsToFile();
                return;
            }
        }
        cout << "Item not found." << endl;
    }

    // Function to view bakery item details
    void viewItemDetails() {
        if (items.empty()) {
            cout << "No items available." << endl;
        } else {
            cout << "Bakery Items Details:" << endl;
            for (const BakeryItem &item : items) {
                cout << "Name: " << item.name << ", Price: " << item.price << ", Quantity: " << item.quantity << endl;
            }
        }
    }

    // Function to add a sale
    void addSale() {
        if (items.empty()) {
            cout << "No items available for sale." << endl;
            return;
        }

        int sellIndex;
        cout << "Available Items for Sale:" << endl;
        for (int i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". " << items[i].name << " (Price: " << items[i].price << ", Quantity: " << items[i].quantity << ")" << endl;
        }

        cout << "Enter the number of the item to sell: ";
        cin >> sellIndex;
        sellIndex--;

        if (sellIndex >= 0 && sellIndex < items.size()) {
            int quantitySold;
            cout << "Enter the quantity to sell: ";
            cin >> quantitySold;

            if (quantitySold > 0 && quantitySold <= items[sellIndex].quantity) {
                double totalAmount = items[sellIndex].price * quantitySold;
                items[sellIndex].quantity -= quantitySold;

                SalesRecord sale;
                sale.itemName = items[sellIndex].name;
                sale.quantitySold = quantitySold;
                sale.totalAmount = totalAmount;
                salesRecords.push_back(sale);

                cout << "Sold " << quantitySold << " " << items[sellIndex].name << "(s) for a total of $" << totalAmount << "." << endl;

                // Save sales records and updated item quantity to files after making a sale
                saveSalesToFile();
                saveItemsToFile();
            } else {
                cout << "Invalid quantity." << endl;
            }
        } else {
            cout << "Invalid item number." << endl;
        }
    }

    // Function to view sales details
    void viewSalesDetails() {
        if (salesRecords.empty()) {
            cout << "No sales records available." << endl;
        } else {
            cout << "Sales Details:" << endl;
            for (const SalesRecord &sale : salesRecords) {
                cout << "Item: " << sale.itemName << ", Quantity Sold: " << sale.quantitySold << ", Total Amount: $" << sale.totalAmount << endl;
            }
            cout << "Total Sold Items: " << salesRecords.size() << endl;
        }
    }

    // Function to save bakery items to a text file
    void saveItemsToFile() {
        ofstream outputFile("bakery_items.txt");
        if (outputFile.is_open()) {
            for (const BakeryItem &item : items) {
                outputFile << item.name << "," << item.price << "," << item.quantity << endl;
            }
            outputFile.close();
            cout << "Bakery items saved to file." << endl;
        } else {
            cout << "Unable to open file for saving bakery items." << endl;
        }
    }

    // Function to load bakery items from a text file
    void loadItemsFromFile() {
        ifstream inputFile("bakery_items.txt");
        if (inputFile.is_open()) {
            items.clear();
            string line;
            while (getline(inputFile, line)) {
                BakeryItem item;
                stringstream ss(line);
                getline(ss, item.name, ',');
                ss >> item.price;
                ss.ignore();
                ss >> item.quantity;
                items.push_back(item);
            }
            inputFile.close();
            cout << "Bakery items loaded from file." << endl;
        } else {
            cout << "Unable to open file for loading bakery items." << endl;
        }
    }

    // Function to save sales records to a text file
    void saveSalesToFile() {
        ofstream outputFile("sales_records.txt");
        if (outputFile.is_open()) {
            for (const SalesRecord &sale : salesRecords) {
                outputFile << sale.itemName << "," << sale.quantitySold << "," << sale.totalAmount << endl;
            }
            outputFile.close();
            cout << "Sales records saved to file." << endl;
        } else {
            cout << "Unable to open file for saving sales records." << endl;
        }
    }

    // Function to load sales records from a text file
    void loadSalesFromFile() {
        ifstream inputFile("sales_records.txt");
        if (inputFile.is_open()) {
            salesRecords.clear();
            string line;
            while (getline(inputFile, line)) {
                SalesRecord sale;
                stringstream ss(line);
                getline(ss, sale.itemName, ',');
                ss >> sale.quantitySold;
                ss.ignore();
                ss >> sale.totalAmount;
                salesRecords.push_back(sale);
            }
            inputFile.close();
            cout << "Sales records loaded from file." << endl;
        } else {
            cout << "Unable to open file for loading sales records." << endl;
        }
    }

    // Function to save company details to a text file
    void saveCompanyDetailsToFile() {
        ofstream outputFile("company_details.txt");
        if (outputFile.is_open()) {
            outputFile << companyInfo.companyName << endl;
            outputFile << companyInfo.companyAddress << endl;
            outputFile << companyInfo.contactEmail << endl;
            outputFile << companyInfo.contactPhone << endl;
            outputFile.close();
            cout << "Company details saved to file." << endl;
        } else {
            cout << "Unable to open file for saving company details." << endl;
        }
    }

    // Function to load company details from a text file
    void loadCompanyDetailsFromFile() {
        ifstream inputFile("company_details.txt");
        if (inputFile.is_open()) {
            getline(inputFile, companyInfo.companyName);
            getline(inputFile, companyInfo.companyAddress);
            getline(inputFile, companyInfo.contactEmail);
            getline(inputFile, companyInfo.contactPhone);
            inputFile.close();
            cout << "Company details loaded from file." << endl;
        } else {
            cout << "Unable to open file for loading company details." << endl;
        }
    }

    // Main function to run the program
    void run() {
        while (true) {
            if (!loggedIn) {
                loggedIn = login();
                if (!loggedIn) {
                    continue;
                }
            }

            cout << "Welcome to " << companyInfo.companyName << " Automated Billing System" << endl;
            cout << "1. Add Item" << endl;
            cout << "2. Update Item" << endl;
            cout << "3. Delete Item" << endl;
            cout << "4. View Bakery Item Details" << endl;
            cout << "5. Add Sale" << endl;
            cout << "6. View Sales Details" << endl;
            cout << "7. Manage Company Details" << endl;
            cout << "8. Logout" << endl;
            cout << "9. Exit" << endl;
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    addItem();
                    break;
                case 2:
                    updateItem();
                    break;
                case 3:
                    deleteItem();
                    break;
                case 4:
                    viewItemDetails();
                    break;
                case 5:
                    addSale();
                    break;
                case 6:
                    viewSalesDetails();
                    break;
                case 7:
                    manageCompanyDetails();
                    break;
                case 8:
                    loggedIn = false;
                    cout << "Logged out." << endl;
                    break;
                case 9:
                    cout << "Exiting the program." << endl;
                    return;
                default:
                    cout << "Invalid choice. Please enter a valid option." << endl;
            }
        }
    }
};

int main() {
    Bakery bakery;
    bakery.run();

    return 0;
}
