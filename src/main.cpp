#include "mfs.h"
#include <Arduino.h>
#define T_WAIT 50

enum status
{
    FIRST_DIGIT,
    SECOND_DIGIT,
    THIRD_DIGIT,
    FOURTH_DIGIT,
    INPUT_FINISHED,
    START_TIMER,
    ALARM
};

int inc_value(int x)
{
    if (x < 9 && x >= 0)
    {
        x++;
    }
    else
    {
        x = 0;
    }

    return x;
}

int dec_value(int x)
{
    if (x < 10 && x > 0)
    {
        x--;
    }
    else
    {
        x = 9;
    }

    return x;
}

int main(void)
{
    init();
    initMFS();
    uint8_t digit1 = 0, digit2 = SEGMENT_CLEAR, digit3 = SEGMENT_CLEAR, digit4 = SEGMENT_CLEAR;

    status state = FIRST_DIGIT;
    uint8_t s1_old = 0;
    uint8_t s2_old = 0;
    uint8_t s3_old = 0;
    Serial.begin(9600);
    unsigned long timer = 0;
    bool punkt = false;
    bool ganzzahl = false;
    uint32_t ausgabe_ganzzahl = 0;
    uint32_t time = 0;
    while (1)
    {

        uint8_t s1 = digitalRead(S1);
        uint8_t s2 = digitalRead(S2);
        uint8_t s3 = digitalRead(S3);
        switch (state)
        {
        case FIRST_DIGIT:
        {
            if (s1 < s1_old)
            {
                digit1 = inc_value(digit1);
            }
            if (s2 < s2_old)
            {
                digit1 = dec_value(digit1);
            }
            if (s3 < s3_old)
            {
                state = SECOND_DIGIT;
                digit2 = 0;
            }
            break;
        }

        case SECOND_DIGIT:
        {
            if (s1 < s1_old)
            {
                digit2 = inc_value(digit2);
            }
            if (s2 < s2_old)
            {
                digit2 = dec_value(digit2);
            }
            if (s3 < s3_old)
            {
                state = THIRD_DIGIT;
                digit3 = 0;
            }
            break;
        }

        case THIRD_DIGIT:
        {
            if (s1 < s1_old)
            {
                digit3 = inc_value(digit3);
            }
            if (s2 < s2_old)
            {
                digit3 = dec_value(digit3);
            }
            if (s3 < s3_old)
            {
                state = FOURTH_DIGIT;
                digit4 = 0;
            }
            break;
        }

        case FOURTH_DIGIT:
        {
            if (s1 < s1_old)
            {
                digit4 = inc_value(digit4);
            }
            if (s2 < s2_old)
            {
                digit4 = dec_value(digit4);
            }
            if (s3 < s3_old)
            {
                state = INPUT_FINISHED;
                punkt = true;
            }
            /* code */
            break;
        }

        case INPUT_FINISHED:
        {
            if (s1 < s1_old)
            {
                state = FIRST_DIGIT;
                digit1 = 0;
                digit2 = SEGMENT_CLEAR;
                digit3 = SEGMENT_CLEAR;
                digit4 = SEGMENT_CLEAR;
                punkt = false;
            }
            if (s2 < s2_old)
            {
                /* code */
            }
            if (s3 < s3_old)
            {
                timer = (digit1 * 1000 + digit2 * 100 + digit3 * 10 + digit4);
                ausgabe_ganzzahl = round(timer / 1000);
                state = START_TIMER;
                punkt = false;
                ganzzahl = true;
                time = millis();
            }
            break;
        }

        case START_TIMER:
        {
            {
                uint64_t time_passed = millis() - time;
                if (time_passed > 1000)
                {
                    timer--;
                    time = millis();
                }
                ausgabe_ganzzahl = timer;
            }
            if (s1 < s1_old)
            {
            }
            if (s3 < s3_old)
            {
                state = FIRST_DIGIT;
                digit1 = 0;
                digit2 = SEGMENT_CLEAR;
                digit3 = SEGMENT_CLEAR;
                digit4 = SEGMENT_CLEAR;
                punkt = false;
                ganzzahl = false;
                digitalWrite(LED1, HIGH);
            }

            if (ausgabe_ganzzahl < 1)
            {
                state = ALARM;
                time = millis();
            }
            break;
        }
        case ALARM:
        {
            digit1 = 'A';
            digit2 = 'L';
            digit3 = 'R';
            digit4 = 'M';
            ganzzahl = false;

            int passed_time = millis() - time;
            if (passed_time < 500)
            {
                digitalWrite(LED1, LOW);
                punkt = false;
                digitalWrite(BUZZER, BUZZER_ON);
                digitalWrite(BUZZER, BUZZER_OFF);
            }
            else
            {
                digitalWrite(LED1, HIGH);

                punkt = true;
            }
            if (passed_time > 1000)
            {
                time = millis();
            }
            if (s3 < s3_old)
            {
                state = FIRST_DIGIT;
                digit1 = 0;
                digit2 = SEGMENT_CLEAR;
                digit3 = SEGMENT_CLEAR;
                digit4 = SEGMENT_CLEAR;
                punkt = false;
                digitalWrite(LED1, HIGH);
            }
            break;
        }
        default:
            break;
        }
        if (ganzzahl)
        {
            writeDecToSegment(ausgabe_ganzzahl);
        }
        else
        {
            writeCharToSegment(0, digit1, punkt);
            writeCharToSegment(1, digit2, punkt);
            writeCharToSegment(2, digit3, punkt);
            writeCharToSegment(3, digit4, punkt);
        }

        s1_old = s1;
        s2_old = s2;
        s3_old = s3;
    }
    return 0;
}
