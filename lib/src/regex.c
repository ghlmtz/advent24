#include <ctype.h>

/* Basic match at beginning of string. Returns 1 for match, 0 otherwise 
 * pattern only supports . with optional {N} argument for now */
int regex_match(const char *pattern, const char *str)
{
    while (*pattern != '\0') 
    {
        switch (*pattern)
        {
            case '.':
            {
                int count = 0;
                pattern++;
                if (*pattern == '{') 
                {
                    pattern++;
                    while (isdigit(*pattern))
                        count = 10 * count + *pattern++ - '0';
                    if (*pattern != '}')
                        return 0;
                    pattern++;
                }
                else 
                    count = 1;
                while (count > 0)
                {
                    if (*str++ == '\0')
                        return 0;
                    count--;
                }
            }
                break;
            default:
                if (*pattern++ != *str++)
                    return 0;
        }
    }
    return 1;
}