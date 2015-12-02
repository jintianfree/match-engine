
#ifdef _WIN32

char *strndup(const char *s, size_t n)
{
    char *p = malloc(n + 1);
    if(!p) {
        return NULL;
    }
    
    strncpy(p, s, n + 1);
    p[n] = 0;
    
    return p;
}

#endif