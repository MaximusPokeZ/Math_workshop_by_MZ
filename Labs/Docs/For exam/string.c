#include <stdio.h>

int strcmp(const char *str1, const char *str2)
{
    while (*str1 != '\0' && *str1 == *str2)
    {
        str1++;
        str2++;
    }

    return *(unsigned char *)(str1) - *(unsigned char *)(str2);
}

size_t strlen(const char *str)
{
    size_t result = 0;
    while (*str != '\0')
    {
        result++;
        str++;
    }
    return result;
}

char *strcpy(char *dest, const char *src)
{
    if (dest == NULL)
        return NULL;
    char *start = dest;
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return start;
}

char *strcat(char *dest, const char *src)
{
    if (dest == NULL)
        return NULL;
    char *res = dest;
    while (*dest != '\0')
    {
        dest++;
    }
    while (*src != '\0')
    {
        *dest++ = *src++;
    }
    return res;
}

char *strchr(const char *str, const int c)
{
    while (*str != '\0' && *str != c)
    {
        str++;
    }
    return (*str == c) ? (char *)str : NULL;
}

char *strstr(const char *str_main, const char *str)
{
    if (*str == '\0')
        return (char *)str_main;
    while (*str_main != '\0')
    {
        const char *res = str_main;
        const char *strptr = str;
        while (*strptr != '\0' && (*res++ == *strptr++))
        {
        }
        if (*strptr == '\0')
            return (char *)str_main;
        str_main++;
    }
    return NULL;
}

char *strpbrk(const char *str, const char *b) //(string break point)
{
    if (str == NULL || b == NULL)
        return NULL;

    while (*str != '\0')
    {
        if (strchr(b, *str) != NULL)
        {
            return (char *)str;
        }
        str++;
    }
    return NULL;
}

size_t strspn(const char *str, const char *prefix) // "строковая длина" (string span)
{
    size_t res = 0;
    while (*str != '\0' && strchr(prefix, *str))
    {
        str++;
        res++;
    }

    return res;
}

size_t strcspn (const char* str, const char* propusk)
{
    size_t res = 0;
    while (*str != '\0' && strchr(propusk, *str) == NULL)
    {
        res++;
        str++;
    }
    return res;
}

char *strtok(char *str, const char *separ)
{
    static char * crnt_pos = NULL;
    if (str != NULL)
    {
        crnt_pos = NULL;
    }
    else if (crnt_pos == NULL) return NULL;

    size_t pref = strspn(str, separ);
    crnt_pos += pref;

    if (crnt_pos != NULL && *crnt_pos == '\0')
    {
        crnt_pos = NULL;
        return NULL; //больше токенов нет
    }

    // Начинаем поиск конца текущего токена
    size_t end = strcspn(crnt_pos, separ);
    char* start_ptr = crnt_pos;
    crnt_pos += end;

    if (crnt_pos != NULL && *crnt_pos != '\0')
    {
        *crnt_pos = '\0';
        crnt_pos++;
    }

    return start_ptr; //от start_ptr до crnt_pos
}

int main()
{

    char s1[] = "Hello";
    char s2[] = "Hello!";

    printf("%d\n", strcmp(s1, s2));

    printf("%lu\n", strlen(s1));

    char str[10];

    char *res = strcpy(str, "Privet");
    printf("%s\n", res);
    res = strcat(res, "!");
    printf("%s\n", res);

    printf("%s\n", strchr(res, 'r'));

    char *pp = strstr(res, "vet");
    printf("%s\n", pp);

    char *new = "abbcajiklkjijcasc";

    printf("%lu\n", strspn(new, "abc"));

    printf("%s\n", strpbrk(new, "slk"));



    char sss[] = "apple,orange;banana";
    const char delim[] = ",;";
    char* token = strtok(sss, delim);
    while (token != NULL) 
    {
        printf("%s\n", token);
        token = strtok(NULL, delim);
    }

    return 0;
}
