#include "../header/user.h"
#include "../header/book.h"
#include "../header/userFines.h"

using namespace std;

User::User() : username(""), password("") {
}

User::User(string user, string pass) : username(user), password(pass) {
}


string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

void User::setUsername(string user) {
    this->username = user;
}

void User::setPassword(string pass) {
    this->password = pass;
}

list<Book*> User::getBookList() const {
    return userBooks;
}

void User::borrowBook(Book* book) { // borrows the book and adds the book to the list of books they borrow
    userBooks.push_back(book);
}

void User::displayBooksOwned() const { // goes through each book and lists its title
    int count = 0;
    for (const auto& i : userBooks) {
        if (getUsername() == i->getCurrentBorrower()) {
            count++;
            cout << count << ". " << i->getBookTitle() << endl << endl;
        }
    }
    if (count == 0) {
        cout << "You are currently not borrowing any books" << endl << endl;
     }
}

void User::displayFines() {
    int count = 1;
    for(const auto& i : userBooks) {
        UserFines currentFine;
        double userFine = currentFine.calculateFine(i->getBorrowedDate());
        if (userFine != 0) {
            cout << count << ". " << i->getBookTitle() << ". Current Fine: " << userFine << endl;
            count++;
        }
        // Do nothing if no fines. 
    }
}

double User::getTotalFine() {
    double totUserFines = 0.0;
    for(const auto& i : userBooks) {
        UserFines tempFine;
        double userFine = tempFine.calculateFine(i->getBorrowedDate());
        if (userFine != 0) {
            totUserFines += userFine;
        }
        // Do nothing if no fines. 
    }
    return totUserFines;
}

