#!/usr/bin/env julia

# Calculate max count for some huffman tree depth.

function fibonacci(n)
	f = zeros(n)
	f[1] = 1
	f[2] = 1
	for i in 3:n
		f[i] = f[i-1] + f[i-2]
	end
	f
end

max_depth = 5
f = fibonacci(max_depth+1)
max_count = cumsum(f)[end]
@show max_depth
@show max_count
