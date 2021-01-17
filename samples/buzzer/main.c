#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

#include <hw/mt3620_rdb.h>

int main(void)
{
    Log_Debug("buzzer application\n");

    int fd = GPIO_OpenAsOutput(MT3620_GPIO4, GPIO_OutputMode_PushPull, GPIO_Value_Low);
    assert(fd >= 0);

    const struct timespec onTime = { .tv_sec = 0, .tv_nsec = 200000000 };
    const struct timespec offTime = { .tv_sec = 1, .tv_nsec = 0 };
    while (true)
    {
        GPIO_SetValue(fd, GPIO_Value_High);
        nanosleep(&onTime, NULL);

        GPIO_SetValue(fd, GPIO_Value_Low);
        nanosleep(&offTime, NULL);
    }

    return 0;
}
