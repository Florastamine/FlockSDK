--[[
Copyright (c) 2016, Florastamine

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------
This is a collection of day-to-day math functions, which includes a collection of
pseudo random number generators (PRNGs), which includes:
* LCG (Linear Congruential Generator), fast and works.
* Multiply-with-carry, not so fast but works great.
* Mersenne Twister: slowest, but generates high quality unique numbers.

[Note] -
The original implementation of PRNGs (in pure Lua) was written by linux-man and posted here:
https://love2d.org/forums/viewtopic.php?t=3424, released under the MIT license, which
I took and made minor modifications to it.
------------------------------------------
+ lcg(seed, param) / linear_congruential_generator(seed, param)
++ Creates a new LCG generator. Pass 0 to seed to use the random one (defined in the local function seed()). Additionally, pass
++ either: "ansi_c", "tp", "mvc"/"mvcpp" or "nr" to specify the generator's parameter.
++ After creating the generator with lcg(), use generator:random(a, b) to generate a new
++ random number.
------------------------------------------
+ mwc(seed, param) / multiply_with_carry(seed, param)
++ Creates a new multiply-with-carry generator. The rest is the same with the LCG generator.
++ After creating the generator with mwc(), use generator:random(a, b) to generate a new
++ random number.
------------------------------------------
++ mt(seed) / mersenne_twister(seed)
++ Creates a new Mersenne Twister generator. After creating the generator with mt(), use generator:random(a, b)
++ to generate a new random number.
]]--

local downpour = require("./downpour")

require("./downpour-io") 

downpour.math = {}

local normalize = downpour.ioutils.numericutils.__normalize 
local bit_and   = downpour.ioutils.numericutils.bit_and 
local bit_or    = downpour.ioutils.numericutils.bit_or 
local bit_xor   = downpour.ioutils.numericutils.bit_xor  

local seed = function ()
	return normalize(os.time())
end

downpour.math.__mersenne_twister__ = {}
downpour.math.__mersenne_twister__.__index = downpour.math.__mersenne_twister__

function downpour.math.__mersenne_twister__:random_seed (s)
	if not s then s = seed() end
	self.mt[0] = normalize(s)

	for i = 1, 623 do
		self.mt[i] = normalize(0x6c078965 * bit_xor(self.mt[i - 1], math.floor(self.mt[i - 1] / 0x40000000)) + i)
	end
end

function downpour.math.__mersenne_twister__:random (a, b)
	local y
	if self.index == 0 then
		for i = 0, 623 do
			y = self.mt[(i + 1) % 624] % 0x80000000
			self.mt[i] = bit_xor(self.mt[(i + 397) % 624], math.floor(y / 2))
			if y % 2 ~= 0 then self.mt[i] = bit_xor(self.mt[i], 0x9908b0df) end
		end
	end

	y = self.mt[self.index]
	y = bit_xor(y, math.floor(y / 0x800))
	y = bit_xor(y, bit_and(normalize(y * 0x80), 0x9d2c5680))
	y = bit_xor(y, bit_and(normalize(y * 0x8000), 0xefc60000))
	y = bit_xor(y, math.floor(y / 0x40000))
	self.index = (self.index + 1) % 624

	if not a then return y / 0x80000000
	elseif not b then
		if a == 0 then return y
		else return 1 + (y % a)
		end
	else
		return a + (y % (b - a + 1))
	end
end

function downpour.math.mersenne_twister (s)
	local t = {}
	setmetatable(t, downpour.math.__mersenne_twister__)
	t.mt = {}
	t.index = 0
	t:random_seed(s)

	return t
end

function downpour.math.mt (s)
	return downpour.math.mersenne_twister(s)
end

downpour.math.__lcg__ = {}
downpour.math.__lcg__.__index = downpour.math.__lcg__

function downpour.math.__lcg__:random_seed (s)
	if not s then s = seed() end
	self.x = normalize(s)
end

function downpour.math.__lcg__:random (a, b)
	local y = (self.a * self.x + self.c) % self.m
	self.x = y
	if not a then return y / 0x10000
	elseif not b then
		if a == 0 then return y
		else return 1 + (y % a) end
	else
		return a + (y % (b - a + 1))
	end
end

function downpour.math.linear_congruential_generator (s, r)
	local t = {}
	setmetatable(t, downpour.math.__lcg__)

	if not r then
		r = "ansi_c"
	end

	if r == "ansi_c" then
		t.a, t.c, t.m = 1103515245, 12345, 0x10000
	elseif r == "nr" then -- https://en.wikipedia.org/wiki/Numerical_Recipes
		t.a, t.c, t.m = 1664525, 1013904223, 0x10000
	elseif (r == "mvc") or (r == "mvcpp") then
		t.a, t.c, t.m = 214013, 2531011, 0x10000
	elseif r == "tp" then
		t.a, t.c, t.m = 33797, 1, 0x10000
	else
	end

	t:random_seed(s)
	return t
end

function downpour.math.lcg (s, r)
	return downpour.math.linear_congruential_generator(s, r)
end

downpour.math.__multiply_with_carry__ = {}
downpour.math.__multiply_with_carry__.__index = downpour.math.__multiply_with_carry__

function downpour.math.__multiply_with_carry__:random (a, b)
	local m = self.m
	local t = self.a * self.x + self.c
	local y = t % m
	self.x = y
	self.c = math.floor(t / m)
	if not a then return y / 0x10000
	elseif not b then
		if a == 0 then return y
		else return 1 + (y % a) end
	else
		return a + (y % (b - a + 1))
	end
end

function downpour.math.__multiply_with_carry__:random_seed (s)
	if not s then s = seed() end
	self.c = self.ic
	self.x = normalize(s)
end

function downpour.math.multiply_with_carry (s, r)
	local t = {}
	setmetatable(t, downpour.math.__multiply_with_carry__)

	if not r then
		r = "ansi_c"
	end

	if r == "ansi_c" then
		t.a, t.c, t.m = 1103515245, 12345, 0x10000
	elseif r == "nr" then -- https://en.wikipedia.org/wiki/Numerical_Recipes
		t.a, t.c, t.m = 1664525, 1013904223, 0x10000
	elseif (r == "mvc") or (r == "mvcpp") then
		t.a, t.c, t.m = 214013, 2531011, 0x10000
	elseif r == "tp" then
		t.a, t.c, t.m = 33797, 1, 0x10000
	else
	end

	t.ic = t.c
	t:random_seed(s)

	return t
end

function downpour.math.mwc (s, r)
	return downpour.math.multiply_with_carry(s, r)
end

return downpour.math
