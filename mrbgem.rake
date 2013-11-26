MRuby::Gem::Specification.new('mruby-arduino') do |spec|


	spec.license = 'MIT'
 	spec.authors = 'kyab'

 	if self.class.const_defined? :ARDUINO_PATH
 		spec.cxx.include_paths << ["#{ARDUINO_PATH}/libraries/Servo/src", "#{ARDUINO_PATH}/libraries/Servo/arch/sam"]
 	end

 	#recommend to use with mruby/examples/targets/chipKitMax32.rb or ArduinoDue.rb

end