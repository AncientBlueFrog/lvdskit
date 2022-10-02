#include "lvdskit.h"
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *get_input(va_list *args, const char *format, ...)
{
    // Growable buffer for characters
    char *buffer = NULL;

    // Capacity of buffer
    size_t capacity = 0;

    // Number of characters actually in buffer
    size_t size = 0;

    // Character read or EOF
    int c;

    // Prompt user
    if (format != NULL)
    {
        // Initialize variadic argument list
        va_list ap;

        // Students' code will pass in printf-like arguments as variadic
        // parameters. The student-facing get_char * macro always sets args to
        // NULL. In this case, we initialize the list of variadic parameters
        // the standard way with va_start.
        if (args == NULL)
        {
            va_start(ap, format);
        }

        // When functions in this library call get_char * they will have
        // already stored their variadic parameters in a `va_list` and so they
        // just pass that in by pointer.
        else
        {
            // Put a copy of argument list in ap so it is not consumed by vprintf
            va_copy(ap, *args);
        }

        // Print prompt
        vprintf(format, ap);

        // Clean up argument list
        va_end(ap);
    }

    // Iteratively get characters from standard input, checking for CR (Mac OS), LF (Linux), and CRLF (Windows)
    while ((c = fgetc(stdin)) != '\r' && c != '\n' && c != EOF)
    {
        // Grow buffer if necessary
        if (size + 1 > capacity)
        {
            // Increment buffer's capacity if possible
            if (capacity < SIZE_MAX)
            {
                capacity++;
            }
            else
            {
                free(buffer);
                return NULL;
            }

            // Extend buffer's capacity
            char *temp = realloc(buffer, capacity);
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        // Append current character to buffer
        buffer[size++] = c;
    }

    // Check whether user provided no input
    if (size == 0 && c == EOF)
    {
        return NULL;
    }

    // Check whether user provided too much input (leaving no room for trailing NUL)
    if (size == SIZE_MAX)
    {
        free(buffer);
        return NULL;
    }

    // If last character read was CR, try to read LF as well
    if (c == '\r' && (c = fgetc(stdin)) != '\n')
    {
        // Return NULL if character can't be pushed back onto standard input
        if (c != EOF && ungetc(c, stdin) == EOF)
        {
            free(buffer);
            return NULL;
        }
    }

    // Minimize buffer
    char *s = realloc(buffer, size + 1);
    if (s == NULL)
    {
        free(buffer);
        return NULL;
    }

    // Terminate char *
    s[size] = '\0';

    // Return char *
    return s;
}

char *get_string(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char *line = get_input(&ap, format);

    va_end(ap);

    if (!line)
    {
        free(line);
        return NULL;
    }

    return line;
}

/**
 * Prompts user for a line of text from standard input and returns the
 * equivalent char; if text is not a single char, user is prompted
 * to retry. If line can't be read, returns CHAR_MAX.
 */
char get_char(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    // Try to get a char from user
    while (true)
    {
        // Get line of text, returning CHAR_MAX on failure
        char *line = get_input(&ap, format);
        if (line == NULL)
        {
            free(line);
            va_end(ap);
            return CHAR_MAX;
        }

        // Return a char if only a char was provided
        char c, d;
        if (sscanf(line, "%c%c", &c, &d) == 1)
        {
            free(line);
            va_end(ap);
            return c;
        }

        free(line);
    }

    va_end(ap);
    return 0;
}

/**
 * Prompts user for a line of text from standard input and returns the
 * equivalent double as precisely as possible; if text does not represent
 * a double or if value would cause underflow or overflow, user is
 * prompted to retry. If line can't be read, returns DBL_MAX.
 */
double get_double(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    // Try to get a double from user
    while (true)
    {
        // Get line of text, returning DBL_MAX on failure
        char *line = get_input(&ap, format);
        if (line == NULL)
        {
            free(line);
            va_end(ap);
            return DBL_MAX;
        }

        // Return a double if only a double was provided
        if (strlen(line) > 0 && !isspace((unsigned char)line[0]))
        {
            char *tail;
            errno = 0;
            double d = strtod(line, &tail);
            if (errno == 0 && *tail == '\0' && isfinite(d) != 0 && d < DBL_MAX)
            {
                // Disallow hexadecimal and exponents
                if (strcspn(line, "XxEePp") == strlen(line))
                {
                    free(line);
                    va_end(ap);
                    return d;
                }
            }
        }

        free(line);
    }

    va_end(ap);
    return 0;
}

/**
 * Prompts user for a line of text from standard input and returns the
 * equivalent float as precisely as possible; if text does not represent
 * a float or if value would cause underflow or overflow, user is prompted
 * to retry. If line can't be read, returns FLT_MAX.
 */
float get_float(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    // Try to get a float from user
    while (true)
    {
        // Get line of text, returning FLT_MAX on failure
        char *line = get_input(&ap, format);

        if (line == NULL)
        {
            free(line);
            va_end(ap);
            return FLT_MAX;
        }

        // Return a float if only a float was provided
        if (strlen(line) > 0 && !isspace((unsigned char)line[0]))
        {
            char *tail;
            errno = 0;
            float f = strtof(line, &tail);
            if (errno == 0 && *tail == '\0' && isfinite(f) != 0 && f < FLT_MAX)
            {
                // Disallow hexadecimal and exponents
                if (strcspn(line, "XxEePp") == strlen(line))
                {
                    free(line);
                    va_end(ap);
                    return f;
                }
            }
        }

        free(line);
    }

    va_end(ap);
    return 0;
}

/**
 * Prompts user for a line of text from standard input and returns the
 * equivalent int; if text does not represent an int in [-2^31, 2^31 - 1)
 * or would cause underflow or overflow, user is prompted to retry. If line
 * can't be read, returns INT_MAX.
 */
int get_int(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    // Try to get an int from user
    while (true)
    {
        // Get line of text, returning INT_MAX on failure
        char *line = get_input(&ap, format);
        if (line == NULL)
        {
            free(line);
            va_end(ap);
            return INT_MAX;
        }

        // Return an int if only an int (in range) was provided
        if (strlen(line) > 0 && !isspace((unsigned char)line[0]))
        {
            char *tail;
            errno = 0;
            long n = strtol(line, &tail, 10);
            if (errno == 0 && *tail == '\0' && n >= INT_MIN && n < INT_MAX)
            {
                free(line);
                va_end(ap);
                return n;
            }
        }

        free(line);
    }

    va_end(ap);
    return 0;
}

/**
 * Prompts user for a line of text from standard input and returns the
 * equivalent long; if text does not represent a long in
 * [-2^63, 2^63 - 1) or would cause underflow or overflow, user is
 * prompted to retry. If line can't be read, returns LONG_MAX.
 */
long get_long(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    // Try to get a long from user
    while (true)
    {
        // Get line of text, returning LONG_MAX on failure
        char *line = get_input(&ap, format);
        if (line == NULL)
        {
            free(line);
            va_end(ap);
            return LONG_MAX;
        }

        // Return a long if only a long (in range) was provided
        if (strlen(line) > 0 && !isspace((unsigned char)line[0]))
        {
            char *tail;
            errno = 0;
            long n = strtol(line, &tail, 10);
            if (errno == 0 && *tail == '\0' && n < LONG_MAX)
            {
                free(line);
                va_end(ap);
                return n;
            }
        }
        free(line);
    }

    va_end(ap);
    return 0;
}
