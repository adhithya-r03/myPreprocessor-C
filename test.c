// Single-line comment should be removed
#include "sample.h"

/* Multi-line comment
   #define IGNORED_MACRO 999 */
#define SCALE 10
#define ADD(a, b) ((a) + (b))

int main(void) {
    int base = SCALE;
    int sum = ADD(base, MAX_ITEMS);
    int nested = ADD(2, 3);
    const char *label = "SCALE and ADD should stay inside quotes";
    const char *msg = GREETING;
    return sum + nested;
}