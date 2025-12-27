#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "clients.txt";
const string UsersFileName = "users.txt";

void ShowTransactionMenu();
void ShowManageUsersMenu();
struct stClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0;
    bool MarkForDelete = false;
};
struct stUser
{
    string Username = "";
    string Password = "";
    int Permissions = 0;
    bool MarkForDelete = false;
};
stUser CurrentUser;

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
    MainMenu = 4
};
enum enManageUsersMenuOptions
{
    eListUsers = 1,
    eAddNewUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eTransactions = 6,
    eMainMenu = 7
};
enum enMainMenuPermissions
{
    eAll = -1,
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClients = 8,
    pFindClient = 16,
    pTranactions = 32,
    pManageUsers = 64
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
// File User Operations
// -----------------------------
stUser ConvertStringToUserStruct(string Line, string delim = "#//#")
{
    stUser User;
    vector<string> vUserData = SplitString(Line, delim);
    User.Username = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);
    return User;
}
string ConvertUserStructToString(stUser User, string delim = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.Username + delim;
    stUserRecord += User.Password + delim;
    stUserRecord += to_string(User.Permissions);
    return stUserRecord;
}
vector<stUser> LoadUsersDataFromFile(string FileName)
{
    vector<stUser> vUser;
    fstream MyFile(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vUser.push_back(ConvertStringToUserStruct(Line));
        }
        MyFile.close();
    }
    return vUser;
}
void SaveUsersDataToFile(string FileName, vector<stUser> vUser)
{
    fstream MyFile(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (stUser C : vUser)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertUserStructToString(C) << endl;
        }
        MyFile.close();
    }
}
void AddUserDataLineToFile(string FileName, string stUserDataLine)
{
    fstream MyFile(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stUserDataLine << endl;
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
bool FindClientByAccountNumber(string AccountNumber, vector<stClient> vClients, stClient &Client)
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
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient> &vClients)
{
    for (stClient &C : vClients)
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    return false;
}
bool FindUserByUsernameAndPassword(string Username, string Password, stUser &User)
{

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (stUser U : vUsers)
    {

        if (U.Username == Username && U.Password == Password)
        {
            User = U;
            return true;
        }
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
// User Operations
// -----------------------------
string ReadString(string message)
{
    cout << message << endl;
    string s1;
    getline(cin >> ws, s1);

    return s1;
}
int ReadPermissions()
{
    char Answer;
    int Permissions = 0;
    cout << "Do you want to give full access? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        return -1;

    cout << "Show Client List? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions |= enMainMenuPermissions::pListClients;

    cout << "Add New Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions |= enMainMenuPermissions::pAddNewClient;

    cout << "Delete Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions |= enMainMenuPermissions::pDeleteClient;

    cout << "Update Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions |= enMainMenuPermissions::pUpdateClients;

    cout << "Find Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions |= enMainMenuPermissions::pFindClient;

    cout << "Transaction? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions |= enMainMenuPermissions::pTranactions;

    cout << "Manage Users? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions |= enMainMenuPermissions::pManageUsers;

    return Permissions;
}
bool UserExistsByUsername(string Username, string FileName)
{
    vector<stUser> vUser = LoadUsersDataFromFile(FileName);
    for (stUser C : vUser)
        if (C.Username == Username)
            return true;
    return false;
}
stUser ReadNewUser()
{
    stUser user;
    user.Username = ReadString("Enter username");

    while (UserExistsByUsername(user.Username, UsersFileName))
    {
        cout << "\nuser with [" << user.Username << "] already exists, Enter another Username? ";
        user.Username = ReadString("Enter username");
    }
    user.Password = ReadString("Enter Password");

    user.Permissions = ReadPermissions();

    return user;
}
bool FindUserByUsername(string Username, vector<stUser> vUsers, stUser &user)
{
    for (stUser C : vUsers)
    {
        if (C.Username == Username)
        {
            user = C;
            return true;
        }
    }
    return false;
}
bool MarkUserForDeleteByUsername(string Username, vector<stUser> &vUsers)
{
    for (stUser &C : vUsers)
        if (C.Username == Username)
        {
            C.MarkForDelete = true;
            return true;
        }
    return false;
}
stUser ChangeUserRecord(string username)
{
    stUser user;
    user.Username = username;
    user.Password = ReadString("Enter password");
    user.Permissions = ReadPermissions();
    return user;
}
// -----------------------------
// Transaction Logic
// -----------------------------
bool DepositBalanceToClient(string AccountNumber, double Amount, vector<stClient> &vClients)
{
    for (stClient &C : vClients)
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

void PrintUserRecordLine(stUser User)
{
    cout << "| " << setw(15) << left << User.Username
         << "| " << setw(10) << left << User.Password
         << "| " << setw(40) << left << User.Permissions;
}
void PrintUserRecordLineForTotals(stUser User)
{
    cout << "| " << setw(15) << left << User.Username
         << "| " << setw(40) << left << User.Password
         << "| " << setw(12) << left << User.Permissions;
}
void PrintUserCard(stUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername: " << User.Username;
    cout << "\nPassword     : " << User.Password;
    cout << "\nPermissions         : " << User.Permissions;
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
void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
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
            for (stClient &C : vClients)
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

void ShowAllUsersScreen()
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    cout << "\n\t\t\t\t\tUser List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
         << endl;
    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
         << endl;

    if (vUsers.empty())
        cout << "\t\t\t\tNo Users Available In the System!";
    else
        for (stUser user : vUsers)
        {
            PrintUserRecordLine(user);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
         << endl;
}
void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";
    stUser User;
    char AddMore = 'Y';
    do
    {
        User.Username = ReadString("Enter your username");
        User.Password = ReadString("Enter your password");
        User.Permissions = ReadPermissions();

        AddUserDataLineToFile(UsersFileName, ConvertUserStructToString(User));
        cout << "\nUser Added Successfully, do you want to add more users? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}
void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";
    vector<stUser> vUser = LoadUsersDataFromFile(UsersFileName);
    stUser user;
    string Username = ReadString("Enter username");
    if (FindUserByUsername(Username, vUser, user))
        PrintUserCard(user);
    else
        cout << "\nClient with username[" << Username << "] is not found!";
}
void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete user Screen";
    cout << "\n-----------------------------------\n";
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string username = ReadString("Enter username");
    stUser user;
    if (FindUserByUsername(username, vUsers, user))
    {
        PrintUserCard(user);
        char Answer = 'n';
        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUsername(username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Deleted Successfully.";
        }
    }
    else
        cout << "\nClient with username (" << username << ") is Not Found!";
}
void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";
    stUser user;
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string username = ReadString("Enter username");
    if (FindUserByUsername(username, vUsers, user))
    {
        PrintUserCard(user);
        char Answer = 'n';
        cout << "\n\nAre you sure you want update this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUser &C : vUsers)
                if (C.Username == username)
                {
                    C = ChangeUserRecord(username);
                    break;
                }
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
        }
    }
    else
        cout << "\nUser with username (" << username << ") is Not Found!";
}

// -----------------------------
// Menu Control
// -----------------------------
short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
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
short ReadManageUsersMenuOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice;
    cin >> Choice;
    return Choice;
}
bool CheckAccessPermission(enMainMenuPermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuPermissions::eAll)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

void GoBackToMainMenu();
void GoBackToTransactionMenu();
void ShowManageUsersMenu();
void GoBackToManageUsersMenu();
void GoBackToLoginScreen();

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
    case MainMenu:
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
        if (CheckAccessPermission(enMainMenuPermissions::pListClients)) 
            ShowAllClientsScreen();
        else
            ShowAccessDeniedMessage();
        GoBackToMainMenu();
        break;

    case eAddNewClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuPermissions::pAddNewClient))
            ShowAddNewClientsScreen();
        else
            ShowAccessDeniedMessage();
        GoBackToMainMenu();
        break;

    case eDeleteClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
            ShowDeleteClientScreen();
        else
            ShowAccessDeniedMessage();
        GoBackToMainMenu();
        break;

    case eUpdateClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuPermissions::pUpdateClients))
            ShowUpdateClientScreen();
        else
            ShowAccessDeniedMessage();
        GoBackToMainMenu();
        break;

    case eFindClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuPermissions::pFindClient))
            ShowFindClientScreen();
        else
            ShowAccessDeniedMessage();
        GoBackToMainMenu();
        break;

    case eTransactions:
        system("cls");
        if (CheckAccessPermission(enMainMenuPermissions::pTranactions))
            ShowTransactionMenu();
        else
            ShowAccessDeniedMessage();
        GoBackToMainMenu();
        break;

    case eManageUsers:
        system("cls");
        if (CheckAccessPermission(enMainMenuPermissions::pManageUsers))
            ShowManageUsersMenu();
        else
            ShowAccessDeniedMessage();
        GoBackToMainMenu();
        break;
    }
}
void PerformManageUsersMenuOption(enManageUsersMenuOptions Option)
{
    switch (Option)
    {
    case eListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case eAddNewUser:
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenu();
        break;
    case eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenu();
        break;
    case eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenu();
        break;
    case eTransactions:
        system("cls");
        ShowTransactionMenu();
        GoBackToManageUsersMenu();
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
void ShowManageUsersMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show users List.\n";
    cout << "\t[2] Add New user.\n";
    cout << "\t[3] Delete user.\n";
    cout << "\t[4] Update user Info.\n";
    cout << "\t[5] Find user.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Main Menu.\n";
    cout << "===========================================\n";
    PerformManageUsersMenuOption((enManageUsersMenuOptions)ReadManageUsersMenuOption());
}
void ShowLoginScreen()
{
    string Username, Password;
    bool LoginFailed = false;

    while (true)
    {
        system("cls");
        cout << "\tLogin Screen\n";
        if (LoginFailed)
            cout << "Invalid Username/Password!\n";

        cout << "Enter Username? ";
        cin >> Username;
        cout << "Enter Password? ";
        cin >> Password;

        if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        {
            ShowMainMenu();
            return;
        }
        else
        {
            LoginFailed = true;
        }
    }
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
void GoBackToManageUsersMenu()
{
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ShowManageUsersMenu();
}
void GoBackToLoginScreen()
{
    cout << "\n\nPress any key to go back to Login screen...";
    system("pause>0");
    ShowLoginScreen();
}
// -----------------------------
// Main
// -----------------------------
int main()
{
    ShowLoginScreen();
    system("pause>0");
    return 0;
}
