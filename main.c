#include <mysql/mysql.h> // sudo su
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <time.h>
#include <stdbool.h>

// Function prototypes
bool isValidEmail(const char *email);
bool veryfyAccountNo(const int *veryfyAccountNomber);
void initializeDatabase();
void createTableIfNotExists(const char *tableName, const char *query);
void initializeDatabaseTables();
void initialize();
void clearScreen(); // cleare screen , clear terminal view
void mainMenu();
void createNewAcc();
void loginAcc();
void forgetAcc();
int isNumeric(const char *str);
int isNumericPhoneNo(const char *str);
void clearBuffer();
int generateAccountNumber();
int generatePassword();
void displayAccountInfo(const char *passNicNO);
void veryfyNic(const char *verifyNic);
void inLogin(const char *inAccNo, const char *inPassword);
void inLoginMenu(const char *inMfirstName, const char *inMlastName, const char *inMnicNo);
void inMwithdraw(const char *WFirstName, const char *WLastName, const char *WNicNo);
void inMdeposit(const char *DFirstName, const char *DLastName, const char *DNicNo);
void inMtransfer(const char *TFirstName, const char *TLastName, const char *TNicNo);
void inMloan(const char *LFirstName, const char *LLastName, const char *LNicNo);
void outMwithdraw(const char *outWFirstName, const char *outWLastName, const char *outWNicNo);
void inMwithDepHistory(const char *histNic);
void afterOutMWithdraw(const int aOutAccNo);
int validAmountCheck(int amount);
int accountAmountCheck(const int checkAAcountNo);
void updateMoney(int accountNo, int amount);
void updateWithMoney(int accountNo, int amount);
void processDepositOptions(const char *DFirstName, const char *DLastName, const char *DNicNo, int inMSqlacount_no);
void displayAccountBalance(int inMSqlacount_no);
void exitProgram(const char *DFirstName, const char *DLastName);
void tAfterGetNic(const char *tgetNic, const char *myNicNo);
void transfer(const char *holderNic, const char *mytNic);
void hisWithHis(const char *niswNic);
void hisDepHis(const char *nisdNic);
void hisTransHis(const char *nitNic);

char *showName(const char *transNicNo);
char *showNicNo(int showNicNO);

// Useful functions
MYSQL_RES *executeQuery(const char *query);
char *capitalizeFirstLetter(char *str);

// Constants for maximum string lengths
#define MAX_STRING_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define NIC_LENGTH 12
#define PHONE_NO_LENGTH 10
#define MAX_NAME_LENGTH 50 // This function using inLoging function or under
#define MAX_QUERY_LENGTH 500 // This function using mysql query length , using some times
#define MINIMUM_QUERY_LENGTH 100
// Text color escape codes
#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"

// Background color escape codes
#define ANSI_BG_BLACK "\x1b[40m"
#define ANSI_BG_RED "\x1b[41m"
#define ANSI_BG_GREEN "\x1b[42m"
#define ANSI_BG_YELLOW "\x1b[43m"
#define ANSI_BG_BLUE "\x1b[44m"
#define ANSI_BG_MAGENTA "\x1b[45m"
#define ANSI_BG_CYAN "\x1b[46m"
#define ANSI_BG_WHITE "\x1b[47m"

// Text attribute escape codes
#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_UNDERLINE "\x1b[4m"
#define ANSI_BLINK "\x1b[5m"
#define ANSI_REVERSE "\x1b[7m"
// Color Reset
#define ANSI_COLOR_RESET "\x1b[0m"

//Regular bold text
#define BOLD_BLACK "\e[1;30m"
#define BOLD_RED "\e[1;31m"
#define BOLD_GREEN "\e[1;32m"
#define BOLD_YELLOW "\e[1;33m"
#define BOLD_BLUE "\e[1;34m"
#define BOLD_MAGENTA "\e[1;35m"
#define BOLD_CYAN "\e[1;36m"
#define BOLD_WHITE "\e[1;37m"

//Regular underline text
#define UNDERLINE_BLACK "\e[4;30m"
#define UNDERLINE_RED "\e[4;31m"
#define UNDERLINE_GREEN "\e[4;32m"
#define UNDERLINE_YELLOW "\e[4;33m"
#define UNDERLINE_BLUE "\e[4;34m"
#define UNDERLINE_MAGENTA "\e[4;35m"
#define UNDERLINE_CYAN "\e[4;36m"
#define UNDERLINE_WHITE "\e[4;37m"

//Regular background
#define BLACK_BG "\e[40m"
#define RED_BG "\e[41m"
#define GREEN_BG "\e[42m"
#define YELLOW_BG "\e[43m"
#define BLUE_BG "\e[44m"
#define MAGENTA_BG "\e[45m"
#define CYAN_BG "\e[46m"
#define WHITE_BG "\e[47m"

//High intensity background 
#define INTENSITY_BLACK_BG "\e[0;100m"
#define INTENSITY_RED_BG "\e[0;101m"
#define INTENSITY_GREEN_BG "\e[0;102m"
#define INTENSITY_YELLOW_BG "\e[0;103m"
#define INTENSITY_BLUE_BG "\e[0;104m"
#define INTENSITY_MAGENTA_BG "\e[0;105m"
#define INTENSITY_CYAN_BG "\e[0;106m"
#define INTENSITY_WHITE_BG "\e[0;107m"

//High intensity text
#define INTENSITY_BLACK "\e[0;90m"
#define INTENSITY_RED "\e[0;91m"
#define INTENSITY_GREEN "\e[0;92m"
#define INTENSITY_YELLOW "\e[0;93m"
#define INTENSITY_BLUE "\e[0;94m"
#define INTENSITY_MAGENTA "\e[0;95m"
#define INTENSITY_CYAN "\e[0;96m"
#define INTENSITY_WHITE "\e[0;97m"

//Bold high intensity text
#define BOLD_INTENSITY_BLACK "\e[1;90m"
#define BOLD_INTENSITY_RED "\e[1;91m"
#define BOLD_INTENSITY_GREEN "\e[1;92m"
#define BOLD_INTENSITY_YELLOW "\e[1;93m"
#define BOLD_INTENSITY_BLUE "\e[1;94m"
#define BOLD_INTENSITY_MAGENTA "\e[1;95m"
#define BOLD_INTENSITY_CYAN "\e[1;96m"
#define BOLD_INTENSITY_WHITE "\e[1;97m"

//Reset
#define RESET "\e[0m"
#define COLOR_RESET "\e[0m"

// MySQL connection details
MYSQL *conn;
MYSQL_RES *result;
MYSQL_ROW row;

const char *server = "localhost";
const char *user = "root";
const char *password = "";
const char *database = ""; // DATABASE NAME = bank; MAIN MENU DEFINED AND CALL IT

// Function to initialize the database connection
void initializeDatabase()
{
 conn = mysql_init(NULL);

 if (conn == NULL)
 {
 perror("Error: mysql_init() failed");
 exit(EXIT_FAILURE);
 }

 if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL)
 {
 perror("Error: mysql_real_connect() failed");
 mysql_close(conn);
 exit(EXIT_FAILURE);
 }
}

// Function to execute a MySQL query
MYSQL_RES *executeQuery(const char *query)
{
 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "Error: executing MySQL query: %s\n", mysql_error(conn));
 mysql_close(conn);
 exit(EXIT_FAILURE);
 }

 MYSQL_RES *result = mysql_store_result(conn);

 if (result == NULL)
 {
 if (mysql_field_count(conn) > 0)
 {
 fprintf(stderr, "Error: retrieving result set: %s\n", mysql_error(conn));
 mysql_close(conn);
 exit(EXIT_FAILURE);
 }
 }

 return result;
}

// Function to create a table if it does not exist
void createTableIfNotExists(const char *tableName, const char *query)
{
 char createTableQuery[500];
 snprintf(createTableQuery, sizeof(createTableQuery), "CREATE TABLE IF NOT EXISTS %s %s", tableName, query);
 executeQuery(createTableQuery);
}

// Function to initialize database tables
void initializeDatabaseTables()
{
 // Create or switch to the 'bank' database
 executeQuery("CREATE DATABASE IF NOT EXISTS bank");
 executeQuery("USE bank");

 // Create 'customer' table
 createTableIfNotExists("customer", "(id INT AUTO_INCREMENT PRIMARY KEY, first_name VARCHAR(50) NOT NULL, last_name VARCHAR(50) NOT NULL, district VARCHAR(10), address VARCHAR(100), nic VARCHAR(12) NOT NULL, email VARCHAR(100) NOT NULL, phone_no VARCHAR(15) NOT NULL)");

 // Create 'money' table
 createTableIfNotExists("money", "(account_no INT(6) NOT NULL, amount DECIMAL(10, 2) NOT NULL, PRIMARY KEY (account_no))");

 // Create 'account' table
 createTableIfNotExists("account", "(acc_id INT(6) AUTO_INCREMENT PRIMARY KEY, account_no INT(6) NOT NULL, nic VARCHAR(12) NOT NULL, password INT(4) NOT NULL)");

 // Create 'deposits' table
 createTableIfNotExists("deposits", "(deposit_id INT AUTO_INCREMENT PRIMARY KEY, deposit_date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, account_no INT, deposit_amount DECIMAL(10, 2) NOT NULL, FOREIGN KEY (account_no) REFERENCES money(account_no))");

 // Create 'withdraw' table
 createTableIfNotExists("withdraw", "(withdraw_id INT AUTO_INCREMENT PRIMARY KEY, deposit_date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, account_no INT, withdraw_amount DECIMAL(10, 2) NOT NULL, FOREIGN KEY (account_no) REFERENCES money(account_no))");

 // Create 'transactions' table
 createTableIfNotExists("transactions", "(transaction_id INT AUTO_INCREMENT PRIMARY KEY, transfer_acc_no INT, user_acc_no INT, amount DECIMAL(10, 2) NOT NULL, transaction_date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
}

// Function to perform initialization
void initialize()
{
 initializeDatabase();
 initializeDatabaseTables();

 // ALTER TABLE statement (after tables are created)
 executeQuery("ALTER TABLE customer CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;");
}

// MAIN MENU .........................................................

// Function to display the main menu
void mainMenu()
{
 int userChoice;

 printf("\n...............................................................................\n\n");
 printf(ANSI_COLOR_BLACK ANSI_BG_YELLOW"! WELCOME TO DEVELOPMENT BANK SYSTEM !"ANSI_COLOR_RESET );
 printf("\n\n");
 printf(ANSI_COLOR_MAGENTA"Press (1) :"ANSI_COLOR_RESET" Create a new account\n");
 printf(ANSI_COLOR_MAGENTA"Press (2) :"ANSI_COLOR_RESET" Login to your account\n");
 printf(ANSI_COLOR_MAGENTA"Press (3) :"ANSI_COLOR_RESET" Forget account\n");
 printf(ANSI_COLOR_MAGENTA"Press (4) :"ANSI_COLOR_RESET" Exit\n\n");

 printf(ANSI_COLOR_BLUE"Enter your choice : "ANSI_COLOR_RESET);
 scanf("%d", &userChoice);
 

 switch (userChoice)
 {
 case 1:
 createNewAcc();
 break;
 case 2:
 loginAcc();
 break;
 case 3:
 forgetAcc();
 case 4:
 printf("\n\n" ANSI_COLOR_BLACK ANSI_BG_GREEN "THANK YOU FOR USING DEVELOPMENT BANK SYSTEM ."ANSI_RESET ANSI_BLINK ANSI_BG_GREEN " GOODBYE !!! " ANSI_RESET "\n\n");
 mysql_close(conn);
 exit(EXIT_SUCCESS);
 default:
 printf("\n\n"ANSI_COLOR_BLACK ANSI_BG_RED "YOUR ENTERED VALUE IS NOT CORRECT ."ANSI_RESET ANSI_BLINK ANSI_BG_RED " PLEASE TRY AGAIN !!! " ANSI_RESET "\n\n");
 sleep(2);
 mainMenu();
 }

 printf("\n...............................................................................\n\n");
}

// END MAIN MENU......................................................

// CREATE ACCOUNT.....................................................
// Function to create a new account
void createNewAcc()
{
 // Allocate memory for storing user information
 const char *firstName;
 const char *lastName;
 char address[MAX_ADDRESS_LENGTH];
 const char *district;
 char nic[NIC_LENGTH + 1]; // +1 for null terminator
 char email[MAX_STRING_LENGTH];
 char phoneNo[PHONE_NO_LENGTH + 1]; // +1 for null terminator

 printf("\n...............................................................................\n\n");
 printf(ANSI_COLOR_CYAN "FILL THIS FORM TO CREATE A NEW ACCOUNT .\n\n" ANSI_COLOR_RESET);

 // Get user input for each field
 printf(ANSI_COLOR_MAGENTA "First Name : " ANSI_COLOR_RESET);
 scanf("%ms", &firstName); // %ms is used to allocate memory for the string

 printf(ANSI_COLOR_MAGENTA "Last Name : " ANSI_COLOR_RESET);
 scanf("%ms", &lastName);

 printf(ANSI_COLOR_MAGENTA "Address : " ANSI_COLOR_RESET);
 clearBuffer(); // Clear the input buffer before using fgets
 fgets(address, sizeof(address), stdin);
 address[strcspn(address, "\n")] = '\0'; // Remove the trailing newline character

 printf(ANSI_COLOR_MAGENTA "District : " ANSI_COLOR_RESET);
 scanf("%ms", &district);

 // Input validation for NIC (numeric and NIC_LENGTH digits)
 do
 {
 printf(ANSI_COLOR_MAGENTA "NIC No (%d digits) : " ANSI_COLOR_RESET, NIC_LENGTH);
 scanf("%s", nic);
 } while (strlen(nic) != NIC_LENGTH || !isNumeric(nic));
 veryfyNic(nic);

 const char *validNicNo = nic;

 // Input validation for email
 do
 {
 printf(ANSI_COLOR_MAGENTA "Email : " ANSI_COLOR_RESET);
 scanf("%s", email);
 } while (!isValidEmail(email));

 // Input validation for phone number (numeric and PHONE_NO_LENGTH digits)
 do
 {
 printf(ANSI_COLOR_MAGENTA "Phone No (%d digits) : " ANSI_COLOR_RESET, PHONE_NO_LENGTH);
 scanf("%s", phoneNo);
 } while (!isNumericPhoneNo(phoneNo));

 const char *validPhoneNo = phoneNo;

 // Generate a 6-digit account number
 int accountNo = generateAccountNumber();
 veryfyAccountNo(&accountNo);

 // password
 int inPassword = generatePassword();

 // First balance
 int firstBalance = 0;

 // Construct and execute the SQL query using parameterized queries
 char query[500];
 snprintf(query, sizeof(query), "INSERT INTO customer (first_name, last_name, district, address, nic, email, phone_no) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
 firstName, lastName, district, address, validNicNo, email, validPhoneNo);

 executeQuery(query);

 // Insert account information into the account table
 snprintf(query, sizeof(query), "INSERT INTO account (account_no, nic, password) VALUES (%d, '%s', '%d')",
 accountNo, nic, inPassword);

 executeQuery(query);

 //
 snprintf(query, sizeof(query), "INSERT INTO money (account_no, amount) VALUES (%d, %d)", accountNo, firstBalance);

 executeQuery(query);

 printf("\n"ANSI_COLOR_BLACK ANSI_BG_GREEN "ACCOUNT CREATED SUCCESSFUL ."ANSI_RESET ANSI_BLINK ANSI_BG_GREEN " LOGIN YOUR ACCOUNT !!! "ANSI_RESET"\n");
 sleep(3);
 // Display the user account no and password
 if (1 == 1)
 {
 displayAccountInfo(nic);
 sleep(3);
 }
 mainMenu();

 printf("\n...............................................................................\n\n");

 // Don't forget to free the memory allocated for firstName, lastName, and district
 free((char *)firstName);
 free((char *)lastName);
 free((char *)district);
}

// Function to generate a 6-digit account number
int generateAccountNumber()
{
 // Seed the random number generator with the current time
 unsigned int seed = (unsigned int)time(NULL);
 srand(seed);

 // Generate a random 6-digit number
 return rand_r(&seed) % 900000 + 100000;
}

// Function to generate a 4-digit password
int generatePassword()
{
 // Seed the random number generator with the current time
 unsigned int seed = (unsigned int)time(NULL);
 srand(seed);

 // Generate a random 4-digit password
 return rand_r(&seed) % 9000 + 1000;
}

// Function to check if a string contains only numeric characters
int isNumeric(const char *str)
{
 while (*str)
 {
 if (!isdigit(*str))
 {
 return 0; // Not numeric
 }
 str++;
 }
 return 1; // Numeric
}

// Function to validate a phone number (numeric and PHONE_NO_LENGTH digits)
int isNumericPhoneNo(const char *phoneNo)
{
 return (strlen(phoneNo) == PHONE_NO_LENGTH && isNumeric(phoneNo));
}

// Function to check if an email is valid
bool isValidEmail(const char *email)
{
 int i, atCount = 0, dotCount = 0;
 bool valid = true;

 // Check for at least one '@' and '.'
 for (i = 0; email[i] != '\0'; i++)
 {
 if (email[i] == '@')
 {
 atCount++;
 }
 else if (email[i] == '.')
 {
 dotCount++;
 }
 }

 if (atCount != 1 || dotCount < 1)
 {
 valid = false;
 }
 else
 {
 // Check for '@' and '.' positions
 int atIndex = -1, dotIndex = -1;
 for (i = 0; email[i] != '\0'; i++)
 {
 if (email[i] == '@')
 {
 atIndex = i;
 }
 else if (email[i] == '.')
 {
 dotIndex = i;
 }
 }

 // Ensure '@' comes before '.'
 if (atIndex > dotIndex - 2 || dotIndex == i - 1)
 {
 valid = false;
 }
 }

 return valid;
}

// Function to display the user account no and password
char *showNicNo(int showNicNO)
{
 char query[MINIMUM_QUERY_LENGTH];
 snprintf(query, MINIMUM_QUERY_LENGTH, "SELECT nic FROM account WHERE account_no='%d'", showNicNO);

 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "mysql_query failed\n");
 mysql_close(conn);
 exit(1);
 }

 MYSQL_RES *result = mysql_store_result(conn);
 if (result == NULL)
 {
 fprintf(stderr, "mysql_store_result failed\n");
 mysql_close(conn);
 exit(1);
 }

 MYSQL_ROW row = mysql_fetch_row(result);

 if (row != NULL)
 {
 // Assuming NIC is a varchar, strdup the string to return a copy
 return strdup(row[0]);
 }
 else
 {
 printf("\n"ANSI_COLOR_BLACK ANSI_BG_RED "NO ACCOUNT FOUND FOR ACCOUNT NUMBER %d"ANSI_RESET"\n", showNicNO);
 // Return NULL for no account found
 return NULL;
 }
}

// Function to display account information based on NIC
void displayAccountInfo(const char *passNicNo)
{
 printf("\n...............................................................................\n\n");

 // Construct and execute the SQL query to retrieve account information
 char query[500];
 snprintf(query, sizeof(query), "SELECT account_no, password FROM account WHERE nic = '%s'", passNicNo);

 MYSQL_RES *result = executeQuery(query);

 // Display data
 MYSQL_ROW row;
 while ((row = mysql_fetch_row(result)))
 {
 // Assuming account number is the first field and password is the second field
 printf(ANSI_COLOR_GREEN"ACCOUNT NUMBER : %s"ANSI_RESET"\n", row[0] ? row[0] : "NULL");
 printf(ANSI_COLOR_GREEN"PASSWORD : %s "ANSI_RESET"\n", row[1] ? row[1] : "NULL");

 printf("\n");
 }

 mysql_free_result(result);

 printf("\n...............................................................................\n\n");
}

char *showName(const char *transNicNo)
{
 // Assuming a reasonable maximum size for names
 char myFNames[100];
 char myLNames[100];
 char fullName[200]; // Adjust the size accordingly

 // Construct and execute the SQL query to retrieve account information
 char query[500];
 snprintf(query, sizeof(query), "SELECT first_name, last_name FROM customer WHERE nic = '%s'", transNicNo);

 MYSQL_RES *result = executeQuery(query);

 // Initialize strings
 myFNames[0] = '\0';
 myLNames[0] = '\0';
 fullName[0] = '\0';

 // Display data
 MYSQL_ROW row;
 while ((row = mysql_fetch_row(result)))
 {
 // Assuming account number is the first field and password is the second field
 snprintf(myFNames, sizeof(myFNames), "Account Holder Name : %s", row[0] ? row[0] : "NULL");
 snprintf(myLNames, sizeof(myLNames), " %s\n", row[1] ? row[1] : "NULL");

 // Concatenate myFNames and myLNames into fullName
 snprintf(fullName, sizeof(fullName), "%s%s", myFNames, myLNames);
 }

 mysql_free_result(result);

 // Allocate and copy fullName to a new string
 char *resultFullName = strdup(fullName);
 return resultFullName;
}

// Function to verify if a NIC is already in use
void veryfyNic(const char *verifyNic)
{
 // Construct and execute the SQL query to check if the NIC is already in use
 char query[500];
 snprintf(query, sizeof(query), "SELECT * FROM customer WHERE nic = '%s'", verifyNic);

 MYSQL_RES *result = executeQuery(query);

 if (result == NULL)
 {
 printf("Error checking NIC availability.\n");
 // Handle error or return an error code
 }
 else
 {
 if (mysql_num_rows(result) > 0)
 {
 printf("\n\n"ANSI_COLOR_BLACK ANSI_BG_RED"NIC NUMBER IS ALREADY IN USE ."ANSI_RESET ANSI_BLINK ANSI_BG_RED " TRY AGAIN !!!" ANSI_RESET"\n\n");
 sleep(2);
 mainMenu();
 }
 else
 {
 // printf("NIC number is available.\n");
 }
 return;
 }

 mysql_free_result(result);
}

bool veryfyAccountNo(const int *veryfyAccountNomber)
{
 char query[500];
 snprintf(query, sizeof(query), "SELECT account_no FROM account WHERE account_no = %d", *veryfyAccountNomber);

 MYSQL_RES *result = executeQuery(query);

 if (result == NULL)
 {
 printf(ANSI_BG_RED ANSI_COLOR_BLACK"ERROR CHECKING ACCOUNT AVAILABILITY !!!"ANSI_RESET"\n");
 // Handle error or return an error code
 return false;
 }
 else
 {
 if (mysql_num_rows(result) > 0)
 {
 printf(ANSI_BG_RED ANSI_COLOR_BLACK"ACCOUNT NUMBER IS NOT AVAILABALE ."ANSI_RESET ANSI_BLINK" GENERATING A NEW ONE !!!"ANSI_RESET"\n");
 // Generate a new account number or handle as needed
 }
 else
 {
 // printf("Account number is available.\n");
 }

 mysql_free_result(result);
 return true; // Account number is available
 }
}

// END CREATE ACCOUNT.................................................

// LOGIN ACCOUNT......................................................
// Function to log in to an account
void loginAcc()
{
 char accountNo[MAX_STRING_LENGTH];
 char inputPassword[MAX_STRING_LENGTH];

 printf("\n...............................................................................\n\n");
 printf(ANSI_COLOR_CYAN "FILL THIS FORM TO LOGIN YOUR ACCOUNT .\n\n" ANSI_COLOR_RESET);
 printf(ANSI_COLOR_MAGENTA"Enter your Account No : "ANSI_RESET);
 scanf("%s", accountNo);

 printf(ANSI_COLOR_MAGENTA"Enter your Password : "ANSI_RESET);
 scanf("%s", inputPassword);

 char query[500];
 snprintf(query, sizeof(query), "SELECT * FROM account WHERE account_no = '%s' AND password = '%s'", accountNo, inputPassword);

 MYSQL_RES *result = executeQuery(query);

 if (result == NULL)
 {
 printf("Error executing query.\n");
 // Handle error or return an error code
 }
 else
 {
 if (mysql_num_rows(result) > 0)
 {
 printf("\n"ANSI_BG_GREEN ANSI_BLINK "LOGIN SUCCESSFUL !!!"ANSI_RESET"\n");
 printf("\n...............................................................................\n\n");
 inLogin(accountNo, inputPassword);
 }
 else
 {
 printf("\n"ANSI_BG_RED ANSI_COLOR_BLACK"INVALID ACCOUNT NUMBER / PASSWORD ."ANSI_RESET ANSI_BLINK ANSI_BG_RED" PLEASE TRY AGAIN !!!"ANSI_RESET"\n");
 sleep(2);
 mainMenu();
 }
 }
}

// Function to enterd account_no and password correct after.
void inLogin(const char *inAccNo, const char *inPassword)
{
 char query[500];
 snprintf(query, sizeof(query), "SELECT nic FROM account WHERE account_no = '%s'", inAccNo);

 MYSQL_RES *result = executeQuery(query);

 if (result == NULL)
 {
 printf("\nError executing query.\n");
 // Handle error or return an error code
 }
 else
 {
 MYSQL_ROW row = mysql_fetch_row(result);

 if (row != NULL)
 {
 char *inNicNo = row[0]; // Use char* instead of char
 // printf("NIC Number: %s\n", inNicNo);

 // Move the declaration outside the if block
 char query2[MAX_QUERY_LENGTH];
 snprintf(query2, sizeof(query2), "SELECT first_name, last_name FROM customer WHERE nic = '%s'", inNicNo);

 MYSQL_RES *result2 = executeQuery(query2);

 if (result2 == NULL)
 {
 printf("\nError executing second query.\n");
 // Handle error or return an error code
 }
 else
 {
 MYSQL_ROW row2 = mysql_fetch_row(result2);

 if (row2 != NULL)
 {
 char *firstName = row2[0];
 char *lastName = row2[1];
 // printf("Account Holder Name : %s %s\n", firstName, lastName);
 inLoginMenu(firstName, lastName, inNicNo);
 }
 else
 {
 printf("Customer not found.\n");
 }

 mysql_free_result(result2);
 }
 }
 else
 {
 printf("Account not found.\n");
 }
 }

 mysql_free_result(result);
}

void inLoginMenu(const char *inMfirstName, const char *inMlastName, const char *inMnicNo)
{
 // Local variables to store modified names
 char inMFirstName[MAX_NAME_LENGTH];
 char inMLastName[MAX_NAME_LENGTH];

 // Copy the input names to local variables and capitalize the first letters
 snprintf(inMFirstName, sizeof(inMFirstName), "%s", inMfirstName);
 snprintf(inMLastName, sizeof(inMLastName), "%s", inMlastName);
 capitalizeFirstLetter(inMFirstName);
 capitalizeFirstLetter(inMLastName);

 // Print the formatted login information
 printf("\n"ANSI_COLOR_CYAN"Welcome, %s %s "ANSI_RESET"\n\n", inMFirstName, inMLastName);

 // Rest of your code...

 int inMchoice;

 // Display menu options
 printf(ANSI_COLOR_MAGENTA"Press (1) : "ANSI_RESET"Withdraw\n");
 printf(ANSI_COLOR_MAGENTA"Press (2) : "ANSI_RESET"Deposit\n");
 printf(ANSI_COLOR_MAGENTA"Press (3) : "ANSI_RESET"Transfer to Another Account\n");
 printf(ANSI_COLOR_MAGENTA"Press (4) : "ANSI_RESET"Contact Us\n");
 printf(ANSI_COLOR_MAGENTA"Press (5) : "ANSI_RESET"Withdraw & Deposit History\n");
 printf(ANSI_COLOR_MAGENTA"Press (6) : "ANSI_RESET"Main Menu\n");
 printf(ANSI_COLOR_MAGENTA"Press (7) : "ANSI_RESET"Exit\n");

 // Prompt for user choice
 printf("\n"ANSI_COLOR_BLUE"Enter Your Choice : "ANSI_RESET);

 // Use a loop to ensure valid input
 while (scanf("%d", &inMchoice) != 1 || inMchoice < 1 || inMchoice > 7)
 {
 printf(ANSI_BG_RED ANSI_COLOR_BLACK"YOUR INPUT IS INVALID ."ANSI_RESET ANSI_BLINK ANSI_BG_RED" PLEASE ENTER A NUMBER (BETWEEN 1 AND 7)"ANSI_RESET);
 while (getchar() != '\n')
 ; // Clear input buffer
 }

 // Switch statement for user's choice
 switch (inMchoice)
 {
 case 1:
 inMwithdraw(inMfirstName, inMlastName, inMnicNo);
 break;
 case 2:
 inMdeposit(inMfirstName, inMlastName, inMnicNo);
 break;
 case 3:
 inMtransfer(inMfirstName, inMlastName, inMnicNo);
 break;
 case 4:
 inMloan(inMfirstName, inMlastName, inMnicNo);
 break;
 case 5:
 inMwithDepHistory(inMnicNo);
 break;
 case 6:
 mainMenu();
 break;
 case 7:
 printf("\n\n" ANSI_COLOR_BLACK ANSI_BG_GREEN "THANK YOU FOR USING DEVELOPMENT BANK SYSTEM ."ANSI_RESET ANSI_BLINK ANSI_BG_GREEN " GOODBYE !!! " ANSI_RESET "\n\n");
 sleep(2);
 mysql_close(conn);
 exit(EXIT_SUCCESS);
 break;
 default:
 mainMenu();
 break;
 }
}

// Function to capitalize the first letter of a string
char *capitalizeFirstLetter(char *str)
{
 if (str != NULL && str[0] != '\0')
 {
 str[0] = toupper(str[0]);
 }
 return str;
}

// END INLOGIN MENU

// Function to withdraw money befor veryfication nic and account holder
void inMwithdraw(const char *WFirstName, const char *WLastName, const char *WNicNo)
{
 char WSnicNo[20]; // Assuming a maximum length for NIC number

 int attempt = 0;
 while (attempt < 3)
 {
 printf("\n"ANSI_COLOR_MAGENTA"Enter [Account Holder] NIC number: "ANSI_RESET);
 scanf("%s", WSnicNo);

 if (strcmp(WSnicNo, WNicNo) == 0)
 {
 printf("\nHello, %s %s\n", WFirstName, WLastName);
 outMwithdraw(WFirstName, WLastName, WNicNo);
 break;
 }
 else
 {
 printf("\nThis [ACCOUNT HOLDER] NIC is incorrect. Please try again.\n");
 attempt++;
 sleep(2);
 }
 }

 if (attempt == 3)
 {
 printf("\nThree incorrect attempts. Redirecting to main menu.\n");
 sleep(2);
 mainMenu();
 }
}

// Function to withdraw money MYSQL codes and more options
void outMwithdraw(const char *outWFirstName, const char *outWLastName, const char *outWNicNo)
{
 int outWSqlacount_no; // mysql account_no add this varible

 // Construct the SQL query
 char query[MAX_QUERY_LENGTH];
 snprintf(query, MAX_QUERY_LENGTH, "SELECT account_no FROM account WHERE nic='%s'", outWNicNo);

 // Execute the query
 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "mysql_query failed\n");
 mysql_close(conn);
 exit(1);
 }

 // Get the result set
 result = mysql_store_result(conn);
 if (result == NULL)
 {
 fprintf(stderr, "mysql_store_result failed\n");
 mysql_close(conn);
 exit(1);
 }

 // Fetch the row
 row = mysql_fetch_row(result);

 // Check if a row is returned
 if (row != NULL)
 {
 // Access the account_no value
 outWSqlacount_no = (atoi(row[0]));
 printf("\n");
 afterOutMWithdraw(outWSqlacount_no); // Withdraw money
 }
 else
 {
 printf("No account found for NIC %s\n", outWNicNo);
 }

 // Free the result set and close the connection
 mysql_free_result(result);
 mysql_close(conn);
}

// Function to two function after withdraw money
void afterOutMWithdraw(const int aOutAccNo)
{
 printf(".................................................................................\n\n");
 int inMyAmount = accountAmountCheck(aOutAccNo);
 printf(ANSI_BG_YELLOW "Your current amount is : %d\n" ANSI_COLOR_RESET, inMyAmount);
 printf(".................................................................................\n\n");

 int maxAttempts = 3;
 int aOWmoney;
 int withdrawalSuccessful = 0; // Flag to track successful withdrawal
 int myOption;

 for (int attempts = 0; attempts < maxAttempts; ++attempts)
 {
 printf(" * Minimum amount is [Rs.100.00] and Maximum amount is [Rs.5000.00]\n");
 printf(" * Enter your amount in Multiples of [100]\n\n");
 printf("Enter your Withdraw amount : ");
 scanf("%d", &aOWmoney);

 if (validAmountCheck(aOWmoney))
 {
 if (aOWmoney > inMyAmount)
 {
 printf(ANSI_COLOR_GREEN "\nYOUR ACCOUNT BALANCE IS NOT VALID\n\n" ANSI_COLOR_RESET);
 sleep(2);
 }
 else
 {
 printf("WITHDRAWAL SUCCESSFUL !!!\n");
 withdrawalSuccessful = 1; // Set the flag to indicate success

 // Update the account balance
 char query[500];
 snprintf(query, sizeof(query), "INSERT INTO withdraw (account_no, withdraw_amount) VALUES (%d, %d)", aOutAccNo, aOWmoney);

 executeQuery(query);
 updateWithMoney(aOutAccNo, aOWmoney);

 printf("\n\n(1) : View your current account balance\n");
 printf("(2) : Withdraw Additional Money\n");
 printf("(3) : Go To Main Menu\n");
 printf("(4) : Exit\n\n");

 printf("Enter Your Option : ");
 scanf("%d", &myOption);

 switch (myOption)
 {
 case 1:
 printf("Your new balance is : %d", accountAmountCheck(aOutAccNo));
 sleep(2);
 mainMenu();
 break;
 case 2:
 // Allow the user to withdraw additional money
 break;
 case 3:
 mainMenu();
 return;
 case 4:
 printf("\nThank you for using development bank service. Good Bye!\n\n");
 sleep(2);
 mysql_close(conn);
 exit(EXIT_SUCCESS);
 default:
 break;
 }

 break;
 }
 }
 else
 {
 printf("\nAmount is invalid.\n");
 printf("Minimum amount is [Rs.100.00] and Maximum amount is [Rs.5000.00]\n");
 printf("Enter your amount in Multiples of [100]\n\n");
 sleep(2);

 if (attempts < maxAttempts - 1)
 {
 printf("You have %d attempts remaining.\n\n", maxAttempts - attempts - 1);
 }
 else
 {
 printf("Out of attempts. Exiting main menu.\n");
 return; // Exit the function without calling mainMenu()
 }
 }
 }

 if (withdrawalSuccessful)
 {
 mainMenu(); // Call mainMenu() only if withdrawal was successful
 }
}

// Function to check the amount for a given account number
int accountAmountCheck(const int checkAAcountNo)
{

 int inAmount;

 if (conn == NULL)
 {
 fprintf(stderr, "mysql_init() failed\n");
 return EXIT_FAILURE;
 }

 char query[MINIMUM_QUERY_LENGTH];
 snprintf(query, sizeof(query), "SELECT amount FROM money WHERE account_no = %d", checkAAcountNo);

 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "mysql_query() failed\n");
 mysql_close(conn);
 return EXIT_FAILURE;
 }

 MYSQL_RES *result = mysql_store_result(conn);

 if (result == NULL)
 {
 fprintf(stderr, "mysql_store_result() failed\n");
 mysql_close(conn);
 return EXIT_FAILURE;
 }

 MYSQL_ROW row = mysql_fetch_row(result);

 if (row != NULL)
 {
 inAmount = atof(row[0]); // Convert the amount from string to double
 }
 else
 {
 fprintf(stderr, "Account not found\n");
 mysql_free_result(result);
 mysql_close(conn);
 return EXIT_FAILURE;
 }

 mysql_free_result(result);

 return inAmount;
}

// Function to Check if the amount is between 100 and 5000 (inclusive) and is a multiple of 100
int validAmountCheck(int amount)
{

 if (amount >= 100 && amount <= 5000 && amount % 100 == 0)
 {
 return 1; // Valid amount
 }
 else
 {
 return 0; // Invalid amount
 }
}

// Function Deposit start
// Function to get the account number based on NIC
int getAccountNumber(const char *DNicNo)
{
 char query[MAX_QUERY_LENGTH];
 snprintf(query, MAX_QUERY_LENGTH, "SELECT account_no FROM account WHERE nic='%s'", DNicNo);

 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "mysql_query failed\n");
 mysql_close(conn);
 exit(1);
 }

 MYSQL_RES *result = mysql_store_result(conn);
 if (result == NULL)
 {
 fprintf(stderr, "mysql_store_result failed\n");
 mysql_close(conn);
 exit(1);
 }

 MYSQL_ROW row = mysql_fetch_row(result);

 if (row != NULL)
 {
 return atoi(row[0]);
 }
 else
 {
 printf("No account found for NIC %s\n", DNicNo);
 return -1; // Return -1 for no account found
 }
}

// Function to update the amount in the money table
void updateMoney(int accountNo, int amount)
{
 char query[MAX_QUERY_LENGTH];
 snprintf(query, MAX_QUERY_LENGTH, "UPDATE money SET amount = amount + %d WHERE account_no = %d", amount, accountNo);

 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "mysql_query failed\n");
 mysql_close(conn);
 exit(1);
 }
}

// Function to update amount withdreaw
void updateWithMoney(int accountNo, int amount)
{
 char query[MAX_QUERY_LENGTH];
 snprintf(query, MAX_QUERY_LENGTH, "UPDATE money SET amount = amount - %d WHERE account_no = %d", amount, accountNo);

 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "mysql_query failed\n");
 mysql_close(conn);
 exit(1);
 }
}

void inMdeposit(const char *DFirstName, const char *DLastName, const char *DNicNo)
{
 int inMdAmount;
 int inMdSwitch;

 int inMSqlacount_no = getAccountNumber(DNicNo);

 if (inMSqlacount_no != -1)
 {
 printf("\nHello, %s %s Enter your [DEPOSIT] amount : ", DFirstName, DLastName);
 scanf("%d", &inMdAmount);

 if (validAmountCheck(inMdAmount))
 {

 updateMoney(inMSqlacount_no, inMdAmount);
 printf("\nYour deposit successfully added.\n");
 char query[500];
 snprintf(query, sizeof(query), "INSERT INTO deposits (account_no, deposit_amount) VALUES (%d, %d)", inMSqlacount_no, inMdAmount);
 sleep(2);
 processDepositOptions(DFirstName, DLastName, DNicNo, inMSqlacount_no);
 }
 else
 {
 printf("Invalid deposit amount. Please try again.\n");
 inMdeposit(DFirstName, DLastName, DNicNo);
 }
 }
}

void processDepositOptions(const char *DFirstName, const char *DLastName, const char *DNicNo, int inMSqlacount_no)
{
 int inMdSwitch;

 do
 {
 // Display menu options
 printf("\n(1) : ADD [Deposit] money\n");
 printf("(2) : Go to [MAIN MENU]\n");
 printf("(3) : Check your balance\n");
 printf("(4) : Exit\n\n");
 printf("Enter option number : ");
 scanf("%d", &inMdSwitch);

 switch (inMdSwitch)
 {
 case 1:
 inMdeposit(DFirstName, DLastName, DNicNo);
 break; // Continue with deposit loop
 case 2:
 mainMenu();
 return; // Exit the function after returning from mainMenu
 case 3:
 displayAccountBalance(inMSqlacount_no);
 break;
 case 4:
 exitProgram(DFirstName, DLastName);
 default:
 printf("Invalid option. Please choose a valid option.\n");
 }
 } while (1);
}

void displayAccountBalance(int inMSqlacount_no)
{
 int inMAmount = accountAmountCheck(inMSqlacount_no);
 printf(ANSI_COLOR_GREEN "\nYour account balance is : %d\n" ANSI_COLOR_RESET, inMAmount);
}

// Function transfer start
// Function to transfer money acount to acount
void getUserInput(int *accountNumber)
{
 printf("\nEnter [Transfer] Account no: ");
 scanf("%d", accountNumber);
}

void inMtransfer(const char *TFirstName, const char *TLastName, const char *TNicNo)
{

 printf("Enter your [Transfer] account no :");
 printf("\nHello");

 printf("Hello");

 int inMtaccNo;

 printf("\n\n");
 printf("Hello %s %s\n", TFirstName, TLastName);
 int inMtMyAccNo = getAccountNumber(TNicNo);
 int inMtMyBalance = accountAmountCheck(inMtMyAccNo);

 printf("Your Account Balance is: %d\n", inMtMyBalance);

 do
 {
 getUserInput(&inMtaccNo);

 char *inMtnicc = showNicNo(inMtaccNo);

 if (inMtnicc != NULL)
 {
 // printf("\nNIC Number of Target Account Holder: %s\n", inMtnicc);
 tAfterGetNic(inMtnicc, TNicNo);
 free(inMtnicc); // Free memory allocated by strdup
 break; // Exit loop if NIC is found
 }
 else
 {
 printf("\nInvalid Account Number. Please try again.\n\n");
 printf("Try Again Press [1]\n");
 printf("Go Main Menu Press [2]\n");
 printf("Exit Press [3]\n");
 printf("\nEnter your number : ");
 scanf("%d", &inMtaccNo);

 switch (inMtaccNo)
 {
 case 1:
 // Try again
 break;

 case 2:
 mainMenu();
 break;

 case 3:
 exitProgram(TFirstName, TLastName);
 return;

 default:
 printf("\nInvalid input. Please try again.\n");
 break;
 }
 }
 } while (1);
}

// After get nic no
void tAfterGetNic(const char *tgetNic, const char *myNicNo)
{
 int transRespond;

 char *tAFullName = showName(tgetNic);
 printf("\n%s\n", tAFullName);
 free(tAFullName);

 printf("[1] : Correct your acount enter account no and holder name.\n");
 printf("[2] : Wrong acount no and user name try go to main menu.\n");
 printf("[3] : Exit\n\n");

 printf("Type your respond 1 or 2 or 3 : ");
 scanf("%d", &transRespond);

 switch (transRespond)
 {
 case 1:
 transfer(tgetNic, myNicNo);
 break;
 case 2:
 mainMenu();
 case 3:
 exit(0);
 default:
 printf("Invalid input try again. Good bye Development Bank.");
 break;
 }
}

// transfer
void transfer(const char *holderNic, const char *mytNic)
{
 int transANo = getAccountNumber(holderNic);
 int myANo = getAccountNumber(mytNic);

 int trnsAAmount = accountAmountCheck(transANo);
 int myAAmount = accountAmountCheck(myANo);

 int transAmount;
 int inmyval;

 int xChoise;

 do
 {
 printf("\nEnter your transfer amount: ");
 scanf("%d", &transAmount);

 if (transAmount % 10 != 0)
 {
 printf("Invalid amount [must be a multiple of 10]. Try again.\n");
 }
 else
 {
 if (transAmount <= myAAmount)
 {
 updateMoney(transANo, transAmount);
 updateWithMoney(myANo, transAmount);
 printf("\nTransaction Succsussful.\n\n");
 // Mysql transactions table update...
 char query[500];
 snprintf(query, sizeof(query), "INSERT INTO transactions (transfer_acc_no, user_acc_no, amount) VALUES (%d, %d, %d)", transANo, myANo, transAmount);

 executeQuery(query);

 printf("[1] : Go to Main Menu.\n");
 printf("[2] : Exit\n\n");

 printf("Your choise press 1 or 2 : ");
 scanf("%d", &xChoise);

 switch (xChoise)
 {
 case 1:
 mainMenu();
 break;
 case 2:
 exit(0);
 break;
 default:
 printf("Try again : ");
 break;
 }

 break;
 }
 else
 {
 printf("Insufficient funds in your account.\n\n");
 printf("[1] : Try Again\n");
 printf("[2] : Exit\n");
 printf("[3] : Go to Main Menu\n\n");
 printf("Exit or try again: ");
 scanf("%d", &inmyval);

 switch (inmyval)
 {
 case 1:
 // Retry the transfer process
 break;
 case 2:
 exit(0);
 case 3:
 mainMenu();
 default:
 break;
 }
 }
 }
 } while (1); // Infinite loop until a valid transfer amount is entered
}

// Contact Us
void inMloan(const char *LFirstName, const char *LLastName, const char *LNicNo)
{
 printf("Welcome to Development Bank\n");
 printf("Bank Developer Name = Dhanuja Dissanayke\n");
 printf("Conntact No : 0725563410");

 sleep(3); // Pause for 3 seconds
 mainMenu(); // Return to the main menu
}

// Function to view withdraw and deposit history
void inMwithDepHistory(const char *histNic)
{
 int historyChoise;

 printf("\n\n[1] : Print your WITHDRAWAL history.\n");
 printf("[2] : Print your DEPOSIT history.\n");
 printf("[3] : Print your TRANSACTION history.\n");
 printf("[4] : Go to main Menu.\n\n");

 printf("Enter Your Choice : ");
 scanf("%d", &historyChoise);

 printf("\n\n");

 switch (historyChoise)
 {
 case 1:
 hisWithHis(histNic);
 break;
 case 2:
 hisDepHis(histNic);
 break;
 case 3:
 hisTransHis(histNic);
 break;
 case 4:
 mainMenu();
 break;
 default:
 printf("Invalid Choise. Try again.");
 break;
 }
}

// Function Get data Withdrawal history...
void hisWithHis(const char *niswNic)
{
 int hisWAno = getAccountNumber(niswNic);

 // Check if the connection is NULL before executing the query
 if (conn == NULL)
 {
 fprintf(stderr, "Error: MySQL connection is NULL.\n");
 return;
 }

 char query[256];
 snprintf(query, sizeof(query), "SELECT withdraw_id, deposit_date_time, withdraw_amount FROM withdraw WHERE account_no = %d", hisWAno);

 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "Error: %s\n", mysql_error(conn));
 return;
 }

 result = mysql_store_result(conn);

 MYSQL_FIELD *fields = mysql_fetch_fields(result);

 // Print frame and column headers with color
 printf("\x1B[34m"); // Blue color
 printf("+");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 for (int j = 0; j < 32; j++)
 {
 printf("-");
 }
 printf("+");
 }
 printf("\n");

 printf("|");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 if (strcmp(fields[i].name, "account_no") != 0)
 {
 printf("\x1B[34;1m%-30s\x1B[0m|", fields[i].name);
 }
 }
 printf("\n");

 printf("+");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 for (int j = 0; j < 32; j++)
 {
 printf("-");
 }
 printf("+");
 }
 printf("\n");
 printf("\x1B[0m"); // Reset color

 // Print data with alternating row colors
 int rowCounter = 0;
 while ((row = mysql_fetch_row(result)) != NULL)
 {
 if (rowCounter % 2 == 0)
 {
 printf("\x1B[32m"); // Green color for even rows
 }
 else
 {
 printf("\x1B[36m"); // Cyan color for odd rows
 }

 printf("|");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 if (strcmp(fields[i].name, "account_no") != 0)
 {
 printf("%-30s|", row[i] ? row[i] : "NULL");
 }
 }
 printf("\n");

 printf("+");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 for (int j = 0; j < 32; j++)
 {
 printf("-");
 }
 printf("+");
 }
 printf("\n");
 printf("\x1B[0m"); // Reset color
 rowCounter++;
 }

 mysql_free_result(result);
}

// Function to Get data Deposit history...
void hisDepHis(const char *nisdNic)
{
 int hisDAno = getAccountNumber(nisdNic);

 // Check if the connection is NULL before executing the query
 if (conn == NULL)
 {
 fprintf(stderr, "Error: MySQL connection is NULL.\n");
 return;
 }

 char query[256];
 snprintf(query, sizeof(query), "SELECT deposit_id, deposit_date_time, deposit_amount FROM deposits WHERE account_no = %d", hisDAno);

 if (mysql_query(conn, query) != 0)
 {
 fprintf(stderr, "Error: %s\n", mysql_error(conn));
 return;
 }

 result = mysql_store_result(conn);

 MYSQL_FIELD *fields = mysql_fetch_fields(result);

 // Print frame and column headers with color
 printf("\x1B[34m"); // Blue color
 printf("+");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 for (int j = 0; j < 32; j++)
 {
 printf("-");
 }
 printf("+");
 }
 printf("\n");

 printf("|");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 if (strcmp(fields[i].name, "account_no") != 0)
 {
 printf("\x1B[34;1m%-30s\x1B[0m|", fields[i].name);
 }
 }
 printf("\n");

 printf("+");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 for (int j = 0; j < 32; j++)
 {
 printf("-");
 }
 printf("+");
 }
 printf("\n");
 printf("\x1B[0m"); // Reset color

 // Print data with alternating row colors
 int rowCounter = 0;
 while ((row = mysql_fetch_row(result)) != NULL)
 {
 if (rowCounter % 2 == 0)
 {
 printf("\x1B[32m"); // Green color for even rows
 }
 else
 {
 printf("\x1B[36m"); // Cyan color for odd rows
 }

 printf("|");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 if (strcmp(fields[i].name, "account_no") != 0)
 {
 printf("%-30s|", row[i] ? row[i] : "NULL");
 }
 }
 printf("\n");

 printf("+");
 for (int i = 0; i < mysql_num_fields(result); i++)
 {
 for (int j = 0; j < 32; j++)
 {
 printf("-");
 }
 printf("+");
 }
 printf("\n");
 printf("\x1B[0m"); // Reset color
 rowCounter++;
 }

 mysql_free_result(result);
}

// Function to Get Transaction History...
void hisTransHis(const char *nitNic)
{
 int hisTAno = getAccountNumber(nitNic);
}

// END LOGIN ACCOUNT................................................
// Function to forget in account user name and password etc.
void forgetAcc()
{
}

// END LOGIN ACCOUNT..................................................
// Function to clear the terminal
void clearScreen()
{
// This system-specific command clears the terminal
#ifdef _WIN32
 system("cls");
#else
 system("clear");
#endif
}

// Function to clear the input buffer
void clearBuffer()
{
 int c;
 while ((c = getchar()) != '\n' && c != EOF)
 {
 }
}

// Exit
void exitProgram(const char *DFirstName, const char *DLastName)
{
 printf("\nThank you %s %s for using development bank service. Good Bye!\n\n", DFirstName, DLastName);
 sleep(2);
 mysql_close(conn);
 exit(EXIT_SUCCESS);
}

// Main function.
int main()
{
 initialize();
 mainMenu();
 return 0;
}