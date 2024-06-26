#include <iostream>
#include <iomanip>
#include <fstream>
#include "../header/helper.h"
#include "../header/termcolor.hpp"
#include "../header/library.h"
#include "../header/book.h"
#include "../header/userDatabase.h"
#include "../header/settleFines.h"

using namespace std;

int main() {
    Library database(11);                               // First, we must create our database                          
    userDatabase* userList = new userDatabase();        // Then, we create our user account list
    try {
    ifstream databaseFile;
    databaseFile.open("database.txt");                  // We must first open our database file

    if (databaseFile.is_open()) {
        string newBookTitle;
        string newBookAuthor;
        string newBookGenre;
        string newBookSummary;
        string trashVariable;

        while(getline(databaseFile, newBookTitle, '|') &&
              getline(databaseFile, newBookAuthor, '|') &&
              getline(databaseFile, newBookGenre, '|') &&
              getline(databaseFile, newBookSummary)) {
            database.insert(newBookTitle, newBookAuthor, newBookGenre, newBookSummary);
        }
        databaseFile.close();
    }

    else {
        databaseFile.close();
        throw runtime_error("ERROR: Couldn't open database file :(");        //Run time error in case file does not open
    }
    }

    catch(const exception& error) {
        cout << error.what() << endl;
    }

    try {
        ifstream accountFile;
        accountFile.open("accounts.txt");

        if (accountFile.is_open()) {
            string userName;
            string passWord;
            while (getline(accountFile, userName, ',') &&
                   getline(accountFile, passWord)) {
                    User* newUser = new User(userName, passWord);
                    userList->addUser(newUser);
            }
        }
        else {
            accountFile.close();
            throw runtime_error("ERROR: Couldn't open database file :(");
        }
    }

    catch (const exception& error) {
        cout << error.what() << endl;
    }

    //Once we create the database, we need to get our user logged in!
    int warningMessage = 0;
    char userInput;
    bool userContinue = true;
    User* newUser;
    string beginPrompt = "************************* Library Management System *************************";
    string createaccount = "I --- Create an Account --- I";
    string signin = "II --- Sign In --- II";
    string exitProgram = "III --- Exit Program --- III"; 

    do {
    cout << termcolor::yellow;
    centerText(beginPrompt, 160);
    cout << "\n\n";
    centerText(createaccount, 160);
    cout << "\n\n";
    centerText(signin, 160);
    cout << "\n\n";
    centerText(exitProgram, 160);
    cout << "\n\n";
    cout << termcolor::white;

    cin >> userInput;

    switch(userInput) {
        case '1':
        {
            // User wants to create a new account
            newUser = userList->createAccount();
            if (newUser != nullptr) {
                userContinue = false;
                break;
            }
            cout << termcolor::red;
            string createAccountError = "ERROR: Username has been taken!";
            centerText(createAccountError, 160);
            cout << "\n\n\n";
            break;

        }
        case '2':
        {
            // User wants to log into an existing account
            newUser = userList->signIn();
            if (newUser != nullptr) {
                userContinue = false;
                break;
            }

            cout << termcolor::red;
            string SignInError = "ERROR: Either Username/Password was entered incorrectly!";
            centerText(SignInError, 160);
            cout << "\n\n\n";
            break;
        }
        default:
        {
            // User wants to exit the program
            delete userList;
            return 0;
        }
    }
    } while (userContinue);



    // Then, the actual program must begin
    userContinue = true;

    do {
        outputMenu(warningMessage);
        cin >> userInput;

        switch(userInput) {
            case '1':
            {
                bool userContinueProfile = true;
                do {
                outputUserProfile(0);
                char userProfileInput;
                cin >> userProfileInput;

                switch(userProfileInput) {
                    case '1':
                    {
                        // TODO: Output Account Information
                        cout << "Username: " << newUser->getUsername() << endl;
                        cout << "Password: " << newUser->getPassword() << endl;
                        cout << "\n";
                        break;
                    }
                    case '2':
                    {   
                        // TODO: Output list of borrowed books
                        newUser->displayBooksOwned();
                        break;
                    }
                    case '3':
                    {
                        // TODO: View a list of book fines
                        newUser->displayFines();
                        break;
                    }

                    default:
                    {   
                        //User wants to exit
                        userContinueProfile = false;
                        break;

                    }
                }
                } while (userContinueProfile);

                warningMessage = 0;
                break;
            }
            case '2':
            {
                //TODO: Implement "search for book" function
                bool userContinueSearch = true;
                int searchType = 0;

                do {
                outputSearchMenu(searchType);
                char userSearchInput;
                cin >> userSearchInput;


                switch(userSearchInput) {
                    case '1':
                    {
                        // TODO: Call Search Function
                        cout << termcolor::yellow;
                        string searchInstructions = "Search for your book! (case-sensitive)\n\n";
                        centerText(searchInstructions, 160);
                        cout << termcolor::white;
                        string titleLookUp = "Title: ";
                        centerText(titleLookUp, 160);
                        string bookFind;
                        cin.ignore();
                        getline(cin, bookFind);
                        cout << "\n\n";

                        Book* bookFound = database.bookSearch(bookFind);

                        if (bookFound != nullptr) {
                            // TODO: Display Book Page and the appropriate options for borrowing
                            bookFound->displayInfo();
                            string borrowBook = "I --- Borrow Book --- I";
                            string returnBook = "II --- Return Book --- II";
                            string exitBookMenu = "III --- Exit to Search Menu --- III";
                            centerText(borrowBook, 160);
                            cout << "\n\n";
                            centerText(returnBook, 160);
                            cout << "\n\n";
                            centerText(exitBookMenu, 160);
                            cout << "\n\n";

                            cin >> userInput;

                            switch(userInput) {
                                case '1':
                                {   
                                    if (bookFound->getStatus()) {
                                    newUser->borrowBook(bookFound);
                                    bookFound->setCurrentBorrower(newUser->getUsername());
                                    bookFound->setStatus(false);
                                    Date currentBookDate;
                                    bookFound->setBorrowedDate(currentBookDate.getCurrentDate());
                                    cout << "\n\n --- Date Borrowed --- \n\n";
                                    bookFound->getBorrowedDate().outputCurrentDate();
                                    // bookFound->
                                    break;
                                    }
                                    else {
                                        cout << termcolor::red;
                                        string bookTaken = "Sorry, this book has been checked out by another user!";
                                        cout << "\n\n";
                                        centerText(bookTaken, 160);
                                        cout << "\n\n";
                                        break;
                                    }
                                }
                                case '2':
                                {
                                    // Return book function needed     
                                    if (bookFound->getCurrentBorrower() != newUser->getUsername()) {
                                        cout << termcolor::red;
                                        cout << "\n\n";
                                        string returnError = "Sorry, you can't return a book that doesn't belong to you!";
                                        centerText(returnError, 160);
                                        cout << "\n\n";
                                    }       
                                    else {                      
                                        bookFound->setStatus(true);
                                        bookFound->setCurrentBorrower("");
                                    }
                                    //newUser->returnBook(bookFound); return book needs to be a function within the user class. 
                                    break;
                                }
                                default:
                                {
                                    //User wants to exit, do nothing
                                    break;
                                }
                            }
                            
                        }

                        else {
                            cout << termcolor::red;
                            string errorBook = "No books found in the database, please try again!";
                            centerText(errorBook, 160);
                            cout << "\n\n" << termcolor::reset;
                        }
                        break;
                    }
                    case '2':
                    {   
                        // TODO: Call Add Book Function
                        // No add book function, just prompt the user to input new book information
                        // ONLY WORKS IF THE USER IS AN ADMIN!!
                        if (newUser->getUsername() != "Admin1") {
                            string adminError = "You do not have permission to do that!";
                            cout << termcolor::red << "\n\n";
                            centerText(adminError, 160);
                            cout << termcolor::reset << "\n\n";
                            break;
                        }

                        // If the user is an admin, we prompt them to input the book information;

                        string prompt1 = "Please enter your new book's information!";
                        string prompt2 = "Book Title: ";
                        string prompt3 = "Book Author: ";
                        string prompt4 = "Book Genre: ";
                        string prompt5 = "Book Summary: ";
                        string warningPrompt = "None of your inputs can be empty strings, and your summary must be at least 100 characters!";
                        string newBookTitle;
                        string newBookAuthor;
                        string newBookGenre;
                        string newBookSummary;
                        Book* bookTest = nullptr;

                        do {
                            cout << termcolor::yellow;
                            cout << "\n\n";
                            centerText(prompt1, 160);
                            cout << "\n\n";
                            centerText(warningPrompt, 160);
                            cout << "\n\n";
                            centerText(prompt2, 160);
                            cin.ignore();
                            getline(cin, newBookTitle);
                            cout << endl;
                            centerText(prompt3, 160);
                            getline(cin, newBookAuthor);
                            cout << endl;
                            centerText(prompt4, 160);
                            getline(cin, newBookGenre);
                            cout << endl;
                            centerText(prompt5, 160);
                            getline(cin, newBookSummary);
                            cout << "\n";
                            bookTest = database.bookSearch(newBookTitle);
                        } while((newBookTitle.size() < 1) || (newBookAuthor.size() < 1) || (newBookGenre.size() < 1) || (newBookSummary.size() < 100) || (bookTest != nullptr));

                        // Now that we have what we need, we need to write to the text file and insert it in the library. 

                        try {
                            ofstream databaseFile;
                            databaseFile.open("database.txt", ios::app);                  // We must first open our database file

                            if (databaseFile.is_open()) {
                                databaseFile << newBookTitle << "|" << newBookAuthor << "|" << newBookGenre << "|" << newBookSummary << "\n";

                                database.insert(newBookTitle, newBookAuthor, newBookGenre, newBookSummary);
                            
                            databaseFile.close();
                            bookTest = nullptr;
                        }

                        else {
                            databaseFile.close();
                            throw runtime_error("ERROR: Couldn't open database file :(");        //Run time error in case file does not open
                        }
                        }

                        catch(const exception& error) {
                        cout << error.what() << endl;
                        }


                        break;
                    }

                    default:
                    {
                        // Do nothing, user wants to return to the main menu
                        userContinueSearch = false;
                        break;
                    }
                }
                } while (userContinueSearch);


                warningMessage = 0;
                break;
            }
            case '3':
            {   
             //Implement settle fines functionality
                newUser->displayFines();

                //ask user if they want to pay fines
                cout << "Do you want to settle your fines? (Y/N): ";
                char payFines;
                cin >> payFines;

                if (payFines == 'Y' || payFines == 'y'){
                    PaymentProcessor payment_Processor;

                    double finesToPay = newUser -> getTotalFine();
                    payment_Processor.settleFine(finesToPay);
                }
                else {
                    cout << "Fines not settled. Returning to main menu.\n";
                }
                
                warningMessage = 0;
                break;
            }
            case '4':
            {
                if (userList->isAdmin(newUser)) {
                    userList->displayAllUsers();
                }

                else {
                    cout << termcolor::red;
                    string adminWarning = "You do not have permission to do that!";
                    cout << "\n\n";
                    centerText(adminWarning, 160);
                    cout << "\n\n";
                    cout << termcolor::reset;
                }
                warningMessage = 0;
                break;
            }
            case '5':
            {   
                //signInOrCreate();
                do {
                cout << termcolor::yellow;
                centerText(beginPrompt, 160);
                cout << "\n\n";
                centerText(createaccount, 160);
                cout << "\n\n";
                centerText(signin, 160);
                cout << "\n\n";
                centerText(exitProgram, 160);
                cout << "\n\n";
                cout << termcolor::white;

                cin >> userInput;
                switch(userInput) {
                case '1':
                {

                newUser = userList->createAccount();
                userContinue = false;
                break;

                }
                case '2':
                {
                    // User wants to log into an existing account
                newUser = userList->signIn();
                if (newUser != nullptr) {
                userContinue = false;
                break;
                }

                cout << termcolor::red;
                string SignInError = "ERROR: Either Username/Password was entered incorrectly!";
                centerText(SignInError, 160);
                cout << "\n\n\n";
                break;
            }
            default:
            {
            // User wants to exit the program
            delete userList;
            return 0;
        }
    }
    } while (userContinue);

                userContinue = true;
                warningMessage = 0;
                break;
            }
            case '6':
            {  
                userContinue = false; 
                warningMessage = 0;
                break;
            }
    
            default:
            {
                warningMessage = 1;
                break;
            }
        }

    } while (userContinue);

    //Reset back to normal color. 
    //delete newUser; testing

    cout << termcolor::reset;

    string EndOfProgram = "Program has been terminated";
    centerText(EndOfProgram, 160);
    cout << "\n\n\n\n\n\n\n\n\n\n\n";

    delete userList;
    return 0;
}