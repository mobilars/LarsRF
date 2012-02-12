
#include <stdarg.h>

//void putc(unsigned);
//void puts(char *);
static void xtoa(unsigned long x, const unsigned long *dp);
static void puth(unsigned n);
void smallprintf(char *format, ...);