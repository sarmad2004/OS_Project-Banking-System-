#include <iostream>
#include <string>
#include <mutex>
using namespace std;
const int MaxAcc = 1000;
struct Acc {
    int id;
    string custId;
    double bal;
    bool active;
};
class BankSys {
public:
    Acc accs[MaxAcc];
    int nextId = 1;
    int accCount = 0;
    mutex mtx; 
    Acc* find_acc(int id) {
        for (int i = 0; i < accCount; i++) {  // find an acc by id 
            if (accs[i].active && accs[i].id == id) {
                return &accs[i];
            }
        }
        return nullptr;
    }
    // System call to create an account
    int create_acc(const string& custId, double initBal) {
        lock_guard<mutex> lock(mtx);
        if (accCount >= MaxAcc) {
            cout << "Error: Max accounts reached" << endl;
            return -1;
        }
        if (initBal < 0) {
            cout << "Error: Initial balance cannot be negative" << endl;
            return -1;
        }

        int id = nextId++;
        accs[accCount++] = { id, custId, initBal, true };
        return id;
    }
    // System call to deposit money into an account
    bool deposit(int id, double amt) {
        lock_guard<mutex> lock(mtx);
        Acc* acc = find_acc(id);
        if (!acc) {
            cout << "Error: Account not found" << endl;
            return false;
        }
        if (amt <= 0) {
            cout << "Error: Deposit amount must be positive" << endl;
            return false;
        }
        acc->bal += amt;
        return true;
    }
    // System call to withdraw money from an account
    bool withdraw(int id, double amt) {
        lock_guard<mutex> lock(mtx);
        Acc* acc = find_acc(id);
        if (!acc) {
            cout << "Error: Account not found." << endl;
            return false;
        }
        if (amt <= 0) {
            cout << "Error: Withdrawal amount must be positive." << endl;
            return false;
        }
        if (acc->bal < amt) {
            cout << "Error: Insufficient funds." << endl;
            return false;
        }
        acc->bal -= amt;
        return true;
    }
    // System call to check balance
    double check_bal(int id) {
        lock_guard<mutex> lock(mtx);
        Acc* acc = find_acc(id);
        if (!acc) {
            cout << "Error: Account not found." << endl;
            return -1;
        }
        return acc->bal;
    }
};
int main() {
    BankSys bank;
    int bal1;
    cout << "Enter Money for creating your account  : ";
    cin >> bal1;
    int acc1 = bank.create_acc("cust1", bal1);
    if (acc1 != -1) {
        cout << "Account " << acc1 << " created with balance  : " << bank.check_bal(acc1) << endl;
    }
    int deposit;
    cout << "Enter the amount you want to deposit  : ";
    cin >> deposit;
    if (bank.deposit(acc1, deposit)) {
        cout << "After deposit, balance  : " << bank.check_bal(acc1) << endl;
    }
    int withdraw;
    cout << "Enter the amount you want to withdraw  : ";
    cin >> withdraw;
    if (bank.withdraw(acc1, withdraw)) {
        cout << "After withdrawal, balance  : " << bank.check_bal(acc1) << endl;
    }
    return 0;
}
