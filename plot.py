import matplotlib.pyplot as plt
import math
import sys

data = {}
xticks=set()

for line in sys.stdin:
    name, n, _, time, _ = line.split()
    n = int(n)
    time = float(time)
    if name not in data:
        data[name] = {}
    if n not in data[name]:
        data[name][n] = []
    data[name][n].append(time)
    xticks.add(n)

def average(lst):
    return sum(lst) / len(lst)

def percentile_90(lst):
    sorted_lst = sorted(lst)
    index = math.ceil((90/100) * len(sorted_lst))
    return sorted_lst[index-1]

cmap = plt.cm.get_cmap('hsv', len(data) + 1)

plt.figure(figsize=(13, 6), num='Час виконання різних алгоритмів').subplots_adjust(left=0.06, right=0.988, top=0.975, bottom=0.098)
plt.xscale('log')
plt.yscale('log')

for i, name in enumerate(data):
    color = cmap(i)
    xs = list(data[name].keys())
    ys_avg = [average(data[name][x]) for x in xs]
    ys_90_perc = [percentile_90(data[name][x]) for x in xs]
    ys_max = [max(data[name][x]) for x in xs]
    plt.plot(xs, ys_avg, label=f'{name} (середнє значення)', linestyle='-', marker='.', color=color)
    plt.plot(xs, ys_90_perc, label=f'{name} (90-й процентиль)', linestyle='--', marker='.', color=color)
    plt.plot(xs, ys_max, label=f'{name} (максимальне значення)', linestyle=':', marker='.', color=color)

plt.xlabel('Кількість вершин графа')
plt.ylabel('Час (секунди)')
xticks=list(xticks)
plt.xticks(xticks, xticks)
plt.gca().format_coord = lambda x, y: 'x={:g}, y={:g}'.format(x, y)
plt.legend(loc='lower right')
plt.rcParams['savefig.dpi'] = 200
plt.show()
