#include <core/logger.h>
#include <core/asserts.h>

int main(void)
{
    V_FATAL("This is a fatal message. %f", 3.14);
    V_ERROR("This is an error message. %f", 2.71);
    V_WARN("This is a warning message. %f", 1.61);
    V_INFO("This is an info message. %f", 0.577);
    V_DEBUG("This is a debug message. %f", 1.414);
    V_TRACE("This is a trace message. %f", 2.236);

    V_ASSERT(1 + 1 == 3);

    return 0;
}