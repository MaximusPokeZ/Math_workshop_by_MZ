#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

double polynom_gorner(double x, int n, ...) {
    va_list ptr;
    va_start(ptr, n);

    double result = 0;
    for(int i = 0; i <= n; i++) 
    {
        result *= x;
        double coeff = va_arg(ptr, double);
        result += coeff;
    }

    va_end(ptr);
    return result;
}


typedef struct 
{
    double x;
    double y;
} Point;

double cross_product(Point a, Point b, Point c) 
{
    return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

bool is_convex(int count, ...) {
    if (count < 3) 
    {
        return false; 
    }

    va_list ptr;
    va_start(ptr, count);

    Point a = va_arg(ptr, Point); 
    Point b = va_arg(ptr, Point);
    Point for_end[] = {a, b};
    char sign = 0;

    for (int i = 2; i < count + 2; i++) 
    {
        Point c;
        if (i < count) c = va_arg(ptr, Point);
        else c = for_end[i % count];
        double cross = cross_product(a, b, c);
        if (cross == 0) continue;
        if (sign == 0) sign = (cross > 0) ? 1 : -1;
        else if ((sign == 1 && cross < 0) || (sign == -1 && cross > 0)) {
            va_end(ptr);
            return false; //sign changed
        }
        a = b;
        b = c;
    }

    va_end(ptr);
    return true; // same signs
}

int main() 
{
    if (is_convex(4, (Point) {0, 0}, (Point) {0, 5}, (Point) {0.25, 0.25}, (Point) {5, 0})) 
    {
        printf("Polygon 1 is convex\n");
    }
    else 
    {
        printf("Polygon 1 is not convex\n");
    }

    if (is_convex(5, (Point) {0, 1}, (Point) {-1, 3}, (Point) {-2, 0}, (Point) {2, 0}, (Point) {1, 3})) 
    {
        printf("Polygon 2 is convex\n");
    } 
    else
    {
        printf("Polygon 2 is not convex\n");
    }



    printf("polynom : %lf\n", polynom_gorner(3, 2, 3.0, -1.0, 8.0));
    return 0;
}
