#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;

struct Transaction {
    string date;       // format: YYYY-MM-DD
    string type;       // Income / Expense
    string description;
    float amount;
};

class FinanceTracker {
private:
    vector<Transaction> transactions;

public:
    void addTransaction() {
        Transaction t;
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> t.date;
        cout << "Enter type (Income/Expense): ";
        cin >> t.type;
        cin.ignore();
        cout << "Enter description: ";
        getline(cin, t.description);
        cout << "Enter amount: ";
        cin >> t.amount;

        transactions.push_back(t);
        cout << "Transaction added successfully!\n";
    }

    void viewTransactions() {
        if (transactions.empty()) {
            cout << "No transactions found.\n";
            return;
        }

        cout << "\n--- All Transactions ---\n";
        for (size_t i = 0; i < transactions.size(); i++) {
            cout << i + 1 << ". Date: " << transactions[i].date
                 << " | Type: " << transactions[i].type
                 << " | Amount: " << fixed << setprecision(2) << transactions[i].amount
                 << " | Description: " << transactions[i].description << "\n";
        }
    }

    void searchTransactions() {
        float limit;
        cout << "Enter amount limit to search (e.g., 100): ";
        cin >> limit;

        cout << "\n--- Transactions above " << limit << " ---\n";
        for (auto &t : transactions) {
            if (t.amount > limit) {
                cout << "Date: " << t.date
                     << " | Type: " << t.type
                     << " | Amount: " << t.amount
                     << " | Desc: " << t.description << "\n";
            }
        }
    }

    void sortTransactions() {
        sort(transactions.begin(), transactions.end(),
             [](Transaction &a, Transaction &b) {
                 return a.amount < b.amount;
             });
        cout << "Transactions sorted by amount!\n";
    }

    void saveToFile() {
        ofstream fout("finance.dat");
        if (!fout) {
            cout << "Error saving file!\n";
            return;
        }
        for (auto &t : transactions) {
            fout << t.date << "|" << t.type << "|" << t.amount << "|" << t.description << "\n";
        }
        fout.close();
        cout << "Data saved successfully!\n";
    }

    void loadFromFile() {
        ifstream fin("finance.dat");
        if (!fin) {
            cout << "No saved data found!\n";
            return;
        }
        transactions.clear();
        string line;
        while (getline(fin, line)) {
            stringstream ss(line);
            Transaction t;
            string amt;
            getline(ss, t.date, '|');
            getline(ss, t.type, '|');
            getline(ss, amt, '|');
            getline(ss, t.description, '|');
            t.amount = stof(amt);
            transactions.push_back(t);
        }
        fin.close();
        cout << "Data loaded successfully!\n";
    }

    void monthlyReport() {
        float monthSum[13] = {0};

        for (auto &t : transactions) {
            int month;
            sscanf(t.date.c_str(), "%*d-%d-%*d", &month);
            monthSum[month] += t.amount;
        }

        cout << "\n--- Monthly Spending Report ---\n";
        for (int m = 1; m <= 12; m++) {
            if (monthSum[m] > 0) {
                cout << "Month " << setw(2) << m << " : ";
                int stars = (int)(monthSum[m] / 100); // 1 star = 100 units
                for (int k = 0; k < stars; k++) cout << "*";
                cout << " (" << fixed << setprecision(2) << monthSum[m] << ")\n";
            }
        }
    }
};

int main() {
    FinanceTracker tracker;
    int choice;

    while (true) {
        cout << "\n===== Personal Finance Tracker =====\n";
        cout << "1. Add Transaction\n";
        cout << "2. View All Transactions\n";
        cout << "3. Search Transactions\n";
        cout << "4. Sort Transactions by Amount\n";
        cout << "5. Save to File\n";
        cout << "6. Load from File\n";
        cout << "7. Monthly Report\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: tracker.addTransaction(); break;
            case 2: tracker.viewTransactions(); break;
            case 3: tracker.searchTransactions(); break;
            case 4: tracker.sortTransactions(); break;
            case 5: tracker.saveToFile(); break;
            case 6: tracker.loadFromFile(); break;
            case 7: tracker.monthlyReport(); break;
            case 8: exit(0);
            default: cout << "Invalid choice!\n";
        }
    }
    return 0;
}
