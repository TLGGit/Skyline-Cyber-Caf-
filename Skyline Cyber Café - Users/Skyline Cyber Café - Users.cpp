#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <regex>
#include <chrono>
#include <iomanip> 

using namespace std;

// Session class for tracking user activity
class Session {
private:
    string sessionId;
    chrono::system_clock::time_point startTime;
    chrono::system_clock::time_point endTime;
    int pagesPrinted;
    int pagesScanned;
    double totalAmount;
    bool isActive;
    string startTimeStr;

    // Cost constants
    static constexpr double COST_PER_PRINT = 0.20;
    static constexpr double COST_PER_SCAN = 0.15;

public:
    Session(const string& id) :
        sessionId(id),
        startTime(chrono::system_clock::now()),
        pagesPrinted(0),
        pagesScanned(0),
        totalAmount(0.0),
        isActive(true) {

        // Store formatted start time
        time_t now = chrono::system_clock::to_time_t(startTime);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);
        startTimeStr = buffer;
        startTimeStr.pop_back();
    }

    void endSession() {
        if (isActive) {
            endTime = chrono::system_clock::now();
            calculateBill();
            isActive = false;
        }
    }

    chrono::system_clock::time_point getStartTimePoint() const {
        return startTime;
    }

    chrono::system_clock::time_point getEndTimePoint() const {
        return endTime;
    }

    void addPrintJob(int pages) {
        if (isActive) {
            pagesPrinted += pages;
        }
    }

    void addScanJob(int pages) {
        if (isActive) {
            pagesScanned += pages;
        }
    }

    void calculateBill() {
        auto duration = chrono::duration_cast<chrono::minutes>(
            endTime - startTime).count();

        double printCost = pagesPrinted * COST_PER_PRINT;
        double scanCost = pagesScanned * COST_PER_SCAN;

        totalAmount = printCost + scanCost;
    }

    // Getters
    string getSessionId() const { return sessionId; }
    double getTotalAmount() const { return totalAmount; }
    bool getIsActive() const { return isActive; }
    string getStartTime() const { return startTimeStr; }
    int getPrintedPages() const { return pagesPrinted; }
    int getScannedPages() const { return pagesScanned; }
    chrono::system_clock::time_point getEndTime() const { return endTime; }

    void printBill() const {
        cout << "\n=== Session Bill ===\n";
        cout << "Session ID: " << sessionId << "\n";
        cout << "Start Time: " << startTimeStr << "\n";

        auto duration = chrono::duration_cast<chrono::minutes>(
            endTime - startTime).count();

        cout << "Duration: " << duration << " minutes\n";
        cout << "Pages Printed: " << pagesPrinted << "\n";
        cout << "Pages Scanned: " << pagesScanned << "\n";
        cout << "Total Amount: $" << fixed << setprecision(2) << totalAmount << "\n";
        cout << "================\n";
    }
};

// Structure to hold user information
struct User {
    string name;
    string email;
    string password;
    string userId;
    string joiningDate;
    vector<Session> sessions;
    double totalSpent;

    User() : totalSpent(0.0) {}
};

class SkylineCyberCafe {
private:
    vector<User> users;
    vector<string> adminEmails = { "admin1@skylinecyber.com", "admin2@skylinecyber.com" };

    string generateUserId() {
        srand(static_cast<unsigned int>(time(0)));
        return "USER" + to_string(rand() % 10000);
    }

    bool isPasswordStrong(const string& password) {
        regex strongPasswordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$");
        return regex_match(password, strongPasswordRegex);
    }

    User* findUser(const string& userId) {
        for (auto& user : users) {
            if (user.userId == userId) {
                return &user;
            }
        }
        return nullptr;
    }

    void handleSessionActivities(Session& session) {
        int choice;
        do {
            cout << "\nSession Activities Menu:"
                << "\n1. Print Pages"
                << "\n2. Scan Pages"
                << "\n3. End Session"
                << "\nChoose an option: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1: {
                int pages;
                cout << "Enter number of pages to print: ";
                cin >> pages;
                session.addPrintJob(pages);
                cout << "Print job added successfully!\n";
                break;
            }
            case 2: {
                int pages;
                cout << "Enter number of pages to scan: ";
                cin >> pages;
                session.addScanJob(pages);
                cout << "Scan job added successfully!\n";
                break;
            }
            case 3:
                cout << "Ending session...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 3);
    }

    void adminMenu() {
        int choice;
        do {
            cout << "\nAdmin Menu:"
                << "\n1. View User Details"
                << "\n2. Edit User Details"
                << "\n3. Delete User"
                << "\n4. list Usernames"
                << "\n5 Calculate Total Session Duration"
                << "\n6 Calculate Total Value of Prints and Scans"
                << "\n7. Exit Admin Menu"
                << "\nChoose an option: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1:
                viewUserDetails();
                break;
            case 2:
                editUserDetails();
                break;
            case 3:
                deleteUser();
                break;
            case 4:
                listUsernames();
                break;
            case 5:
                calculateTotalDuration();
                break;
            case 6:
                calculateTotalValue();
                break;
            case 7:
                cout << "Exiting Admin Menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        } while (choice != 7);
    }

    void viewUserDetails() {
        string userId;
        cout << "Enter User ID: ";
        getline(cin, userId);

        User* user = findUser(userId);
        if (user == nullptr) {
            cout << "User not found!\n";
            return;
        }

        cout << "\nUser Details:\n"
            << "Name: " << user->name << "\n"
            << "Email: " << user->email << "\n"
            << "User ID: " << user->userId << "\n"
            << "Joining Date: " << user->joiningDate << "\n"
            << "Total Spent: $" << fixed << setprecision(2) << user->totalSpent << "\n";
    }

    void editUserDetails() {
        string userId;
        cout << "Enter User ID: ";
        getline(cin, userId);

        User* user = findUser(userId);
        if (user == nullptr) {
            cout << "User not found!\n";
            return;
        }

        cout << "Enter new name (leave blank to keep current): ";
        string newName;
        getline(cin, newName);
        if (!newName.empty()) {
            user->name = newName;
        }

        cout << "Enter ne email (leave blank to keep current): ";
        string newEmail;
        getline(cin, newEmail);
        if (!newName.empty()) {
            user->email = newEmail;
        }

        cout << "User details successfully updated!\n";
    }

    void deleteUser() {
        string userId;
        cout << "Enter User ID: ";
        getline(cin, userId);

        auto it = remove_if(users.begin(), users.end(), [&](const User& user) {
            return user.userId == userId;
            });

        if (it != users.end()) {
            users.erase(it, users.end());
            cout << "User deleted successfully!\n";
        }
        else {
            cout << "User not found!\n";
        }
    }

    void listUsernames() {
        cout << "\nList if Usernames:\n";
        for (const auto& user : users) {
            cout << user.name << "User not found!\n";
        }
    }

    void calculateTotalDuration() {
        int totalDuration = 0;
        for (const auto& user : users) {
            for (const auto& session : user.sessions) {
                auto duration = session.getEndTimePoint() - session.getStartTimePoint();
                totalDuration += chrono::duration_cast<chrono::minutes>(duration).count();
            }
        }
        cout << "Total Duration of All Sessions: " << totalDuration << " minutes\n";
    }

    void calculateTotalValue() {
        double totalValue = 0;
        for (const auto& user : users) {
            totalValue += user.totalSpent;
        }
        cout << "Total Value of All Prints and Scans: $" << fixed << setprecision(2) << totalValue << "\n";
    }

public:
    void registerUser() {
        User newUser;
        cout << "Enter your name: ";
        getline(cin, newUser.name);
        cout << "Enter your email: ";
        getline(cin, newUser.email);

        do {
            cout << "Enter your password (at least 8 characters, including uppercase, lowercase, number, and special character): ";
            getline(cin, newUser.password);
            if (!isPasswordStrong(newUser.password)) {
                cout << "Password is not strong enough. Please try again." << endl;
            }
        } while (!isPasswordStrong(newUser.password));

        newUser.userId = generateUserId();

        time_t now = time(0);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);
        newUser.joiningDate = buffer;
        newUser.joiningDate.pop_back();

        users.push_back(newUser);
        cout << "Registration successful!" << endl;
        cout << "Your User ID: " << newUser.userId << endl;
        cout << "Joining Date: " << newUser.joiningDate << endl;
    }

    void displayUsers() {
        cout << "\nRegistered Users: \n";
        for (const auto& user : users) {
            cout << "Name: " << user.name
                << ", Email: " << user.email
                << ", User ID: " << user.userId
                << ", Joining Date: " << user.joiningDate
                << ", Total Spent: NZD " << fixed << setprecision(2) << user.totalSpent << endl;
        }
    }

    void startSession() {
        string userId;
        cout << "Enter User ID: ";
        getline(cin, userId);

        User* user = findUser(userId);
        if (user == nullptr) {
            cout << "User not found!" << endl;
            return;
        }

        // Check if user already has an active session
        if (!user->sessions.empty() && user->sessions.back().getIsActive()) {
            cout << "User already has an active session!" << endl;
            return;
        }

        string sessionId = userId + "_" + to_string(user->sessions.size() + 1);
        user->sessions.emplace_back(sessionId);
        cout << "Session started successfully!" << endl;

        // Handle session activities
        handleSessionActivities(user->sessions.back());

        // End session and calculate bill
        auto& currentSession = user->sessions.back();
        currentSession.endSession();
        currentSession.printBill();
        user->totalSpent += currentSession.getTotalAmount();
    }

    void viewSessionHistory() {
        string userId;
        cout << "Enter User ID: ";
        getline(cin, userId);

        User* user = findUser(userId);
        if (user == nullptr) {
            cout << "User not found!" << endl;
            return;
        }

        if (user->sessions.empty()) {
            cout << "No session history found for this user." << endl;
            return;
        }

        cout << "\n=== Session History for " << user->name << " ===\n";
        for (const auto& session : user->sessions) {
            cout << "\nSession ID: " << session.getSessionId()
                << "\nStart Time: " << session.getStartTime()
                << "\nPages Printed: " << session.getPrintedPages()
                << "\nPages Scanned: " << session.getScannedPages()
                << "\nTotal Amount: $" << fixed << setprecision(2)
                << session.getTotalAmount()
                << "\n-------------------";
        }
        cout << "\nTotal Amount Spent: $" << fixed << setprecision(2)
            << user->totalSpent << endl;
    }

    void adminLogin() {
        string email;
        cout << "Enter Admin Email: ";
        getline(cin, email);

        for (const auto& adminEmail : adminEmails) {
            if (email == adminEmail) {
                cout << "Admin login successful!\n";
                adminMenu();
                return;
            }
        }

        cout << "Admin login failed. Email not recognised.\n";
    }
};

int main() {
    SkylineCyberCafe cafe;
    int choice;
    do {
        cout << "\n1. Register User"
            << "\n2. Display Registered Users"
            << "\n3. Start Session"
            << "\n4. View Session History"
            << "\n5. Admin Login"
            << "\n6. Exit"
            << "\nChoose an option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cafe.registerUser();
            break;
        case 2:
            cafe.displayUsers();
            break;
        case 3:
            cafe.startSession();
            break;
        case 4:
            cafe.viewSessionHistory();
            break;
        case 5:
            cafe.adminLogin();
            break;
        case 6:
            cout << "Exiting the program." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 6);

    return 0;
}