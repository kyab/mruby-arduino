mruby-arduino
----------------------------

mruby-arduino is mrbgem which wraps and add Arduino library functions to mruby.

This mrbgem is currently intended to use with Arduino compatible board "chipKIT Max32" from Digilent Inc., with their IDE named "MPIDE", but should work with any Arduino compatible environment like Arduino Due. (Notice: Your board should have enough RAM, say around 90kb, to run mruby).

chipKIT Max32:
http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,892,894&Prod=CHIPKIT-MAX32

How to use
-------------------------------------
At first you should make cross-build setting for your board. For chipKIT Max32, please add following lines from /path/to/[mruby](https://github.com/mruby/mruby)/examples/targets/chipKitMax32.rb to  mruby/build_config.rb.

Then, add below line to include mruby-arduino 
```
MRuby::CrossBuild.new("chipKitMax32") do |conf|
...
...
	conf.gem :git => "https://github.com/kyab/mruby-arduino.git", :branch => "master"
...
end
```
Then, type "make" in mruby top dir.

Notice you don't have to download/clone mruby-arduino from github. Mrbgem automatically get codes from this github repositry.

Next is setting for MPIDE (or Arduino IDE). Open Mpide.app//Contents/Resources/Java/hardware/pic32/platforms.txt and edit below lines to add header search path and library.
```
...
pic32.recipe.c.combine.pattern={0}{1}::{2}::{3}{4}::-o::{5}{6}.elf::{7}::{8}::-L/path/to/mruby/build/chipKitMax32/lib::-lmruby::-L{9}::-lm::-T::{10}/{11}
...
pic32.compiler.cpp.flags=-O2::-c::-mno-smart-io::-w::-fno-exceptions::-ffunction-sections::-fdata-sections::-g::-mdebugger::-Wcast-align::-fno-short-double::-I/path/to/mruby/include

Sample Sketch
```
```
------------------------------------------


Sample Code
-------------------------------------------
    
    class ArduinoSample
        include Arduino
        def initialize
            Serial.begin(9600)
            @servo = Servo.new
            @servo.attach(3)
            pinMode(5, OUTPUT)
        end

        def run
            Serial.println("Hello World")
            digitalWrite(5,HIGH)
            delay(1000)
            digitalWrite(5, LOW)
            
            @servo.write(90)
            delay(500)
            @servo.write(30)
        end
    end

    ArduinoSample.new.run
             

Implemented APIs
-----------------
all classes, functions, and constants are under module Arduino
- pinMode
- digitalWrite
- digitalRead
- analogReference
- analogRead
- analogWrite
- tone
- noTone
- shiftOut
- shiftIn
- pulseIn
- millis
- micros
- delay
- delayMicroseconds
- constrain
- map
- randomSeed
- random
- interrupts
- noInterrupts
- HIGH
- LOW
- INPUT
- OUTPUT
- INPUT_PULLUP

- Serial.available
- Serial.begin
- Serial.println

- Servo#attach
- Servo#write