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
	huffman_tree_depth.jl <block_len_log2>

""")
	exit(1)
end

block_len_log2 = parse(Int, ARGS[1])
@show block_len_log2

block_len = 1 << block_len_log2

max_depth = 0
for d in 1:32
	f = fibonacci(d+1)
	max_freq = cumsum(f)[end]
	if max_freq >= block_len
		max_depth = d
		break
	end
end

@show max_depth

