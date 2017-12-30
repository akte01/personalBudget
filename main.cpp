#include <iostream>
#include "Markup.h"
#include <sstream>
#include <vector>
#include <windows.h>
#include "budzet.h"

using namespace std;

int main()
{
    int loggedUserId = 0;
    while(1)
    {
        int userFirstChoice;

        if (loggedUserId == 0)
        {
            system("cls");
            cout<<">>>TWOJ BUDZET<<<"<<endl;
            cout<<"------------------"<<endl;
            cout<<endl;
            cout<< "1. Rejestracja"<<endl;
            cout<< "2. Logowanie"<<endl;
            cout<< "3. Zakoncz program"<<endl;
            cout<< "-------------------"<<endl;
            cout<< endl;
            cout<< "Wybierz opcje: ";

            cin>>userFirstChoice;

            switch (userFirstChoice)
            {
            case 1:
            {
                UserOptions registration;
                registration.registerUser();
            }
            break;
            case 2:
            {
                UserOptions login;
                loggedUserId = login.logInUser();
            }
            break;
            case 3:
                exit(0);
                break;
            }
        }
        else
        {
            int userSecondChoice;

            system("cls");
            cout<<">>>TWOJ BUDZET<<<"<<endl;
            cout<<"------------------"<<endl;
            cout<<endl;
            cout<< "1. Dodaj przychod"<<endl;
            cout<< "2. Dodaj wydatek"<<endl;
            cout<< "3. Bilans z biezacego miesiaca"<<endl;
            cout<< "4. Bilans z poprzedniego miesiaca"<<endl;
            cout<< "5. Bilans z wybranego okresu"<<endl;
            cout<< "6. Zmien haslo"<<endl;
            cout<< "7. Wyloguj sie"<<endl;
            cout<<endl;
            cout<<"Wybierz opcje: ";
            cin>>userSecondChoice;

            switch(userSecondChoice)
            {
            case 1:

            {
                IncomeOptions adding;
                adding.addIncome(loggedUserId);
            }

            break;

            case 2:
            {
                ExpenseOptions adding;
                adding.addExpense(loggedUserId);
            }
            break;

            case 3:
            {
                BalanceOptions currentMonth;
                currentMonth.showBalanceForCurrentMonth(loggedUserId);
            }
            break;

            case 4:
            {
                BalanceOptions previousMonth;
                previousMonth.showBalanceForPreviousMonth(loggedUserId);
            }

            break;

            case 5:
                {
                 BalanceOptions selectedPeriod;
                 selectedPeriod.showBalanceForSelectedPeriod(loggedUserId);
                }
                break;

            case 6:
            {
                UserOptions password;
                password.changePassword(loggedUserId);
            }
            break;

            case 7:
            {
                loggedUserId = 0;
                cout<<"Wylogowales sie!"<<endl;
                Sleep(1000);
            }
            break;
            }
        }
    }
    return 0;
}
