#define MALLOC_ERROR_WARNING(x) "Failed to allocate memory in: " x
#define FUNCTION_ERROR_WARNING(x) "Error in function: " x
#define START_LVDS_ERROR(x, y)                                                                                         \
    void *structure = x;                                                                                               \
    void (*clean_structure)(void *) = (void (*)(void *))y;                                                             \
    char *error_msg = NULL;

#define DEF_ERR_MSG(x) error_msg = x
#define UNDEF_ERR_MSG error_msg = NULL
#define TRY(x)                                                                                                         \
    if (!(x))                                                                                                          \
    {                                                                                                                  \
        if (error_msg != NULL)                                                                                         \
            printf("%s\n", error_msg);                                                                                 \
        if ((clean_structure != NULL) && (structure == NULL))                                                          \
            clean_structure(structure);                                                                                \
        return 0;                                                                                                      \
    }
