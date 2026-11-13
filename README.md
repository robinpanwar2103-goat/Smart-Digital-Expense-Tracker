# Smart Digital Expense Tracker with Budget Prediction

**Lab Sheet:** 09  
**Course:** BCA I Semester (Session 2026–2027)  
**Language:** C++ (C++17)

## Project objective
Develop a menu-driven expense management system that lets a user register and log in, record daily expenses, categorize spending, view monthly reports, set a budget, receive warnings, and estimate whether the monthly budget may be exceeded.

## Features
1. User registration
2. Login system
3. Add daily expense
4. View expense history
5. Category-wise expense report
6. Monthly expense report
7. Set monthly budget
8. Budget warning at 80% and when exceeded
9. Average-based month-end expense prediction
10. Search expenses by ID, date, category, or description
11. Delete an expense by ID
12. Save data in text files

## Requirements
- A C++ compiler supporting C++17 (for example, g++)
- A text editor or IDE

## Compile and run
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o expense_tracker
```
- Windows: `expense_tracker.exe`
- Linux/macOS: `./expense_tracker`

The program creates these data files in the same folder when needed:
- `users.txt`
- `expenses.txt`
- `budgets.txt`

Do not upload real passwords or personal financial information. The lab program stores passwords as plain text for simplicity; it is an educational demonstration, not production-grade authentication.

## Prediction method
The program calculates average spending per elapsed day and multiplies it by the number of days in the selected month. This is a simple estimate, not AI-based prediction.

## Repository contents
- `main.cpp` — complete C++ source code
- `LAB_REPORT.md` — objective, algorithm, source reference, sample run, and conclusion
- `.gitignore` — excludes generated executables and personal data files
