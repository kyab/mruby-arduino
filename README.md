#mruby-arduino
mruby-arduino is mrbgem which wraps and add Arduino library functions to mruby.

This mrbgem is currently intended to use with Arduino compatible board "chipKIT Max32" from Digilent Inc., or Arduino Due, but should work with any compatible environment. (Notice: Your board should have enough RAM, say around 90kb, to run mruby).

chipKIT Max32:
http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,892,894&Prod=CHIPKIT-MAX32

Arduino Due:
http://arduino.cc/en/Main/arduinoBoardDue

#Getting Started
##Build mruby
###chipKIT Max32
At first you should make cross-build setting for chipKIT Max32, please add contents of your /path/to/[mruby](https://github.com/mruby/mruby)/examples/targets/chipKitMax32.rb to mruby/build_config.rb.

Then, add below line to include mruby-arduino. 
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

Next is setting for MPIDE. Open Mpide.app/Contents/Resources/Java/hardware/pic32/platforms.txt. Edit below lines to tell header search path and library options for mruby.
```
...
pic32.recipe.c.combine.pattern={0}{1}::{2}::{3}{4}::-o::{5}{6}.elf::{7}::{8}::-L/path/to/mruby/build/chipKitMax32/lib::-lmruby::-L{9}::-lm::-T::{10}/{11}
...
pic32.compiler.cpp.flags=-O2::-c::-mno-smart-io::-w::-fno-exceptions::-ffunction-sections::-fdata-sections::-g::-mdebugger::-Wcast-align::-fno-short-double::-I/path/to/mruby/include
```

Please re-launch MPIDE.

###Arduino Due
At first you should make cross-build setting for Arduino Due, please add contents of your /path/to/[mruby](https://github.com/mruby/mruby)/examples/targets/ArduinoDue.rb to mruby/build_config.rb.

Then, add below line to include mruby-arduino. 
```
MRuby::CrossBuild.new("Arduino Due") do |conf|
...
...
    conf.gem :git => "https://github.com/kyab/mruby-arduino.git", :branch => "master"
...
end
```
Then, type "make" in mruby top dir.

Notice you don't have to download/clone mruby-arduino from github. Mrbgem automatically get codes from this github repositry.

Next is setting for Arduino IDE. Open /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/sam/platform.txt and Edit below lines to tell header search path and library options for mruby.
```
...
compiler.c.flags=-c -g -Os -w -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -Dprintf=iprintf -I/path/to/mruby/include
...
compiler.cpp.flags=-c -g -Os -w -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -Dprintf=iprintf -I/path/to/mruby/include
...
...
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} -mcpu={build.mcu} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -lm -lgcc -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols -Wl,--start-group "{build.path}/syscalls_sam3.c.o" {object_files} "{build.variant.path}/{build.variant_system_lib}" "{build.path}/{archive_file}" -Wl,--end-group  "-L/path/to/mruby/build/Arduino Due/lib" -lmruby
...
```

Please Re-launch Arduino IDE.

###STM32F4Discovery
TODO


##Run the sample
Open samples/blink/blink.pde in MPIDE or Arduino IDE, then build and upload(This may take minutes).
You will see pin 13 blinks each 1 seconds(1000 msec). Also check Serial Monitor.

If you have any trouble in building, uploading, or running, please report issue.

##Modifying ruby code.
blink.c is a bytecode generated from blinker.rb.
When you modify blinker.rb, please re-generate(compile) blinker.rb to blinker.c with mrbc, which is mruby bytecode compiler.
```
/path/to/mruby/bin/mrbc -Bblinker -obkinker.c blinker.rb
```
  

#Implemented APIs
All classes, functions, and constants are under module Arduino
- pinMode
- digitalWrite
- digitalRead
- analogReference
- analogRead
- analogWrite
- tone (chipKit Max32 only)
- noTone (chipKit Max32 only)
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

#Like interactive?
If you want interactive way like irb, check out hostbased mirb:
https://github.com/kyab/mruby-bin-mirb-hostbased


