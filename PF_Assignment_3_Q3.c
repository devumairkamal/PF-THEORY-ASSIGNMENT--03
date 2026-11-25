#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book
{
    int id;
    char title[100];
    float price;
    int quantity;
};
struct Book *allocateBooks(int n)
{
    return malloc(n * sizeof(struct Book));
}
void inputBooks(struct Book *books, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("\nBook %d\nID: ", i + 1);
        scanf("%d", &books[i].id);
        getchar();
        printf("Title: ");
        fgets(books[i].title, sizeof(books[i].title), stdin);
        books[i].title[strcspn(books[i].title, "\n")] = '\0';
        printf("Price: ");
        scanf("%f", &books[i].price);
        printf("Quantity: ");
        scanf("%d", &books[i].quantity);
    }
}
void displayBooks(struct Book *books, int n)
{
    int i;
    printf("\n%-6s %-25s %10s %6s\n", "ID", "Title", "Price", "Qty");
    for (i = 0; i < n; i++)
        printf("%-6d %-25s %10.2f %6d\n", books[i].id, books[i].title,
               books[i].price, books[i].quantity);
}
float calculateTotalValue(struct Book *books, int n)
{
    float total = 0;
    int i;
    for (i = 0; i < n; i++) total += books[i].price * books[i].quantity;
    return total;
}
int findMostExpensive(struct Book *books, int n)
{
    int i, max = 0;
    for (i = 1; i < n; i++)
        if (books[i].price > books[max].price) max = i;
    return max;
}
float calculateMedianPrice(struct Book *books, int n)
{
    float *prices = malloc(n * sizeof(float)), temp, median;
    int i, j;
    if (prices == NULL) return -1.0f;
    for (i = 0; i < n; i++) prices[i] = books[i].price;
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (prices[i] > prices[j])
            {
                temp = prices[i];
                prices[i] = prices[j];
                prices[j] = temp;
            }
    if (n % 2 == 1) median = prices[n / 2];
    else median = (prices[n / 2 - 1] + prices[n / 2]) / 2.0f;
    free(prices);
    return median;
}
float calculateReorderCost(struct Book *books, int n, int *reorderCount)
{
    float total = 0, cost;
    int i, copies;
    *reorderCount = 0;
    for (i = 0; i < n; i++)
        if (books[i].quantity < 5)
        {
            copies = 10 - books[i].quantity;
            cost = copies * books[i].price * 0.80f;
            printf("%s - order %d copies - Rs. %.2f\n", books[i].title, copies, cost);
            total += cost;
            (*reorderCount)++;
        }
    return total;
}
int main(void)
{
    struct Book *books;
    int n, expensive, reorderCount;
    char line[100], extra;
    float totalValue, median, reorderCost;
    while (1)
    {
        printf("Enter number of books (greater than 0): ");
        if (fgets(line, sizeof(line), stdin) == NULL) return 1;
        if (sscanf(line, "%d %c", &n, &extra) == 1 && n > 0) break;
        printf("Invalid number. Please enter a positive integer.\n");
    }
    books = allocateBooks(n);
    if (books == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }
    inputBooks(books, n);
    displayBooks(books, n);
    totalValue = calculateTotalValue(books, n);
    expensive = findMostExpensive(books, n);
    median = calculateMedianPrice(books, n);
    printf("\nTotal inventory value: Rs. %.2f\n", totalValue);
    printf("Most expensive book: %s (Rs. %.2f)\n",
           books[expensive].title, books[expensive].price);
    if (median < 0)
        printf("Could not calculate median (memory allocation failed).\n");
    else
        printf("Median price: Rs. %.2f\n", median);
    printf("\nReorder report (stock below 5, restock to 10 copies):\n");
    reorderCost = calculateReorderCost(books, n, &reorderCount);
    if (reorderCount == 0) printf("No books need to be reordered.\n");
    printf("Books to reorder: %d\n", reorderCount);
    printf("Total reorder cost: Rs. %.2f\n", reorderCost);
    free(books);
    return 0;
}
