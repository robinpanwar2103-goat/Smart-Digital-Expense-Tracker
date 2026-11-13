/*
    Project: Smart Digital Expense Tracker with Budget Prediction
    Lab Sheet: 09
    Language: C++
    Description:
      Register/login users, record expenses, view/search/delete expenses,
      show category and monthly reports, set a monthly budget, and estimate
      whether the budget may be exceeded.

    Compile:
      g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o expense_tracker

    Run:
      Windows: expense_tracker.exe
      Linux/macOS: ./expense_tracker
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <ctime>
#include <map>

using namespace std;

struct User {
    string username;
    string password;
};

struct Expense {
    int id{};
    string username;
    string date;       // YYYY-MM-DD
    string category;
    string description;
    double amount{};
};

const string USERS_FILE = "users.txt";
const string EXPENSES_FILE = "expenses.txt";
const string BUDGETS_FILE = "budgets.txt";

string trim(const string& value) {
    size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

string cleanField(string value) {
    // Keep the simple text-file format safe by replacing its delimiter.
    replace(value.begin(), value.end(), '|', '/');
    return trim(value);
}

string currentDate() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    ostringstream out;
    out << put_time(local, "%Y-%m-%d");
    return out.str();
}

bool validDate(const string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

int readInt(const string& prompt, int minimum = 0) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minimum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a number";
        if (minimum > 0) cout << " greater than or equal to " << minimum;
        cout << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readAmount(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Please enter an amount greater than zero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readNonEmptyLine(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);
        value = cleanField(value);
        if (value.empty()) cout << "This field cannot be empty.\n";
    } while (value.empty());
    return value;
}

vector<string> split(const string& line, char delimiter) {
    vector<string> fields;
    string field;
    stringstream stream(line);
    while (getline(stream, field, delimiter)) fields.push_back(field);
    return fields;
}

vector<User> loadUsers() {
    vector<User> users;
    ifstream file(USERS_FILE);
    string line;
    while (getline(file, line)) {
        auto fields = split(line, '|');
        if (fields.size() == 2) users.push_back({fields[0], fields[1]});
    }
    return users;
}

bool usernameExists(const string& username) {
    for (const auto& user : loadUsers())
        if (user.username == username) return true;
    return false;
}

void registerUser() {
    cout << "\n--- User Registration ---\n";
    string username = readNonEmptyLine("Create username: ");
    if (username.find('|') != string::npos || usernameExists(username)) {
        cout << "Username already exists or is invalid. Try another one.\n";
        return;
    }
    string password = readNonEmptyLine("Create password: ");
    ofstream file(USERS_FILE, ios::app);
    if (!file) {
        cout << "Could not open user file.\n";
        return;
    }
    file << username << '|' << password << '\n';
    cout << "Registration successful. You can now log in.\n";
}

bool loginUser(string& loggedInUser) {
    cout << "\n--- Login ---\n";
    string username = readNonEmptyLine("Username: ");
    string password = readNonEmptyLine("Password: ");
    for (const auto& user : loadUsers()) {
        if (user.username == username && user.password == password) {
            loggedInUser = username;
            cout << "Login successful. Welcome, " << username << "!\n";
            return true;
        }
    }
    cout << "Incorrect username or password.\n";
    return false;
}

vector<Expense> loadExpenses() {
    vector<Expense> expenses;
    ifstream file(EXPENSES_FILE);
    string line;
    while (getline(file, line)) {
        auto f = split(line, '|');
        if (f.size() == 6) {
            try {
                Expense e;
                e.id = stoi(f[0]);
                e.username = f[1];
                e.date = f[2];
                e.category = f[3];
                e.description = f[4];
                e.amount = stod(f[5]);
                expenses.push_back(e);
            } catch (...) {
                // Ignore damaged records instead of stopping the program.
            }
        }
    }
    return expenses;
}

void saveAllExpenses(const vector<Expense>& expenses) {
    ofstream file(EXPENSES_FILE);
    for (const auto& e : expenses) {
        file << e.id << '|' << e.username << '|' << e.date << '|'
             << cleanField(e.category) << '|' << cleanField(e.description)
             << '|' << fixed << setprecision(2) << e.amount << '\n';
    }
}

int nextExpenseId(const vector<Expense>& expenses) {
    int largest = 0;
    for (const auto& e : expenses) largest = max(largest, e.id);
    return largest + 1;
}

string chooseCategory() {
    const vector<string> categories = {
        "Food", "Travel", "Education", "Shopping", "Bills",
        "Health", "Entertainment", "Other"
    };
    cout << "\nChoose category:\n";
    for (size_t i = 0; i < categories.size(); ++i)
        cout << i + 1 << ". " << categories[i] << '\n';
    int choice = readInt("Enter choice: ", 1);
    while (choice > static_cast<int>(categories.size())) {
        cout << "Choose a number from 1 to " << categories.size() << ".\n";
        choice = readInt("Enter choice: ", 1);
    }
    return categories[choice - 1];
}

void addExpense(const string& username) {
    cout << "\n--- Add Daily Expense ---\n";
    vector<Expense> expenses = loadExpenses();
    Expense e;
    e.id = nextExpenseId(expenses);
    e.username = username;

    cout << "Use today's date (" << currentDate() << ")? (1=Yes, 2=Enter date): ";
    int dateChoice = readInt("", 1);
    if (dateChoice == 1) e.date = currentDate();
    else {
        do {
            e.date = readNonEmptyLine("Enter date (YYYY-MM-DD): ");
            if (!validDate(e.date)) cout << "Date format is invalid. Try YYYY-MM-DD.\n";
        } while (!validDate(e.date));
    }

    e.category = chooseCategory();
    e.description = readNonEmptyLine("Description: ");
    e.amount = readAmount("Amount (Rs.): ");

    expenses.push_back(e);
    ofstream file(EXPENSES_FILE, ios::app);
    if (!file) {
        cout << "Could not save expense.\n";
        return;
    }
    file << e.id << '|' << e.username << '|' << e.date << '|'
         << e.category << '|' << e.description << '|'
         << fixed << setprecision(2) << e.amount << '\n';
    cout << "Expense saved successfully. Expense ID: " << e.id << '\n';
}

vector<Expense> userExpenses(const string& username) {
    vector<Expense> result;
    for (const auto& e : loadExpenses())
        if (e.username == username) result.push_back(e);
    return result;
}

void printExpenseHeader() {
    cout << left << setw(6) << "ID" << setw(13) << "Date"
         << setw(18) << "Category" << setw(25) << "Description"
         << right << setw(12) << "Amount\n";
    cout << string(74, '-') << '\n';
}

void printExpense(const Expense& e) {
    cout << left << setw(6) << e.id << setw(13) << e.date
         << setw(18) << e.category << setw(25) << e.description.substr(0, 23)
         << right << setw(12) << fixed << setprecision(2) << e.amount << '\n';
}

void viewHistory(const string& username) {
    cout << "\n--- Expense History ---\n";
    auto expenses = userExpenses(username);
    if (expenses.empty()) {
        cout << "No expenses recorded yet.\n";
        return;
    }
    sort(expenses.begin(), expenses.end(),
         [](const Expense& a, const Expense& b) {
             return a.date == b.date ? a.id < b.id : a.date > b.date;
         });
    printExpenseHeader();
    for (const auto& e : expenses) printExpense(e);
}

void categoryReport(const string& username) {
    cout << "\n--- Category-wise Expense ---\n";
    map<string, double> totals;
    for (const auto& e : userExpenses(username)) totals[e.category] += e.amount;
    if (totals.empty()) {
        cout << "No expenses available.\n";
        return;
    }
    double grandTotal = 0;
    for (const auto& item : totals) {
        cout << left << setw(20) << item.first << "Rs. "
             << fixed << setprecision(2) << item.second << '\n';
        grandTotal += item.second;
    }
    cout << "Total spending: Rs. " << fixed << setprecision(2) << grandTotal << '\n';
}

string currentMonth() {
    return currentDate().substr(0, 7);
}

void monthlyReport(const string& username) {
    cout << "\n--- Monthly Expense Report ---\n";
    string month = readNonEmptyLine("Enter month (YYYY-MM), or press Enter is not possible; type current: ");
    if (month == "current") month = currentMonth();
    if (month.size() != 7 || month[4] != '-') {
        cout << "Invalid month format. Use YYYY-MM.\n";
        return;
    }
    double total = 0;
    int count = 0;
    for (const auto& e : userExpenses(username)) {
        if (e.date.substr(0, 7) == month) {
            cout << e.date << " | " << e.category << " | " << e.description
                 << " | Rs. " << fixed << setprecision(2) << e.amount << '\n';
            total += e.amount;
            ++count;
        }
    }
    cout << "Number of expenses: " << count << "\nTotal for " << month
         << ": Rs. " << fixed << setprecision(2) << total << '\n';
}

map<string, double> loadBudgets() {
    map<string, double> budgets;
    ifstream file(BUDGETS_FILE);
    string line;
    while (getline(file, line)) {
        auto f = split(line, '|');
        if (f.size() == 3) {
            try { budgets[f[0] + "|" + f[1]] = stod(f[2]); }
            catch (...) {}
        }
    }
    return budgets;
}

double getBudget(const string& username, const string& month) {
    auto budgets = loadBudgets();
    auto it = budgets.find(username + "|" + month);
    return it == budgets.end() ? 0.0 : it->second;
}

void setBudget(const string& username) {
    cout << "\n--- Set Monthly Budget ---\n";
    string month = readNonEmptyLine("Enter month (YYYY-MM), or type current: ");
    if (month == "current") month = currentMonth();
    if (month.size() != 7 || month[4] != '-') {
        cout << "Invalid month format. Use YYYY-MM.\n";
        return;
    }
    double amount = readAmount("Enter budget (Rs.): ");
    auto budgets = loadBudgets();
    budgets[username + "|" + month] = amount;
    ofstream file(BUDGETS_FILE);
    if (!file) {
        cout << "Could not save budget.\n";
        return;
    }
    for (const auto& item : budgets) {
        auto fields = split(item.first, '|');
        if (fields.size() == 2)
            file << fields[0] << '|' << fields[1] << '|'
                 << fixed << setprecision(2) << item.second << '\n';
    }
    cout << "Monthly budget saved successfully.\n";
}

double monthlyTotal(const string& username, const string& month) {
    double total = 0;
    for (const auto& e : userExpenses(username))
        if (e.date.substr(0, 7) == month) total += e.amount;
    return total;
}

void budgetWarning(const string& username) {
    cout << "\n--- Budget Status ---\n";
    string month = readNonEmptyLine("Enter month (YYYY-MM), or type current: ");
    if (month == "current") month = currentMonth();
    double budget = getBudget(username, month);
    if (budget <= 0) {
        cout << "No budget has been set for " << month << ".\n";
        return;
    }
    double spent = monthlyTotal(username, month);
    cout << "Budget: Rs. " << fixed << setprecision(2) << budget
         << "\nSpent:  Rs. " << spent
         << "\nLeft:   Rs. " << max(0.0, budget - spent) << '\n';
    if (spent > budget) cout << "WARNING: You have exceeded your monthly budget!\n";
    else if (spent >= budget * 0.8) cout << "Alert: You have used at least 80% of your budget.\n";
    else cout << "Good job! You are within your budget.\n";
}

void predictExpense(const string& username) {
    cout << "\n--- Expense Prediction ---\n";
    string month = readNonEmptyLine("Enter month (YYYY-MM), or type current: ");
    if (month == "current") month = currentMonth();
    if (month.size() != 7 || month[4] != '-') {
        cout << "Invalid month format. Use YYYY-MM.\n";
        return;
    }
    double budget = getBudget(username, month);
    if (budget <= 0) {
        cout << "Please set a budget for " << month << " first.\n";
        return;
    }

    double spent = monthlyTotal(username, month);
    int year = stoi(month.substr(0, 4));
    int mon = stoi(month.substr(5, 2));
    if (mon < 1 || mon > 12 || year < 1) {
        cout << "Invalid month.\n";
        return;
    }
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    int daysInMonth[] = {31, (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28,
                         31,30,31,30,31,31,30,31,30,31};
    int elapsed = daysInMonth[mon - 1];
    if (month == currentMonth()) elapsed = local->tm_mday;
    else if (month > currentMonth()) elapsed = 1;
    double predicted = spent / max(1, elapsed) * daysInMonth[mon - 1];

    cout << "Current spending: Rs. " << fixed << setprecision(2) << spent
         << "\nEstimated month-end spending: Rs. " << predicted
         << "\nMonthly budget: Rs. " << budget << '\n';
    if (predicted > budget)
        cout << "Prediction: Your spending may EXCEED the budget by Rs. "
             << predicted - budget << ".\n";
    else
        cout << "Prediction: You are likely to stay within budget. Estimated balance: Rs. "
             << budget - predicted << ".\n";
    cout << "Note: This is a simple average-based estimate, not an AI prediction.\n";
}

void searchExpense(const string& username) {
    cout << "\n--- Search Expense ---\n";
    string keyword = readNonEmptyLine("Enter ID, date, category, or description keyword: ");
    auto expenses = userExpenses(username);
    bool found = false;
    printExpenseHeader();
    for (const auto& e : expenses) {
        string idText = to_string(e.id);
        if (idText == keyword || e.date.find(keyword) != string::npos ||
            e.category.find(keyword) != string::npos ||
            e.description.find(keyword) != string::npos) {
            printExpense(e);
            found = true;
        }
    }
    if (!found) cout << "No matching expense found.\n";
}

void deleteExpense(const string& username) {
    cout << "\n--- Delete Expense ---\n";
    int id = readInt("Enter expense ID to delete: ", 1);
    auto expenses = loadExpenses();
    auto oldSize = expenses.size();
    expenses.erase(remove_if(expenses.begin(), expenses.end(),
        [&](const Expense& e) { return e.username == username && e.id == id; }),
        expenses.end());
    if (expenses.size() == oldSize) {
        cout << "Expense ID not found.\n";
        return;
    }
    saveAllExpenses(expenses);
    cout << "Expense deleted successfully.\n";
}

void showMenu() {
    cout << "\n========== SMART DIGITAL EXPENSE TRACKER ==========\n"
         << "1. Add Daily Expense\n"
         << "2. View Expense History\n"
         << "3. Category-wise Expense\n"
         << "4. Monthly Expense Report\n"
         << "5. Set Monthly Budget\n"
         << "6. Budget Warning / Status\n"
         << "7. Expense Prediction\n"
         << "8. Search Expense\n"
         << "9. Delete Expense\n"
         << "10. Logout\n"
         << "====================================================\n";
}

int main() {
    cout << "===============================================\n"
         << "  SMART DIGITAL EXPENSE TRACKER WITH BUDGET\n"
         << "                  PREDICTION\n"
         << "===============================================\n";

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\n";
        int choice = readInt("Enter choice: ", 1);
        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            string username;
            if (!loginUser(username)) continue;
            bool loggedIn = true;
            while (loggedIn) {
                showMenu();
                int option = readInt("Enter your choice: ", 1);
                switch (option) {
                    case 1: addExpense(username); break;
                    case 2: viewHistory(username); break;
                    case 3: categoryReport(username); break;
                    case 4: monthlyReport(username); break;
                    case 5: setBudget(username); break;
                    case 6: budgetWarning(username); break;
                    case 7: predictExpense(username); break;
                    case 8: searchExpense(username); break;
                    case 9: deleteExpense(username); break;
                    case 10: loggedIn = false; cout << "Logged out successfully.\n"; break;
                    default: cout << "Please choose an option from 1 to 10.\n";
                }
            }
        } else if (choice == 3) {
            cout << "Thank you for using the Expense Tracker. Goodbye!\n";
            break;
        } else {
            cout << "Please choose 1, 2, or 3.\n";
        }
    }
    return 0;
}
