#include <Arduino.h>
#include "and_model.h"
#include <tflm_esp32.h>
#include <eloquent_tinyml.h>
#define ARENA_SIZE 2000
#define TF_NUM_OPS 2
Eloquent::TF::Sequential<TF_NUM_OPS, ARENA_SIZE> tf;
void setup() {
Serial.begin(115200);
delay(2000);
Serial.println();
Serial.println("TinyML AND model ready");
Serial.println("Type: x0 x1, for example: 1 1");
Serial.println("--------------------------------");
tf.setNumInputs(2);
tf.setNumOutputs(1);
tf.resolver.AddFullyConnected();
tf.resolver.AddLogistic();
while (!tf.begin(g_and_model).isOk()) {
Serial.println(tf.exception.toString());
delay(1000);
}
Serial.println("Model loaded OK");
Serial.println("--------------------------------");
Serial.println("Enter input:");
}
void loop() {
static float x[2];
if (Serial.available() == 0) {
return;
}
x[0] = Serial.parseFloat();
x[1] = Serial.parseFloat();
while (Serial.available()) {
Serial.read();
}
Serial.print("Input: ");
Serial.print(x[0], 6);
Serial.print(", ");
Serial.println(x[1], 6);
if (!tf.predict(x).isOk()) {
Serial.println(tf.exception.toString());
return;
}
float probability = tf.output(0);
Serial.print("Output probability = ");
Serial.print(probability, 6);
Serial.print(" -> result = ");
Serial.println(probability >= 0.5f ? 1 : 0);
Serial.println("--------------------------------");
Serial.println("Enter next input:");
}