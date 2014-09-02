#include "mruby-arduino.h"
#ifdef MRUBY_ARDUINO_BOARD_GALILEO

#include <new>
	
#include "mruby.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/data.h"

#include <Arduino.h>

// Servo class is not offically supported yet.
#ifdef MRUBY_ARDUINO_GALILEO_ENABLE_SERVO
#pragma message "Unofficial Servo class enabled."
#include <Wire.h>
#include <Servo.h>
#endif /*MRUBY_ARDUINO_GALILEO_ENABLE_SERVO*/

mrb_value mrb_serial_available(mrb_state *mrb, mrb_value self){
  return mrb_fixnum_value(Serial.available());
}

mrb_value mrb_serial_begin(mrb_state *mrb, mrb_value self){
  mrb_int speed = 0;
  mrb_get_args(mrb,"i",&speed);
  Serial.begin(speed);
  return mrb_nil_value();
}


mrb_value mrb_serial_println(mrb_state *mrb, mrb_value self){
  mrb_value s;
  mrb_get_args(mrb,"S", &s);
  for (int i = 0; i < RSTRING_LEN(s); i++){
    Serial.print( RSTRING_PTR(s)[i] );
  }
  Serial.println("");
  return mrb_nil_value();
}

#ifdef MRUBY_ARDUINO_GALILEO_ENABLE_SERVO
void mrb_servo_free(mrb_state *mrb, void *ptr){
  Servo *servo = (Servo *)ptr;
  servo->~Servo();
  mrb_free(mrb, servo);
}

struct mrb_data_type mrb_servo_type = {"Servo", mrb_servo_free};

mrb_value mrb_servo_initialize(mrb_state *mrb, mrb_value self){
  void *p = mrb_malloc(mrb, sizeof(Servo));
  Servo *newServo = new(p) Servo();
  DATA_PTR(self) = newServo;  
  DATA_TYPE(self) = &mrb_servo_type;  
  return self;
}

mrb_value mrb_servo_attach(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  
  mrb_int pin = 0;
  mrb_get_args(mrb, "i", &pin);
  servo->attach(pin);
  return mrb_nil_value();
}

mrb_value mrb_servo_write(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  mrb_int angle = 0;
  mrb_get_args(mrb, "i", &angle);
  servo->write(angle);
  return mrb_nil_value();
}

mrb_value mrb_servo_set48hz(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  servo->set48hz();
  return mrb_nil_value();
}

mrb_value mrb_servo_set188hz(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  servo->set188hz();
  return mrb_nil_value();
}

mrb_value mrb_servo_detach(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  servo->detach();
  return mrb_nil_value();
}

#endif /*MRUBY_ARDUINO_GALILEO_ENABLE_SERVO*/

mrb_value mrb_arduino_pinMode(mrb_state *mrb, mrb_value self){
  mrb_int pin, mode;
  mrb_get_args(mrb, "ii", &pin, &mode);
  pinMode(pin, mode);
  return mrb_nil_value();
}

mrb_value mrb_arduino_digitalWrite(mrb_state *mrb, mrb_value self){
  mrb_int pin, value;
  mrb_get_args(mrb, "ii", &pin, &value);
  digitalWrite(pin, value);
  return mrb_nil_value();
}

mrb_value mrb_arduino_digitalRead(mrb_state *mrb, mrb_value self){
  mrb_int pin;
  mrb_get_args(mrb, "i", &pin);
  int val = digitalRead(pin);
  return mrb_fixnum_value(val);
}

mrb_value mrb_arduino_analogReference(mrb_state *mrb, mrb_value self){
  mrb_int type;
  mrb_get_args(mrb, "i", &type);
  analogReference(type);
  
  return mrb_nil_value();
}

mrb_value mrb_arduino_analogWrite(mrb_state *mrb, mrb_value self){
  mrb_int pin, value;
  mrb_get_args(mrb, "ii", &pin, &value);
  analogWrite(pin, value);
  return mrb_nil_value();
}

mrb_value mrb_arduino_analogRead(mrb_state *mrb, mrb_value self){
  mrb_int pin;
  mrb_get_args(mrb, "i", &pin);
  int val = analogRead(pin);
  return mrb_fixnum_value(val);
}


mrb_value mrb_arduino_millis(mrb_state *mrb, mrb_value self){
  return mrb_fixnum_value(millis());
}

mrb_value mrb_arduino_micros(mrb_state *mrb, mrb_value self){
  return mrb_fixnum_value(micros());
}

mrb_value mrb_arduino_delay(mrb_state *mrb, mrb_value self){
  mrb_int ms;
  mrb_get_args(mrb, "i", &ms);
  delay(ms);
  return mrb_nil_value();
}

mrb_value mrb_arduino_delayMicroseconds(mrb_state *mrb, mrb_value self){
  mrb_int us;
  mrb_get_args(mrb, "i", &us);
  delayMicroseconds(us);
  return mrb_nil_value();
}


mrb_value mrb_arduino_map(mrb_state *mrb, mrb_value self){
  mrb_int value, fromLow, fromHigh, toLow, toHigh;
  mrb_get_args(mrb, "iiiii", &value, &fromLow, &fromHigh, &toLow, &toHigh);
  mrb_int ret = map(value, fromLow, fromHigh, toLow, toHigh);
  return mrb_fixnum_value(ret);
}

extern "C"
void
mruby_arduino_init_galileo(mrb_state* mrb) {

  RClass *serialClass = mrb_define_class(mrb, "Serial", mrb->object_class);
  mrb_define_class_method(mrb, serialClass, "available", mrb_serial_available, ARGS_NONE());
  mrb_define_class_method(mrb, serialClass, "begin",mrb_serial_begin, ARGS_REQ(1));
  mrb_define_class_method(mrb, serialClass, "println", mrb_serial_println, ARGS_REQ(1));

#ifdef MRUBY_ARDUINO_GALILEO_ENABLE_SERVO
  RClass *servoClass = mrb_define_class(mrb, "Servo", mrb->object_class);
  MRB_SET_INSTANCE_TT(servoClass, MRB_TT_DATA);
  mrb_define_method(mrb, servoClass, "initialize", mrb_servo_initialize, ARGS_NONE());
  mrb_define_method(mrb, servoClass, "attach", mrb_servo_attach, ARGS_REQ(1));
  mrb_define_method(mrb, servoClass, "write", mrb_servo_write, ARGS_REQ(1));
  mrb_define_method(mrb, servoClass, "detach", mrb_servo_detach, ARGS_NONE());
#endif /*MRUBY_ARDUINO_GALILEO_ENABLE_SERVO*/
  
  RClass *arduinoModule = mrb_define_module(mrb, "Arduino");
  mrb_define_module_function(mrb, arduinoModule, "pinMode", mrb_arduino_pinMode, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalWrite", mrb_arduino_digitalWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalRead", mrb_arduino_digitalRead, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "analogReference", mrb_arduino_analogReference, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "analogWrite", mrb_arduino_analogWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "analogRead", mrb_arduino_analogRead, ARGS_REQ(1));

	mrb_define_module_function(mrb, arduinoModule, "millis", mrb_arduino_millis, ARGS_NONE());
  mrb_define_module_function(mrb, arduinoModule, "micros", mrb_arduino_micros, ARGS_NONE());
  mrb_define_module_function(mrb, arduinoModule, "delay", mrb_arduino_delay, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "delayMicroseconds", mrb_arduino_delayMicroseconds, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "map", mrb_arduino_map, ARGS_REQ(5));

  mrb_define_const(mrb, arduinoModule, "HIGH", mrb_fixnum_value(HIGH));
  mrb_define_const(mrb, arduinoModule, "LOW", mrb_fixnum_value(LOW));
  mrb_define_const(mrb, arduinoModule, "INPUT", mrb_fixnum_value(INPUT));
  mrb_define_const(mrb, arduinoModule, "OUTPUT", mrb_fixnum_value(OUTPUT));
  mrb_define_const(mrb, arduinoModule, "INPUT_PULLUP", mrb_fixnum_value(INPUT_PULLUP));
  mrb_define_const(mrb, arduinoModule, "INPUT_FAST", mrb_fixnum_value(INPUT_FAST));
  mrb_define_const(mrb, arduinoModule, "OUTPUT_FAST", mrb_fixnum_value(OUTPUT_FAST));

  mrb_define_const(mrb, arduinoModule, "DEFAULT", mrb_fixnum_value(DEFAULT));
 
}


#endif /*MRUBY_ARDUINO_BOARD_GALILEO*/