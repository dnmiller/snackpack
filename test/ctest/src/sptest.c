#include "ctest.h"
#include <stdio.h>

int main(void)
{
    ct_record_error(0.2334, "hi");
    ct_record_error(0.4334, "ii");
    ct_record_error(0.3334, ".i");
    ct_record_error(-0.2334, "hi");
    ct_print_errors();
    return 0;
}
