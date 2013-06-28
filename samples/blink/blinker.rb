#To (re)compile C bytecode:
#
#/path/to/mruby/bin/mrbc -Bblinker -oblinker.c blinker.rb
#

class Blinker
	include Arduino
	attr_accessor :interval ,:pin
	
	def initialize(pin,interval_ms)
		Serial.println("Blinker initialized")
		@pin = pin
		@interval = interval_ms
		pinMode(@pin, OUTPUT)
	end

	def run
		Serial.println("blink!")

		digitalWrite(@pin, HIGH)
		delay(@interval)
		digitalWrite(@pin, LOW)
		delay(@interval)
	end
end


