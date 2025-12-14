#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char month[20];
    char category[30];
    float amount;
} Expense;

static void print_menu() {
    printf("\n===== SMART FILE-BASED EXPENSE TRACKER (C BACKEND) =====\n");
    printf("1) Add Expense\n");
    printf("2) View Expenses (in memory)\n");
    printf("3) Save to file (expenses.txt)\n");
    printf("4) Load from file (expenses.txt)\n");
    printf("5) Exit\n");
    printf("Choose: ");
}

int main() {
    Expense *list = NULL;
    int count = 0;
    int choice;

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        if (choice == 1) {
            // Add new record (dynamic memory growth)
            count++;
            Expense *tmp = (Expense*)realloc(list, count * sizeof(Expense));
            if (!tmp) {
                printf("Memory allocation failed!\n");
                free(list);
                return 1;
            }
            list = tmp;

            printf("Enter Month (e.g., March): ");
            scanf("%19s", list[count-1].month);

            printf("Enter Category (e.g., Food): ");
            scanf("%29s", list[count-1].category);

            printf("Enter Amount: ");
            scanf("%f", &list[count-1].amount);

            printf(">> Expense added successfully (in memory).\n");
        }
        else if (choice == 2) {
            printf("\n--- IN-MEMORY EXPENSES (%d) ---\n", count);
            for (int i = 0; i < count; i++) {
                printf("%2d) %s | %s | %.2f\n", i + 1,
                       list[i].month, list[i].category, list[i].amount);
            }
            if (count == 0) printf("No records yet.\n");
        }
        else if (choice == 3) {
            FILE *fp = fopen("expenses.txt", "w");
            if (!fp) { printf("Cannot open expenses.txt for writing.\n"); continue; }

            for (int i = 0; i < count; i++) {
                fprintf(fp, "%s %s %.2f\n",
                        list[i].month, list[i].category, list[i].amount);
            }
            fclose(fp);
            printf(">> Saved %d record(s) to expenses.txt\n", count);
        }
        else if (choice == 4) {
            FILE *fp = fopen("expenses.txt", "r");
            if (!fp) { printf("No expenses.txt found. Save first or add records.\n"); continue; }

            // Reset current memory
            free(list);
            list = NULL;
            count = 0;

            Expense temp;
            while (fscanf(fp, "%19s %29s %f",
                          temp.month, temp.category, &temp.amount) == 3) {
                count++;
                Expense *tmp = (Expense*)realloc(list, count * sizeof(Expense));
                if (!tmp) { printf("Memory allocation failed while loading.\n"); free(list); fclose(fp); return 1; }
                list = tmp;
                list[count-1] = temp;
            }
            fclose(fp);
            printf(">> Loaded %d record(s) from expenses.txt\n", count);
        }
        else if (choice == 5) {
            printf("Exiting backend. Bye!\n");
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }

    free(list);
    return 0;
}
