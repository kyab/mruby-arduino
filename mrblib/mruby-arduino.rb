module Arduino
	module_function :constrain

	def constrain(x ,a ,b)
		if x < a
			x = a
		end

		if x > b
			x = b
		end
		x
	end
end