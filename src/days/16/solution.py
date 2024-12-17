import heapq
from collections import defaultdict

def Manhattan(z: complex, w: complex):
	return int(abs(z.real - w.real) + abs(z.imag - w.imag))

class Grid():
	def __init__(self, empty: str | int = '.'):
		self.empty = empty
		self.grid: dict[complex, str | int] = dict()

	def populate2DList(self, listOfLists: list[list[int]] | list[str]):
		x = 0
		y = 0
		for y, row in enumerate(listOfLists):
			for x, col in enumerate(row):
				self.grid[x + y*1j] = col
		self.lenx = x + 1
		self.leny = y + 1

	def toInts(self):
		for key in self.grid.keys():
			self.grid[key] = int(self.grid[key])
	
	def neighbours(self, location: complex):
		dels = [-1, 1, 1j, -1j]
		return [location + d for d in dels if self[location + d] != self.empty]

	def neighboursDiag(self, location: complex):
		dels = [-1, 1, 1j, -1j, 1+1j, 1-1j, -1-1j, -1+1j]
		return [location + d for d in dels if self[location + d] != self.empty]

	def cells(self):
		return [k for k in self.grid.keys() if self.grid[k] != self.empty]

	def __getitem__(self, key: complex):
		return self.grid.get(key, self.empty)

	def __setitem__(self, key: complex, value: int):
		self.grid[key] = value

def Astar(grid, start, dir):
    deltas = [1, 1j, -1, -1j]
    openSet = []
    bestScore = None
    bestDir = None
    heapq.heappush(openSet, (0, (start, dir)))
    camefrom = defaultdict(list)
    gScore = defaultdict(lambda: 1000000)
    gScore[(start, dir)] = 0

    while openSet:
        current, dir = heapq.heappop(openSet)[1]
        current = complex(current)
        if current == end:
                if bestScore is None: 
                      bestScore = gScore[(current, dir)]
                      bestDir = dir
                

        for d in [(4 + (dir - 1) % 4) % 4, dir, (4 + (dir + 1) % 4) % 4]:
            tentative_gScore = gScore[(current, dir)] + 1
            neighbour = deltas[d] + current
            if d != dir:
                  neighbour = current
                  tentative_gScore += 999
            if grid[neighbour] == '#':
                continue
            if tentative_gScore <= gScore[(neighbour, d)]:
                if tentative_gScore == gScore[(neighbour, d)]:
                    camefrom[(neighbour, d)].append((current, dir))
                else:
                    camefrom[(neighbour, d)] = [(current, dir)]
                    gScore[(neighbour, d)] = tentative_gScore
                fScore = tentative_gScore + Manhattan(neighbour, end)
                if bestScore is None or fScore <= bestScore:
                  heapq.heappush(openSet, (fScore, (str(neighbour), d)))

    fancy = set()
    simple = set()
    simple.add(start)
    simple.add(end)
    def recurse(current, dir, camefrom):
      if (current, dir) in camefrom.keys():
        fancy.add((current, dir))
        simple.add(current)
        for el in camefrom[(current, dir)]:
           if (el[0], el[1]) not in fancy:
             recurse(el[0], el[1], camefrom)

    recurse(end, bestDir, camefrom)
    print(len(simple))

    return gScore[(end, 3)]

with open("input") as f:
    lines = f.read().rstrip().split('\n')
grid = Grid('#')
grid.populate2DList(lines)
start = [k for k in grid.cells() if grid[k] == 'S'][0]
end = [k for k in grid.cells() if grid[k] == 'E'][0]
print(Astar(grid, start, 0))
