#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <conio.h>  // For _getch() on Windows
using namespace std;

// Define status
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
typedef int Status;

// Student structure definition
typedef struct studentStru {
    int id;                 // student number
    char name[30];          // name
    float score[3];         // scores (3 courses)
    struct studentStru* next; // pointer to next node
} StudentNode, *StudentLink;

// Linked list header structure
typedef struct STUStru {
    StudentLink Head;       // pointer to header
    StudentLink Tail;       // pointer to tail
    int count;              // number of records
} STU;

// Global variables
STU stu;                    // student list
StudentLink* IdIndex;       // student ID index array
StudentLink* NameIndex;     // name index array
int indexCount = 0;         // number of indexed records

// Function prototypes
void InitList();
void DestroyList();
Status LoadFromFile(const char* filename);
Status SaveToFile(const char* filename);
Status AddStudent(int id, const char* name, float s1, float s2, float s3);
Status DeleteStudent(int id);
Status ModifyName(int id, const char* newName);
Status ModifyScore(int id, int courseIndex, float newScore);
StudentLink FindStudent(int id);
bool CheckDuplicateID(int id);
void DisplayAll();
void DisplayStudent(StudentLink p);
void CreateIndex();
void SortIndexById();
void SortIndexByName();
int BinarySearchById(int id);
void DisplayByIdOrder();
void DisplayByNameOrder();
void ShowMenu();
void ClearInput();

// Password functions
bool passwordFileExists();
void savePassword(const string& password);
string loadPassword();
string getPasswordInput(bool showPassword);
void setupPassword();
bool verifyPassword();

// Initialize linked list
void InitList() {
    stu.Head = new StudentNode;
    stu.Head->next = NULL;
    stu.Tail = stu.Head;
    stu.count = 0;
    IdIndex = NULL;
    NameIndex = NULL;
    indexCount = 0;
}

// Destroy linked list and free memory
void DestroyList() {
    StudentLink p = stu.Head;
    while (p) {
        StudentLink q = p->next;
        delete p;
        p = q;
    }
    stu.Head = NULL;
    stu.Tail = NULL;
    stu.count = 0;
    
    if (IdIndex) {
        delete[] IdIndex;
        IdIndex = NULL;
    }
    if (NameIndex) {
        delete[] NameIndex;
        NameIndex = NULL;
    }
    indexCount = 0;
}

// Check if ID already exists
bool CheckDuplicateID(int id) {
    StudentLink p = stu.Head->next;
    while (p) {
        if (p->id == id) {
            return true;
        }
        p = p->next;
    }
    return false;
}

// Read student information from file
Status LoadFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file: " << filename << endl;
        return ERROR;
    }
    
    int count;
    file >> count;
    if (file.fail()) {
        cout << "File format error!" << endl;
        file.close();
        return ERROR;
    }
    
    // Clear current list
    DestroyList();
    InitList();
    
    for (int i = 0; i < count; i++) {
        int id;
        char name[30];
        float s1, s2, s3;
        file >> id >> name >> s1 >> s2 >> s3;
        if (file.fail()) {
            cout << "Error reading record " << i + 1 << endl;
            file.close();
            return ERROR;
        }
        AddStudent(id, name, s1, s2, s3);
    }
    
    file.close();
    cout << "Successfully loaded " << count << " student records." << endl;
    return OK;
}

// Save student information to file
Status SaveToFile(const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot create file: " << filename << endl;
        return ERROR;
    }
    
    file << stu.count << endl;
    
    StudentLink p = stu.Head->next;
    while (p) {
        file << p->id << " " << p->name << " " 
             << p->score[0] << " " << p->score[1] << " " << p->score[2] << endl;
        p = p->next;
    }
    
    file.close();
    cout << "Successfully saved " << stu.count << " student records." << endl;
    return OK;
}

// Add a new student
Status AddStudent(int id, const char* name, float s1, float s2, float s3) {
    // Check if ID already exists
    if (CheckDuplicateID(id)) {
        cout << "\n|======================================================|" << endl;
        cout <<   "|| ERROR: ID " << id << " already exists in the system!||" << endl;
        cout <<   "|| Your ID number is same as previous.                 ||" << endl;
        cout <<   "|| Please assign a new ID number                       ||" << endl;
        cout <<   "|=======================================================|" << endl << endl;
        return ERROR;
    }
    
    StudentLink newNode = new StudentNode;
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->score[0] = s1;
    newNode->score[1] = s2;
    newNode->score[2] = s3;
    newNode->next = NULL;
    
    stu.Tail->next = newNode;
    stu.Tail = newNode;
    stu.count++;
    
    // Invalidate index
    if (IdIndex) {
        delete[] IdIndex;
        IdIndex = NULL;
    }
    if (NameIndex) {
        delete[] NameIndex;
        NameIndex = NULL;
    }
    indexCount = 0;
    
    return OK;
}

// Delete student by ID
Status DeleteStudent(int id) {
    StudentLink p = stu.Head;
    while (p->next && p->next->id != id) {
        p = p->next;
    }
    
    if (!p->next) {
        cout << "Student ID " << id << " not found!" << endl;
        return ERROR;
    }
    
    StudentLink q = p->next;
    p->next = q->next;
    if (q == stu.Tail) {
        stu.Tail = p;
    }
    delete q;
    stu.count--;
    
    // Invalidate index
    if (IdIndex) {
        delete[] IdIndex;
        IdIndex = NULL;
    }
    if (NameIndex) {
        delete[] NameIndex;
        NameIndex = NULL;
    }
    indexCount = 0;
    
    cout << "Student ID " << id << " deleted successfully." << endl;
    return OK;
}

// Modify student name
Status ModifyName(int id, const char* newName) {
    StudentLink p = FindStudent(id);
    if (!p) {
        cout << "Student ID " << id << " not found!" << endl;
        return ERROR;
    }
    
    strcpy(p->name, newName);
    cout << "Name updated successfully." << endl;
    
    // Invalidate name index only (id index still valid)
    if (NameIndex) {
        delete[] NameIndex;
        NameIndex = NULL;
    }
    indexCount = 0;
    
    return OK;
}

// Modify student score
Status ModifyScore(int id, int courseIndex, float newScore) {
    if (courseIndex < 0 || courseIndex > 2) {
        cout << "Invalid course index! Use 0, 1, or 2." << endl;
        return ERROR;
    }
    
    StudentLink p = FindStudent(id);
    if (!p) {
        cout << "Student ID " << id << " not found!" << endl;
        return ERROR;
    }
    
    p->score[courseIndex] = newScore;
    cout << "Score updated successfully." << endl;
    return OK;
}

// Find student by ID
StudentLink FindStudent(int id) {
    StudentLink p = stu.Head->next;
    while (p) {
        if (p->id == id) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// Display all students
void DisplayAll() {
    if (stu.count == 0) {
        cout << "No student records." << endl;
        return;
    }
    
    cout << "\n//////////////////////////////////////////////////////////" << endl;
    cout << setw(10) << "ID" << setw(20) << "Name" 
         << setw(10) << "Score1" << setw(10) << "Score2" << setw(10) << "Score3" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    StudentLink p = stu.Head->next;
    while (p) {
        DisplayStudent(p);
        p = p->next;
    }
    cout << "//////////////////////////////////////////////////////////" << endl;
    cout << "Total: " << stu.count << " students." << endl;
}

// Display a single student
void DisplayStudent(StudentLink p) {
    cout << setw(10) << p->id 
         << setw(20) << p->name 
         << setw(10) << fixed << setprecision(1) << p->score[0]
         << setw(10) << p->score[1]
         << setw(10) << p->score[2] << endl;
}

// Create index by ID and name
void CreateIndex() {
    if (stu.count == 0) {
        cout << "No student records to index." << endl;
        return;
    }
    
    // Free old indices
    if (IdIndex) {
        delete[] IdIndex;
        IdIndex = NULL;
    }
    if (NameIndex) {
        delete[] NameIndex;
        NameIndex = NULL;
    }
    
    indexCount = stu.count;
    IdIndex = new StudentLink[indexCount];
    NameIndex = new StudentLink[indexCount];
    
    StudentLink p = stu.Head->next;
    for (int i = 0; i < indexCount && p; i++) {
        IdIndex[i] = p;
        NameIndex[i] = p;
        p = p->next;
    }
    
    // Sort ID index
    SortIndexById();
    // Sort Name index
    SortIndexByName();
    
    cout << "Index created successfully with " << indexCount << " records." << endl;
}

// Sort ID index in ascending order (bubble sort)
void SortIndexById() {
    for (int i = 0; i < indexCount - 1; i++) {
        for (int j = 0; j < indexCount - 1 - i; j++) {
            if (IdIndex[j]->id > IdIndex[j + 1]->id) {
                StudentLink temp = IdIndex[j];
                IdIndex[j] = IdIndex[j + 1];
                IdIndex[j + 1] = temp;
            }
        }
    }
}

// Sort Name index in ascending order (bubble sort)
void SortIndexByName() {
    for (int i = 0; i < indexCount - 1; i++) {
        for (int j = 0; j < indexCount - 1 - i; j++) {
            if (strcmp(NameIndex[j]->name, NameIndex[j + 1]->name) > 0) {
                StudentLink temp = NameIndex[j];
                NameIndex[j] = NameIndex[j + 1];
                NameIndex[j + 1] = temp;
            }
        }
    }
}

// Binary search for student ID on the ID index
int BinarySearchById(int id) {
    if (!IdIndex || indexCount == 0) {
        cout << "Please create index first (option 5)." << endl;
        return -1;
    }
    
    int low = 0, high = indexCount - 1;
    int step = 0;
    
    while (low <= high) {
        step++;
        int mid = (low + high) / 2;
        cout << "Step " << step << ": low=" << low << ", high=" << high 
             << ", mid=" << mid << ", IdIndex[" << mid << "]->id=" << IdIndex[mid]->id << endl;
        
        if (id == IdIndex[mid]->id) {
            cout << "Found at index position " << mid << " (step " << step << ")" << endl;
            return mid;
        }
        else if (id < IdIndex[mid]->id) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    
    cout << "Student ID " << id << " not found. (steps: " << step << ")" << endl;
    return -1;
}

// Display student information in ascending order of ID (using index)
void DisplayByIdOrder() {
    if (!IdIndex || indexCount == 0) {
        cout << "Please create index first (option 5)." << endl;
        return;
    }
    
    cout << "\n========== Students sorted by ID (using index) ==========" << endl;
    cout << setw(10) << "ID" << setw(20) << "Name" 
         << setw(10) << "Score1" << setw(10) << "Score2" << setw(10) << "Score3" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    for (int i = 0; i < indexCount; i++) {
        DisplayStudent(IdIndex[i]);
    }
    cout << "==========================================================" << endl;
}

// Display student information in ascending order of name (using index)
void DisplayByNameOrder() {
    if (!NameIndex || indexCount == 0) {
        cout << "Please create index first (option 5)." << endl;
        return;
    }
    
    cout << "\n========== Students sorted by Name (using index) ==========" << endl;
    cout << setw(10) << "ID" << setw(20) << "Name" 
         << setw(10) << "Score1" << setw(10) << "Score2" << setw(10) << "Score3" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    for (int i = 0; i < indexCount; i++) {
        DisplayStudent(NameIndex[i]);
    }
    cout << "==========================================================" << endl;
}

// Function to add a single student with proper input handling
void AddSingleStudent() {
    int id;
    char name[30];
    float s1, s2, s3;
    
    cout << "\n=== ADD NEW STUDENT ===" << endl;
    cout << "(Enter ID = -1 to go back)" << endl << endl;
    
    while (true) {
        cout << "Enter ID: ";
        cin >> id;
        
        if (cin.fail()) {
            ClearInput();
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        if (id == -1) {
            cout << "\nGoing back to modify menu..." << endl << endl;
            return;
        }
        
        // Check for duplicate ID
        if (CheckDuplicateID(id)) {
            cout << "\n╔══════════════════════════════════════════════════╗" << endl;
            cout << "║  ERROR: ID " << id << " already exists in the system!     ║" << endl;
            cout << "║  Your ID number is same as previous.            ║" << endl;
            cout << "║  Please assign a new ID number.                 ║" << endl;
            cout << "╚══════════════════════════════════════════════════╝" << endl << endl;
            continue;  // Ask for ID again
        }
        
        cout << "Enter Name: ";
        cin >> name;
        
        cout << "Enter Score 1 (0-100): ";
        cin >> s1;
        cout << "Enter Score 2 (0-100): ";
        cin >> s2;
        cout << "Enter Score 3 (0-100): ";
        cin >> s3;
        
        if (AddStudent(id, name, s1, s2, s3) == OK) {
            cout << "\n✓ Student added successfully!" << endl;
            cout << "  ID: " << id << ", Name: " << name 
                 << ", Scores: " << s1 << ", " << s2 << ", " << s3 << endl << endl;
        }
        // After successful addition, continue loop to add more students
        cout << "Enter ID for next student (or -1 to go back):" << endl;
    }
}

// Submenu for insert/modify/delete
void SubMenuModify() {
    int choice;
    while (true) {
        cout << "\n------- Modify Menu -------" << endl;
        cout << "1. Add new student" << endl;
        cout << "2. Delete student (by ID)" << endl;
        cout << "3. Modify student name" << endl;
        cout << "4. Modify student score" << endl;
        cout << "5. Back to main menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            ClearInput();
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        
        switch (choice) {
            case 1:
                AddSingleStudent();
                break;
            case 2: {
                int id;
                cout << "Enter student ID to delete: ";
                cin >> id;
                DeleteStudent(id);
                break;
            }
            case 3: {
                int id;
                char newName[30];
                cout << "Enter student ID: ";
                cin >> id;
                cout << "Enter new name: ";
                cin >> newName;
                ModifyName(id, newName);
                break;
            }
            case 4: {
                int id, course;
                float newScore;
                cout << "Enter student ID: ";
                cin >> id;
                cout << "Enter course index (0, 1, or 2): ";
                cin >> course;
                cout << "Enter new score: ";
                cin >> newScore;
                ModifyScore(id, course, newScore);
                break;
            }
            case 5:
                return;
            default:
                cout << "Invalid choice. Please enter 1-5." << endl;
        }
    }
}

// Submenu for search
void SubMenuSearch() {
    int choice;
    while (true) {
        cout << "\n------- Search Menu -------" << endl;
        cout << "1. Search by ID (linear search)" << endl;
        cout << "2. Search by name" << endl;
        cout << "3. Back to main menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            ClearInput();
            cout << "Invalid input." << endl;
            continue;
        }
        
        if (choice == 3) break;
        
        switch (choice) {
            case 1: {
                int id;
                cout << "Enter student ID to search: ";
                cin >> id;
                StudentLink p = FindStudent(id);
                if (p) {
                    cout << "Found: ";
                    DisplayStudent(p);
                } else {
                    cout << "Student ID " << id << " not found." << endl;
                }
                break;
            }
            case 2: {
                char name[30];
                bool found = false;
                cout << "Enter name to search: ";
                cin >> name;
                StudentLink p = stu.Head->next;
                while (p) {
                    if (strcmp(p->name, name) == 0) {
                        DisplayStudent(p);
                        found = true;
                    }
                    p = p->next;
                }
                if (!found) {
                    cout << "No student found with name: " << name << endl;
                }
                break;
            }
            default:
                cout << "Invalid choice." << endl;
        }
    }
}

// Show main menu
void ShowMenu() {
    cout << "\n========================================" << endl;
    cout << "   Student Score Management System" << endl;
    cout << "========================================" << endl;
    cout << " 0: Load student information from a file" << endl;
    cout << " 1: Read student information file" << endl;
    cout << " 2: Insert, modify, or delete student information" << endl;
    cout << " 3: Display student information" << endl;
    cout << " 4: Search student information" << endl;
    cout << " 5: Create index by ID and name" << endl;
    cout << " 6: Binary search for student ID on the ID index" << endl;
    cout << " 7: Display students by ID (ascending)" << endl;
    cout << " 8: Display students by Name (ascending)" << endl;
    cout << " 9: Save information" << endl;
    cout << "10: Quit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

// Clear input buffer
void ClearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// ============ PASSWORD FUNCTIONS WITH SHOW/HIDE OPTION ============

// Function to get password with show/hide option
string getPasswordInput(bool showPassword) {
    string password;
    char ch;
    
    if (showPassword) {
        // Show password mode - characters are visible
        cout << "Enter password: ";
        cin >> password;
        return password;
    } else {
        // Hide password mode - show asterisks
        cout << "Enter password: ";
        
        #ifdef _WIN32
            // Windows version with _getch()
            while (true) {
                ch = _getch();
                
                if (ch == 13) { // Enter key
                    cout << endl;
                    break;
                }
                else if (ch == 8) { // Backspace
                    if (!password.empty()) {
                        password.pop_back();
                        cout << "\b \b"; // Remove last asterisk
                    }
                }
                else if (ch == 27) { // Escape key - cancel
                    cout << endl;
                    return "";
                }
                else if (ch >= 32 && ch <= 126) { // Printable characters
                    password += ch;
                    cout << '*';
                }
            }
        #else
            // Linux/macOS version - disable echo
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            
            getline(cin, password);
            
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            cout << endl;
        #endif
        
        return password;
    }
}

// Check if password file exists
bool passwordFileExists() {
    ifstream file("password.txt");
    return file.good();
}

// Save password to file
void savePassword(const string& password) {
    ofstream file("password.txt");
    if (file.is_open()) {
        file << password;
        file.close();
        cout << "\n✓ Password set successfully!" << endl;
    } else {
        cout << "✗ Error: Could not save password!" << endl;
    }
}

// Load password from file
string loadPassword() {
    string password;
    ifstream file("password.txt");
    if (file.is_open()) {
        getline(file, password);
        file.close();
    }
    return password;
}

// Show password input mode selection
int selectPasswordMode() {
    int choice;
    cout << "\n----- Password Input Mode -----" << endl;
    cout << "1. Show Password (visible while typing)" << endl;
    cout << "2. Hide Password (show **** while typing)" << endl;
    cout << "Select mode (1 or 2): ";
    cin >> choice;
    
    while (choice != 1 && choice != 2) {
        cout << "Invalid choice! Please select 1 or 2: ";
        cin >> choice;
    }
    
    return choice;
}

// Main password setup function
void setupPassword() {
    string newPassword, confirmPassword;
    int attempts = 3;
    int mode;
    
    cout << "\n===== PASSWORD SETUP =====" << endl;
    cout << "Set a new password for the system.\n" << endl;
    
    // Select password mode
    mode = selectPasswordMode();
    bool showPassword = (mode == 1);
    
    while (attempts > 0) {
        newPassword = getPasswordInput(showPassword);
        
        if (newPassword.empty()) {
            cout << "✗ Password cannot be empty!" << endl;
            attempts--;
            continue;
        }
        
        cout << "Confirm password: ";
        string confirm = getPasswordInput(showPassword);
        
        if (newPassword == confirm) {
            savePassword(newPassword);
            return;
        } else {
            attempts--;
            cout << "✗ Passwords do not match! Attempts left: " << attempts << endl;
        }
    }
    
    cout << "\n✗ Too many failed attempts! Password setup failed." << endl;
    cout << "Please run the program again to set password." << endl;
    exit(0);
}

// Verify password
bool verifyPassword() {
    string storedPassword = loadPassword();
    
    if (storedPassword.empty()) {
        cout << "\n✗ No password set. Please set a password first." << endl;
        return false;
    }
    
    int attempts = 3;
    string inputPassword;
    int mode;
    
    // Select password mode
    mode = selectPasswordMode();
    bool showPassword = (mode == 1);
    
    while (attempts > 0) {
        cout << "\nAttempts left: " << attempts << endl;
        inputPassword = getPasswordInput(showPassword);
        
        if (inputPassword == storedPassword) {
            cout << "\n✓ Password correct! Access granted." << endl;
            return true;
        } else {
            attempts--;
            if (attempts > 0) {
                cout << "✗ Incorrect password! Please try again." << endl;
            }
        }
    }
    
    cout << "\n✗ Too many failed attempts! Access denied." << endl;
    return false;
}

// Main function
int main() {
    cout << "========================================" << endl;
    cout << "   Student Score Management System" << endl;
    cout << "========================================" << endl;
    
    // Check if password file exists
    if (!passwordFileExists()) {
        cout << "\nFirst time setup required!" << endl;
        setupPassword();
    }
    
    // Verify password
    if (!verifyPassword()) {
        cout << "\nProgram terminated." << endl;
        return 0;
    }
    
    // Your main program code goes here
    cout << "\nWelcome to the system!" << endl;
    cout << "Your main program starts here..." << endl;

    InitList();
    int choice;
    char filename[100];
    
    cout << "\nDefault data file: students.txt" << endl;
    
    while (true) {
        ShowMenu();
        cin >> choice;
        
        if (cin.fail()) {
            ClearInput();
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        
        switch (choice) {
            case 0:
            case 1: {
                cout << "Enter filename (default: students.txt): ";
                cin >> filename;
                if (strlen(filename) == 0) {
                    strcpy(filename, "students.txt");
                }
                LoadFromFile(filename);
                break;
            }
            
            case 2: {
                SubMenuModify();
                break;
            }
            
            case 3: {
                DisplayAll();
                break;
            }
            
            case 4: {
                SubMenuSearch();
                break;
            }
            
            case 5: {
                CreateIndex();
                break;
            }
            
            case 6: {
                if (!IdIndex || indexCount == 0) {
                    cout << "Please create index first (option 5)." << endl;
                    break;
                }
                int id;
                cout << "Enter student ID to binary search: ";
                cin >> id;
                int pos = BinarySearchById(id);
                if (pos >= 0) {
                    cout << "Record: ";
                    DisplayStudent(IdIndex[pos]);
                }
                break;
            }
            
            case 7: {
                DisplayByIdOrder();
                break;
            }
            
            case 8: {
                DisplayByNameOrder();
                break;
            }
            
            case 9: {
                cout << "Enter filename (default: students.txt): ";
                cin >> filename;
                if (strlen(filename) == 0) {
                    strcpy(filename, "students.txt");
                }
                SaveToFile(filename);
                break;
            }
            
            case 10: {
                cout << "Goodbye!" << endl;
                DestroyList();
                return 0;
            }
            
            default: {
                cout << "Invalid choice. Please enter 0-10." << endl;
                break;
            }
        }
    }
    
    return 0;
}