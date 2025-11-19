/**
 * @file main.cpp
 * @brief Simple library management system demonstrating OOP principles in C++.
 * @author
 * @date 2025
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
using namespace std;

/**
 * @brief Represents a book author.
 */
class Author {
    string name;
public:
    /**
     * @brief Constructor.
     * @param n Author's name.
     */
    explicit Author(string n) : name(move(n)) {}

    /**
     * @brief Get the author's name.
     * @return Name of the author.
     */
    string getName() const { return name; }
};

/**
 * @brief Abstract base class representing a book.
 */
class Book {
protected:
    int id;
    string title;
    Author author;
    int year;
    bool available;
    string genre;
public:
    /**
     * @brief Constructor.
     * @param i Book ID.
     * @param t Title.
     * @param a Author.
     * @param y Year of publication.
     * @param g Genre.
     */
    Book(int i, string t, Author a, int y, string g)
        : id(i), title(move(t)), author(move(a)), year(y), available(true), genre(move(g)) {}

    virtual ~Book() = default;

    /**
     * @brief Print book information.
     */
    virtual void printInfo() const = 0;

    /**
     * @brief Clone the book object.
     * @return Unique pointer to a new copy of the book.
     */
    virtual unique_ptr<Book> clone() const = 0;

    /**
     * @brief Borrow the book (mark as unavailable).
     * @return True if borrowed successfully, false if already borrowed.
     */
    bool borrow() { if (!available) return false; available = false; return true; }

    /**
     * @brief Return the book (mark as available).
     */
    void returnBook() { available = true; }

    /**
     * @brief Get the book title.
     * @return Book title.
     */
    string getTitle() const { return title; }
};

/**
 * @brief Printed book (physical copy).
 */
class PrintedBook : public Book {
    int pages;
public:
    /**
     * @brief Constructor.
     * @param i Book ID.
     * @param t Title.
     * @param a Author.
     * @param y Year of publication.
     * @param g Genre.
     * @param p Number of pages.
     */
    PrintedBook(int i, string t, Author a, int y, string g, int p)
        : Book(i, move(t), move(a), y, move(g)), pages(p) {}

    void printInfo() const override {
        cout << "[Printed] " << title << " (" << year << "), " << author.getName()
             << ", " << pages << " pages, " << (available ? "available" : "borrowed") << "\n";
    }

    unique_ptr<Book> clone() const override { return make_unique<PrintedBook>(*this); }
};

/**
 * @brief Electronic book.
 */
class EBook : public Book {
    double sizeMB;
public:
    /**
     * @brief Constructor.
     * @param i Book ID.
     * @param t Title.
     * @param a Author.
     * @param y Year.
     * @param g Genre.
     * @param s File size in MB.
     */
    EBook(int i, string t, Author a, int y, string g, double s)
        : Book(i, move(t), move(a), y, move(g)), sizeMB(s) {}

    void printInfo() const override {
        cout << "[EBook] " << title << " (" << year << "), " << author.getName()
             << ", " << fixed << setprecision(1) << sizeMB << " MB, "
             << (available ? "available" : "borrowed") << "\n";
    }

    unique_ptr<Book> clone() const override { return make_unique<EBook>(*this); }
};

/**
 * @brief Audiobook.
 */
class AudioBook : public Book {
    double duration;
public:
    /**
     * @brief Constructor.
     * @param i Book ID.
     * @param t Title.
     * @param a Author.
     * @param y Year.
     * @param g Genre.
     * @param d Duration in hours.
     */
    AudioBook(int i, string t, Author a, int y, string g, double d)
        : Book(i, move(t), move(a), y, move(g)), duration(d) {}

    void printInfo() const override {
        cout << "[Audio] " << title << " (" << year << "), " << author.getName()
             << ", " << fixed << setprecision(1) << duration << " hours, "
             << (available ? "available" : "borrowed") << "\n";
    }

    unique_ptr<Book> clone() const override { return make_unique<AudioBook>(*this); }
};

/**
 * @brief Book catalog that stores all books in the library.
 */
class Catalog {
    vector<unique_ptr<Book>> books;
public:
    /**
     * @brief Add a book to the catalog.
     * @param b Book object (will be cloned).
     */
    void addBook(const Book& b) { books.push_back(b.clone()); }

    /**
     * @brief List all books in the catalog.
     */
    void listAll() const { for (const auto& b : books) b->printInfo(); }

    /**
     * @brief Template-based search function.
     * @tparam Pred Predicate type.
     * @param p Predicate function to test books.
     * @return Vector of pointers to matching books.
     */
    template<typename Pred>
    vector<Book*> search(Pred p) {
        vector<Book*> result;
        for (auto& b : books)
            if (p(*b)) result.push_back(b.get());
        return result;
    }
};

/**
 * @brief Abstract base class for all library users.
 */
class User {
protected:
    string name;
    int borrowed;
public:
    /**
     * @brief Constructor.
     * @param n User name.
     */
    User(string n) : name(move(n)), borrowed(0) {}

    /** @brief Display user information. */
    virtual void showRole() const = 0;

    /** @brief Check if the user can borrow more books. */
    virtual bool canBorrow() const = 0;

    /** @brief Register a borrowed book. */
    void borrowBook() { borrowed++; }

    /** @brief Return a borrowed book. */
    void returnBook() { if (borrowed > 0) borrowed--; }

    /** @brief Get user's name. */
    string getName() const { return name; }
};

/**
 * @brief Represents a student user.
 */
class Student : public User {
    string faculty;
    int yearStudy;
public:
    /**
     * @brief Constructor.
     * @param n Name.
     * @param f Faculty.
     * @param y Year of study.
     */
    Student(string n, string f, int y) : User(move(n)), faculty(move(f)), yearStudy(y) {}

    void showRole() const override {
        cout << name << " - Student, " << faculty << ", year " << yearStudy << "\n";
    }

    bool canBorrow() const override { return borrowed < 5; }
};

/**
 * @brief Represents a librarian user.
 */
class Librarian : public User {
    string employeeId;
public:
    /**
     * @brief Constructor.
     * @param n Name.
     * @param id Employee ID.
     */
    Librarian(string n, string id) : User(move(n)), employeeId(move(id)) {}

    void showRole() const override {
        cout << name << " - Librarian, ID: " << employeeId << "\n";
    }

    bool canBorrow() const override { return true; }
};

/**
 * @brief Main library class containing catalog and users.
 */
class Library {
    Catalog catalog;
    vector<unique_ptr<User>> users;
    int nextBookId = 1;
public:
    /**
     * @brief Get catalog reference.
     * @return Reference to Catalog.
     */
    Catalog& getCatalog() { return catalog; }

    /**
     * @brief Generate new book ID.
     * @return New ID number.
     */
    int newBookId() { return nextBookId++; }

    /**
     * @brief Add a new student to the library.
     * @param n Name.
     * @param f Faculty.
     * @param y Year of study.
     * @return Pointer to the created Student.
     */
    Student* addStudent(string n, string f, int y) {
        auto u = make_unique<Student>(move(n), move(f), y);
        Student* ptr = u.get();
        users.push_back(move(u));
        return ptr;
    }

    /**
     * @brief Add a new librarian to the library.
     * @param n Name.
     * @param id Employee ID.
     * @return Pointer to the created Librarian.
     */
    Librarian* addLibrarian(string n, string id) {
        auto u = make_unique<Librarian>(move(n), move(id));
        Librarian* ptr = u.get();
        users.push_back(move(u));
        return ptr;
    }

    /**
     * @brief Display all users in the system.
     */
    void listUsers() const {
        for (auto& u : users) u->showRole();
    }
};

/**
 * @brief Display console menu.
 */
void printMenu() {
    cout << "\n=== Menu ===\n";
    cout << "1. Add book\n2. List catalog\n3. Add student\n4. Add librarian\n5. List users\n0. Exit\n";
}

/**
 * @brief Program entry point.
 */
int main() {
    Library lib;

    lib.getCatalog().addBook(PrintedBook(lib.newBookId(), "Book1", Author("Author1"), 2020, "History", 200));
    lib.getCatalog().addBook(EBook(lib.newBookId(), "Book2", Author("Author2"), 2021, "Poetry", 2.5));
    lib.getCatalog().addBook(AudioBook(lib.newBookId(), "Book3", Author("Author3"), 2019, "Drama", 3.0));

    int choice;
    while (true) {
        printMenu();
        cout << "Choice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        cin.ignore();
        if (choice == 0) break;

        if (choice == 1) {
            int type; cout << "Type (1-Printed,2-EBook,3-Audio): "; cin >> type; cin.ignore();
            string title, author, genre; int year;
            cout << "Title: "; getline(cin, title);
            cout << "Author: "; getline(cin, author);
            cout << "Year: "; cin >> year; cin.ignore();
            cout << "Genre: "; getline(cin, genre);
            int id = lib.newBookId();
            if (type == 1) { int pages; cout << "Pages: "; cin >> pages; cin.ignore(); lib.getCatalog().addBook(PrintedBook(id, title, Author(author), year, genre, pages)); }
            else if (type == 2) { double size; cout << "Size MB: "; cin >> size; cin.ignore(); lib.getCatalog().addBook(EBook(id, title, Author(author), year, genre, size)); }
            else { double dur; cout << "Duration hours: "; cin >> dur; cin.ignore(); lib.getCatalog().addBook(AudioBook(id, title, Author(author), year, genre, dur)); }
        }
        else if (choice == 2) { lib.getCatalog().listAll(); }
        else if (choice == 3) {
            string n, f; int y;
            cout << "Name: "; getline(cin, n);
            cout << "Faculty: "; getline(cin, f);
            cout << "Year: "; cin >> y; cin.ignore();
            lib.addStudent(n, f, y);
        }
        else if (choice == 4) {
            string n, id;
            cout << "Name: "; getline(cin, n);
            cout << "Employee ID: "; getline(cin, id);
            lib.addLibrarian(n, id);
        }
        else if (choice == 5) { lib.listUsers(); }
    }

    cout << "Exiting...\n";
    return 0;
}
