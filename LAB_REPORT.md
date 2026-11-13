# Lab Sheet–09: Smart Digital Expense Tracker with Budget Prediction

**Course:** BCA I Semester  
**Session:** 2026–2027  
**Programming Language:** C++

## 1. Objective
To develop a smart expense management system that helps users record daily expenses, categorize spending, monitor monthly budgets, and predict whether the monthly budget will be exceeded.

## 2. Problem statement
Many students spend money daily but do not track their expenses. This project provides an easy way to manage expenses and alerts users before they exceed their monthly budget.

## 3. Modules implemented
1. User Registration
2. Login System
3. Add Daily Expense
4. View Expense History
5. Category-wise Expense
6. Monthly Expense Report
7. Budget Setting
8. Budget Warning
9. Expense Prediction
10. Search Expense
11. Delete Expense
12. File Saving

## 4. Concepts used
- Variables and data types
- Loops and conditional statements
- Vectors (dynamic arrays)
- Structures
- Functions
- File handling
- Strings
- Searching and sorting
- Maps for category totals

## 5. Algorithm / logic
1. Start the program and display the welcome screen.
2. Ask the user to register, log in, or exit.
3. During registration, check that the username is unique and save the account.
4. During login, compare the entered credentials with the saved user records.
5. After successful login, display the expense-management menu.
6. For adding an expense, collect date, category, description, and amount, assign an ID, and save the record.
7. Read the saved records to display history, category totals, and monthly reports.
8. Save a monthly budget and compare it with the user's spending to show a warning.
9. Predict month-end spending using average daily spending multiplied by the number of days in the month.
10. Search records using an ID or text keyword; delete a selected record by ID.
11. Save changes to text files and allow the user to log out or exit.

## 6. Files used for storage
- `users.txt`: registered usernames and passwords
- `expenses.txt`: expense ID, username, date, category, description, and amount
- `budgets.txt`: username, month, and budget amount

## 7. Sample input and output
*Illustrative sample run (amounts and names are examples).*

```text
SMART DIGITAL EXPENSE TRACKER WITH BUDGET PREDICTION

1. Register
2. Login
3. Exit
Enter choice: 1
Create username: student01
Create password: pass123
Registration successful. You can now log in.

1. Register
2. Login
3. Exit
Enter choice: 2
Username: student01
Password: pass123
Login successful. Welcome, student01!

1. Add Daily Expense
2. View Expense History
3. Category-wise Expense
4. Monthly Expense Report
5. Set Monthly Budget
6. Budget Warning / Status
7. Expense Prediction
8. Search Expense
9. Delete Expense
10. Logout
Enter your choice: 1
Use today's date? (1=Yes, 2=Enter date): 1
Choose category: 1
Description: Lunch
Amount (Rs.): 120
Expense saved successfully. Expense ID: 1
```

## 8. How to compile
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o expense_tracker
```

## 9. Conclusion
The Smart Digital Expense Tracker provides a simple way to record and manage daily expenses. It generates category-wise and monthly summaries, stores information in files, and warns users when spending approaches or exceeds the budget. Its average-based prediction gives an estimate of possible month-end spending. The project demonstrates C++ functions, structures, strings, loops, searching, sorting, and file handling.

## 10. Future enhancements
- AI-based spending prediction
- Mobile application
- QR bill scanner
- Cloud backup
