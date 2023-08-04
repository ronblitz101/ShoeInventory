#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
 
#define MAX_RECORDS 100
#define FILE_NAME "shoe_inventory.txt"

struct ShoeRecord {
    int stockNumber;
    int styleNumber;
    int sizes[12]; 
    float price;
};

struct ShoeRecord inventory[MAX_RECORDS];
int numRecords = 0;

void saveInventoryToFile() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("\t\tError opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++) {
        fprintf(fp, "%d %d %.2f", inventory[i].stockNumber, inventory[i].styleNumber, inventory[i].price);
        for (int j = 0; j < 12; j++) {
            fprintf(fp, " %d", inventory[i].sizes[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void loadInventoryFromFile() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("\t\tError opening file for reading.\n");
        return;
    }

    numRecords = 0;
    while (fscanf(fp, "%d %d %f", &inventory[numRecords].stockNumber, &inventory[numRecords].styleNumber,
                  &inventory[numRecords].price) == 3) {
        for (int i = 0; i < 12; i++) {
            fscanf(fp, " %d", &inventory[numRecords].sizes[i]);
        }
        numRecords++;
    }

    fclose(fp);
}

int findRecordByStockNumber(int stockNumber) {
    for (int i = 0; i < numRecords; i++) {
        if (inventory[i].stockNumber == stockNumber) {
            return i;
        }
    }
    return -1; // Record not found
}

int findRecordByStyleNumber(int styleNumber) {
    for (int i = 0; i < numRecords; i++) {
        if (inventory[i].styleNumber == styleNumber) {
            return i;
        }
    }
    return -1; // Record not found
}

int getValidIntInput() {
    int value;
    char c;
    while (scanf("%d", &value) != 1) {
        printf("\n\tInvalid input. Please enter a number: ");
        while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer
    }
    return value;
}

float getValidFloatInput() {
    float value;
    char c;
    while (scanf("%f", &value) != 1) {
        printf("\n\tInvalid input. Please enter a number: ");
        while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer
    }
    return value;
}

void enterNewRecord() {
    if (numRecords >= MAX_RECORDS) {
        printf("\t\tInventory is full. Cannot add more records.\n");
        return;
    }

    struct ShoeRecord newRecord;

    printf("\nEnter stock number: ");
    newRecord.stockNumber = getValidIntInput();

    printf("Enter style number (0 to 50): ");
    newRecord.styleNumber = getValidIntInput();

    printf("Enter price: ");
    newRecord.price = getValidFloatInput();

    for (int i = 0; i < 12; i++) {
        printf("\nEnter number of pairs for size %d: ", i + 3);
        newRecord.sizes[i] = getValidIntInput();
    }

    inventory[numRecords++] = newRecord;
    saveInventoryToFile();
    printf("\n\tRecord added successfully.\n");

    printf("\n\t\tPress any key to go back to menu...\n");
    getch();
}

void displayRecord(int index) {
    if (index >= 0 && index < numRecords) {
        printf("\nStock Number: %d\n", inventory[index].stockNumber);
        printf("Style Number: %d\n", inventory[index].styleNumber);
        printf("Price: %.2f\n", inventory[index].price);
        for (int i = 0; i < 12; i++) {
            printf("Size %d: %d pairs\n", i + 3, inventory[index].sizes[i]);
        }
    } else {
        printf("\n\tRecord not found.\n");
    }

    printf("\n\t\tPress any key to go back to menu...\n");
    getch();
}

void changePrice(int index) {
    if (index >= 0 && index < numRecords) {
        printf("\nEnter new price: ");
        scanf("%f", &inventory[index].price);
        saveInventoryToFile();
        printf("\n\tPrice changed successfully.\n");
    } else {
        printf("\n\t\tRecord not found.\n");
    }

    printf("\n\t\tPress any key to go back to menu...\n");
    getch();
}

void changeStockOnHand(int index) {
    if (index >= 0 && index < numRecords) {
        int sizeToChange;
        printf("\nEnter the size (3 to 14) to change stock on hand: ");
        scanf("%d", &sizeToChange);

        if (sizeToChange >= 3 && sizeToChange <= 14) {
            printf("Enter the new stock on hand for size %d: ", sizeToChange);
            scanf("%d", &inventory[index].sizes[sizeToChange - 3]);
            saveInventoryToFile();
            printf("\n\tStock on hand for size %d changed successfully.\n", sizeToChange);
        } else {
            printf("\n\t\tInvalid size. Please enter a size between 3 and 14.\n");
        }
    } else {
        printf("\n\t\tRecord not found.\n");
    }
    
    printf("\n\t\tPress any key to go back to menu...\n");
    getch();
}

void deleteRecord(int index) {
    if (index >= 0 && index < numRecords) {
        for (int i = index; i < numRecords - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        numRecords--;
        saveInventoryToFile();
        printf("\n\tRecord deleted successfully.\n");
    } else {
        printf("\n\t\tRecord not found.\n");
    }

    printf("\n\t\tPress any key to go back to menu...\n");
    getch();
}

int exitMenu() {
    char repeat;

    while (1) {
        printf("\nDo you confirm the exit:\n[Y]es\t\t[N]o\n\t= ");
        scanf(" %c", &repeat);

        repeat = tolower(repeat);
        if (repeat == 'y') {
            printf("\n\tExiting...\n\n");
            return 0;
        } else if (repeat == 'n') {
            return 1;
        } else {
            printf("\n\t\tError: Invalid choice. Please try again\n");
        }
    }
}

int main() {
    loadInventoryFromFile();

    int choice, stockNumber, styleNumber, index, repeat = 1;

    while (repeat == 1) {
        printf("\n===========================================\n   ____________________________________ \n  |                                    |\n  |           SHOE INVENTORY           | \n  |____________________________________| \n\n===========================================\n");
        printf("\n1. Create a new record\n");
        printf("2. Read a record\n");
        printf("3. Update the price of a stock item\n");
        printf("4. Update the number on hand\n");
        printf("5. Delete a record\n");
        printf("6. Exit\n");
        printf("\n===========================================\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        printf("\n===========================================\n");

        switch (choice) {
            case 1:
                enterNewRecord();
                break;
            case 2:
                printf("\nEnter stock number: ");
                 scanf("%d", &stockNumber);
                printf("Enter style number: ");
                scanf("%d", &styleNumber);

                index = -1;
                for (int i = 0; i < numRecords; i++) {
                    if (inventory[i].stockNumber == stockNumber && inventory[i].styleNumber == styleNumber) {
                        index = i;
                         break;
                    }
                }

                if (index == -1) {
                    printf("\n\t\tRecord not found.\n");
                } else {
                    displayRecord(index);
                }
                break;

            case 3:
                printf("\nEnter stock number: ");
                scanf("%d", &stockNumber);
                printf("Enter style number: ");
                scanf("%d", &styleNumber);

                index = -1;
                for (int i = 0; i < numRecords; i++) {
                    if (inventory[i].stockNumber == stockNumber && inventory[i].styleNumber == styleNumber) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    printf("\n\t\tRecord not found.\n");
                } else {
                    changePrice(index);
                }
                break;

            case 4:
                printf("\nEnter stock number: ");
                scanf("%d", &stockNumber);
                printf("Enter style number: ");
                scanf("%d", &styleNumber);

                index = -1;
                for (int i = 0; i < numRecords; i++) {
                    if (inventory[i].stockNumber == stockNumber && inventory[i].styleNumber == styleNumber) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    printf("\n\t\tRecord not found.\n");
                } else {
                    changeStockOnHand(index);
                }
                break;

            case 5:
                printf("\nEnter stock number: ");
                scanf("%d", &stockNumber);
                printf("Enter style number: ");
                scanf("%d", &styleNumber);
                index = -1;
                for (int i = 0; i < numRecords; i++) {
                    if (inventory[i].stockNumber == stockNumber && inventory[i].styleNumber == styleNumber) {
                        index = i;
                        break;
                    }
                }
                deleteRecord(index);
                break;

            case 6:
                repeat = exitMenu();
                break;

            default:
                printf("\n\t\tInvalid choice. Please try again.\n");
        }
    }

    return 0;
}
