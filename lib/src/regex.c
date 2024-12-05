#include <ctype.h>
#include <stdlib.h>

static int get_count(const char **pattern)
{
    int count = 0;
    if (**pattern == '{') 
    {
        (*pattern)++;
        while (isdigit(**pattern)) 
        {
            count = 10 * count + **pattern - '0';
            (*pattern)++;
        }
        if (**pattern != '}')
        {
            pattern = NULL;
            return 0;
        }
        (*pattern)++;
    }
    else 
        count = 1;
    return count;
}

/* Scans for count instance of ch in str 
 * moves str to end of match if success
 * returns 0 for failure, 1 for success 
 * ch = '\0' looks for any character 
 */
static int scan_char(const char **str, char ch, int count)
{
    const char *s = *str;
    while (count > 0)
    {
        if (*s == '\0')
            return 0;
        if (ch == '\0')
            s++;
        else if (*s++ != ch)
            return 0;
        count--;
    }
    *str = s;
    return 1;
}

/* Basic match at beginning of string. Returns 1 for match, 0 otherwise 
 * pattern supports:
 *   . for any character
 *   optional {N} argument (including {0})
 *   \d for digits
 *   \n \t \r \\
 * pattern does NOT support:
 *   any other escape sequences
 */
int regex_match(const char *pattern, const char *str)
{
    while (*pattern != '\0') 
    {
        switch (*pattern)
        {
            case '\\':
                pattern++;
                switch (*pattern)
                {
                    case 'd':
                    {
                        pattern++;
                        int count = get_count(&pattern);
                        while (count > 0)
                        {
                            if (*str == '\0')
                                return 0;
                            if (!isdigit(*str++))
                                return 0;
                            count--;
                        }
                    }
                        break;
                    case 'n':
                        pattern++;
                        if (!scan_char(&str, '\n', get_count(&pattern)))
                            return 0;
                        break;
                    case 't':
                        pattern++;
                        if (!scan_char(&str, '\t', get_count(&pattern)))
                            return 0;
                        break;
                    case 'r':
                        pattern++;
                        if (!scan_char(&str, '\r', get_count(&pattern)))
                            return 0;
                        break;
                    case '\\':
                        pattern++;
                        if (!scan_char(&str, '\\', get_count(&pattern)))
                            return 0;
                        break;
                    default:
                        return 0;
                }
                break;
            case '.':
            {
                pattern++;
                int count = get_count(&pattern);
                if (!scan_char(&str, 0, count))
                    return 0;
            }
                break;
            default:
            {
                char ch = *pattern;
                pattern++;
                int count = get_count(&pattern);
                if (!scan_char(&str, ch, count))
                    return 0;
            }
        }
    }
    return 1;
}