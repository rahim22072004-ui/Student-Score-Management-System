Student Score Management System
📚 Complete Documentation
🎯 Project Overview
This is a Student Score Management System implemented in C language as a Data Structure Course Design project. The system demonstrates practical applications of fundamental data structures and algorithms through a functional student information management application.

📋 Table of Contents
System Requirements

Project Structure

Data Structures

Features & Menu Options

How to Compile & Run

File Format

Indexing Mechanism

Algorithm Demonstrations

Sample Usage

Error Handling

Known Issues

Future Enhancements

💻 System Requirements
Compiler: GCC, MinGW, or Visual Studio (any C compiler)

Operating System: Windows, Linux, or macOS

Memory: Minimum 128MB RAM

Disk Space: 10MB free space

📁 Project Structure
Student-Score-Management-System/
│
├── main.c                      # Complete source code
├── README.md                   # This documentation file
├── LICENSE                     # MIT License
├── .gitignore                  # Git ignore file
│
├── sample_data/
│   └── students.txt            # Sample data file
│
└── docs/
    ├── algorithm.md            # Algorithm documentation
    ├── screenshots/            # UI screenshots
    └── user_guide.md           # User manual

🏗️ Data Structures
1. Student Node Structure
    typedef struct studentStru {
    int id;                 // Student ID (unique)
    char name[30];          // Student name
    float score[3];         // Three course scores
    struct studentStru *next; // Pointer to next node
} StudentNode, *StudentLink;

2. Linked List Header Structure

typedef struct STUStru {
    StudentLink Head;       // Pointer to head node (sentinel)
    StudentLink Tail;       // Pointer to tail node
    int count;              // Total number of records
} STU;

3. Index Arrays (Global)

StudentLink *IdIndex;       // Sorted ID index array
StudentLink *NameIndex;     // Sorted Name index array
int indexCount;             // Number of indexed records

🎯 Features & Menu Options
Main Menu
====================================
0: Load student information from a file
1: Read student information file
2: Insert, modify, or delete student information
3: Display student information
4: Search student information
5: Create index by ID and name
6: Binary search for student ID on the ID index
7: Display students by ID (ascending)
8: Display students by Name (ascending)
9: Save information
10: Quit
====================================

Sub-Menus
1. Insert/Modify/Delete Menu
------- Modify Menu -------
1. Add new student
2. Delete student
3. Modify student name
4. Modify student score
5. Back to main menu

2. Search Menu
------- Search Menu -------
1. Search by ID
2. Search by name
3. Back to main menu

------- Search Menu -------
1. Search by ID
2. Search by name
3. Back to main menu
# Compile
gcc main.c -o student_management

# Run
./student_management

Using MinGW (Windows)
# Compile
gcc main.c -o student_management.exe

# Run
student_management.exe

Using Visual Studio
Create a new Win32 Console Application project

Add the source file (main.c) to the project

Build the solution (F7)

Run the program (F5)

📄 File Format
Input/Output File Format
The system uses a simple text file format:
[record_count]
[id] [name] [score1] [score2] [score3]
[id] [name] [score1] [score2] [score3]
...
Sample students.txt:
8
10001 WangQiangqiang 100.0 0.0 0.0
10008 WangH 89.0 0.0 0.0
10005 ZhaoX 92.0 0.0 0.0
10031 QianM 73.0 0.0 0.0
10018 JiangV 94.0 0.0 0.0
10013 WanH 69.0 0.0 0.0
10019 TianR 85.0 0.0 0.0
10010 ZuoU 77.0 0.0 0.0

🔍 Indexing Mechanism
How Indexing Works
Index Creation (Option 5)

Creates two sorted arrays of pointers

IdIndex: Sorted by student ID (ascending)

NameIndex: Sorted by student name (alphabetical)

Index Benefits

Fast Search: O(log n) using binary search

Sorted Display: Display records without modifying linked list

Memory Efficient: Only stores pointers, not duplicate data

Index Visualization
Memory Address  Student ID   Name          Next Pointer
------------   -----------   ----------   -------------
2600            10001        Wang         3200
3200            10008        WangH        2580
2580            10005        ZhaoX        3620
3620            10031        QianM        4008
4008            10018        JiangV       2100
2100            10013        WanH         8660
8660            10019        TianR        5200
5200            10010        ZuoU         0

📊 Algorithm Demonstrations
1. Binary Search Algorithm
int BinarySearchById(int id) {
    int low = 0, high = indexCount - 1;
    int step = 0;
    
    while (low <= high) {
        step++;
        int mid = (low + high) / 2;
        printf("Step %d: low=%d, high=%d, mid=%d, ID=%d\n", 
               step, low, high, mid, IdIndex[mid]->id);
        
        if (id == IdIndex[mid]->id)
            return mid;
        else if (id < IdIndex[mid]->id)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return -1;
}

2. Index Creation Algorithm
void CreateIndex(STU stu1) {
    indexCount = stu1.count;
    IdIndex = (StudentLink*)malloc(indexCount * sizeof(StudentLink));
    NameIndex = (StudentLink*)malloc(indexCount * sizeof(StudentLink));
    
    // Copy pointers
    StudentLink p = stu1.Head->next;
    for (int i = 0; i < indexCount && p; i++) {
        IdIndex[i] = p;
        NameIndex[i] = p;
        p = p->next;
    }
    
    // Sort indices
    SortIndexById();
    SortIndexByName();
}

💡 Sample Usage
Example 1: Adding Students
Please input id number, name, and score (id number = -1 for quit):
10025 JohnDoe 85
Student added successfully.
Please input id number, name, and score (id number = -1 for quit):
-1
Example 2: Binary Search
Enter your choice: 6
Enter student ID to binary search: 10005

Step 1: low=0, high=7, mid=3, IdIndex[3]->id=10018
Step 2: low=0, high=2, mid=1, IdIndex[1]->id=10008
Step 3: low=0, high=0, mid=0, IdIndex[0]->id=10005
Found at index position 0 (step 3)

Record: ID:10005    Name:ZhaoX       Score1:92.0   Score2:0.0    Score3:0.0

Example 3: Display Sorted Records
========== Students sorted by ID (using index) ==========
ID        Name            Score1  Score2  Score3
----------------------------------------------------------
10001     WangQiangqiang  100.0   0.0     0.0
10005     ZhaoX           92.0    0.0     0.0
10008     WangH           89.0    0.0     0.0
10010     ZuoU            77.0    0.0     0.0
10013     WanH            69.0    0.0     0.0
10018     JiangV          94.0    0.0     0.0
10019     TianR           85.0    0.0     0.0
10031     QianM           73.0    0.0     0.0
==========================================================
⚠️ Error Handling
The system includes robust error handling for:

File Operations

Invalid file path

File not found

Invalid file format

User Input

Non-numeric input

Out-of-range values

Duplicate student IDs

Memory Management

Allocation failures

Memory leaks (prevented)

Index Operations

Index not created before use

Empty list operations

🐛 Known Issues
Input Validation

Limited validation for name input (assumes single word)

No input for multiple scores in initial add

Memory Management

Index arrays are invalidated on any modification

Must recreate index after modifications

File Format

Uses simple text format (not secure)

No data encryption

🚀 Future Enhancements
Potential improvements for future versions:

Validation

Add comprehensive input validation

Support multi-word names

Validate score ranges

Features

Add grade calculation (A, B, C, D, F)

Calculate GPA

Add statistics (average, max, min)

Export to CSV/Excel

Performance

Use quicksort instead of bubble sort

Implement hash table for O(1) search

Use binary file format for faster I/O

UI Enhancement

Add color to output

Implement pagination for large datasets

Create GUI version

📝 License
This project is licensed under the MIT License - see the LICENSE file for details.

👥 Contributors
Project Maintainer: [Your Name]

Department: Computer Science

Course: Data Structure Course Design

📧 Contact
Email: rahim22072004@gmail.com

GitHub: https://github.com/rahim22072004-ui

Project Link: https://github.com/rahim22072004-ui/Student-Score-Management-System

⭐ Acknowledgments
Department of Computer Science

Data Structure Course Design Guide

All testers and contributors

📊 Quick Reference Card
Function	Description
LoadFromFile()	Load data from text file
funInsertStudent()	Add new student
funDeleteStudent()	Delete student by ID
funModifyName()	Change student name
funModifyScore()	Update student score
PrintStudentInfo()	Display all records
funSearchStudent()	Search by ID or name
CreateIndex()	Build ID and name indices
BinarySearchById()	Search using binary algorithm
DisplayByIdOrder()	Show sorted by ID
DisplayByNameOrder()	Show sorted by name
SaveToFile()	Save data to file
🎓 Educational Value
This project demonstrates:

Data Structures

Singly linked lists

Arrays as indices

Dynamic memory allocation

Algorithms

Binary search

Bubble sort

Linear search

Index-based searching

Programming Concepts

Modular programming

Function pointers

File I/O operations

Memory management

Error handling

Last Updated: March 2026

⭐Star this repository if you find it helpful!