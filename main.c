#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 100  // Max number of items
#define POPULATION_SIZE 50  // Number of chromosomes in each generation
#define GENERATIONS 100  // Number of generations to evolve
#define MUTATION_RATE 0.05  // Mutation rate

// Color codes for console output
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"

typedef struct {
    char name[30];
    double amount;
    int importance;
    int isSavingsGoal;  // 1 if it's a savings goal, 0 if it's an expense
} FinancialItem;

FinancialItem items[MAX_ITEMS];
int population[POPULATION_SIZE][MAX_ITEMS];  // Each chromosome represents allocation (1 = funded, 0 = not funded)
double fitness[POPULATION_SIZE];  // Fitness of each chromosome

int totalItems;
double totalIncome;

// Function to add a financial item
void addFinancialItem(char* name, double amount, int importance, int isSavingsGoal, int index) {
    strcpy(items[index].name, name);
    items[index].amount = amount;
    items[index].importance = importance;
    items[index].isSavingsGoal = isSavingsGoal;
}

// Function to calculate fitness
double calculateFitness(int chromosome[]) {
    double totalImportance = 0;
    double totalCost = 0;

    for (int i = 0; i < totalItems; i++) {
        if (chromosome[i] == 1) {  // If the item is funded
            totalImportance += items[i].importance;
            totalCost += items[i].amount;
        }
    }

    if (totalCost > totalIncome) {
        return 0;  // Penalize over-budget solutions by assigning zero fitness
    }

    return totalImportance;
}

// Initialize the population with random allocation decisions
void initializePopulation() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < totalItems; j++) {
            population[i][j] = rand() % 2;  // Randomly allocate each item (1 or 0)
        }
    }
}

// Function to evaluate the fitness of the entire population
void evaluatePopulation() {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness[i] = calculateFitness(population[i]);
    }
}

// Function to select a parent using roulette wheel selection
int selectParent() {
    double sumFitness = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        sumFitness += fitness[i];
    }

    double threshold = ((double) rand() / RAND_MAX) * sumFitness;
    double partialSum = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        partialSum += fitness[i];
        if (partialSum >= threshold) {
            return i;
        }
    }
    return POPULATION_SIZE - 1;
}

// Function to perform crossover between two parents
void crossover(int parent1[], int parent2[], int child1[], int child2[]) {
    int crossoverPoint = rand() % totalItems;

    for (int i = 0; i < totalItems; i++) {
        if (i < crossoverPoint) {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        } else {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }
    }
}

// Function to apply mutation
void mutate(int chromosome[]) {
    for (int i = 0; i < totalItems; i++) {
        if (((double) rand() / RAND_MAX) < MUTATION_RATE) {
            chromosome[i] = !chromosome[i];  // Flip the allocation decision
        }
    }
}

// Generate the next generation
void generateNextGeneration() {
    int newPopulation[POPULATION_SIZE][MAX_ITEMS];

    for (int i = 0; i < POPULATION_SIZE; i += 2) {
        int parent1Index = selectParent();
        int parent2Index = selectParent();

        int child1[MAX_ITEMS], child2[MAX_ITEMS];
        crossover(population[parent1Index], population[parent2Index], child1, child2);

        mutate(child1);
        mutate(child2);

        for (int j = 0; j < totalItems; j++) {
            newPopulation[i][j] = child1[j];
            newPopulation[i + 1][j] = child2[j];
        }
    }

    // Copy new generation to population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < totalItems; j++) {
            population[i][j] = newPopulation[i][j];
        }
    }
}

// Main genetic algorithm
void geneticAlgorithm() {
    initializePopulation();
    for (int i = 0; i < GENERATIONS; i++) {
        evaluatePopulation();
        generateNextGeneration();
    }
    evaluatePopulation();
}

// Find and display the best solution in the population
void displayBestSolution() {
    int bestIndex = 0;
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (fitness[i] > fitness[bestIndex]) {
            bestIndex = i;
        }
    }

    double totalAllocatedSavings = 0, totalAllocatedExpenses = 0;

    printf("\nOptimal Savings and Expense Allocation:\n");

    // Display savings goals
    printf("\n--- Savings Goals ---\n");
    for (int i = 0; i < totalItems; i++) {
        if (items[i].isSavingsGoal && population[bestIndex][i] == 1) {
            printf(GREEN " %s (Amount: Rs.%.2lf, Importance: %d): Funded" RESET "\n", items[i].name, items[i].amount, items[i].importance);
            totalAllocatedSavings += items[i].amount;
        } else if (items[i].isSavingsGoal) {
            printf(RED " %s (Amount: Rs.%.2lf, Importance: %d): Not Funded" RESET "\n", items[i].name, items[i].amount, items[i].importance);
        }
    }

    // Display expenses
    printf("\n--- Expenses ---\n");
    for (int i = 0; i < totalItems; i++) {
        if (!items[i].isSavingsGoal && population[bestIndex][i] == 1) {
            printf(GREEN " %s (Amount: Rs.%.2lf, Importance: %d): Funded" RESET "\n", items[i].name, items[i].amount, items[i].importance);
            totalAllocatedExpenses += items[i].amount;
        } else if (!items[i].isSavingsGoal) {
            printf(RED " %s (Amount: Rs.%.2lf, Importance: %d): Not Funded" RESET "\n", items[i].name, items[i].amount, items[i].importance);
        }
    }

    printf("\n--- Summary ---\n");
    printf("Total Income: Rs.%.2lf\n", totalIncome);
    printf("Total Allocated for Savings: Rs.%.2lf\n", totalAllocatedSavings);
    printf("Total Allocated for Expenses: Rs.%.2lf\n", totalAllocatedExpenses);
    printf("Remaining Budget: Rs.%.2lf\n", totalIncome - (totalAllocatedSavings + totalAllocatedExpenses));
}

int main() {
    srand(time(0));
    char restart;

    do {
        printf("\nPersonal Finance tracker\n");
        printf("--------------------------------------\n");
        printf("\nEnter total income : Rs.");
        scanf("%lf", &totalIncome);

        int numSavings, numExpenses;
        printf("\nEnter the number of savings goals: ");
        scanf("%d", &numSavings);

        for (int i = 0; i < numSavings; i++) {
            char name[30];
            double amount;
            int importance;

            printf("\nEnter name of goal %d: ", i + 1);
            scanf("%s", name);
            printf("Enter target amount for %s: Rs.", name);
            scanf("%lf", &amount);
            printf("Enter importance (1-10) for %s: ", name);
            scanf("%d", &importance);

            addFinancialItem(name, amount, importance, 1, i);
        }

        printf("\nEnter the number of expenses: ");
        scanf("%d", &numExpenses);

        for (int i = 0; i < numExpenses; i++) {
            char name[30];
            double amount;
            int importance;

            printf("\nEnter category of expense %d: ", i + 1);
            scanf("%s", name);
            printf("Enter amount for %s: Rs.", name);
            scanf("%lf", &amount);
            printf("Enter importance (1-10) for %s: ", name);
            scanf("%d", &importance);

            addFinancialItem(name, amount, importance, 0, numSavings + i);
        }

        totalItems = numSavings + numExpenses;

        geneticAlgorithm();
        displayBestSolution();

        printf("\nWould you like to run the program again? (Y/N): ");
        scanf(" %c", &restart);
    } while (restart == 'Y' || restart == 'y');

    printf("\nThank you for using the Financial Allocation Tool!\n");
    return 0;
}m
