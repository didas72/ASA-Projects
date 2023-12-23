from pulp import *
parts = input().split()
t, p, mpd = int(parts[0]), int(parts[1]), int(parts[2])
vars : list[tuple[LpVariable, float, int, list[int]]] = []
packs : list[tuple[LpVariable, float]] = []
prob = LpProblem(sense = const.LpMaximize)
for iter in range(t):
	parts = input().split()
	vars.append((LpVariable("Toy#"+str(iter+1), 0, cat = const.LpInteger), float(parts[0]), int(parts[1]), []))
for iter in range(p):
	parts = input().split()
	packs.append((LpVariable("Pack#"+str(iter+1), 0, cat = const.LpInteger), float(parts[3])))
	vars[int(parts[0])-1][3].append(iter)
	vars[int(parts[1])-1][3].append(iter)
	vars[int(parts[2])-1][3].append(iter)
prob += (lpSum([var[1] * var[0] for var in vars] + [pack[1] * pack[0] for pack in packs]), "Objective")
for i in range(len(vars)):
	prob += (lpSum([vars[i][0]] + [packs[varIdx][0] for varIdx in vars[i][3]]) <= vars[i][2], "Restriction#"+str(i+1))
prob += (lpSum([var[0] for var in vars] + [3 * pack[0] for pack in packs]) <= mpd, "TotalRestriction")
prob.solve(GLPK(msg=False))
print(value(prob.objective))
