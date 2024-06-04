#include <Arduino.h>
#include <unity.h>

#include <roof.h>

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_roof_open(){
    digitalWrite(inputPinOpen, HIGH);
    delay(100);
    digitalWrite(inputPinOpen, LOW);
    delay(100);
    digitalWrite(inputPinOpen, HIGH);
    delay(100);
    digitalWrite(inputPinOpen, LOW);
    delay(100);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(outputPinOpen));
}

void setup(){
    delay(2000);
    pinMode(outputPinOpen, OUTPUT);
    UNITY_BEGIN();
}

void loop(){
    RUN_TEST(test_roof_open);
    UNITY_END();
}