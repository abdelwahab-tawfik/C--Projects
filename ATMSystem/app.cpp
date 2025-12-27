#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
enum enMainMenuOptions
{
    eQuickWithdraw = 1,
    eNormalWithDraw = 2,
    eDeposit = 3,
    eCheckBalance = 4,
    eChangePassword = 5,
    eShowInformation = 6,
    eUpdateInformation = 7,
    eExit = 8
};

const string ClientsFileName = "Clients.txt";
stClient CurrentClient;
// --- Prototypes ---
void ShowMainMenu();
void Login();
void ShowQuickWithdrawScreen();
void ShowNormalWithDrawScreen();
void ShowDepositScreen();
// --- Functions ---
vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
            vString.push_back(sWord);
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
        vString.push_back(S1);
    return vString;
}
stClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    stClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}
string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
vector<stClient> LoadCleintsDataFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertLinetoRecord(Line));
        }
        MyFile.close();
    }
    return vClients;
}
vector<stClient> SaveCleintsDataToFile(string FileName, vector<stClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                MyFile << ConvertRecordToLine(C) << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient> &vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want to perform this transaction? y / n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
    }
    return false;
}
string ReadString(string message)
{
    string s1 = "";
    cout << message << endl;
    getline(cin >> ws, s1);
    return s1;
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
// --- [1] Quick Withdraw ---
short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice > 9)
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;
    }
    return Choice;
}
short getQuickWithDrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}
void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
    if (QuickWithDrawOption == 9)
        return;
    short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);
    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n ";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;
}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n\t\tQuick Withdraw\n===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n\t[3] 100\t\t[4] 200\n\t[5] 400\t\t[6] 600\n\t[7] 800\t\t[8] 1000\n\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance;
    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}
// --- [2] Normal Withdraw ---
int ReadWithDrawAmount()
{
    int Amount;
    cout << "\nEnter an amount multiple of 5's ? ";
    cin >> Amount;
    while (Amount % 5 != 0 || Amount <= 0)
    {
        cout << "\nInvalid! Enter an amount multiple of 5's ? ";
        cin >> Amount;
    }
    return Amount;
}
void PerfromNormalWithdrawOption()
{
    int WithDrawBalance = ReadWithDrawAmount();
    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        system("pause>0");
        ShowNormalWithDrawScreen();
        return;
    }
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;
}
void ShowNormalWithDrawScreen()
{
    system("cls");
    cout << "===========================================\n\t\tNormal Withdraw Screen\n===========================================\n";
    PerfromNormalWithdrawOption();
}
// --- [3] Deposit ---
double ReadDepositAmount()
{
    double Amount;
    cout << "\nEnter a positive Deposit Amount? ";
    cin >> Amount;
    while (Amount <= 0)
    {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> Amount;
    }
    return Amount;
}
void PerfromDepositOption()
{
    double DepositAmount = ReadDepositAmount();
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
    CurrentClient.AccountBalance += DepositAmount;
}
void ShowDepositScreen()
{
    system("cls");
    cout << "===========================================\n\t\tDeposit Screen\n===========================================\n";
    PerfromDepositOption();
}
// --- [4] Check Balance ---
void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===========================================\n\t\tCheck Balance Screen\n===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << "\n";
}
// --- [5] Change Password ---
void ChangePassword()
{
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string PinCode = ReadString("Enter pin code");

    while (PinCode != CurrentClient.PinCode)
    {
        PinCode = ReadString("ReEnter pin code");
    }
    string NewPinCode = ReadString("Enter New pin code");
    string ReEnterNewPinCode = ReadString("ReEnter New pin code");
    while (NewPinCode != ReEnterNewPinCode)
    {
        NewPinCode = ReadString("ERROR NOT MATCH...Enter New pin code");
    }
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            C.PinCode = NewPinCode;
            CurrentClient.PinCode = NewPinCode;
            break;
        }
    }

    SaveCleintsDataToFile(ClientsFileName, vClients);

    cout << "\nPassword changed successfully.\n";
}
// --- [6] Show Infomation ---
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
// --- [7] Change Infomation ---
void ChangeInformation()
{
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            C = ChangeClientRecord(CurrentClient.AccountNumber);
            break;
        }
    }
    SaveCleintsDataToFile(ClientsFileName, vClients);
    cout << "\nInformation changed successfully.\n";
}
// --- Navigation & Login ---
void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenu();
}
void PerfromMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenu();
        break;
    case eNormalWithDraw:
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToMainMenu();
        break;
    case eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenu();
        break;
    case eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenu();
        break;
    case eChangePassword:
        system("cls");
        ChangePassword();
        GoBackToMainMenu();
        break;
    case eShowInformation:
        system("cls");
        PrintClientCard(CurrentClient);
        GoBackToMainMenu();
        break;
    case eUpdateInformation:
        system("cls");
        ChangeInformation();
        GoBackToMainMenu();
        break;
    case eExit:
        Login();
        break;
    }
}
void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n\t\tATM Main Menu Screen\n===========================================\n";
    cout << "\t[1] Quick Withdraw.\n\t[2] Normal Withdraw.\n\t[3] Deposit\n\t[4] Check Balance.\n\t[5] Change Password.\n\t[6] Show Information.\n\t[7] Update Information.\n\t[8] Logout.\n";
    cout << "===========================================\n";
    short Choice;
    cout << "Choose what do you want to do? [1 to 8]? ";
    cin >> Choice;
    PerfromMainMenuOption((enMainMenuOptions)Choice);
}
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient &Client)
{
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
void Login()
{
    bool LoginFaild = false;
    string AccountNumber, PinCode;
    do
    {
        system("cls");
        cout << "\n---------------------------------\n\tLogin Screen\n---------------------------------\n";
        if (LoginFaild)
            cout << "Invalid Account Number/PinCode!\n";
        cout << "Enter Account Number? ";
        cin >> AccountNumber;
        cout << "Enter Pin? ";
        cin >> PinCode;
        LoginFaild = !FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient);
    } while (LoginFaild);
    ShowMainMenu();
}
int main()
{
    Login();
    return 0;
}