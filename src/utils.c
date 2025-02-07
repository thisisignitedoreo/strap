
#include "utils.h"

#ifdef _WIN32
 #include <Windows.h>
#else
 #include <unistd.h>
#endif

void utils_sleep(float seconds) {
#ifdef _WIN32
    Sleep((int)(seconds*1000.f));
#else
    usleep((useconds_t)(seconds*1000000.f));
#endif
}
