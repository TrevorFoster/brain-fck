
def factor(n)
	factors = []
	for i in (2..n / 2).reverse_each
		if n % i == 0
			factors << [i, n / i]
		end
	end

	return factors[factors.length / 2]
end

def isprime(n)
	return true if n == 2

	for i in (2..(n / 2 + 1))
		return false if n % i == 0
	end

	return true
end

def closestComposite(n)
	i = 1
	while 1
		if !isprime(n + i)
			return n + i
		elsif !isprime(n - i)
			return n - i
		end
		i += 1
	end
end

def translate(message)
	p = 0
	buffer = Array.new(1000){0}
	compiled = ""

	message.each_byte do |c|
		if buffer[p] == c
			compiled += '.'
			buffer[p] = c
			next
		end

		left, diff = c - buffer[p], 0

		if isprime(left.abs)
			if left.abs < 4
				compiled += ("++-"[left <=> 0]) * left.abs + "."
				buffer[p] = c
				next
			end

			newLeft = (left <=> 0) * closestComposite(left.abs)
			diff = (newLeft - left)
			left = newLeft
		end

		i, j = factor(left.abs)

		if p != 0
			compiled += "<" * p
			p = 0
		end

		compiled += "+" * i
		compiled += "[>" + ("++-"[left <=> 0]) * j + "<-]>" + ("++-"[-diff <=> 0]) * diff.abs + "."

		p += 1
		buffer[p] = c
	end

	return compiled
end

if ARGV.length < 1
	puts "Please pass a string as an argument"
	exit
end

puts translate(ARGV[0])