from pulp import *
parts = input().split()
t, p, mpd = int(parts[0]), int(parts[1]), int(parts[2])
toys : 'list[tuple[LpVariable, int, int, list[int]]]' = []
packs : 'list[tuple[LpVariable, int]]' = []
prob = LpProblem(sense = const.LpMaximize)
for iterc in range(t):
	parts = input().split()
	toys.append((LpVariable("Toy#"+str(iterc+1), 0, cat = const.LpInteger), int(parts[0]), int(parts[1]), []))
for iterc in range(p):
	parts = input().split()
	packs.append((LpVariable("Pack#"+str(iterc+1), 0, cat = const.LpInteger), int(parts[3])))
	toys[int(parts[0])-1][3].append(iterc)
	toys[int(parts[1])-1][3].append(iterc)
	toys[int(parts[2])-1][3].append(iterc)
prob += (lpSum([var[1] * var[0] for var in toys] + [pack[1] * pack[0] for pack in packs]), "Objective")
for i in range(t):
	prob += (lpSum([toys[i][0]] + [packs[varIdx][0] for varIdx in toys[i][3]]) <= toys[i][2], "Restriction#"+str(i+1))
prob += (lpSum([var[0] for var in toys] + [3 * pack[0] for pack in packs]) <= mpd, "TotalRestriction")
prob.solve(GLPK(msg=0))
print(int(value(prob.objective)))
