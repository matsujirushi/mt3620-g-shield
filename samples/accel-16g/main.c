#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/i2c.h>

#include <hw/mt3620_rdb.h>

const uint8_t SLAVE_ADDRESS = 0x53;
const uint8_t REG_POWER_CTL = 0x2d;
const uint8_t REG_DATAX0    = 0x32;

int main(void)
{
    Log_Debug("accel-16g application\n");

    int result;

    int fd = I2CMaster_Open(MT3620_RDB_HEADER2_ISU0_I2C);
    assert(fd >= 0);
    result = I2CMaster_SetBusSpeed(fd, I2C_BUS_SPEED_STANDARD);
    assert(result == 0);
    result = I2CMaster_SetTimeout(fd, 100);
    assert(result == 0);
    result = I2CMaster_SetDefaultTargetAddress(fd, SLAVE_ADDRESS);
    assert(result == 0);

    const uint8_t initData[] = { REG_POWER_CTL, 0x08 };
    result = I2CMaster_Write(fd, SLAVE_ADDRESS, initData, sizeof(initData));
    assert(result == sizeof(initData));

    const struct timespec sleepTime = { .tv_sec = 0, .tv_nsec = 200000000 };
    while (true)
    {
        const uint8_t writeData[] = { REG_DATAX0 };
        uint8_t readData[6];
        result = I2CMaster_WriteThenRead(fd, SLAVE_ADDRESS, writeData, sizeof(writeData), readData, sizeof(readData));
        assert(result == sizeof(writeData) + sizeof(readData));

        int16_t val;
        ((uint8_t*)&val)[0] = readData[0];
        ((uint8_t*)&val)[1] = readData[1];
        float x = val * 2.0f / 512;
        ((uint8_t*)&val)[0] = readData[2];
        ((uint8_t*)&val)[1] = readData[3];
        float y = val * 2.0f / 512;
        ((uint8_t*)&val)[0] = readData[4];
        ((uint8_t*)&val)[1] = readData[5];
        float z = val * 2.0f / 512;

        Log_Debug("%.3f\t%.3f\t%.3f\n", x, y, z);

        nanosleep(&sleepTime, NULL);
    }

    return 0;
}
