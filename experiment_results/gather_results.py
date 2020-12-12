import numpy as np
import matplotlib.pyplot as plt


def inputer(file, tab):
    for line in file:
        tab.append(line.split())


def data_accumulator(tab):
    # gather info how many lines there will be for one set size
    first_element = tab[0][0]
    line_amount = 0
    for size, time, score in tab:
        if first_element != size:
            break
        else:
            line_amount += 1

    # prepare data for plots
    time_to_size = []
    iterations = 0
    acum_time = 0
    acum_score = 0
    for size, time, score in tab:
        if iterations < line_amount - 1:
            acum_time += int(time)
            acum_score += int(score)
            iterations += 1
        else:
            acum_time += int(time)
            acum_score += int(score)
            iterations += 1
            time_to_size.append([int(size), acum_time / iterations, acum_score / iterations])
            acum_time = 0
            acum_score = 0
            iterations = 0

    return time_to_size


# setup
brute_results = []
hill_results = []
tabu_results = []
anneal_results = []

# reading files
brute_file = open("brute experiment results.txt")
hill_file = open("hill experiment results.txt")
tabu_file = open("tabu experiment results.txt")
anneal_file = open("anneal experiment results.txt")

# make data usable
inputer(brute_file, brute_results)
inputer(hill_file, hill_results)
inputer(tabu_file, tabu_results)
inputer(anneal_file, anneal_results)

# prepare data for ploting
brute_time_to_size = data_accumulator(brute_results)
brute_time_to_size.sort()
hill_time_to_size = data_accumulator(hill_results)
hill_time_to_size.sort()
tabu_time_to_size = data_accumulator(tabu_results)
tabu_time_to_size.sort()
anneal_time_to_size = data_accumulator(anneal_results)
anneal_time_to_size.sort()

# Displaying data about time to size
for key, value in enumerate(brute_time_to_size):
    plt.plot(brute_time_to_size[key][0], brute_time_to_size[key][1], "r^", hill_time_to_size[key][0], hill_time_to_size[key][1], "b^", tabu_time_to_size[key][0], tabu_time_to_size[key][1], "g^", anneal_time_to_size[key][0], anneal_time_to_size[key][1], "y^")
plt.title("Time to size")
plt.xlabel("Set size")
plt.ylabel("Time amount")
plt.show()

for key, value in enumerate(brute_time_to_size):
    plt.plot(brute_time_to_size[key][1], brute_time_to_size[key][2], "r^", hill_time_to_size[key][1], hill_time_to_size[key][2], "b^", tabu_time_to_size[key][1], tabu_time_to_size[key][2], "g^", anneal_time_to_size[key][1], anneal_time_to_size[key][2], "y^")
plt.title("Score to time")
plt.xlabel("Time")
plt.ylabel("Score")
plt.show()
