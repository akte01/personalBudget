#include <iostream>
#include "Markup.h"
#include <sstream>
#include <vector>
#include "budzet.h"
#include <windows.h>
#include <algorithm>
#include <iomanip>

using namespace std;

User::User(string userId, string login, string password, string name, string surname)
{
    this -> userId = userId;
    this -> login = login;
    this -> password = password;
    this -> name = name;
    this -> surname = surname;
}

Date::Date(string year, string month, string day)
{
    this -> year = year;
    this -> month = month;
    this -> day = day;
}

Income::Income(string incomeId, string userId, string date, string item, string amount)
{
    this -> incomeId = incomeId;
    this -> userId = userId;
    this -> date = date;
    this -> item = item;
    this -> amount = amount;
}

Expense::Expense(string expenseId, string userId, string date, string item, string amount)
{
    this -> expenseId = expenseId;
    this -> userId = userId;
    this -> date = date;
    this -> item = item;
    this -> amount = amount;
}

string intToStringConversion(int IdNumber)
{
    ostringstream ss;
    ss << IdNumber;
    string str = ss.str();
    return str;
}

void UserOptions::saveUser(vector <User> &users)
{
    CMarkup xml;
    xml.Load("users.xml");
    xml.AddElem("User");
    xml.IntoElem();
    xml.AddElem("userId", users[0].userId);
    xml.AddElem("login", users[0].login);
    xml.AddElem("password", users[0].password);
    xml.AddElem("name", users[0].name);
    xml.AddElem("surname", users[0].surname);
    xml.OutOfElem();
    xml.Save("users.xml");
    users.clear();
}

bool UserOptions::checkLogin(string login)
{
    CMarkup xml;
    xml.Load("users.xml");
    xml.ResetPos();
    while (xml.FindElem("User"))
    {
        xml.IntoElem();
        xml.FindElem( "login" );
        if (xml.GetData() == login)
        {
            return true;
        }
        else
        {
            xml.OutOfElem();
        }
    }
    return false;
}

void UserOptions::registerUser()
{
    system("cls");
    cout<< ">>>REJESTRACJA NOWEGO UZYTKOWNIKA<<<" <<endl;
    cout<< "------------------------------------" <<endl;
    cout<< endl;
    vector <User> users;
    string newIdNumber = "";
    newIdNumber = giveNewIdNumber(newIdNumber);
    string name, surname, login, password;
    cout<< "Podaj imie: ";
    cin>> name;
    cout<< "Podaj nazwisko: ";
    cin>> surname;
    cout<< "Podaj login: ";
    cin>> login;
    while (checkLogin(login) == 1)
    {
        cout<< "Podana nazwa uzytkownika juz istnieje, prosze podaj inna nazwe: ";
        cin>> login;
    }
    cout<< "Podaj haslo: ";
    cin>> password;
    users.push_back(User(newIdNumber, login, password, name, surname));
    saveUser(users);
}


string UserOptions::giveNewIdNumber (string newIdNumber)
{
    vector <string> idNumbers;
    string idNumber;
    CMarkup xml;
    xml.Load("users.xml");
    xml.ResetPos();
    while ( xml.FindElem("User") )
    {
        xml.IntoElem();
        xml.FindElem( "userId" );
        idNumber = xml.GetData();
        idNumbers.push_back(idNumber);
        xml.OutOfElem();
    }
    newIdNumber = intToStringConversion(idNumbers.size() + 1);
    return newIdNumber;
}

int UserOptions::logInUser()
{
    system("cls");
    cout<< ">>>LOGOWANIE<<<" <<endl;
    cout<< "---------------" <<endl;
    cout<< endl;
    string login, password, idNumber, choice;
    cout<< "Podaj login: ";
    cin>> login;
    if (checkLogin(login) == 1)
    {
        for(int attempt = 0; attempt < 3; attempt++)
        {
            cout<< "Podaj haslo. Pozostalo prob " << 3 - attempt << ": ";
            cin>> password;
            cout<< endl;
            CMarkup xml;
            xml.Load("users.xml");
            xml.ResetPos();
            while (xml.FindElem("User"))
            {
                xml.IntoElem();
                xml.FindElem( "login" );
                if (xml.GetData() == login)
                {
                    xml.FindElem( "password" );
                    if (xml.GetData() == password)
                    {
                        cout<< "Zalogowano!" <<endl;
                        Sleep(1500);
                        xml.ResetMainPos();
                        xml.FindElem( "userId" );
                        idNumber = xml.GetData();
                        return atoi(idNumber.c_str());
                    }
                    else cout<< "Niepoprawne haslo!";
                    Sleep(1500);
                }
                else
                {
                    xml.OutOfElem();
                }
            }
        }
    }
    else
    {
        cout<< "Nie ma takiego uzytkownika" <<endl;
        cout<< endl;
        cout<< "Jesli chcesz dokonac rejestracji nowego uzytkownika wcisnij '1' lub wcisnij dowolny klawisz, aby przejsc do menu glownego"<<endl;
        cin>> choice;
        if (choice == "1")
        {
            registerUser();
        }
        else return 0;
    }
}

void UserOptions::changePassword(int loggedUserId)
{
    string password = "";
    string newPassword = "";
    system("cls");
    cout<< ">>>ZMIANA HASLA<<<" <<endl;
    cout<< "------------------" <<endl;
    cout<< endl;
    cout<< "Podaj nowe haslo: ";
    cout<< endl;
    cin>> newPassword;

    CMarkup xml;
    xml.Load("users.xml");
    xml.ResetPos();
    while ( xml.FindElem("User") )
    {
        xml.IntoElem();
        xml.FindElem( "userId" );
        if (xml.GetData() == intToStringConversion(loggedUserId))
        {
            xml.FindElem("password");
            xml.RemoveElem();
            xml.AddElem("password", newPassword);
            xml.Save("users.xml");
        }
        else
        {
            xml.OutOfElem();
        }
    }
}

string IncomeOptions::loadDate (string enteredDate)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    int day = st.wDay;
    int month = st.wMonth;
    int year = st.wYear;
    string convertDay;

    if (day < 10)
    {
        convertDay = '0' + intToStringConversion(day);
        enteredDate = intToStringConversion(year) + '-' + intToStringConversion(month) + '-' + intToStringConversion(day);
    }
    else enteredDate = intToStringConversion(year) + '-' + intToStringConversion(month) + '-' + intToStringConversion(day);
    return enteredDate;
}

void IncomeOptions::convertDate (vector <Date> &dates, string enteredDate)
{
    string year, month, day;
    int dataNumber = 1;
    string dateNumber = "";
    string date = enteredDate + "-";
    for (int i = 0; i < date.length(); i++)
    {
        if (date[i] != '-')
        {
            dateNumber += date[i];
        }
        else
        {
            switch(dataNumber)
            {
            case 1:
                year = dateNumber;
                break;
            case 2:
                month = dateNumber;
                break;
            case 3:
                day = dateNumber;
                break;
            }
            dataNumber++;
            dateNumber = "";
        }
    }
    dates.push_back(Date(year, month, day));
}

bool IncomeOptions::compareMinDate (string enteredDate)
{
    vector <Date> dates;
    convertDate(dates, enteredDate);
    if ((atoi(dates[0].year.c_str()) < 2000) || (atoi(dates[0].month.c_str()) > 12)) return false;
    else  return true;
}

int IncomeOptions::calculateNumberOfDaysOfMonth(int month, int year)
{
    if (month == 2)
    {
        if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
    else if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
        return 30;

    else return 31;
}

bool IncomeOptions::compareMaxDate (string enteredDate)
{
    vector <Date> dates;
    string todayDate = "";
    int lastDayOfMonth, month, year;
    todayDate = loadDate(todayDate);
    convertDate(dates, todayDate);
    convertDate(dates, enteredDate);
    month = atoi(dates[0].month.c_str());
    year = atoi(dates[0].year.c_str());
    lastDayOfMonth = calculateNumberOfDaysOfMonth(month, year);
    if ((atoi(dates[1].year.c_str()) > atoi(dates[0].year.c_str())) || (atoi(dates[1].month.c_str()) > atoi(dates[0].month.c_str()))
            || (atoi(dates[1].day.c_str()) > lastDayOfMonth)) return false;
    else return true;
}

bool IncomeOptions::checkDate (string enteredDate)
{
    if ((compareMinDate(enteredDate) == 0) || (compareMaxDate(enteredDate) == 0)) return false;
    else return true;
}

string IncomeOptions::convertAmount (string amount)
{
    for (int i = 0; i < amount.length(); i++)
    {
        if (amount[i] == ',')
        {
            amount[i] = '.';
        }
    }
    return amount;
}

string IncomeOptions::giveNewIdNumberForIncome (string newIdNumberForIncome)
{
    vector <string> idNumbersForIncome;
    string idNumberForIncome;
    CMarkup xml;
    xml.Load("incomes.xml");
    xml.ResetPos();
    while ( xml.FindElem("Income") )
    {
        xml.IntoElem();
        xml.FindElem( "incomeId" );
        idNumberForIncome = xml.GetData();
        idNumbersForIncome.push_back(idNumberForIncome);
        xml.OutOfElem();
    }
    newIdNumberForIncome = intToStringConversion(idNumbersForIncome.size() + 1);
    return newIdNumberForIncome;
}

void IncomeOptions::saveIncome(vector <Income> &incomes)
{
    CMarkup xml;
    xml.Load("incomes.xml");
    xml.AddElem("Income");
    xml.IntoElem();
    xml.AddElem("incomeId", incomes[0].incomeId);
    xml.AddElem("userId", incomes[0].userId);
    xml.AddElem("date", incomes[0].date);
    xml.AddElem("item", incomes[0].item);
    xml.AddElem("amount", incomes[0].amount);
    xml.OutOfElem();
    xml.Save("incomes.xml");
    incomes.clear();
}

void IncomeOptions::addIncome(int loggedUserId)
{
    vector <Income> incomes;
    char choiceDate;
    string date = "";
    string item = "";
    string amount = "";
    string newIdNumberForIncome = "";
    string userId = "";
    system("cls");
    cout<< ">>>DODAWANIE PRZYCHODU<<<" <<endl;
    cout<< "-------------------------" <<endl;
    cout<< endl;

    userId = intToStringConversion(loggedUserId);
    newIdNumberForIncome = giveNewIdNumberForIncome(newIdNumberForIncome);

    cout<< "Czy podany dochod dotyczy dnia dzisiejszego? (Y/N): ";
    cin>> choiceDate;

    if (toupper(choiceDate) == 'Y')
    {
        date = loadDate(date);

    }
    else if (toupper(choiceDate) == 'N')
    {
        cout<< "Wprowadz date (rrrr-mm-dd): ";
        cin>> date;
        while (checkDate(date) == 0)
        {
            cout<< "Podana data jest niepoprawna!";
            cout<< "Wprowadz date (rrrr-mm-dd): ";
            cin>> date;
        }
    }
    else cout<< "Nie ma takiego wyboru" <<endl;

    cout<< "Podaj zrodlo przychodu: ";
    cin>> item;

    cout<< "Podaj wartosc przychodu: ";
    cin>> amount;
    amount = convertAmount(amount);

    incomes.push_back(Income(newIdNumberForIncome, userId, date, item, amount));
    saveIncome(incomes);
}

void ExpenseOptions::saveExpense(vector <Expense> &expenses)
{
    CMarkup xml;
    xml.Load("expenses.xml");
    xml.AddElem("Expense");
    xml.IntoElem();
    xml.AddElem("expenseId", expenses[0].expenseId);
    xml.AddElem("userId", expenses[0].userId);
    xml.AddElem("date", expenses[0].date);
    xml.AddElem("item", expenses[0].item);
    xml.AddElem("amount", expenses[0].amount);
    xml.OutOfElem();
    xml.Save("expenses.xml");
    expenses.clear();
}

string ExpenseOptions::giveNewIdNumberForExpense (string newIdNumberForExpense)
{
    vector <string> idNumbersForExpense;
    string idNumberForExpense;
    CMarkup xml;
    xml.Load("expenses.xml");
    xml.ResetPos();
    while ( xml.FindElem("Expense") )
    {
        xml.IntoElem();
        xml.FindElem( "expenseId" );
        idNumberForExpense = xml.GetData();
        idNumbersForExpense.push_back(idNumberForExpense);
        xml.OutOfElem();
    }
    newIdNumberForExpense = intToStringConversion(idNumbersForExpense.size() + 1);
    return newIdNumberForExpense;
}

void ExpenseOptions::addExpense(int loggedUserId)
{
    vector <Expense> expenses;
    char choiceDate;
    string date = "";
    string item = "";
    string amount = "";
    string newIdNumberForExpense = "";
    string userId = "";
    system("cls");
    cout<< ">>>DODAWANIE WYDATKU<<<" <<endl;
    cout<< "-----------------------" <<endl;
    cout<< endl;

    userId = intToStringConversion(loggedUserId);
    newIdNumberForExpense = giveNewIdNumberForExpense(newIdNumberForExpense);

    cout<< "Czy podany wydatek dotyczy dnia dzisiejszego? (Y/N): ";
    cin>> choiceDate;

    if (toupper(choiceDate) == 'Y')
    {
        date = loadDate(date);

    }
    else if (toupper(choiceDate) == 'N')
    {
        cout<< "Wprowadz date (rrrr-mm-dd): ";
        cin>> date;
        while (checkDate(date) == 0)
        {
            cout<< "Podana data jest niepoprawna!";
            cout<< "Wprowadz date (rrrr-mm-dd): ";
            cin>> date;
        }
    }
    else cout<< "Nie ma takiego wyboru" <<endl;

    cout<< "Podaj nazwe wydatku: ";
    cin>> item;

    cout<< "Podaj wartosc wydatku: ";
    cin>> amount;
    amount = convertAmount(amount);

    expenses.push_back(Expense(newIdNumberForExpense, userId, date, item, amount));
    saveExpense(expenses);
}

void BalanceOptions::importIncomesForLoggedUser(vector <Income> &incomes, int loggedUserId)
{
    string incomeId, userId, date, item, amount;
    CMarkup xml;
    xml.Load("incomes.xml");
    xml.ResetPos();
    while (xml.FindElem("Income"))
    {
        xml.IntoElem();
        xml.FindElem( "userId" );
        if (xml.GetData() == intToStringConversion(loggedUserId))
        {
            xml.ResetMainPos();
            xml.FindElem( "incomeId" );
            incomeId = xml.GetData();
            xml.FindElem( "userId" );
            userId = xml.GetData();
            xml.FindElem( "date" );
            date = xml.GetData();
            xml.FindElem( "item" );
            item = xml.GetData();
            xml.FindElem( "amount" );
            amount = xml.GetData();
            incomes.push_back(Income(incomeId, userId, date, item, amount));
            xml.OutOfElem();
        }
        else
        {
            xml.OutOfElem();
        }
    }
}

void BalanceOptions::importExpensesForLoggedUser(vector <Expense> &expenses, int loggedUserId)
{
    string expenseId, userId, date, item, amount;
    CMarkup xml;
    xml.Load("expenses.xml");
    xml.ResetPos();
    while (xml.FindElem("Expense"))
    {
        xml.IntoElem();
        xml.FindElem( "userId" );
        if (xml.GetData() == intToStringConversion(loggedUserId))
        {
            xml.ResetMainPos();
            xml.FindElem( "expenseId" );
            expenseId = xml.GetData();
            xml.FindElem( "userId" );
            userId = xml.GetData();
            xml.FindElem( "date" );
            date = xml.GetData();
            xml.FindElem( "item" );
            item = xml.GetData();
            xml.FindElem( "amount" );
            amount = xml.GetData();
            expenses.push_back(Expense(expenseId, userId, date, item, amount));
            xml.OutOfElem();
        }
        else
        {
            xml.OutOfElem();
        }
    }
}

void BalanceOptions::showIncomeForCurrentMonth(vector <Income> &incomes, vector <Income> &incomesForCurrentMonth)
{
    vector <Date> dates;
    vector <int> incomeIds;
    SYSTEMTIME st;
    GetLocalTime(&st);
    int day = st.wDay;
    int month = st.wMonth;
    int year = st.wYear;
    int incomeId;
    for (int i = 0; i < incomes.size(); i++)
    {
        convertDate(dates, incomes[i].date);
    }

    for (int i = 0; i < dates.size(); i++)
    {
        if ((atoi(dates[i].year.c_str()) == year) && (atoi(dates[i].month.c_str()) == month))
        {
            incomeId = i;
            incomeIds.push_back(incomeId);
        }
    }

    for (int i = 0; i < incomes.size(); i++)
    {
        for (int j = 0; j < incomeIds.size(); j++)
        {
            if (i == incomeIds[j])
            {
                incomesForCurrentMonth.push_back(Income(incomes[i].incomeId, incomes[i].userId, incomes[i].date, incomes[i].item, incomes[i].amount));
            }
        }
    }
}

void BalanceOptions::showSortedIncomeForChosenMonth(vector <Income> &incomesForCurrentMonth, vector <Income> &sortedIncomesForChosenMonth)
{
    vector <Date> dates;
    vector <int> incomeIds;
    vector <int> days;
    int minDayPosition;
    for (int i = 0; i < incomesForCurrentMonth.size(); i++)
    {
        convertDate(dates, incomesForCurrentMonth[i].date);
    }

    for (int i = 0; i < dates.size(); i++)
    {
        days.push_back(atoi(dates[i].day.c_str()));
    }

    while (days.size() > 0)
    {
        minDayPosition = min_element(days.begin(), days.end()) - days.begin();
        sortedIncomesForChosenMonth.push_back(Income(incomesForCurrentMonth[minDayPosition].incomeId, incomesForCurrentMonth[minDayPosition].userId,
                                              incomesForCurrentMonth[minDayPosition].date, incomesForCurrentMonth[minDayPosition].item,
                                              incomesForCurrentMonth[minDayPosition].amount));
        days.erase(days.begin() + minDayPosition);
        incomesForCurrentMonth.erase(incomesForCurrentMonth.begin() + minDayPosition);
    }
}

void BalanceOptions::showExpenseForCurrentMonth(vector <Expense> &expenses, vector <Expense> &expensesForCurrentMonth)
{
    vector <Date> dates;
    vector <int> expenseIds;
    SYSTEMTIME st;
    GetLocalTime(&st);
    int day = st.wDay;
    int month = st.wMonth;
    int year = st.wYear;
    int expenseId;
    for (int i = 0; i < expenses.size(); i++)
    {
        convertDate(dates, expenses[i].date);
    }

    for (int i = 0; i < dates.size(); i++)
    {
        if ((atoi(dates[i].year.c_str()) == year) && (atoi(dates[i].month.c_str()) == month))
        {
            expenseId = i;
            expenseIds.push_back(expenseId);
        }
    }

    for (int i = 0; i < expenses.size(); i++)
    {
        for (int j = 0; j < expenseIds.size(); j++)
        {
            if (i == expenseIds[j])
            {
                expensesForCurrentMonth.push_back(Expense(expenses[i].expenseId, expenses[i].userId, expenses[i].date, expenses[i].item, expenses[i].amount));
            }
        }
    }
}

void BalanceOptions::showSortedExpenseForChosenMonth(vector <Expense> &expensesForCurrentMonth, vector <Expense> &sortedExpensesForChosenMonth)
{
    vector <Date> dates;
    vector <int> expenseIds;
    vector <int> days;
    int minDayPosition;
    for (int i = 0; i < expensesForCurrentMonth.size(); i++)
    {
        convertDate(dates, expensesForCurrentMonth[i].date);
    }

    for (int i = 0; i < dates.size(); i++)
    {
        days.push_back(atoi(dates[i].day.c_str()));
    }

    while (days.size() > 0)
    {
        minDayPosition = min_element(days.begin(), days.end()) - days.begin();
        sortedExpensesForChosenMonth.push_back(Expense(expensesForCurrentMonth[minDayPosition].expenseId, expensesForCurrentMonth[minDayPosition].userId,
                                               expensesForCurrentMonth[minDayPosition].date, expensesForCurrentMonth[minDayPosition].item,
                                               expensesForCurrentMonth[minDayPosition].amount));
        days.erase(days.begin() + minDayPosition);
        expensesForCurrentMonth.erase(expensesForCurrentMonth.begin() + minDayPosition);
    }
}

float BalanceOptions::sumUpIncomes(vector <Income> sortedIncomesForChosenMonth)
{
    float totalIncome = 0;
    for (int i = 0; i < sortedIncomesForChosenMonth.size(); i++)
    {
        totalIncome += atof(sortedIncomesForChosenMonth[i].amount.c_str());
    }
    return totalIncome;
}

float BalanceOptions::sumUpExpenses(vector <Expense> sortedExpensesForChosenMonth)
{
    float totalExpense = 0;
    for (int i = 0; i < sortedExpensesForChosenMonth.size(); i++)
    {
        totalExpense += atof(sortedExpensesForChosenMonth[i].amount.c_str());
    }
    return totalExpense;
}

void BalanceOptions::showBalanceForCurrentMonth(int loggedUserId)
{
    vector <Income> incomes;
    vector <Expense> expenses;
    vector <Income> incomesForCurrentMonth;
    vector <Income> sortedIncomesForChosenMonth;
    vector <Expense> expensesForCurrentMonth;
    vector <Expense> sortedExpensesForChosenMonth;
    float totalIncome, totalExpense;

    system("cls");
    cout<< ">>>BILANS DLA BIEZACEGO MIESIACA<<<" <<endl;
    cout<< "-----------------------------------" <<endl;
    cout<< endl;

    importIncomesForLoggedUser(incomes, loggedUserId);
    importExpensesForLoggedUser(expenses, loggedUserId);
    showIncomeForCurrentMonth(incomes, incomesForCurrentMonth);
    showSortedIncomeForChosenMonth(incomesForCurrentMonth, sortedIncomesForChosenMonth);

    showExpenseForCurrentMonth(expenses, expensesForCurrentMonth);
    showSortedExpenseForChosenMonth(expensesForCurrentMonth, sortedExpensesForChosenMonth);

    totalIncome = sumUpIncomes(sortedIncomesForChosenMonth);
    totalExpense = sumUpExpenses(sortedExpensesForChosenMonth);

    cout<< "Twoje przychody w biezacym miesiacu to: " <<endl;
    cout<< endl;
    for (int i = 0; i < sortedIncomesForChosenMonth.size(); i++)
    {
        cout<<sortedIncomesForChosenMonth[i].item <<endl;
        cout<<sortedIncomesForChosenMonth[i].date <<endl;
        cout<<sortedIncomesForChosenMonth[i].amount <<endl;
    }
    cout<< "Twoje wydatki w biezacym miesiacu to: " <<endl;
    cout<< endl;

    for (int i = 0; i < sortedExpensesForChosenMonth.size(); i++)
    {
        cout<<sortedExpensesForChosenMonth[i].item <<endl;
        cout<<sortedExpensesForChosenMonth[i].date <<endl;
        cout<<sortedExpensesForChosenMonth[i].amount <<endl;
    }

    cout<< endl;
    cout<< "Suma przychodow w tym miesiacu wynosi: "<< totalIncome <<endl;
    cout<< "Suma wydatkow w tym miesiacu wynosi: "<< totalExpense <<endl;
    cout<< "Bilans dla biezacego miesiaca: "<< totalIncome - totalExpense <<endl;
    system("pause");
}

void BalanceOptions::showIncomeForPreviousMonth(vector <Income> &incomes, vector <Income> &incomesForPreviousMonth)
{
    vector <Date> dates;
    vector <int> incomeIds;
    SYSTEMTIME st;
    GetLocalTime(&st);
    int day = st.wDay;
    int month = st.wMonth;
    int year = st.wYear;
    int incomeId;
    for (int i = 0; i < incomes.size(); i++)
    {
        convertDate(dates, incomes[i].date);
    }

    for (int i = 0; i < dates.size(); i++)
    {
        if ((atoi(dates[i].year.c_str()) == year) && (atoi(dates[i].month.c_str()) == month - 1))
        {
            incomeId = i;
            incomeIds.push_back(incomeId);
        }
    }

    for (int i = 0; i < incomes.size(); i++)
    {
        for (int j = 0; j < incomeIds.size(); j++)
        {
            if (i == incomeIds[j])
            {
                incomesForPreviousMonth.push_back(Income(incomes[i].incomeId, incomes[i].userId, incomes[i].date, incomes[i].item, incomes[i].amount));
            }
        }
    }
}

void BalanceOptions::showExpenseForPreviousMonth(vector <Expense> &expenses, vector <Expense> &expensesForPreviousMonth)
{
    vector <Date> dates;
    vector <int> expenseIds;
    SYSTEMTIME st;
    GetLocalTime(&st);
    int day = st.wDay;
    int month = st.wMonth;
    int year = st.wYear;
    int expenseId;
    for (int i = 0; i < expenses.size(); i++)
    {
        convertDate(dates, expenses[i].date);
    }

    for (int i = 0; i < dates.size(); i++)
    {
        if ((atoi(dates[i].year.c_str()) == year) && (atoi(dates[i].month.c_str()) == month - 1))
        {
            expenseId = i;
            expenseIds.push_back(expenseId);
        }
    }

    for (int i = 0; i < expenses.size(); i++)
    {
        for (int j = 0; j < expenseIds.size(); j++)
        {
            if (i == expenseIds[j])
            {
                expensesForPreviousMonth.push_back(Expense(expenses[i].expenseId, expenses[i].userId, expenses[i].date, expenses[i].item, expenses[i].amount));
            }
        }
    }
}

void BalanceOptions::showBalanceForPreviousMonth(int loggedUserId)
{
    vector <Income> incomes;
    vector <Expense> expenses;
    vector <Income> incomesForPreviousMonth;
    vector <Income> sortedIncomesForChosenMonth;
    vector <Expense> expensesForPreviousMonth;
    vector <Expense> sortedExpensesForChosenMonth;
    float totalIncome, totalExpense;

    system("cls");
    cout<< ">>>BILANS DLA POPRZEDNIEGO MIESIACA<<<" <<endl;
    cout<< "--------------------------------------" <<endl;
    cout<< endl;

    importIncomesForLoggedUser(incomes, loggedUserId);
    importExpensesForLoggedUser(expenses, loggedUserId);

    showIncomeForPreviousMonth(incomes, incomesForPreviousMonth);
    showSortedIncomeForChosenMonth(incomesForPreviousMonth, sortedIncomesForChosenMonth);

    showExpenseForPreviousMonth(expenses, expensesForPreviousMonth);
    showSortedExpenseForChosenMonth(expensesForPreviousMonth, sortedExpensesForChosenMonth);

    totalIncome = sumUpIncomes(sortedIncomesForChosenMonth);
    totalExpense = sumUpExpenses(sortedExpensesForChosenMonth);

    cout<< "Twoje przychody w poprzednim miesiacu to: " <<endl;
    cout<< endl;
    for (int i = 0; i < sortedIncomesForChosenMonth.size(); i++)
    {
        cout<<sortedIncomesForChosenMonth[i].item <<endl;
        cout<<sortedIncomesForChosenMonth[i].date <<endl;
        cout<<sortedIncomesForChosenMonth[i].amount <<endl;
    }
    cout<< "Twoje wydatki w poprzednim miesiacu to: " <<endl;
    cout<< endl;

    for (int i = 0; i < sortedExpensesForChosenMonth.size(); i++)
    {
        cout<<sortedExpensesForChosenMonth[i].item <<endl;
        cout<<sortedExpensesForChosenMonth[i].date <<endl;
        cout<<sortedExpensesForChosenMonth[i].amount <<endl;
    }

    cout<< endl;
    cout<< "Suma przychodow w poprzednim miesiacu wynosi: "<< totalIncome <<endl;
    cout<< "Suma wydatkow w poprzednim miesiacu wynosi: "<< totalExpense <<endl;
    cout<< "Bilans dla poprzedniego miesiaca: "<< totalIncome - totalExpense <<endl;
    system("pause");
}

void BalanceOptions::createDatesWithoutSeparators(vector <Date> &dates, vector <int> &datesWithoutSeparators)
{
    string dateWithoutSeparators = "";
    for (int i = 0; i < dates.size(); i++)
    {
        dateWithoutSeparators = dates[i].year + dates[i].month + dates[i].day;
        datesWithoutSeparators.push_back(atoi(dateWithoutSeparators.c_str()));
    }
}
void BalanceOptions::showIncomeForSelectedPeriod(vector <Income> &incomes, vector <Income> &incomesForSelectedPeriod, vector <Date> datesForSelectedPeriod)
{
    vector <Date> dates;
    vector <int> datesForIncomesWithoutSeparators;
    vector <int> selcetedDates;
    vector <int> incomeIds;
    int incomeId;

    for (int i = 0; i < incomes.size(); i++)
    {
        convertDate(dates, incomes[i].date);
    }

    createDatesWithoutSeparators(dates, datesForIncomesWithoutSeparators);
    createDatesWithoutSeparators(datesForSelectedPeriod, selcetedDates);

    for (int i = 0; i < datesForIncomesWithoutSeparators.size(); i++)
    {
        if ((datesForIncomesWithoutSeparators[i] >= selcetedDates[0]) && (datesForIncomesWithoutSeparators[i] <= selcetedDates[1]))
        {
            incomeId = i;
            incomeIds.push_back(incomeId);
        }
    }

    for (int i = 0; i < incomes.size(); i++)
    {
        for (int j = 0; j < incomeIds.size(); j++)
        {
            if (i == incomeIds[j])
            {
                incomesForSelectedPeriod.push_back(Income(incomes[i].incomeId, incomes[i].userId, incomes[i].date, incomes[i].item, incomes[i].amount));
            }
        }
    }
}

void BalanceOptions::showSortedIncomeForSelectedPeriod(vector <Income> &incomesForSelectedPeriod, vector <Income> &sortedIncomesForSelectedPeriod)
{
    vector <Date> dates;
    vector <int> expenseIds;
    vector <int> datesWithoutSeparators;
    int minDayPosition;
    for (int i = 0; i < incomesForSelectedPeriod.size(); i++)
    {
        convertDate(dates, incomesForSelectedPeriod[i].date);
    }

    createDatesWithoutSeparators(dates, datesWithoutSeparators);

    while (datesWithoutSeparators.size() > 0)
    {
        minDayPosition = min_element(datesWithoutSeparators.begin(), datesWithoutSeparators.end()) - datesWithoutSeparators.begin();
        sortedIncomesForSelectedPeriod.push_back(Income(incomesForSelectedPeriod[minDayPosition].incomeId, incomesForSelectedPeriod[minDayPosition].userId,
                incomesForSelectedPeriod[minDayPosition].date, incomesForSelectedPeriod[minDayPosition].item,
                incomesForSelectedPeriod[minDayPosition].amount));
        datesWithoutSeparators.erase(datesWithoutSeparators.begin() + minDayPosition);
        incomesForSelectedPeriod.erase(incomesForSelectedPeriod.begin() + minDayPosition);
    }
}

void BalanceOptions::showExpenseForSelectedPeriod(vector <Expense> &expenses, vector <Expense> &expensesForSelectedPeriod, vector <Date> datesForSelectedPeriod)
{
    vector <Date> dates;
    vector <int> datesForExpensesWithoutSeparators;
    vector <int> selcetedDates;
    vector <int> expenseIds;
    int expenseId;

    for (int i = 0; i < expenses.size(); i++)
    {
        convertDate(dates, expenses[i].date);
    }

    createDatesWithoutSeparators(dates, datesForExpensesWithoutSeparators);
    createDatesWithoutSeparators(datesForSelectedPeriod, selcetedDates);

    for (int i = 0; i < datesForExpensesWithoutSeparators.size(); i++)
    {
        if ((datesForExpensesWithoutSeparators[i] >= selcetedDates[0]) && (datesForExpensesWithoutSeparators[i] <= selcetedDates[1]))
        {
            expenseId = i;
            expenseIds.push_back(expenseId);
        }
    }

    for (int i = 0; i < expenses.size(); i++)
    {
        for (int j = 0; j < expenseIds.size(); j++)
        {
            if (i == expenseIds[j])
            {
                expensesForSelectedPeriod.push_back(Expense(expenses[i].expenseId, expenses[i].userId, expenses[i].date, expenses[i].item, expenses[i].amount));
            }
        }
    }
}

void BalanceOptions::showSortedExpenseForSelectedPeriod(vector <Expense> &expensesForSelectedPeriod, vector <Expense> &sortedExpensesForSelectedPeriod)
{
    vector <Date> dates;
    vector <int> expenseIds;
    vector <int> datesWithoutSeparators;
    int minDayPosition;
    for (int i = 0; i < expensesForSelectedPeriod.size(); i++)
    {
        convertDate(dates, expensesForSelectedPeriod[i].date);
    }

    createDatesWithoutSeparators(dates, datesWithoutSeparators);

    while (datesWithoutSeparators.size() > 0)
    {
        minDayPosition = min_element(datesWithoutSeparators.begin(), datesWithoutSeparators.end()) - datesWithoutSeparators.begin();
        sortedExpensesForSelectedPeriod.push_back(Expense(expensesForSelectedPeriod[minDayPosition].expenseId, expensesForSelectedPeriod[minDayPosition].userId,
                expensesForSelectedPeriod[minDayPosition].date, expensesForSelectedPeriod[minDayPosition].item,
                expensesForSelectedPeriod[minDayPosition].amount));
        datesWithoutSeparators.erase(datesWithoutSeparators.begin() + minDayPosition);
        expensesForSelectedPeriod.erase(expensesForSelectedPeriod.begin() + minDayPosition);
    }
}

void BalanceOptions::showBalanceForSelectedPeriod(int loggedUserId)
{
    vector <Income> incomes;
    vector <Expense> expenses;
    vector <Income> incomesForSelectedPeriod;
    vector <Income> sortedIncomesForSelectedPeriod;
    vector <Expense> expensesForSelectedPeriod;
    vector <Expense> sortedExpensesForSelectedPeriod;
    vector <Date> datesForSelectedPeriod;
    float totalIncome, totalExpense;
    string dateFrom, dateTo;

    system("cls");
    cout<< ">>>BILANS DLA WYBRANEGO OKRESU<<<" <<endl;
    cout<< "---------------------------------" <<endl;
    cout<< endl;

    cout<< "Podaj date, od ktorej ma byc liczony wybrany okres (rrrr-mm-dd): ";
    cin>> dateFrom;
    while (checkDate(dateFrom) == 0)
    {
        cout<< "Podana data jest niepoprawna!";
        cout<< "Wprowadz date jeszcze raz (rrrr-mm-dd): ";
        cin>> dateFrom;
    }
    cout<< "Podaj date, do ktorej ma byc liczony wybrany okres (rrrr-mm-dd): ";
    cin>> dateTo;
    while (checkDate(dateTo) == 0)
    {
        cout<< "Podana data jest niepoprawna!";
        cout<< "Wprowadz date jeszcze raz (rrrr-mm-dd): ";
        cin>> dateTo;
    }
    convertDate(datesForSelectedPeriod, dateFrom);
    convertDate(datesForSelectedPeriod, dateTo);

    importIncomesForLoggedUser(incomes, loggedUserId);
    importExpensesForLoggedUser(expenses, loggedUserId);

    showIncomeForSelectedPeriod(incomes, incomesForSelectedPeriod, datesForSelectedPeriod);
    showSortedIncomeForSelectedPeriod(incomesForSelectedPeriod, sortedIncomesForSelectedPeriod);

    showExpenseForSelectedPeriod(expenses, expensesForSelectedPeriod, datesForSelectedPeriod);
    showSortedExpenseForSelectedPeriod(expensesForSelectedPeriod, sortedExpensesForSelectedPeriod);

    totalIncome = sumUpIncomes(sortedIncomesForSelectedPeriod);
    totalExpense = sumUpExpenses(sortedExpensesForSelectedPeriod);

    cout<< endl;
    cout<< "Twoje przychody w wybranym okresie to: " <<endl;
    cout<< endl;
    for (int i = 0; i < sortedIncomesForSelectedPeriod.size(); i++)
    {
        cout<<sortedIncomesForSelectedPeriod[i].item <<endl;
        cout<<sortedIncomesForSelectedPeriod[i].date <<endl;
        cout<<sortedIncomesForSelectedPeriod[i].amount <<endl;
    }
    cout<< endl;
    cout<< "Twoje wydatki w wybranym okresie to: " <<endl;
    cout<< endl;

    for (int i = 0; i < sortedExpensesForSelectedPeriod.size(); i++)
    {
        cout<<sortedExpensesForSelectedPeriod[i].item <<endl;
        cout<<sortedExpensesForSelectedPeriod[i].date <<endl;
        cout<<sortedExpensesForSelectedPeriod[i].amount <<endl;
    }

    cout<< endl;
    cout<< "Suma przychodow w wybranym okresie wynosi: "<< totalIncome <<endl;
    cout<< "Suma wydatkow w wybranym okresie wynosi: "<< totalExpense <<endl;
    cout<< "Bilans dla wybranego okresu: "<< totalIncome - totalExpense <<endl;
    cout<< endl;
    system("pause");
}
