
#include "utils.h"

#ifdef _WIN32
#include <Windows.h>
#endif

void utils_sleep(float seconds) {
#ifdef _WIN32
    Sleep((int)(seconds*1000.f));
#else
    #error " oops"
#endif
}
