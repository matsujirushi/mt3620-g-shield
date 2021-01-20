#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/adc.h>

#include <hw/mt3620_rdb.h>

static const float MAX_VOLTAGE = 2.5f;

int main(void)
{
    Log_Debug("rotary-angle application\n");

    int result;

    int fd = ADC_Open(MT3620_RDB_ADC_CONTROLLER0);
    assert(fd >= 0);
    int bitCount = ADC_GetSampleBitCount(fd, MT3620_ADC_CHANNEL0);
    assert(bitCount >= 0);
    result = ADC_SetReferenceVoltage(fd, MT3620_ADC_CHANNEL0, MAX_VOLTAGE);
    assert(result == 0);

    const struct timespec sleepTime = { .tv_sec = 0, .tv_nsec = 200000000 };
    while (true)
    {
        uint32_t adcData;
        result = ADC_Poll(fd, MT3620_ADC_CHANNEL0, &adcData);
        assert(result == 0);

        const float voltage = (float)adcData * MAX_VOLTAGE / (float)((1 << bitCount) - 1) * 2.0f;
        Log_Debug("%.3f\n", voltage);

        nanosleep(&sleepTime, NULL);
    }

    return 0;
}
