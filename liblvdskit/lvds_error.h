#define ERROR_HANDLING_INIT                                                                                            \
    {                                                                                                                  \
        char *error_msg = NULL;                                                                                        \
    }

#define MALLOC_ERROR_WARNING(x) "Failed to allocate memory in: " x "\n"
#define FUNCTION_ERROR_WARNING(x) "Error in function: " x "\n"
#define STRUCT_AND_CLEANER_EQUALS(x, y)                                                                                \
    void *structure = x;                                                                                               \
    void (*clean_structure)(void *) = (void (*)(void *))y;

#define ERR_MSG_EQLS(x) error_msg = x
#define TRY(x)                                                                                                         \
    if (!(x))                                                                                                          \
    {                                                                                                                  \
        if (roberto.verbose)                                                                                           \
            printf("%s", error_msg);                                                                                   \
        if ((clean_structure != NULL) && (structure == NULL))                                                          \
            clean_structure(structure);                                                                                \
        return 0;                                                                                                      \
    }

extern char *error_msg;
