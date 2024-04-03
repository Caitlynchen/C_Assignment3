//A01366116 Caitlyn Chen
//A01364159 Jiahong Xu        
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

typedef struct
{
    int employeeID;
    char first_name[50];
    char last_name[50];
    char student_number[10];
    float employee_salary;
} employee;

int Compare_Employees(const employee *a, const employee *b)
{
    // First compare first names
    int firstNameComp = strcmp(a->first_name, b->first_name);
    if (firstNameComp != 0)
        return firstNameComp;

    // If first names are the same, then compare last names
    int lastNameComp = strcmp(a->last_name, b->last_name);
    return lastNameComp;
}

void quickSort(employee arr[], int low, int high)
{
    if (low < high)
    {
        employee pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++)
        {
            if (Compare_Employees(&arr[j], &pivot) <= 0)
            {
                i++;
                employee temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        employee temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        int p = i + 1;

        quickSort(arr, low, p - 1);
        quickSort(arr, p + 1, high);
    }
}

void sort_employees(employee *employees, int count, FILE *output_file)
{
    if (!output_file)
    {
        fprintf(stderr, "Output file is not open.\n");
        return;
    }

    quickSort(employees, 0, count - 1);

    for (int i = 0; i < count; ++i)
    {
        fprintf(output_file, "%d,%s %s,%.2f\n",
                employees[i].employeeID,
                employees[i].first_name,
                employees[i].last_name,
                employees[i].employee_salary);
    }
}

int is_valid_salary(const char *salaryStr)
{
    char *endptr;
    float salary = strtof(salaryStr, &endptr);

    if (*endptr != '\0' || salary < 0.0)
    {
        return 0;
    }
    return 1; 
}

int is_valid_id(int id, const int *id_check_array, int capacity)
{
    
    if (id <= 0 || id >= capacity || id_check_array[id] != 0)
    {
        return 0; 
    }
    return 1; 
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");
    if (!inputFile || !outputFile)
    {
        perror("Error opening file");
        if (inputFile)
            fclose(inputFile);
        if (outputFile)
            fclose(outputFile);
        return 1;
    }

    int capacity = 1000; 
    employee *employees = malloc(capacity * sizeof(employee));
    int *id_check_array = calloc(capacity, sizeof(int));
    if (!employees || !id_check_array)
    {
        perror("Memory allocation failed");
        fclose(inputFile);
        if (outputFile)
            fclose(outputFile);
        if (employees)
            free(employees);
        if (id_check_array)
            free(id_check_array);
        return 1;
    }

    int error_occurred = 0; // Error flag
    int count = 0;
    int end_designator_found = 0; // Flag to check for 'E'
    char buffer[1000];
    int id_num = 0;
    while (fgets(buffer, sizeof(buffer), inputFile))
    {
        buffer[strcspn(buffer, "\n")] = 0; 
        if (strlen(buffer) == 0)
        {
            continue;
        }

        char id[50];
        char firstName[50], lastName[50], salaryStr[50];
        float salary;

        // Parse the line
        int fieldsParsed = sscanf(buffer, "%49[^,],%49[^ ] %49[^,],%49s", id, firstName, lastName, salaryStr);

        if (strcmp(buffer, "E") == 0)
        {
            end_designator_found = 1; // Set the flag if 'E' is found
            break;
        }

        // Check for parsing errors
        if (fieldsParsed != 4)
        {
            fprintf(stderr, "Error: Incorrect format on line: %s\n", buffer);
            error_occurred = 1;
            return 0; 
        }
 
        size_t len = strlen(id);
        size_t num_len = strspn(id, "0123456789");
        if (len > 0 && len == num_len){

            id_num = atoi(id);
            // Check for ID and salary validity
            if (!is_valid_id(id_num, id_check_array, capacity))
            {
                fprintf(stderr, "Error: Invalid ID on line: %s\n", buffer);
                error_occurred = 1;
                return 0; 
            }
        }else{
            fprintf(stderr, "Error: Invalid ID on line: %s\n", buffer);
            return 0;
        }

        if (!is_valid_salary(salaryStr))
        {
            fprintf(stderr, "Error: Invalid salary on line: %s\n", buffer);
            error_occurred = 1;
            return 0; 
        }

        salary = strtof(salaryStr, NULL);
        employees[count].employeeID = id_num;
        strcpy(employees[count].first_name, firstName);
        strcpy(employees[count].last_name, lastName);
        employees[count].employee_salary = salary;
        id_check_array[id_num] = 1;
        count++;
    }

    if (count == 0)
    {
        fprintf(stderr, "Error: Empty file\n");
    }

    if (!end_designator_found)
    {
        fprintf(stderr, "Error: End designator 'E' not found in the input file.\n");
        error_occurred = 1;
    }

    if (!error_occurred)
    {
        quickSort(employees, 0, count - 1);          
        sort_employees(employees, count, outputFile); 
    }
    else
    {
    }
    free(id_check_array);
    free(employees);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}