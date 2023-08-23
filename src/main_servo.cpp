#ifdef PJ_SERVO
#include <Arduino.h>
#define ID 0
// FF ID RESISTOR LEN VALUE...
uint8_t deg_buffer[3];
uint8_t recv_buffer[4];
uint8_t recv_ptr = 0;
constexpr uint8_t servo_pin[3] = {1, 2, 3};
// PA1,2,3,(+6?)
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
}
void buf_update(uint8_t data)
{
    if (data == 0xFF)
    {
        recv_ptr = 0;
    }
    else
    {
        recv_buffer[recv_ptr++] = data;
        if (recv_ptr == 4)
        {
            if (data >> 4 == ID)
            {
                uint8_t checksum = recv_buffer[0] + recv_buffer[1] + recv_buffer[2];
                if (data & 0x0f == checksum & 0x0f)
                {
                    memcpy(deg_buffer, recv_buffer, 3);
                }
            }
            recv_ptr = 0;
        }
    }
}
uint16_t mapToMicros(uint8_t deg)
{
    return (uint16_t)(deg * 56U + 10000U) >> 3U;
}
void servo_run()
{
    uint8_t success = 0;
    uint16_t microsMap[3] = {};
    for (int i = 0; i < 3; i++)
    {
        microsMap[i] = mapToMicros(deg_buffer[i]);
    }
    uint32_t start = micros();
    digitalWriteFast(servo_pin[0], HIGH);
    digitalWriteFast(servo_pin[1], HIGH);
    digitalWriteFast(servo_pin[2], HIGH);
    while (success < 3)
    {
        uint32_t elapsed = micros() - start;
        for (int i = 0; i < 3; i++)
        {
            if (elapsed > microsMap[i])
            {
                digitalWrite(servo_pin[i], LOW);
                microsMap[i] = UINT16_MAX;
                success++;
            }
        }
    }
}
uint8_t data;
uint32_t last_time = 0;
void loop()
{
    // put your main code here, to run repeatedly:
    last_time = millis();
    if (Serial.available())
    {
        data = Serial.read();
        buf_update(data);
    }
    servo_run();
    while (last_time < micros() + 20000);
}
#endif