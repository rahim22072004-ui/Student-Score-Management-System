#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"

typedef struct studentStru
{
    int id;                 // id number
    char name[30];          // name
    float score[3];         // score
    struct studentStru *next;
} StudentNode, *StudentLink;

typedef struct STUStru
{
    StudentLink Head;
    StudentLink Tail;
    int count;
} STU;

StudentLink *IdIndex;       // student ID index array
StudentLink *NameIndex;     // name index array
int indexCount = 0;         // number of indexed records

// Function prototypes
void CreateStudentLink(STU &stu1);
void InsertStudent(STU &stu1, StudentNode SNode);
void funInsertStudent(STU &stu1);
void PrintStudentInfo(STU stu1);
void DisplayStudent(StudentLink p);
void funModifyStudent(STU &stu1);
void funDeleteStudent(STU &stu1);
void funModifyName(STU &stu1);
void funModifyScore(STU &stu1);
StudentLink FindStudentById(STU stu1, int id);
void funSearchStudent(STU stu1);
void CreateIndex(STU stu1);
void SortIndexById();
void SortIndexByName();
int BinarySearchById(int id);
void DisplayByIdOrder(STU stu1);
void DisplayByNameOrder(STU stu1);
void SaveToFile(STU stu1);
void LoadFromFile(STU &stu1);
void DestroyList(STU &stu1);
void ClearInput();

// Initialize linked list
void CreateStudentLink(STU &stu1)
{
    stu1.Head = (StudentLink)malloc(sizeof(StudentNode));
    stu1.Head->next = NULL;
    stu1.Tail = stu1.Head;
    stu1.count = 0;
    IdIndex = NULL;
    NameIndex = NULL;
    indexCount = 0;
}

// Destroy linked list and free memory
void DestroyList(STU &stu1)
{
    StudentLink p = stu1.Head;
    while (p)
    {
        StudentLink q = p->next;
        free(p);
        p = q;
    }
    stu1.Head = NULL;
    stu1.Tail = NULL;
    stu1.count = 0;
    
    if (IdIndex)
    {
        free(IdIndex);
        IdIndex = NULL;
    }
    if (NameIndex)
    {
        free(NameIndex);
        NameIndex = NULL;
    }
    indexCount = 0;
}

// Insert a student record
void InsertStudent(STU &stu1, StudentNode SNode)
{
    StudentLink p;
    p = (StudentLink)malloc(sizeof(StudentNode));
    memcpy(p, &SNode, sizeof(StudentNode));
    p->next = NULL;
    stu1.Tail->next = p;
    stu1.Tail = p;
    stu1.count++;
    
    // Invalidate index
    if (IdIndex)
    {
        free(IdIndex);
        IdIndex = NULL;
    }
    if (NameIndex)
    {
        free(NameIndex);
        NameIndex = NULL;
    }
    indexCount = 0;
}

// Insert student information (menu 2 sub-function)
void funInsertStudent(STU &stu1)
{
    StudentNode SNode;
    printf("Please input id number, name, and score (id number = -1 for quit):\n");
    scanf("%d %s %f", &SNode.id, SNode.name, &SNode.score[0]);
    SNode.score[1] = 0;
    SNode.score[2] = 0;
    
    while (SNode.id != -1)
    {
        // Check if ID already exists
        if (FindStudentById(stu1, SNode.id))
        {
            printf("Student ID %d already exists!\n", SNode.id);
        }
        else
        {
            InsertStudent(stu1, SNode);
            printf("Student added successfully.\n");
        }
        printf("Please input id number, name, and score (id number = -1 for quit):\n");
        scanf("%d %s %f", &SNode.id, SNode.name, &SNode.score[0]);
        SNode.score[1] = 0;
        SNode.score[2] = 0;
    }
}

// Delete student by ID
void funDeleteStudent(STU &stu1)
{
    int id;
    printf("Enter student ID to delete: ");
    scanf("%d", &id);
    
    StudentLink p = stu1.Head;
    while (p->next && p->next->id != id)
    {
        p = p->next;
    }
    
    if (!p->next)
    {
        printf("Student ID %d not found!\n", id);
        return;
    }
    
    StudentLink q = p->next;
    p->next = q->next;
    if (q == stu1.Tail)
    {
        stu1.Tail = p;
    }
    free(q);
    stu1.count--;
    
    // Invalidate index
    if (IdIndex)
    {
        free(IdIndex);
        IdIndex = NULL;
    }
    if (NameIndex)
    {
        free(NameIndex);
        NameIndex = NULL;
    }
    indexCount = 0;
    
    printf("Student ID %d deleted successfully.\n", id);
}

// Modify student name
void funModifyName(STU &stu1)
{
    int id;
    char newName[30];
    printf("Enter student ID: ");
    scanf("%d", &id);
    
    StudentLink p = FindStudentById(stu1, id);
    if (!p)
    {
        printf("Student ID %d not found!\n", id);
        return;
    }
    
    printf("Enter new name: ");
    scanf("%s", newName);
    strcpy(p->name, newName);
    printf("Name updated successfully.\n");
    
    // Invalidate name index only
    if (NameIndex)
    {
        free(NameIndex);
        NameIndex = NULL;
    }
    indexCount = 0;
}

// Modify student score
void funModifyScore(STU &stu1)
{
    int id, course;
    float newScore;
    printf("Enter student ID: ");
    scanf("%d", &id);
    
    StudentLink p = FindStudentById(stu1, id);
    if (!p)
    {
        printf("Student ID %d not found!\n", id);
        return;
    }
    
    printf("Enter course index (0, 1, or 2): ");
    scanf("%d", &course);
    if (course < 0 || course > 2)
    {
        printf("Invalid course index! Use 0, 1, or 2.\n");
        return;
    }
    
    printf("Enter new score: ");
    scanf("%f", &newScore);
    p->score[course] = newScore;
    printf("Score updated successfully.\n");
}

// Modify menu
void funModifyStudent(STU &stu1)
{
    int choice;
    while (1)
    {
        printf("\n------- Modify Menu -------\n");
        printf("1. Add new student\n");
        printf("2. Delete student\n");
        printf("3. Modify student name\n");
        printf("4. Modify student score\n");
        printf("5. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1: funInsertStudent(stu1); break;
            case 2: funDeleteStudent(stu1); break;
            case 3: funModifyName(stu1); break;
            case 4: funModifyScore(stu1); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

// Find student by ID
StudentLink FindStudentById(STU stu1, int id)
{
    StudentLink p = stu1.Head->next;
    while (p)
    {
        if (p->id == id)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// Display a single student
void DisplayStudent(StudentLink p)
{
    printf("ID:%-8d Name:%-15s Score1:%-6.1f Score2:%-6.1f Score3:%-6.1f\n",
           p->id, p->name, p->score[0], p->score[1], p->score[2]);
}

// Search student information
void funSearchStudent(STU stu1)
{
    int choice;
    while (1)
    {
        printf("\n------- Search Menu -------\n");
        printf("1. Search by ID\n");
        printf("2. Search by name\n");
        printf("3. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 3) return;
        
        switch (choice)
        {
            case 1:
            {
                int id;
                printf("Enter student ID: ");
                scanf("%d", &id);
                StudentLink p = FindStudentById(stu1, id);
                if (p)
                {
                    printf("Found: ");
                    DisplayStudent(p);
                }
                else
                {
                    printf("Student ID %d not found.\n", id);
                }
                break;
            }
            case 2:
            {
                char name[30];
                int found = 0;
                printf("Enter name: ");
                scanf("%s", name);
                StudentLink p = stu1.Head->next;
                while (p)
                {
                    if (strcmp(p->name, name) == 0)
                    {
                        DisplayStudent(p);
                        found = 1;
                    }
                    p = p->next;
                }
                if (!found)
                {
                    printf("No student found with name: %s\n", name);
                }
                break;
            }
            default:
                printf("Invalid choice.\n");
        }
    }
}

// Print all student information
void PrintStudentInfo(STU stu1)
{
    if (stu1.count == 0)
    {
        printf("No student records.\n");
        return;
    }
    
    StudentLink s;
    s = stu1.Head->next;
    printf("//////////////////////////////////////////////////////////\n");
    printf("ID        Name            Score1  Score2  Score3\n");
    printf("----------------------------------------------------------\n");
    while (s != NULL)
    {
        printf("%-8d   %-15s %-6.1f  %-6.1f  %-6.1f\n", 
               s->id, s->name, s->score[0], s->score[1], s->score[2]);
        s = s->next;
    }
    printf("//////////////////////////////////////////////////////////\n");
    printf("Total: %d students.\n", stu1.count);
}

// Create index by ID and name
void CreateIndex(STU stu1)
{
    if (stu1.count == 0)
    {
        printf("No student records to index.\n");
        return;
    }
    
    // Free old indices
    if (IdIndex)
    {
        free(IdIndex);
        IdIndex = NULL;
    }
    if (NameIndex)
    {
        free(NameIndex);
        NameIndex = NULL;
    }
    
    indexCount = stu1.count;
    IdIndex = (StudentLink*)malloc(indexCount * sizeof(StudentLink));
    NameIndex = (StudentLink*)malloc(indexCount * sizeof(StudentLink));
    
    StudentLink p = stu1.Head->next;
    for (int i = 0; i < indexCount && p; i++)
    {
        IdIndex[i] = p;
        NameIndex[i] = p;
        p = p->next;
    }
    
    // Sort ID index
    SortIndexById();
    // Sort Name index
    SortIndexByName();
    
    printf("Index created successfully with %d records.\n", indexCount);
}

// Sort ID index in ascending order (bubble sort)
void SortIndexById()
{
    for (int i = 0; i < indexCount - 1; i++)
    {
        for (int j = 0; j < indexCount - 1 - i; j++)
        {
            if (IdIndex[j]->id > IdIndex[j + 1]->id)
            {
                StudentLink temp = IdIndex[j];
                IdIndex[j] = IdIndex[j + 1];
                IdIndex[j + 1] = temp;
            }
        }
    }
}

// Sort Name index in ascending order (bubble sort)
void SortIndexByName()
{
    for (int i = 0; i < indexCount - 1; i++)
    {
        for (int j = 0; j < indexCount - 1 - i; j++)
        {
            if (strcmp(NameIndex[j]->name, NameIndex[j + 1]->name) > 0)
            {
                StudentLink temp = NameIndex[j];
                NameIndex[j] = NameIndex[j + 1];
                NameIndex[j + 1] = temp;
            }
        }
    }
}

// Binary search for student ID on the ID index
int BinarySearchById(int id)
{
    if (!IdIndex || indexCount == 0)
    {
        printf("Please create index first (option 5).\n");
        return -1;
    }
    
    int low = 0, high = indexCount - 1;
    int step = 0;
    
    while (low <= high)
    {
        step++;
        int mid = (low + high) / 2;
        printf("Step %d: low=%d, high=%d, mid=%d, IdIndex[%d]->id=%d\n", 
               step, low, high, mid, mid, IdIndex[mid]->id);
        
        if (id == IdIndex[mid]->id)
        {
            printf("Found at index position %d (step %d)\n", mid, step);
            return mid;
        }
        else if (id < IdIndex[mid]->id)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    
    printf("Student ID %d not found. (steps: %d)\n", id, step);
    return -1;
}

// Display student information in ascending order of ID (using index)
void DisplayByIdOrder(STU stu1)
{
    if (!IdIndex || indexCount == 0)
    {
        printf("Please create index first (option 5).\n");
        return;
    }
    
    printf("\n========== Students sorted by ID (using index) ==========\n");
    printf("ID        Name            Score1  Score2  Score3\n");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < indexCount; i++)
    {
        printf("%-8d   %-15s %-6.1f  %-6.1f  %-6.1f\n",
               IdIndex[i]->id, IdIndex[i]->name, 
               IdIndex[i]->score[0], IdIndex[i]->score[1], IdIndex[i]->score[2]);
    }
    printf("==========================================================\n");
}

// Display student information in ascending order of name (using index)
void DisplayByNameOrder(STU stu1)
{
    if (!NameIndex || indexCount == 0)
    {
        printf("Please create index first (option 5).\n");
        return;
    }
    
    printf("\n========== Students sorted by Name (using index) ==========\n");
    printf("ID        Name            Score1  Score2  Score3\n");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < indexCount; i++)
    {
        printf("%-8d   %-15s %-6.1f  %-6.1f  %-6.1f\n",
               NameIndex[i]->id, NameIndex[i]->name,
               NameIndex[i]->score[0], NameIndex[i]->score[1], NameIndex[i]->score[2]);
    }
    printf("==========================================================\n");
}

// Save student information to file
void SaveToFile(STU stu1)
{
    char filename[100];
    printf("Enter filename (default: students.txt): ");
    scanf("%s", filename);
    if (strlen(filename) == 0)
    {
        strcpy(filename, "students.txt");
    }
    
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        printf("Cannot create file: %s\n", filename);
        return;
    }
    
    fprintf(fp, "%d\n", stu1.count);
    
    StudentLink p = stu1.Head->next;
    while (p)
    {
        fprintf(fp, "%d %s %.1f %.1f %.1f\n", 
                p->id, p->name, p->score[0], p->score[1], p->score[2]);
        p = p->next;
    }
    
    fclose(fp);
    printf("Successfully saved %d student records to %s.\n", stu1.count, filename);
}

// Read student information from file
void LoadFromFile(STU &stu1)
{
    char filename[100];
    printf("Enter filename (default: students.txt): ");
    scanf("%s", filename);
    if (strlen(filename) == 0)
    {
        strcpy(filename, "students.txt");
    }
    
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    
    int count;
    fscanf(fp, "%d", &count);
    if (count <= 0)
    {
        printf("No records in file.\n");
        fclose(fp);
        return;
    }
    
    // Clear current list
    DestroyList(stu1);
    CreateStudentLink(stu1);
    
    for (int i = 0; i < count; i++)
    {
        StudentNode SNode;
        fscanf(fp, "%d %s %f %f %f", &SNode.id, SNode.name, 
               &SNode.score[0], &SNode.score[1], &SNode.score[2]);
        InsertStudent(stu1, SNode);
    }
    
    fclose(fp);
    printf("Successfully loaded %d student records from %s.\n", stu1.count, filename);
}

// Clear input buffer
void ClearInput()
{
    while (getchar() != '\n');
}

// Main function
int main()
{
    int flag = 1;
    int sel;
    STU stu1;
    CreateStudentLink(stu1);
    
    printf("Welcome to Student Score Management System!\n");
    
    while (flag)
    {
        printf("====================================\n");
		printf("0: load student information from a file\n");
        printf("1: Read student information file\n");
        printf("2: Insert, modify, or delete student information\n");
        printf("3: Display student information\n");
        printf("4: Search student information\n");
        printf("5: Create index by ID and name\n");
        printf("6: Binary search for student ID on the ID index\n");
        printf("7: Display students by ID (ascending)\n");
        printf("8: Display students by Name (ascending)\n");
        printf("9: Save information\n");
        printf("10: Quit\n");
        printf("====================================\n");
        printf("Enter your choice: ");
        scanf("%d", &sel);
        
        switch (sel)
        {
            case 1: LoadFromFile(stu1); break;
            case 2: funModifyStudent(stu1); break;
            case 3: PrintStudentInfo(stu1); break;
            case 4: funSearchStudent(stu1); break;
            case 5: CreateIndex(stu1); break;
            case 6:
            {
                if (!IdIndex || indexCount == 0)
                {
                    printf("Please create index first (option 5).\n");
                    break;
                }
                int id;
                printf("Enter student ID to binary search: ");
                scanf("%d", &id);
                int pos = BinarySearchById(id);
                if (pos >= 0)
                {
                    printf("Record: ");
                    DisplayStudent(IdIndex[pos]);
                }
                break;
            }
            case 7: DisplayByIdOrder(stu1); break;
            case 8: DisplayByNameOrder(stu1); break;
            case 9: SaveToFile(stu1); break;
            case 10: 
                flag = 0;
                printf("Goodbye!\n");
                DestroyList(stu1);
                break;
            default:
                printf("Invalid choice. Please enter 1-10.\n");
        }
    }
    return 0;
}