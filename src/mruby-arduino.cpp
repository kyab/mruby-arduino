	
#include "mruby.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/data.h"

#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <Servo/Servo.h>


mrb_value mrb_serial_begin(mrb_state *mrb, mrb_value self){
  mrb_int speed = 0;
  int n = mrb_get_args(mrb,"i",&speed);
  Serial.begin(speed);
  return mrb_nil_value();
}

mrb_value mrb_serial_println(mrb_state *mrb, mrb_value self){
  mrb_value s;
  mrb_get_args(mrb,"S", &s);
  Serial.println(RSTRING_PTR(s));
  return mrb_nil_value();
}

void mrb_servo_free(mrb_state *mrb, void *ptr){
  delete (Servo *)ptr;
}

struct mrb_data_type mrb_servo_type = {"Servo", mrb_servo_free};

mrb_value mrb_servo_initialize(mrb_state *mrb, mrb_value self){
  Servo *newServo = new Servo();
  Serial.println("newServo");
  Serial.println((int)newServo);
  DATA_PTR(self) = newServo;  
  DATA_TYPE(self) = &mrb_servo_type;  
  return self;
}

mrb_value mrb_servo_attach(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  
  mrb_int pin = 0;
  int n = mrb_get_args(mrb, "i", &pin);
  servo->attach(pin);
  return mrb_nil_value();
}

mrb_value mrb_servo_write(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  mrb_int angle = 0;
  int n = mrb_get_args(mrb, "i", &angle);
  servo->write(angle);
  return mrb_nil_value();
}


mrb_value mrb_arduino_pinMode(mrb_state *mrb, mrb_value self){
  mrb_int pin, mode;
  int n = mrb_get_args(mrb, "ii", &pin, &mode);
  pinMode(pin, mode);
  return mrb_nil_value();
}

mrb_value mrb_arduino_digitalWrite(mrb_state *mrb, mrb_value self){
  mrb_int pin, value;
  int n = mrb_get_args(mrb, "ii", &pin, &value);
  digitalWrite(pin, value);
  return mrb_nil_value();
}

mrb_value mrb_arduino_digitalRead(mrb_state *mrb, mrb_value self){
  mrb_int pin;
  int n = mrb_get_args(mrb, "i", &pin);
  int val = digitalRead(pin);
  return mrb_fixnum_value(val);
}

mrb_value mrb_arduino_analogWrite(mrb_state *mrb, mrb_value self){
  mrb_int pin, value;
  int n = mrb_get_args(mrb, "ii", &pin, &value);
  analogWrite(pin, value);
  return mrb_nil_value();
}

mrb_value mrb_arduino_analogRead(mrb_state *mrb, mrb_value self){
  mrb_int pin;
  int n = mrb_get_args(mrb, "i", &pin);
  int val = analogRead(pin);
  return mrb_fixnum_value(val);
}
  
mrb_value mrb_arduino_delay(mrb_state *mrb, mrb_value self){
  mrb_int ms;
  int n = mrb_get_args(mrb, "i", &ms);
  delay(ms);
  return mrb_nil_value();
}

mrb_value mrb_arduino_millis(mrb_state *mrb, mrb_value self){
  return mrb_fixnum_value(millis());
}

mrb_value mrb_arduino_map(mrb_state *mrb, mrb_value self){
  mrb_int value, fromLow, fromHigh, toLow, toHigh;
  int n = mrb_get_args(mrb, "iiiii", &value, &fromLow, &fromHigh, &toLow, &toHigh);
  mrb_int ret = map(value, fromLow, fromHigh, toLow, toHigh);
  return mrb_fixnum_value(ret);
}

extern "C"
void
mrb_c_mruby_arduino_gem_init(mrb_state* mrb) {


  RClass *serialClass = mrb_define_class(mrb, "Serial", mrb->object_class);
  mrb_define_class_method(mrb, serialClass,"begin",mrb_serial_begin, ARGS_REQ(1));
  mrb_define_class_method(mrb, serialClass,"println", mrb_serial_println, ARGS_REQ(1));

  RClass *servoClass = mrb_define_class(mrb, "Servo", mrb->object_class);
  MRB_SET_INSTANCE_TT(servoClass, MRB_TT_DATA);
  mrb_define_method(mrb, servoClass,"initialize", mrb_servo_initialize, ARGS_NONE());
  mrb_define_method(mrb, servoClass,"attach", mrb_servo_attach, ARGS_REQ(1));
  mrb_define_method(mrb, servoClass,"write", mrb_servo_write, ARGS_REQ(1));
  
  RClass *arduinoModule = mrb_define_module(mrb, "Arduino");
  mrb_define_module_function(mrb, arduinoModule, "pinMode", mrb_arduino_pinMode, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalWrite", mrb_arduino_digitalWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalRead", mrb_arduino_digitalRead, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "analogWrite", mrb_arduino_analogWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "analogRead", mrb_arduino_analogRead, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "delay", mrb_arduino_delay, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "millis", mrb_arduino_millis, ARGS_NONE());
  mrb_define_module_function(mrb, arduinoModule, "map", mrb_arduino_map, ARGS_REQ(5));
  mrb_define_const(mrb, arduinoModule, "HIGH", mrb_fixnum_value(1));
  mrb_define_const(mrb, arduinoModule, "LOW", mrb_fixnum_value(0));
  mrb_define_const(mrb, arduinoModule, "INPUT", mrb_fixnum_value(0));
  mrb_define_const(mrb, arduinoModule, "OUTPUT", mrb_fixnum_value(1));
  mrb_define_const(mrb, arduinoModule, "INPUT_PULLUP", mrb_fixnum_value(2));
}

extern "C"
void
mrb_c_mruby_arduino_gem_final(mrb_state* mrb) {

}