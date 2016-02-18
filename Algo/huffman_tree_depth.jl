#!/usr/bin/env julia

# Calculate max count for some huffman tree depth.

function fibonacci(n)
	f = zeros(Int, n)
	f[1] = 1
	f[2] = 1
	for i in 3:n
		f[i] = f[i-1] + f[i-2]
	end
	f
end

if length(ARGS) ≠ 1
	println("""
Usage:
	huffman_tree_depth.jl MAX_DEPTH
""")
	exit(1)
end

max_depth = parse(Int, ARGS[1])
f = fibonacci(max_depth+1)
max_count = cumsum(f)[end]

@show max_depth
@show max_count

