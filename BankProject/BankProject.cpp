#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "clients.txt";

void ShowTransactionMenu();
struct stClient
{
    string AccountNumber="";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance=0;
    bool MarkForDelete = false;
};

enum enMainMenuOptions
{
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    transactions = 6,
    eManageUsers = 7,
    eExit = 8
};
enum enTransactionsMenu
{
    deposit = 1,
    withdraw = 2,
    totalBalances = 3,
    mainMenu = 4
};

// -----------------------------
// Utility Functions
// -----------------------------
vector<string> SplitString(string S1, string delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}
// -----------------------------
// File Operations
// -----------------------------
stClient ConvertStringToStruct(string Line, string delim = "#//#")
{
    stClient Client;
    vector<string> vClientData = SplitString(Line, delim);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}
string ConvertStructToString(stClient Client, string delim = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + delim;
    stClientRecord += Client.PinCode + delim;
    stClientRecord += Client.Name + delim;
    stClientRecord += Client.Phone + delim;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
vector<stClient> LoadClientsDataFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertStringToStruct(Line));
        }
        MyFile.close();
    }
    return vClients;
}
void SaveClientsDataToFile(string FileName, vector<stClient> vClients)
{
    fstream MyFile(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClient C : vClients)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertStructToString(C) << endl;
        }
        MyFile.close();
    }
}
void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}
// -----------------------------
// Client Operations
// -----------------------------
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector<stClient> vClients = LoadClientsDataFromFile(FileName);
    for (stClient C : vClients)
        if (C.AccountNumber == AccountNumber)
            return true;
    return false;
}
stClient ReadNewClient()
{
    stClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}
bool FindClientByAccountNumber(string AccountNumber, vector<stClient> vClients, stClient& Client)
{
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
    for (stClient& C : vClients)
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    return false;
}
stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;
    Client.AccountNumber = AccountNumber;
    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}
// -----------------------------
// Transaction Logic
// -----------------------------
bool DepositBalanceToClient(string AccountNumber, double Amount, vector<stClient>& vClients)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.AccountBalance += Amount;
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\nTransaction Successful. New balance = " << C.AccountBalance << endl;
            return true;
        }
    }
    return false;
}
// -----------------------------
// UI Functions
// -----------------------------
void PrintClientRecordLine(stClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber
        << "| " << setw(10) << left << Client.PinCode
        << "| " << setw(40) << left << Client.Name
        << "| " << setw(12) << left << Client.Phone
        << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintClientRecordLineForTotals(stClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber
        << "| " << setw(40) << left << Client.Name
        << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintClientCard(stClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}
// -----------------------------
// Screens (User Interfaces)
// -----------------------------
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}
void ShowAllClientsScreen()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;

    if (vClients.empty())
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (stClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;
}
void ShowTotalBalancesScreen()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    double Total = 0;
    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;

    for (stClient Client : vClients)
    {
        PrintClientRecordLineForTotals(Client);
        Total += Client.AccountBalance;
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;
    cout << "\t\t\t\t\tTotal Balances = " << Total << endl;
}
void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    char AddMore = 'Y';
    do
    {
        stClient Client = ReadNewClient();
        AddDataLineToFile(ClientsFileName, ConvertStructToString(Client));
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}
void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}
void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        char Answer = 'n';
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Deleted Successfully.";
        }
    }
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
}
void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        char Answer = 'n';
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stClient& C : vClients)
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
        }
    }
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
}
void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClient(AccountNumber, Amount, vClients);
}
void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount exceeds balance (" << Client.AccountBalance << "). Please enter smaller amount: ";
        cin >> Amount;
    }

    DepositBalanceToClient(AccountNumber, Amount * -1, vClients);
}
// -----------------------------
// Menu Control
// -----------------------------
short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice;
    cin >> Choice;
    return Choice;
}
short ReadTransactionMenuOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice;
    cin >> Choice;
    return Choice;
}

void GoBackToMainMenu();
void GoBackToTransactionMenu();

void PerformTransactionMenuOption(enTransactionsMenu Option)
{
    switch (Option)
    {
    case deposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenu();
        break;
    case withdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionMenu();
        break;
    case totalBalances:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionMenu();
        break;
    case mainMenu:
        system("cls");
        GoBackToMainMenu();
        break;
    }
}
void PerformMainMenuOption(enMainMenuOptions Option)
{
    switch (Option)
    {
    case eListClients:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    case eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;
    case eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    case eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    case eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case transactions:
        system("cls");
        ShowTransactionMenu();
        break;
    case eExit:
        system("cls");
        cout << "\n-----------------------------------\n";
        cout << "\tProgram Ends :-)";
        cout << "\n-----------------------------------\n";
        break;
    }
}

// -----------------------------
// Menus
// -----------------------------
void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Exit.\n";
    cout << "===========================================\n";
    PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}
void ShowTransactionMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransaction Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "===========================================\n";
    PerformTransactionMenuOption((enTransactionsMenu)ReadTransactionMenuOption());
}
void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenu();
}
void GoBackToTransactionMenu()
{
    cout << "\n\nPress any key to go back to Transaction Menu...";
    system("pause>0");
    ShowTransactionMenu();
}
// -----------------------------
// Main
// -----------------------------
int main()
{
    ShowMainMenu();
    system("pause>0");
    return 0;
}
