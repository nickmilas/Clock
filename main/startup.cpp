#ifdef __cplusplus
extern "C" // Using extern "C" here tells the C++ compiler to use C naming/calling conventions (i.e. no name mangling) for functions in this library/header
{
#include "lvgl.h"
}
#endif

void main(void)
{
    lv_init();
}
