// ------------------------------------------------------------------------ 
// G Number           : G21407227
// Name               : Tharushi Liyanage
// Expected Grade Band: 74%
// Assignment Number  : 2 (Payroll System)
// ------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream> //Allow string stream to be used
#include <vector>
#include <string> 
#include <iomanip>
#include <cctype>
using namespace std;

//Constants 
const int DECIMALS = 2;
const double TAX_FREE = 12570.00;
const double TAX_RATE = 0.20;
const int MONTHS = 12;

const string DRIVERS_FILE = "drivers.txt";
const string ERROR_FILE = "errors.txt";
const string BLANKS = " \t\r\n";
const string TXT_EXT = ".txt";

const int EXIT = 0;
const int PROCESS_FILE = 1;
const int VIEW_ALL = 2;
const int SEARCH = 3;
const int NO_CHOICE = -1;
const int MIN_CHOICE = 0;
const int MAX_CHOICE = 3;

const double ZERO = 0.0;
const int START = 0;
const int OK_CODE = 0;
const int NO_DRIVERS = 1;

//Struct

//To store monthly pay record
struct MonthlyRecord
{
	string sourceFile;
	double miles = ZERO;
	double payBeforeTax = ZERO;
	double payAfterTax = ZERO;
};

//To store Driver's details
struct Driver
{
	string id;
	string name;
	double ratePerMile = ZERO;
	vector<MonthlyRecord> history;
};

//Helper Functions

//Convert a string to uppercase
string toUpper(string text)
{
	for (char& ch : text)
	{
		ch = toupper(ch);
	}
	return text;
}

//Check if a token is a valid double
bool isDouble(const string& token, double& value)
{
	stringstream ss(token);
	ss >> value;
	return !ss.fail() && ss.eof(); //Ensure the value is a valid double
}

//Check if a token is a valid whole number
bool isInt(const string& token, int& value)
{
	stringstream ss(token);
	ss >> value;
	return !ss.fail() && ss.eof(); //Ensure the value is a valid number
}

//Check if the file name entered by the user is valid
bool isTxtFile(const string& name)
{
	if (name.length() <= TXT_EXT.length())
	{
		return false;
	}
	string ext = name.substr(name.length() - TXT_EXT.length());
	return toUpper(ext) == toUpper(TXT_EXT);
}

//Calculates monthly pay before/after tax from miles
void recordMonth(Driver& driver, const string& file, double miles)
{
	double gross = miles * driver.ratePerMile;
	double yearly = gross * MONTHS;
	double net = gross;

	if (yearly > TAX_FREE)
	{
		double tax = (yearly - TAX_FREE) * TAX_RATE;
		net = gross - (tax / MONTHS);
	}

	//Record the monthly pay information in history
	MonthlyRecord rec;
	rec.sourceFile = file;
	rec.miles = miles;
	rec.payBeforeTax = gross;
	rec.payAfterTax = net;
	driver.history.push_back(rec);
}

//Adds up miles from all monthly records in history and returns the total
double getTotalMiles(const Driver& driver)
{
	double total = ZERO;
	for (const MonthlyRecord& rec : driver.history)
	{
		total += rec.miles;
	}
	return total;
}

//Adds up all the money the driver earned(after tax) across every month in history
double getTotalPay(const Driver& driver)
{
	double total = ZERO;
	for (const MonthlyRecord& rec : driver.history)
	{
		total += rec.payAfterTax;
	}
	return total;
}

//Print driver summary
void printDriverSummary(const Driver& driver)
{
	cout << "\nDriver Summary" << endl;
	cout << "----------------------------------------------" << endl;
	cout << "ID: " << driver.id << endl;
	cout << "Name: " << driver.name << endl;
	cout << "Rate: $" << fixed << setprecision(DECIMALS) << driver.ratePerMile << " per mile" << endl;
	cout << "Total miles driven: " << getTotalMiles(driver) << endl;
	cout << "Total earnings    : $" << getTotalPay(driver) << endl;
	cout << "----------------------------------------------" << endl;
}

//Print driver history
void printDriverHistory(const Driver& driver)
{
	cout << "\nDriver History" << endl;
	cout << "----------------------------------------------" << endl;
	cout << "Driver: " << driver.name << " (ID: " << driver.id << ")" << endl;

	for (const MonthlyRecord& rec : driver.history)
	{
		cout << "\nFrom file: " << rec.sourceFile << endl;
		cout << "Miles recorded : " << fixed << setprecision(DECIMALS) << rec.miles << endl;
		cout << "Gross earnings : $" << rec.payBeforeTax << endl;
		cout << "Net earnings   : $" << rec.payAfterTax << endl;
	}

	cout << "\nOverall totals across all months:" << endl;
	cout << "Miles: " << fixed << setprecision(DECIMALS) << getTotalMiles(driver) << endl;
	cout << "Pay  : $" << getTotalPay(driver) << endl;
	cout << "----------------------------------------------" << endl;

}

//File Loading and Processing Functions
vector<Driver> loadDrivers(const string& path)
{
	vector<Driver> list;
	ifstream file(path);
	if (!file)
	{
		cout << "Error opening file: " << path << endl;
		return list;
	}

	string line;
	while (getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}
		istringstream stream(line); //To extract separate tokens 
		string idTok, nameTok, rateTok;
		stream >> idTok >> nameTok >> rateTok;

		double rate;
		if (!idTok.empty() && !nameTok.empty() && isDouble(rateTok, rate))
		{
			Driver d;
			d.id = toUpper(idTok);
			d.name = nameTok;
			d.ratePerMile = rate;
			list.push_back(d);
		}
	}
	file.close();
	return list;
}

//Finds a driver by ID (case-insensitive)
Driver* findDriver(vector<Driver>& list, const string& id)
{
	string target = toUpper(id);
	for (Driver& d : list)
	{
		if (d.id == target)
		{
			return &d;
		}
	}
	return nullptr;
}

//Check if this pay file has already been processed for any driver, to avoid duplicate records
bool alreadyProcessed(const string& name, const vector<Driver>& list)
{
	for (const Driver& d : list)
	{
		for (const MonthlyRecord& rec : d.history)
		{
			if (rec.sourceFile == name)
			{
				return true;
			}
		}
	}
	return false;
}

//Reads a driver pay file, validates entries, logs errors, and records monthly pay details for each driver
bool processPayFile(const string& path, const string& name, vector<Driver>& list, ofstream& log)
{
	if (alreadyProcessed(name, list))
	{
		cout << "Note: '" << name << "' has already been processed. Skipping to avoid duplicate records." << endl;
		return false;
	}

	ifstream file(path);
	if (!file)
	{
		cout << "Error: could not open '" << name << "'. Please try again." << endl;
		return false;
	}

	string line;
	while (getline(file, line))
	{
		if (line.find_first_not_of(BLANKS) == string::npos)
		{
			continue;
		}

		istringstream stream(line);
		string id;
		string milesTok;

		stream >> id;
		stream >> milesTok;

		if (milesTok.empty())
		{
			log << name << "|Pay entry for " << id << " is incomplete." << endl;
			cout << "Note: Driver " << id << " skipped — mileage missing." << endl;
			continue;
		}

		// Find the driver by ID
		Driver* d = findDriver(list, id);
		if (d == nullptr)
		{
			log << name << "|" << id << " is not a valid driver ID number." << endl;
			cout << "Note: Skipped — driver ID " << id << " not found." << endl;
			continue;
		}

		// Validate the miles (must be a real number and cannot be negative)
		double miles;
		if (!isDouble(milesTok, miles) || miles < ZERO)
		{
			log << name << "|Pay entry for " << id << " has an invalid mileage value." << endl;
			cout << "Note: Driver " << id << " skipped — mileage value invalid." << endl;
			continue;
		}

		recordMonth(*d, name, miles); //Record this months pay details for the driver

		const MonthlyRecord& rec = d->history.back();
		cout << "\n----------------------------------------------" << endl;
		cout << "Driver Payment Details" << endl;
		cout << "----------------------------------------------" << endl;
		cout << "Driver ID: " << d->id << endl;
		cout << "Name     : " << d->name << endl;
		cout << "Rate     : $" << fixed << setprecision(DECIMALS) << d->ratePerMile << " per mile" << endl;
		cout << "Miles    : " << rec.miles << endl;
		cout << "Gross Pay: $" << rec.payBeforeTax << endl;
		cout << "Net Pay  : $" << rec.payAfterTax << endl;
		cout << "----------------------------------------------" << endl;
		cout << "Processing complete for file: " << name << endl;
	}
	return true;
}

//Writes an <name>_output.txt file with each driver's pay for this run
void outputFile(const string& name, const vector<Driver>& list)
{
	string base = name.substr(START, name.find_last_of('.'));
	ofstream out(base + "_output.txt");
	for (const Driver& d : list)
	{
		for (const MonthlyRecord& rec : d.history)
		{
			if (rec.sourceFile == name)
			{
				out << d.id << " " << fixed << setprecision(DECIMALS) << rec.payBeforeTax << " "
					<< rec.payAfterTax << endl;
			}
		}
	}
}

//Continuously prompts the user until they enter a valid menu choice within the allowed range
int getMenuChoice()
{
	string input;
	int choice;
	while (true)
	{
		cout << "Enter your choice: ";
		getline(cin, input);

		if (!isInt(input, choice))
		{
			cout << "Invalid input. Please enter a valid number." << endl;
			continue;
		}

		if (choice < MIN_CHOICE || choice > MAX_CHOICE)
		{
			cout << "Invalid input. Please enter a number between "
				<< MIN_CHOICE << " and " << MAX_CHOICE << "." << endl;
			continue;
		}
		return choice;
	}
}

//Keeps asking for a driver ID until the user provides a non-empty input
string getSearchId()
{
	string id;
	while (true)
	{
		cout << "Enter driver ID: ";
		getline(cin, id);

		if (id.empty())
		{
			cout << "Invalid input. Please enter a driver ID." << endl;
			continue;
		}
		return id;
	}
}

//Prompts the user for a pay file name, validates it, and allows returning to the menu with "quit"
string getPayFileName()
{
	string name;
	while (true)
	{
		cout << "Enter Pay file name ('quit' to return to menu): ";
		getline(cin, name);

		if (name == "quit")
		{
			return "";
		}

		double num;
		if (isDouble(name, num) || !isTxtFile(name))
		{
			cout << "Invalid input. Please enter a valid file name (e.g. jan26.txt)" << endl;
			continue;
		}
		return name;
	}
}

//Runs the main driver pay system, showing a menu to process pay files, view drivers, search drivers, and exit with proper logging.
int main()
{
	vector<Driver> drivers = loadDrivers(DRIVERS_FILE);
	if (drivers.empty())
	{
		cout << "No driver data available. Exiting." << endl;
		return NO_DRIVERS;
	}

	ofstream log(ERROR_FILE);
	log << "Name of the file | Error Description " << endl;

	int choice = NO_CHOICE;
	while (choice != EXIT)
	{
		cout << "\n==============================================" << endl;
		cout << "   Welcome to the Driver Pay Management Tool   " << endl;
		cout << "==============================================" << endl;
		cout << "Please choose one of the following options:" << endl;
		cout << "  " << PROCESS_FILE << " - Process a pay file" << endl;
		cout << "  " << VIEW_ALL << " - View all drivers" << endl;
		cout << "  " << SEARCH << " - Search for a driver" << endl;
		cout << "  " << EXIT << " - Close the program" << endl;
		cout << "==============================================" << endl;
		cout << "Enter the number of your choice and press Enter." << endl;

		choice = getMenuChoice();

		if (choice == PROCESS_FILE)
		{
			while (true)
			{
				string name = getPayFileName();
				if (name.empty())
				{
					break; //User entered 'quit'
				}

				bool ok = processPayFile(name, name, drivers, log);
				if (ok)
				{
					outputFile(name, drivers);
				}
			}
		}
		else if (choice == VIEW_ALL)
		{
			for (const Driver& d : drivers)
			{
				printDriverSummary(d);
			}
		}
		else if (choice == SEARCH)
		{
			string id = getSearchId();
			Driver* d = findDriver(drivers, id);
			if (d != nullptr)
			{
				printDriverHistory(*d);
			}
			else
			{
				cout << "Driver not found." << endl;
			}
		}
	}
	log.close();
	cout << "!!Goodbye. Thank your for working with us!!" << endl;
	return OK_CODE;
}