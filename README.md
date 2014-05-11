#mruby-arduino
mruby-arduino is wrapper mrbgem for Arduino API.

This mrbgem is currently intended to use with Arduino compatible board "chipKIT Max32" from Digilent Inc, "Arduino Due", or "Intel Galileo" but should work with any Arduino compatible environment. (Notice: Your board should have enough RAM, say around 90kb at least, to run mruby).

chipKIT Max32:
http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,892,894&Prod=CHIPKIT-MAX32

Arduino Due:
http://arduino.cc/en/Main/arduinoBoardDue

Intel Galileo:
http://arduino.cc/en/ArduinoCertified/IntelGalileo

#Requirements
- [mruby](https://github.com/mruby/mruby)
- "chipKIT Max32" or "Arduino Due" or "Intel Galileo" and its IDEs, or any Arduino comatiple boards and toolchains which have enough RAM(Arduino UNO or some small RAM boards won't work).

#Getting Started
##chipKIT Max32
### 1. Build mruby with mruby-arduino
At first you should make cross-build setting for chipKIT Max32. Copy `/path/to/mruby/examples/targets/build_config_chipKITMax32.rb` to `/path/to/mruby/build_config.rb` (or any file specified by MRUBY_CONFIG environment variable).

Then, open `build_config.rb` and add below line to include mruby-arduino. 
```ruby
...
...
MRuby::CrossBuild.new("chipKITMax32") do |conf|
...
...
	conf.gem :git => "https://github.com/kyab/mruby-arduino.git", :branch => "master"
...
end
```
Type `make` or `./minirake` on mruby top dir will build mruby library for chipKIT Max32.

Notice you don't have to download/clone mruby-arduino from this github repository by yourself. mruby's build system automatically clone mruby-arduino mrbgem from github.

###2. Settings for MPIDE
Next is to edit some files in MPIDE, to use with mruby.
Open Mpide.app/Contents/Resources/Java/hardware/pic32/platforms.txt. Edit below lines to add header search path and library options for mruby.
```
...
pic32.recipe.c.combine.pattern={0}{1}::{2}::{3}{4}::-o::{5}{6}.elf::{7}::{8}::-L/path/to/mruby/build/chipKITMax32/lib::-lmruby::-L{9}::-lm::-T::{10}/{11}
...
pic32.compiler.cpp.flags=-O2::-c::-mno-smart-io::-w::-fno-exceptions::-ffunction-sections::-fdata-sections::-g::-mdebugger::-Wcast-align::-fno-short-double::-I/path/to/mruby/include
```

Re-launch MPIDE.

##Arduino Due
###1. Build mruby with mruby-arduino
You should install Arduino 1.5.6-r2 from http://arduino.cc/en/Main/Software. Arduino 1.x can not used for Arduino Due.

At first you should make cross-build setting for Arduino Due. Copy `/path/to/mruby/examples/targets/build_config_ArduinoDue.rb` to `/path/to/mruby/build_config.rb` (or any file specified by MRUBY_CONFIG environment variable).

Then, open `build_config.rb` and add below line to include mruby-arduino. 
```ruby
...
...
MRuby::CrossBuild.new("ArduinoDue") do |conf|
...
...
  conf.gem :github =>"kyab/mruby-arduino", :branch => "master" do |g|
    g.cxx.include_paths << ["#{ARDUINO_PATH}/libraries/Servo/src", "#{ARDUINO_PATH}/libraries/Servo/arch/sam"]
  end
...
end
```
Type `make` or `./minirake` on mruby top dir will build mruby library for Arduino Due.

Notice you don't have to download/clone mruby-arduino from this github repository by yourself. mruby's build system automatically clone mruby-arduino mrbgem from github.

###2. Settings for Arduino IDE
Next is to edit some files in Arduino IDE, to use with mruby.
Open /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/sam/platform.txt and Edit below lines to add header search path and library options for mruby.
```
...
compiler.c.flags=-c -g -Os -w -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -Dprintf=iprintf -I/path/to/mruby/include
...
compiler.cpp.flags=-c -g -Os -w -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -Dprintf=iprintf -I/path/to/mruby/include
...
...
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} -mcpu={build.mcu} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -lm -lgcc -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols -Wl,--start-group "{build.path}/syscalls_sam3.c.o" {object_files} "{build.variant.path}/{build.variant_system_lib}" "{build.path}/{archive_file}" -Wl,--end-group  "-L/path/to/mruby/build/ArduinoDue/lib" -lmruby
...
```

Re-launch Arduino IDE.

##STM32F4Discovery
TODO (make issue if you interested).

##Intel Galileo
###1. Build mruby with mruby-arduino
At first you should make cross-build setting for Arduino Due. Copy `/path/to/mruby/examples/targets/build_config_IntelGalileo.rb` to `/path/to/mruby/build_config.rb` (or any file specified by MRUBY_CONFIG environment variable) .

Then, add below line to include mruby-arduino. 
```ruby
...
...
MRuby::CrossBuild.new("Galileo") do |conf|
...
  conf.gem :github =>"kyab/mruby-arduino", :branch => "master" do |g|
    g.cxx.include_paths << "#{GALILEO_X86_PATH}/libraries/Wire"
    g.cxx.include_paths << "#{GALILEO_X86_PATH}/libraries/Servo"
    
    #uncomment below line if you want enable unsupported Servo class
    #g.cxx.defines << "MRUBY_ARDUINO_GALILEO_ENABLE_SERVO"
  end
...
end
```
Type `make` or `./minirake` on mruby top dir will build mruby library for Intel Galileo.

Notice you don't have to download/clone mruby-arduino from this github repository by yourself. mruby's build system automatically clone mruby-arduino mrbgem from github.

###2. Settings for Arduino IDE for Intel Galileo
Next is to edit some files in Arduino IDE for Intel Galileo, to use with mruby.
Open /Applications/Arduino_Galileo.app/Contents/Resources/Java/hardware/arduino/sam/platform.osx.txt and Edit below lines to add header search path and library options for mruby.

note1: Here I assume you renamed original name of Arduino IDE for Intel Galileo(Arduino.app) to Arduino_Galileo.app. Its common way to install both of normal [Arduino IDE] and [Arduino IDE for Intel Galileo].

note2: platform.osx.txt is for Mac OSX. If you are using Windows, edit platform.win.txt. If you are using linux, edit platform.linux.txt(32bit) or platform.linux64.txt(64bit).
```
...
#compiler.c.flags= -m32 -march=i586 --sysroot={runtime.ide.path}/hardware/tools/x86/i586-poky-linux-uclibc -c -g -Os -w -ffunction-sections -fdata-sections -MMD
compiler.c.flags= -m32 -march=i586 --sysroot={runtime.ide.path}/hardware/tools/x86/i586-poky-linux-uclibc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -I/path/to/mruby/include
...
...
#compiler.cpp.flags= -m32 -march=i586 --sysroot={runtime.ide.path}/hardware/tools/x86/i586-poky-linux-uclibc -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD
compiler.cpp.flags= -m32 -march=i586 --sysroot={runtime.ide.path}/hardware/tools/x86/i586-poky-linux-uclibc -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -I/-I/path/to/mruby/include
...
...
## Combine gc-sections, archives, and objects
#recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} -march={build.mcu} -o "{build.path}/{build.project_name}.elf" {object_files} "{build.path}/{archive_file}" "-L{build.path}" -lm -lpthread
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} -march={build.mcu} -o "{build.path}/{build.project_name}.elf" {object_files} "{build.path}/{archive_file}" "-L{build.path}" -lm -lpthread -L/path/to/mruby/build/Galileo/lib -lmruby
...
```

You also needs to edit /Applications/Arduino_Galileo.app/Contents/Resources/Java/hardware/arduino/sam/boards.txt as belows.
This increases maximum sketch size from 256kb to 1024kb. The sketch with mruby easily become bigger than 256kb. 

```
...
#izmir_fd.upload.maximum_size=262144
izmir_fd.upload.maximum_size=1048576
...
```

Re-launch Arduino IDE for Intel Galileo.



##Build and Run the sample sketch (for all boards)
Open samples/blink/blink.pde in MPIDE or Arduino IDE or Arduino IDE for Intel Galileo.
Build and upload it (This may take minutes).

You will see pin 13 blinks each 1 seconds. Also check Serial Monitor.

If you have any trouble in building, uploading, or running, please report issue.

##Modifying ruby code.
blink.c includes a mruby bytecode generated from blinker.rb.
When you modify blinker.rb, please re-generate(compile) blinker.c from blinker.rb with mrbc, which is mruby bytecode compiler.
```sh
/path/to/mruby/bin/mrbc -Bblinker -oblinker.c blinker.rb
```
  

#Supported APIs
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

#Troubleshooting
Measure issue will RAM(heap) shortage. If you can build and upload but nothing happens or freezed, disable almost all mrbgems from build_config.rb like below.

```ruby
...
...
MRuby::CrossBuild.new("ArduinoDue") do |conf|
...
...
  #gems from core
  #conf.gem :core => "mruby-print"
  #conf.gem :core => "mruby-math"
  #conf.gem :core => "mruby-enum-ext"

  #light-weight regular expression
  #conf.gem :github => "masamitsu-murase/mruby-hs-regexp", :branch => "master"

  #Arduino API
  conf.gem :github =>"kyab/mruby-arduino", :branch => "master" do |g|
    g.cxx.include_paths << ["#{ARDUINO_PATH}/libraries/Servo/src", "#{ARDUINO_PATH}/libraries/Servo/arch/sam"]
  end

...
end

```

mruby-arduino is still experimental project, so feedback is very welcome.

#Note for Intel Galileo
On Intel Galileo, Arduino API is implemented above on linux kernel and sketch runs as one linux process, so has some limitations/lack of API(this is not mruby-arduino issue). I strongly recommend you to consult [Intel Galileo Release Notes](https://communities.intel.com/docs/DOC-21837) .

#Contribution
Feel free to report issue, or send pull-request. 

#Like interactive?
If you want interactive way like irb, check out Hostbased mirb:
https://github.com/kyab/mruby-bin-mirb-hostbased
