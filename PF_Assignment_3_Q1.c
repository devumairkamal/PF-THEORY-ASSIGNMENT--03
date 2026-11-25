#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DOT 0
#define LINE 1
#define TRIANGLE 2
#define SQUARE 3
#define RECTANGLE 4
#define AREA 1
#define PERIMETER 2
#define DISTANCE 3

struct Dot { int type; float x, y; };
struct Line { int type; float x1, y1, x2, y2; };
struct Triangle { int type; float x1, y1, x2, y2, x3, y3; };
struct Square { int type; float x, y, side; };
struct Rectangle { int type; float x, y, width, height; };
float calculateProperty(void *shapePtr, int propertyType)
{
    switch (*(int *)shapePtr)
    {
        case DOT:
        {
            struct Dot *s = (struct Dot *)shapePtr;
            if (propertyType == DISTANCE)
                return sqrtf(s->x * s->x + s->y * s->y);
            break;
        }
        case LINE:
        {
            struct Line *s = (struct Line *)shapePtr;
            if (propertyType == PERIMETER)
                return sqrtf((s->x2 - s->x1) * (s->x2 - s->x1) +
                             (s->y2 - s->y1) * (s->y2 - s->y1));
            break;
        }
        case TRIANGLE:
        {
            struct Triangle *s = (struct Triangle *)shapePtr;
            float a = sqrtf((s->x2 - s->x1) * (s->x2 - s->x1) + (s->y2 - s->y1) * (s->y2 - s->y1));
            float b = sqrtf((s->x3 - s->x2) * (s->x3 - s->x2) + (s->y3 - s->y2) * (s->y3 - s->y2));
            float c = sqrtf((s->x1 - s->x3) * (s->x1 - s->x3) + (s->y1 - s->y3) * (s->y1 - s->y3));
            if (propertyType == AREA)
                return fabsf(s->x1 * (s->y2 - s->y3) + s->x2 * (s->y3 - s->y1) + s->x3 * (s->y1 - s->y2)) / 2.0f;
            if (propertyType == PERIMETER)
                return a + b + c;
            break;
        }
        case SQUARE:
        {
            struct Square *s = (struct Square *)shapePtr;
            if (propertyType == AREA) return s->side * s->side;
            if (propertyType == PERIMETER) return 4.0f * s->side;
            break;
        }
        case RECTANGLE:
        {
            struct Rectangle *s = (struct Rectangle *)shapePtr;
            if (propertyType == AREA) return s->width * s->height;
            if (propertyType == PERIMETER) return 2.0f * (s->width + s->height);
            break;
        }
    }
    return 0.0f;
}
int main(void)
{
    void *shapes[5] = {NULL, NULL, NULL, NULL, NULL};
    int i;
    shapes[0] = malloc(sizeof(struct Dot));
    shapes[1] = malloc(sizeof(struct Line));
    shapes[2] = malloc(sizeof(struct Triangle));
    shapes[3] = malloc(sizeof(struct Square));
    shapes[4] = malloc(sizeof(struct Rectangle));
    for (i = 0; i < 5; i++)
    {
        if (shapes[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            for (i = 0; i < 5; i++) free(shapes[i]);
            return 1;
        }
    }
    ((struct Dot *)shapes[0])->type = DOT;
    ((struct Dot *)shapes[0])->x = 3;
    ((struct Dot *)shapes[0])->y = 4;
    ((struct Line *)shapes[1])->type = LINE;
    ((struct Line *)shapes[1])->x1 = 0;
    ((struct Line *)shapes[1])->y1 = 0;
    ((struct Line *)shapes[1])->x2 = 3;
    ((struct Line *)shapes[1])->y2 = 4;
    ((struct Triangle *)shapes[2])->type = TRIANGLE;
    ((struct Triangle *)shapes[2])->x1 = 0;
    ((struct Triangle *)shapes[2])->y1 = 0;
    ((struct Triangle *)shapes[2])->x2 = 4;
    ((struct Triangle *)shapes[2])->y2 = 0;
    ((struct Triangle *)shapes[2])->x3 = 0;
    ((struct Triangle *)shapes[2])->y3 = 3;
    ((struct Square *)shapes[3])->type = SQUARE;
    ((struct Square *)shapes[3])->x = 0;
    ((struct Square *)shapes[3])->y = 0;
    ((struct Square *)shapes[3])->side = 4;
    ((struct Rectangle *)shapes[4])->type = RECTANGLE;
    ((struct Rectangle *)shapes[4])->x = 0;
    ((struct Rectangle *)shapes[4])->y = 0;
    ((struct Rectangle *)shapes[4])->width = 5;
    ((struct Rectangle *)shapes[4])->height = 3;
    printf("Dot distance from origin: %.2f\n", calculateProperty(shapes[0], DISTANCE));
    printf("Line length: %.2f\n", calculateProperty(shapes[1], PERIMETER));
    printf("Triangle area: %.2f, perimeter: %.2f\n",
           calculateProperty(shapes[2], AREA), calculateProperty(shapes[2], PERIMETER));
    printf("Square area: %.2f, perimeter: %.2f\n",
           calculateProperty(shapes[3], AREA), calculateProperty(shapes[3], PERIMETER));
    printf("Rectangle area: %.2f, perimeter: %.2f\n",
           calculateProperty(shapes[4], AREA), calculateProperty(shapes[4], PERIMETER));
    for (i = 0; i < 5; i++) free(shapes[i]);
    return 0;
}
