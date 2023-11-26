#include <cstdint>
#include <unistd.h>
#include "hookapi.h"
#include "logger.h"

#define LOG_TAG "InputInject/Entry"

static void lib_entry() __attribute__((constructor));

static_assert(sizeof(int64_t) == 8);

void lib_entry() {
    logger::currentPid = getpid();
    LOGD("input injector begin, current pid = %d", logger::currentPid);
}
