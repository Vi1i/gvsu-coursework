import random
import struct
import operator
from bitstring import BitArray

def rosenbrockBanana(x, y):
	result = 0;
	result = ((1 - x) ** 2) + (100 * ((y - (x ** 2)) ** 2))
	return result

def goldsteinPrice(x, y):
	result = 0;
	result = ((((x + y + 1) ** 2) * (19 - (14 * x) + (3 * (x ** 2)) - (14 * y) + (6 * x * y) + (3 * (y ** 2)) ) + 1) * ((((2 * x) - (3 * y)) ** 2) * (18 - (32 * x) + (12 * (x ** 2)) + (48 * y) - (36 * x * y) + (27 * (y ** 2)) ) + 30))
	return result

def to_binary(num):
	return BitArray(float=num, length=64).bin

def to_float(num):
	return BitArray("0b"+ num).float

def initPop(pops, r):
	population = {}
	for pop in range(0, pops):
		_x = random.uniform(r[0], r[1])
		_y = random.uniform(r[0], r[1])
		x = to_binary(_x)
		y = to_binary(_y)
		# An example of taking a float into binary, and a binary into a float
		population[pop] = (x, y)

	return population

def selector(population, count):
	temp = random.sample(population, count)
	temp_vals = []
	for key in temp:
		temp_vals.append(eval(0, population[key]))

	index, value = min(enumerate(temp_vals), key=operator.itemgetter(1))

	return temp[index]

def eval(equation, vals):
	_x = to_float(vals[0])
	_y = to_float(vals[1])
	if(equation == 0):
		return rosenbrockBanana(_x, _y)
	elif(equation == 1):
		return goldsteinPrice(_x, _y)
	else:
		print("ERROR")

def crossover(indv1, indv2):
	roll = random.randint(0,128)
	x = ""
	y = ""
	for pos in range(0,128):
		if(pos < roll):
			parent = indv1
		else:
			parent = indv2

		if pos >= 64:
			y += parent[1][pos - 64]
		else:
			x += parent[0][pos]


	child = (x,y)
	return child

def mutation(indv, p):
	x = ""
	y = ""
	for pos in range(0,128):
		roll = random.randint(0,128)
		if roll < p:
			if pos >= 64:
				tmp = pos - 64
				if tmp < 4 and tmp > 0: 
					y += indv[1][pos - 64]
				else:
					y += "0" if indv[1][pos - 64] == "1" else "1"
			else:
				if pos < 4 and pos > 0: 
					x += indv[0][pos]
				else:
					x += "0" if indv[0][pos] == "1" else "1"
		else:
			if pos >= 64:
				y += indv[1][pos - 64]
			else:
				x += indv[0][pos]


	mutant = (x, y)
	return mutant

def fitness(population):
	temp_population = []
	for key in population:
		temp_population.append(eval(0, population[key]))
	index, value = min(enumerate(temp_population), key=operator.itemgetter(1))
	return value

# Set the seed for the random generator
random.seed(1)
crossover_p = 0.6
mutation_p = 0.1

# Create the population
population = initPop(1000, (-2, 2))

print("Generation,Fitness")
for gen in range(0, 1000):
	fit = fitness(population)
	print(str(gen) + "," + str(fit))
	new_pop = {}
	for i in range(0, len(population)):
		# Fnd the selection to variate on
		indv1 = selector(population, 5)
		roll = random.uniform(0, 1)
		if roll >= crossover_p:
			indv2 = selector(population, 5)
			new_pop[i] = crossover(population[indv1], population[indv2])
		else:
			new_pop[i] = population[indv1]

		new_pop[i] = mutation(new_pop[i], mutation_p)
	population = new_pop;