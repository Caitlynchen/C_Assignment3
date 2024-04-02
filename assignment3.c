#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int employeeID;
    char first_name[50];
    char last_name[50];
    char student_number[10]; 
    float employee_salary;
} employee;

int Compare_Employees(const employee *a, const employee *b) {
    // First compare first names
    int firstNameComp = strcmp(a->first_name, b->first_name);
    if (firstNameComp != 0) return firstNameComp;

    // If first names are the same, then compare last names
    int lastNameComp = strcmp(a->last_name, b->last_name);
    return lastNameComp; 
}


void quickSort(employee arr[], int low, int high) {
    if (low < high) {
        employee pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (Compare_Employees(&arr[j], &pivot) <= 0) {
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

void sort_employees(employee* employees, int count, FILE* output_file) {
    if (!output_file) {
        fprintf(stderr, "Output file is not open.\n");
        return;
    }

 
    quickSort(employees, 0, count - 1);

    
    for (int i = 0; i < count; ++i) {
        fprintf(output_file, "%d,%s %s,%.2f\n",
                employees[i].employeeID,
                employees[i].first_name,
                employees[i].last_name,
                employees[i].employee_salary);
    }

}


int is_valid_salary(const char *salaryStr) {
    char *endptr;
    strtof(salaryStr, &endptr);
    if (*endptr != '\0') {
        // If there are any characters left in the string after parsing the float, it's invalid
        return 0;
    }
    return 1;
}
int is_valid_id(int id, const int *id_check_array, int capacity) {
    if (id <= 0 || id >= capacity || id_check_array[id] != 0) {
        return 0; // Invalid if ID is non-positive, out of bounds, or already used
    }
    return 1; // Valid ID
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        if (inputFile) fclose(inputFile);
        if (outputFile) fclose(outputFile);
        return 1;
    }

    int capacity = 1000; // Adjust based on maximum expected IDs
    employee *employees = malloc(capacity * sizeof(employee));
    int *id_check_array = calloc(capacity, sizeof(int));
    if (!employees || !id_check_array) {
        perror("Memory allocation failed");
        fclose(inputFile);
        if (outputFile) fclose(outputFile);
        if (employees) free(employees);
        if (id_check_array) free(id_check_array);
        return 1;
    }

    int count = 0;
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), inputFile)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove the newline character

        if (strcmp(buffer, "E") == 0) { // Check for 'E' designator
            break;
        }

        int id;
        char firstName[50], lastName[50], salaryStr[50];
        float salary;

        // Adjust sscanf to parse salary as a string
        int fieldsParsed = sscanf(buffer, "%d,%49[^ ] %49[^,],%49s", &id, firstName, lastName, salaryStr);

        if (fieldsParsed != 4 || !is_valid_id(id, id_check_array, capacity) || !is_valid_salary(salaryStr)) {
            fprintf(stderr, "Error: Incorrect or invalid format on line: %s\n", buffer);
        } else {
            salary = strtof(salaryStr, NULL); // Convert salary string to float
            employees[count].employeeID = id;
            strcpy(employees[count].first_name, firstName);
            strcpy(employees[count].last_name, lastName);
            employees[count].employee_salary = salary;
            id_check_array[id] = 1; // Mark this ID as seen
            count++;
        }
    }

    quickSort(employees, 0, count - 1); // Placeholder for sorting function
    sort_employees(employees, count, outputFile); // Placeholder for sorting and writing function
    free(id_check_array);
    free(employees);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}