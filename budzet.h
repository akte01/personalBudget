#include <iostream>
#include "Markup.h"
#include <sstream>
#include <vector>

using namespace std;

class UserOptions;
class BalanceOptions;
class IncomeOptions;

class User
{
friend class UserOptions;
private:
    string userId, login, password, name, surname;
public:
    User(string userId, string login, string password, string name, string surname);
};

class UserOptions
{
public:
    string giveNewIdNumber (string newIdNumber);
    void registerUser();
    void saveUser(vector <User> &users);
    int logInUser();
    bool checkLogin(string login);
    void changePassword(int loggedUserId);
    void importUserData();
};

class Date
{
friend class IncomeOptions;
friend class ExpenseOptions;
friend class BalanceOptions;
private:
    string year, month, day;
public:
    Date(string year, string month, string day);
};

class Income
{
friend class IncomeOptions;
friend class BalanceOptions;
private:
    string incomeId, userId, date, item, amount;
public:
    Income(string incomeId, string userId, string date, string item, string amount);
};

class IncomeOptions
{
public:
    void convertDate (vector <Date> &dates, string enteredDate);
    int calculateNumberOfDaysOfMonth(int month, int year);
    bool compareMinDate (string enteredDate);
    bool compareMaxDate (string enteredDate);
    bool checkDate (string enteredDate);
    string loadDate (string enteredDate);
    string convertAmount (string amount);
    string giveNewIdNumberForIncome(string newIdNumberForIncome);
    void saveIncome(vector <Income> &incomes);
    void addIncome(int loggedUserId);
};

class Expense
{
friend class ExpenseOptions;
friend class BalanceOptions;
private:
    string expenseId, userId, date, item, amount;
public:
    Expense(string expenseId, string userId, string date, string item, string amount);
};

class ExpenseOptions : public IncomeOptions
{
public:
    string giveNewIdNumberForExpense (string newIdNumberForExpense);
    void addExpense(int loggedUserId);
    void saveExpense(vector <Expense> &expenses);
};

class BalanceOptions : public IncomeOptions
{
public:

    void showSortedIncomeForChosenMonth(vector <Income> &incomesForCurrentMonth, vector <Income> &sortedIncomesForChosenMonth);
    void showIncomeForCurrentMonth(vector <Income> &incomes, vector <Income> &incomesForCurrentMonth);
    void showExpenseForCurrentMonth(vector <Expense> &expenses, vector <Expense> &expensesForCurrentMonth);
    void showSortedExpenseForChosenMonth(vector <Expense> &expensesForCurrentMonth, vector <Expense> &sortedExpensesForChosenMonth);
    float sumUpIncomes(vector <Income> sortedIncomesForCurrentMonth);
    float sumUpExpenses(vector <Expense> sortedExpensesForCurrentMonth);
    void showIncomeForPreviousMonth(vector <Income> &incomes, vector <Income> &incomesForPreviousMonth);
    void showExpenseForPreviousMonth(vector <Expense> &expenses, vector <Expense> &expensesForPreviousMonth);
    void importIncomesForLoggedUser(vector <Income> &incomes, int loggedUserId);
    void importExpensesForLoggedUser(vector <Expense> &expenses, int loggedUserId);
    void showBalanceForCurrentMonth(int loggedUserId);
    void showBalanceForPreviousMonth(int loggedUserId);
    void createDatesWithoutSeparators(vector <Date> &dates, vector <int> &datesWithoutSeparators);
    void showIncomeForSelectedPeriod(vector <Income> &incomes, vector <Income> &incomesForSelectedPeriod, vector <Date> datesForSelectedPeriod);
    void showSortedIncomeForSelectedPeriod(vector <Income> &incomesForSelectedPeriod, vector <Income> &sortedIncomesForSelectedPeriod);
    void showExpenseForSelectedPeriod(vector <Expense> &expenses, vector <Expense> &expensesForSelectedPeriod, vector <Date> datesForSelectedPeriod);
    void showSortedExpenseForSelectedPeriod(vector <Expense> &expensesForSelectedPeriod, vector <Expense> &sortedExpensesForSelectedPeriod);
    void showBalanceForSelectedPeriod(int loggedUserId);
};
