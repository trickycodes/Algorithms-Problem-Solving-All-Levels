#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "../mylibs/headers/string_utils.h";

using namespace std;

/*
	TODO

	New features to add

	Transactions window or screen that should have the following options:

	1 - Deposit: Deposit a balance to a user account by asking user number, print his/her info,
	and prompt for the ammount to deposit, update his/her account balance and save the new data to the file,
	then refresh it.

	2 - Withdraw: Withdraw a balance from a user account by asking user number, print his/her info,
	and prompt for the ammount to withdraw, update his/her account balance and save the new data to the file,
	then refresh it.

	3 - Total Balance: Sum of all clients balance and print on screen.

	4 - Main Menu: Back to main menu.
*/

// global variables
const string fileName = "bankClients.txt";
bool bankAppFlag = true;

const enum enOptions {
	showClients = 1,
	addNewClient = 2,
	deleteClient = 3,
	updateClient = 4,
	findClient = 5,
	transactions = 6,
	exitApp = 7,
};

const enum enTransactions
{
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalance = 3,
	eBackToMainMenu = 4
};

struct stClient
{
	string accountNumber;
	string pinCode;
	string name;
	string phone;
	double accountBalance;
	bool markForDelete = false;
};

// declarations
void startBankApp();
void showMainMenuScreen();
void showAllClients(const vector<stClient>&);
void redirectToScreen(const enOptions&, vector<stClient>&);
void addClient(const vector<stClient>&);
void addClients(vector<stClient>&);
void addDataLineToFile(const string&, const string&);
string readAccountNumber();
enOptions readUserOption();
vector<stClient> loadClientsData(const string&);
stClient convertLineToRecord(const string&, string = "#//#");
stClient readClientData(const vector<stClient>&);
string convertRecordToLine(const stClient&, string = "#//#");
stClient findClientByAccountNumber(const string&, const vector<stClient>&);
void printClientDetails(const stClient&);
void deleteClientAccount(vector<stClient>&);
void updateClientData(stClient&);
void updateClientAccount(vector<stClient>&);
void markClientToDelete(const string&, vector<stClient>&);
void saveClientsDataToFile(const string&, const vector<stClient>&);
void findClientAccount(const vector<stClient>&);
void exitBankApp();
void goBackToTranstionsWindow(vector<stClient>&);
void transactionsWindow(vector<stClient>&);
void depositAmount(vector<stClient>&);
void withdrawAmount(vector<stClient>&);
void showTotalBalance(const vector<stClient>&);
int calculateTotalBalance(const vector<stClient>&);


// defintions
short readUserOption(short from, short to)
{
	unsigned short option;
	cout << "Choose what you want to do [" << from << "-" << to << "]: ";
	cin >> option;

	return option;
}

stClient readClientData(const vector<stClient>& vClients)
{
	stClient data;

	cout << "\nPlease enter client data:\n" << endl;

	cin.ignore(1, '\n');
	while (true)
	{
		cout << "Enter account number: ";
		getline(cin, data.accountNumber);

		if (findClientByAccountNumber(data.accountNumber, vClients).accountNumber != "")
		{
			cout << "\nClient with [" << data.accountNumber << "] already exits. Please ";
		}
		else
			break;
	}

	cout << "Enter pin code: ";
	getline(cin, data.pinCode);

	cout << "Enter name: ";
	getline(cin, data.name);

	cout << "Enter phone number: ";
	getline(cin, data.phone);

	cout << "Enter account balance: ";
	cin >> data.accountBalance;
	while (cin.fail())
	{
		// user didn't input a number
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "Please enter a valid value for account balance: " << endl;
		cin >> data.accountBalance;
	}

	return data;
}

stClient findClientByAccountNumber(const string& accountNumber, const vector<stClient>& vClients)
{
	for (stClient client : vClients)
	{
		if (client.accountNumber == accountNumber)
			return client;
	}

	// Return a "not found" dummy client
	return { "", "", "", "", 0.0 };
}

void redirectToScreen(const enOptions& userOption, vector<stClient>& vClients)
{
	switch (userOption)
	{
	case enOptions::showClients:
	{
		showAllClients(vClients);
		break;
	}
	case enOptions::addNewClient:
	{
		addClients(vClients);
		break;
	}
	case enOptions::deleteClient:
	{
		deleteClientAccount(vClients);
		break;
	}
	case enOptions::updateClient:
	{
		updateClientAccount(vClients);
		break;
	}
	case enOptions::findClient:
	{
		findClientAccount(vClients);
		break;
	}
	case enOptions::transactions:
	{
		transactionsWindow(vClients);
		break;
	}
	case enOptions::exitApp:
	{
		exitBankApp();
		break;
	}
	default:
		break;
	}

}

void transactionWindowRedirect(const enTransactions& userOption, vector<stClient>& vClients)
{
	switch (userOption)
	{
	case enTransactions::eDeposit:
	{
		depositAmount(vClients);
		goBackToTranstionsWindow(vClients);
		break;
	}
	case enTransactions::eWithdraw:
	{
		withdrawAmount(vClients);
		goBackToTranstionsWindow(vClients);
		break;
	}
	case enTransactions::eTotalBalance:
	{
		showTotalBalance(vClients);
		goBackToTranstionsWindow(vClients);
		break;
	}
	case enTransactions::eBackToMainMenu:
	{
		showMainMenuScreen();
		break;
	}
	default:
	{
		break;
	}
	}
}

void showMainMenuScreen()
{
	system("cls");

	cout << "=====================================\n";
	cout << "           " << "Main Menu Screen\n";
	cout << "=====================================\n";
	cout << "     " << "[1] Show Client List\n";
	cout << "     " << "[2] Add New Client\n";
	cout << "     " << "[3] Delete Client\n";
	cout << "     " << "[4] Update Client Info\n";
	cout << "     " << "[5] Find Client\n";
	cout << "     " << "[6] Transactions\n";
	cout << "     " << "[7] Exit\n";
	cout << "=====================================\n";

}

void showAllClients(const vector<stClient>& vClients)
{
	system("cls");

	cout << "                                   ";
	cout << "Clients List (" << vClients.size() << ") Client(s)." << endl;
	cout << "---------------------------------------------------";
	cout << "--------------------------------------------------" << endl;

	cout << left
		<< "| " << setw(15) << "Account Number "
		<< "| " << setw(10) << "Pin Code "
		<< "| " << setw(30) << "Client Name "
		<< "| " << setw(20) << "Phone Number "
		<< "| " << setw(15) << "Balance " << "|"
		<< endl;

	cout << "---------------------------------------------------";
	cout << "--------------------------------------------------" << endl;

	if (vClients.size() == 0)
	{
		cout << "There are no records of clients in this file!\n" << endl;
		system("pause");
		return;
	}

	for (const stClient& client : vClients)
	{
		cout << left
			<< "| " << setw(15) << client.accountNumber
			<< "| " << setw(10) << client.pinCode
			<< "| " << setw(30) << client.name
			<< "| " << setw(20) << client.phone
			<< "| " << setw(15) << client.accountBalance
			<< "| " << endl;
	}

	cout << "---------------------------------------------------";
	cout << "--------------------------------------------------" << endl;
	system("pause");
}

void addClient(const vector<stClient>& vClients)
{
	cout << "\nAdding new client:" << endl;
	cout << "------------------------" << endl;

	stClient newClient;
	newClient = readClientData(vClients);

	addDataLineToFile(fileName, convertRecordToLine(newClient));
}

void addClients(vector<stClient>& vClients)
{
	char addMore = 'y';

	do
	{
		system("cls");
		cout << "=====================================\n";
		cout << "     Add New Client Screen" << endl;
		cout << "=====================================\n";
		addClient(vClients);
		cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
		cin >> addMore;
	} while (tolower(addMore) == 'y');

	// refresh clients list
	vClients = loadClientsData(fileName);
}

void addDataLineToFile(const string& fileName, const string& dataLine)
{
	fstream MyFile;
	MyFile.open(fileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << dataLine << endl;
		MyFile.close();
	}
}

vector<stClient> loadClientsData(const string& fileName)
{
	fstream file;
	vector<stClient> vClientsData;
	string line;

	file.open(fileName, ios::in);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			vClientsData.push_back(convertLineToRecord(line));
		}

		file.close();
	}

	return vClientsData;
}

stClient convertLineToRecord(const string& line, string delimiter)
{
	vector<string> vTemp = string_utils::splitString(line, delimiter);

	stClient data;

	data.accountNumber = vTemp[0];
	data.pinCode = vTemp[1];
	data.name = vTemp[2];
	data.phone = vTemp[3];
	data.accountBalance = stod(vTemp[4]);

	return data;
}

string convertRecordToLine(const stClient& client, string delimiter)
{
	string strClientRecord = "";

	strClientRecord += client.accountNumber + delimiter;
	strClientRecord += client.pinCode + delimiter;
	strClientRecord += client.name + delimiter;
	strClientRecord += client.phone + delimiter;
	strClientRecord += to_string(client.accountBalance);

	return strClientRecord;
}

void findClientAccount(const vector<stClient>& vClients)
{
	system("cls");

	cout << "=====================================\n";
	cout << "     Find Client Screen" << endl;
	cout << "=====================================\n" << endl;

	string accountNumber = readAccountNumber();
	stClient targetClient = findClientByAccountNumber(accountNumber, vClients);

	if (targetClient.accountNumber != "")
	{
		printClientDetails(targetClient);
		system("pause");
	}
	else
	{
		cout << "Client with account number (" << accountNumber << ") not found!" << endl;
		system("pause");
	}
}

void saveClientsDataToFile(const string& fileName, const vector<stClient>& vClients)
{
	fstream myFile;
	myFile.open(fileName, ios::out);//overwrite
	string dataLine;

	if (myFile.is_open())
	{
		for (stClient c : vClients)
		{
			if (c.markForDelete == false)
			{
				//we only write records that are not marked for delete.
				dataLine = convertRecordToLine(c);
				myFile << dataLine << endl;
			}
		}

		myFile.close();
	}
}

void showTotalBalance(const vector<stClient>& vClients)
{
	showAllClients(vClients);

	cout << "\n     Total Balance: " << calculateTotalBalance(vClients) << endl;
}

void updateClientData(stClient& data)
{

	cin.ignore(1, '\n');
	cout << "Enter pin code: ";
	getline(cin, data.pinCode);

	cout << "Enter name: ";
	getline(cin, data.name);

	cout << "Enter phone number: ";
	getline(cin, data.phone);

	cout << "Enter account balance: ";
	cin >> data.accountBalance;
	while (cin.fail())
	{
		// user didn't input a number
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "Please enter a valid value for account balance: " << endl;
		cin >> data.accountBalance;
	}
}

int calculateTotalBalance(const vector<stClient>& vClients)
{
	double sum = 0;

	for (const stClient c : vClients)
	{
		sum += c.accountBalance;
	}

	return sum;
}

string readAccountNumber()
{
	string accNum = "";

	cout << "Please enter the account number of client: ";
	cin >> accNum;

	return accNum;
}

void updateClientAccount(vector<stClient>& vClients)
{
	system("cls");

	cout << "=====================================\n";
	cout << "     Update Client Screen" << endl;
	cout << "=====================================\n" << endl;

	string accountNumber = readAccountNumber();
	stClient clientToUpdate = findClientByAccountNumber(accountNumber, vClients);

	char updateAnswer = 'n';

	if (clientToUpdate.accountNumber != "")
	{
		printClientDetails(clientToUpdate);

		cout << "\nDo you want to update this client? Y / N: ";
		cin >> updateAnswer;

		if (tolower(updateAnswer) == 'y')
		{

			for (stClient& client : vClients)
			{
				if (client.accountNumber == clientToUpdate.accountNumber)
				{
					updateClientData(client);
					break;
				}
			}

			saveClientsDataToFile(fileName, vClients);

			// refresh clients
			vClients = loadClientsData(fileName);
			cout << "Client updated successfully!" << endl;

			system("pause");
		}
	}

}

void markClientToDelete(const string& accountNumber, vector<stClient>& vClients)
{
	for (stClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			c.markForDelete = true;
			return; // exit the function
			;
		}
	}
}

void deleteClientAccount(vector<stClient>& vClients)
{
	system("cls");

	cout << "=====================================\n";
	cout << "     Delete Client Screen" << endl;
	cout << "=====================================\n" << endl;

	string accountNumber = readAccountNumber();
	stClient clientToDelete = findClientByAccountNumber(accountNumber, vClients);

	char delAnswer = 'n';

	if (clientToDelete.accountNumber != "")
	{
		printClientDetails(clientToDelete);

		cout << "\nDo you want to delete this client? Y / N: ";
		cin >> delAnswer;

		if (tolower(delAnswer) == 'y')
		{
			markClientToDelete(accountNumber, vClients);
			saveClientsDataToFile(fileName, vClients);

			// refresh clients
			vClients = loadClientsData(fileName);
			cout << "Client delete successfully!" << endl;

			system("pause");
		}

		else
		{
			cout << "Client with account number (" << accountNumber << ") not found!" << endl;
			system("pause");
		}
	}
}

void depositAmount(vector<stClient>& vClients)
{
	system("cls");

	cout << "=====================================\n";
	cout << "     Deposit Screen" << endl;
	cout << "=====================================\n" << endl;

	string accountNumber = readAccountNumber();
	stClient client = findClientByAccountNumber(accountNumber, vClients);

	if (client.accountNumber != "")
	{
		printClientDetails(client);

		double balanceToBeAdded;
		cout << "\nEnter the ammount you want to deposit: ";
		cin >> balanceToBeAdded;

		char confirm = 'n';
		cout << "\nAre you sure you want to deposit this ammount? Y / N: ";
		cin >> confirm;

		if (tolower(confirm) == 'y')
		{
			for (stClient& c : vClients)
			{
				if (c.accountNumber == client.accountNumber)
				{
					c.accountBalance += balanceToBeAdded;
					break;
				}
			}
		}
		saveClientsDataToFile(fileName, vClients);

		cout << "\nClient balance updated successfully!" << endl;

		vClients = loadClientsData(fileName); // refresh
	}
}

void withdrawAmount(vector<stClient>& vClients)
{
	system("cls");

	cout << "=====================================\n";
	cout << "     Withdraw Screen" << endl;
	cout << "=====================================\n" << endl;

	string accountNumber = readAccountNumber();
	stClient client = findClientByAccountNumber(accountNumber, vClients);

	if (client.accountNumber != "")
	{
		printClientDetails(client);

		double balanceToWithdraw = 0.0;
		char confirm = 'n';

		while (true)
		{
			cout << "\nEnter the amount you want to withdraw: ";
			cin >> balanceToWithdraw;

			if (client.accountBalance < balanceToWithdraw)
				cout << "Specified amount exceeds balance, try withdrawing another amount" << endl;
			else
				break;
		}

		cout << "\nAre you sure you want to withdraw this amount? y / n: ";
		cin >> confirm;

		if (tolower(confirm) == 'y')
		{
			for (stClient& c : vClients)
			{
				if (c.accountNumber == client.accountNumber)
				{
					c.accountBalance -= balanceToWithdraw;
					break;
				}
			}
			saveClientsDataToFile(fileName, vClients);

			cout << "\nClient balance updated successfully!" << endl;

			vClients = loadClientsData(fileName); // refresh
		}
	}
	else
		cout << "Client with account number [" << accountNumber << "] not found!" << endl;
}

void printClientDetails(const stClient& client)
{
	cout << "\nThe following client details:\n";

	cout << "---------------------------------" << endl;
	cout << "Account Number: " << client.accountNumber << endl;
	cout << "Pin Code: " << client.pinCode << endl;
	cout << "Name: " << client.name << endl;
	cout << "Phone Number: " << client.phone << endl;
	cout << "Account Balance: " << client.accountBalance << endl;
	cout << "---------------------------------" << endl;

	cout << endl;
}

void goBackToTranstionsWindow(vector<stClient>& vClients)
{
	cout << "\nPress any key to return to transations window...";
	system("pause>0");

	transactionsWindow(vClients);
}

void transactionsWindow(vector<stClient>& vClients)
{
	system("cls");

	cout << "=====================================\n";
	cout << "       " << "Transactions Screen\n";
	cout << "=====================================\n";
	cout << "     " << "[1] Deposit\n";
	cout << "     " << "[2] Withdraw\n";
	cout << "     " << "[3] Total Balances\n";
	cout << "     " << "[4] Main Menu\n";
	cout << "=====================================\n";

	enTransactions userOption;

	userOption = (enTransactions)readUserOption(1, 4);

	transactionWindowRedirect(userOption, vClients);
}

void exitBankApp()
{
	system("cls");

	string exitMessage = "Exiting bank app, Have a nice day!";

	bankAppFlag = false;

	cout << exitMessage << endl;

	cout << endl;

}

void startBankApp()
{
	// first load
	vector<stClient> vClients = loadClientsData(fileName);

	enOptions userOption;

	while (bankAppFlag)
	{
		showMainMenuScreen();
		userOption = enOptions(readUserOption(1, 7));

		redirectToScreen(userOption, vClients);
	}
}

int main()
{
	startBankApp();
	return 0;
}