mruby-arduino
----------------------------

mruby-arduino is mrbgem which wraps and add Arduino library functions to mruby.


This mrbgem is intended to use with Arduino compatible board "chipKIT Max32" from Digilent Inc with their IDE named "MPIDE", but should work with any Arduino compatible environment(but your board should have enough RAM, say around 90kb, to run mruby).

http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,892,894&Prod=CHIPKIT-MAX32

How to use
-------------------------------------
At first you should make cross-build setting to mruby/build_config.rb. Please add following lines to mruby/build_config.rb.

	 MRuby::CrossBuild.new("chipKitMax32") do |conf|
	 	toolchain :gcc

	 	conf.gem :git => "https://github.com/kyab/mruby-arduino.git", :branch => "master"
	 	
	 	MPIDE_PATH = "/Applications/mpide.app"

	 	conf.cc do |cc|
	 		cc.command="#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/compiler/pic32-tools/bin/pic32-gcc"
	 		cc.include_paths << ["#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/cores/pic32",
	 							"#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/variants/Max32"]
	 		cc.flags = "-O2 -mno-smart-io -w -ffunction-sections -fdata-sections -g -mdebugger -Wcast-align " +
	 				"-fno-short-double -mprocessor=32MX795F512L -DF_CPU=80000000L -DARDUINO=23 -D_BOARD_MEGA_ " +
	 				"-DMPIDEVER=0x01000202 -DMPIDE=23"
	 		cc.compile_options = "%{flags} -o %{outfile} -c %{infile}"
	 	end

	 	conf.archiver do |archiver|
	 		archiver.command = "#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/compiler/pic32-tools/bin/pic32-ar"
	 		archiver.archive_options = 'rcs %{outfile} %{objs}'
	 	end

	 	conf.bins = []
	 end 

Then, type "make" on mruby top dir.

Notice you don't have to download/clone mruby-arduino from github. Mrbgem automatically get codes from this github repositry.

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
- Arduino#pinMode
- Arduino#digitalWrite
- Arduino#digitalRead
- Arduino#analogWrite
- Arduino#analogRead
- Arduino#delay
- Arduino#millis
- Arduino#map
- Arduino#HIGH
- Arduino#LOW
- Arduino#INPUT
- Arduino#OUTPUT
- Arduino#INPUT_PULLUP

- Serial#begin
- Serial#println

- Servo.attach
- Servo.write