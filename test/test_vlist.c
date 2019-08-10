#include "vlist.h"
#include "test_base.h"

int     test_vlist_init(void)
{
    t_vlist *vlist;

    vlist = malloc(sizeof(*vlist));
    ASSERT(vlist, "Allocation failed");
    ASSERT(vlist_init(vlist, sizeof(int), "int") == 0, "Init should be succesful with valid memory");
    vlist_destroy(vlist);

    vlist = malloc(sizeof(*vlist));
    ASSERT(vlist, "Allocation failed");
    ASSERT(VLIST_INIT(int, vlist) == 0, "It should be possible to init vlist with VLIST_INIT");
    vlist_destroy(vlist);

    return (0);
}

bool    equals_to(void *value, void *expected)
{
    return (*(int*)value == (int)expected);
}

int     test_vlist_add_gen(void)
{
    VLIST_OF(int)   *vlist;
    int             value = 0;
    int             value2 = 2;

    vlist = malloc(sizeof(*vlist));
    ASSERT(vlist, "Allocation failed");
    VLIST_INIT(int, vlist);

    ASSERT(vlist_add_gen(vlist, &value) == 0, "vlist_add_gen should not fail on valid vlist");
    ASSERT(VLIST_ADD(int, vlist, value2) == 0, "VLIST_ADD can add non-pointer values");

    ASSERT(vlist_find(vlist, equals_to, (void*)0) != NULL, "Values added by vlist_add_gen should be present in a vlist");
    ASSERT(vlist_find(vlist, equals_to, (void*)2) != NULL, "Values added by VLIST_ADD should be present in a vlist");

    vlist_destroy(vlist);

    return (0);
}

int     test_vlist_delete_gen(void)
{
    VLIST_OF(int)   *vlist;
    int             value = 0;

    vlist = malloc(sizeof(*vlist));
    ASSERT(vlist, "Allocation failed");
    VLIST_INIT(int, vlist);

    VLIST_ADD(int, vlist, value);
    ASSERT(vlist_delete_gen(vlist,  &value) == 0,
            "vlist_delete_gen should succeed with valid vlist and value address");
    ASSERT(vlist_find(vlist, equals_to, (void*)(long)value) == NULL,
            "Value deleted by vlist_delete_gen should not be found in the vlist");

    VLIST_ADD(int, vlist, value);
    ASSERT(VLIST_DELETE(int, vlist, value) == 0,
            "VLIST_DELETE should succeed with valid vlist and value address");
    ASSERT(vlist_find(vlist, equals_to, (void*)(long)value) == NULL,
            "Value deleted by vlist_delete_gen should not be found in the vlist");

    vlist_destroy(vlist);

    return (0);
}

void    multiply_by_itself(void *value, void *unused)
{
    const int   val = *(int*)value;

    (void)unused;

    *(int*)value = val * val;
}

int     test_vlist_foreach(void)
{
    VLIST_OF(int)   *vlist;
    int             before[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int             after[8] = {1, 4, 9, 16, 25, 36, 49, 64};

    vlist = malloc(sizeof(*vlist));
    ASSERT(vlist, "Allocation failed");
    VLIST_INIT(int, vlist);

    for (int i = 0; i < 8; i++)
        VLIST_ADD(int, vlist, before[i]);

    vlist_foreach(vlist, multiply_by_itself, NULL);

    for (int i = 0; i < 8; i++)
        ASSERT(vlist_find(vlist, equals_to, (void*)(long)after[i]) != NULL,
                "Values modified by vlist_foreach should be found in the same vlist");

    vlist_destroy(vlist);

    return (0);
}

int     test_vlist(void)
{
    int     ret;

    ret = 0;
    ret |= test_vlist_init();
    ret |= test_vlist_add_gen();
    ret |= test_vlist_delete_gen();
    ret |= test_vlist_foreach();

    return (ret);
}
