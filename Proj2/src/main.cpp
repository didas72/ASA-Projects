#include <stdio.h>
#include <stack>
#include <vector>

#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define UNVISITED BLACK
#define WIP 3
#define VISITED 4

std::vector<std::vector<int>> normal, transposed;

int compute_max();

template <typename T>
inline bool contains(std::vector<T> vec, T element)
{
	for (T cur : vec)
		if (cur == element)
			return true;

	return false;
}


int main()
{
	int node_count, edge_count;
	scanf("%d %d", &node_count, &edge_count);

	normal = transposed = std::vector<std::vector<int>>(node_count);

	//Read stuff
	for (int i = 0; i < edge_count; i++)
	{
		int start, end;
		scanf("%d %d", &start, &end);
		normal[start-1].push_back(end-1);
		transposed[end-1].push_back(start-1);
	}

	//Compute stuff
	int max = compute_max();
	printf("%d\n", max);
	return 0;
}

int compute_max()
{
	std::stack<int> ret_stack;
	std::stack<int> iter_stack;
	std::vector<char> state = std::vector<char>(normal.size(), WHITE);

	for (int v = 0; v < (int)normal.size(); v++)
	{
		if (state[v] != WHITE)
			continue;

		iter_stack.push(v);

		while (!iter_stack.empty())
		{
			int cur = iter_stack.top();

			if (state[cur] == WHITE)
			{
				state[cur] = GRAY;
				for (int next : normal[cur])
					if (state[next] == WHITE)
						iter_stack.push(next);
			}
			else if (state[cur] == GRAY)
			{
				iter_stack.pop(); //pop self
				state[cur] = BLACK;
				ret_stack.push(cur);
			}
			else //black, should never reach here
				iter_stack.pop();
		}
	}

	printf("First DFS with %ld elements.\n", ret_stack.size());

	std::vector<std::vector<int>> sccs;
	std::vector<int> scc_map = std::vector<int>(normal.size());

	//from now on is second dfs run
	//reuse state for visited unvisited
	//reuse iter_stack for this dfs

	while (!ret_stack.empty() || !iter_stack.empty())
	{
		int v = ret_stack.top();
		ret_stack.pop();
		std::vector<int> scc;

		printf("Checking %d with status %d. (new scc)\n", v, state[v]);

		if (state[v] == VISITED)
			continue;

		state[v] = VISITED;
		iter_stack.push(v);
		scc.push_back(v);
		scc_map[v] = sccs.size();
		printf("Pushing connection %d.\n", v);

		while (!iter_stack.empty())
		{
			int neighbour = iter_stack.top();
			iter_stack.pop();

			for (int cur : transposed[neighbour])
			{
				if (state[cur] != UNVISITED)
					continue;

				printf("Pushing connection %d.\n", cur);
				state[cur] = VISITED;
				iter_stack.push(cur);
				scc.push_back(cur);
				scc_map[cur] = sccs.size();
			}
		}

		sccs.push_back(scc);
	}

	printf("Second dfs with %ld sccs.\n", sccs.size());

	//now must convert sccs to single nodes
	//sccs holds all scc node arrays
	//both stacks are empty
	state.clear(); //free some more memory
	transposed.clear(); //moar memory

	std::vector<std::vector<int>> dag = std::vector<std::vector<int>>(sccs.size());

	for (int v = 0; v < (int)normal.size(); v++)
	{
		for (auto n : normal[v])
		{
			int vt = scc_map[v], nt = scc_map[n];
			printf("%d->%d %d\n", n, nt, vt);

			if (vt != nt)
				dag[vt].push_back(nt);
		}
	}

	printf("New\n");
	for (auto j : dag)
	{
		for (auto k : j)
			printf("%d ", k);
		printf(".\n");
	}

	//TODO: Replace with better
	/*for (auto scc : sccs)
	{
		std::vector<int> conns;

		printf("New scc.\n");

		for (int node : scc)
		{
			for (int conn : normal[node])
			{
				int translated = scc_map[conn];
				if (!contains(conns, translated))
				{
					printf("Adding %d (%d translated).\n", conn, translated);
					conns.push_back(translated);
				}
			}
		}

		dag.push_back(conns);
	}*/

	std::vector<int> max_dist = std::vector<int>(dag.size());
	int abs_max = 0;

	//now the clean graph should be in dag
	for (int i = dag.size() - 1; i >= 0; i--)
	{
		int cur_max = 0;

		for (int next : dag[i])
		{
			int local_max = max_dist[next] + 1;
			cur_max = MAX(cur_max, local_max);
		}

		max_dist[i] = cur_max;
		abs_max = MAX(abs_max, cur_max);
	}

	return abs_max;
}
