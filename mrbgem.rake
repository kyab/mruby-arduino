MRuby::Gem::Specification.new('mruby-arduino') do |spec|


	spec.license = 'MIT'
 	spec.authors = 'kyab'

	spec.cxx do |cxx|
		cxx.command = "#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/compiler/pic32-tools/bin/pic32-gcc"
		cxx.include_paths << ["#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/cores/pic32",
							"#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/variants/Max32",
							"#{MPIDE_PATH}/Contents/Resources/Java/hardware/pic32/libraries"
						]

		cxx.flags = "-O2 -mno-smart-io -w -ffunction-sections -fdata-sections -g -mdebugger -Wcast-align " +
				"-fno-short-double -mprocessor=32MX795F512L -DF_CPU=80000000L -DARDUINO=23 -D_BOARD_MEGA_ " +
				"-DMPIDEVER=0x01000202 -DMPIDE=23"
				
		cxx.compile_options = "%{flags} -o %{outfile} -c %{infile}"
	end

end