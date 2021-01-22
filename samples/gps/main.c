#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include <applibs/log.h>
#include <applibs/uart.h>

#include <hw/mt3620.h>

int main(void)
{
    Log_Debug("gps application\n");

    UART_Config uartConfig;
    UART_InitConfig(&uartConfig);
    uartConfig.baudRate = 9600;
    uartConfig.flowControl = UART_FlowControl_None;
    int fd = UART_Open(MT3620_ISU0_UART, &uartConfig);
    assert(fd >= 0);

    const struct timespec sleepTime = { .tv_sec = 0, .tv_nsec = 10000000 };
    while (true)
    {
        uint8_t buffer[200];
        const ssize_t readSize = read(fd, buffer, sizeof(buffer));
        assert(readSize >= 0);
        if (readSize >= 1)
        {
            // Remove CR
            char str[readSize];
            int strSize = 0;
            for (int i = 0; i < readSize; ++i)
            {
                if (buffer[i] == '\r') continue;
                str[strSize++] = buffer[i];
            }

            if (strSize >= 1) Log_Debug("%.*s", strSize, str);
        }

        nanosleep(&sleepTime, NULL);
    }

    return 0;
}
