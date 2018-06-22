
#ifndef utils_h
#define utils_h

# define MIN(x, y) ((x) < (y) ? (x) : (y))
# define MAX(x, y) ((x) > (y) ? (x) : (y))

int utf8_to_int(const char *str, unsigned int *outbuf);

#endif
