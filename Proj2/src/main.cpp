#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stack>
#include <vector>

#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define UNVISITED BLACK
#define VISITED 4

int max_recursive(int cur);

std::vector<std::vector<int>> normal, transposed;

template <typename T>
T spop(std::stack<T> stack)
{
	T val = stack.top();
	stack.pop();
	return val;
}

void print_graph(std::vector<std::vector<int>> graph)
{
	for (int i = 0; i < (int)graph.size(); i++)
	{
		printf("%d: ", i+1);
		for (int j = 0; j < (int)graph[i].size(); j++)
			printf("%d ", graph[i][j]+1);
		printf("\n");
	}
}

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

	int max = 0;
	for (int i = 0; i < node_count; i++)
		max = MAX(max, max_recursive(i));

	printf("%d\n", max);

	return 0;
}

int max_recursive(int cur)
{
	int conn_count = normal[cur].size();
	int max = 0;

	if (!conn_count)
		return 0;

	for (int i = 1; i <= conn_count; i++)
		max = MAX(max, max_recursive(graph[cur][i]));

	return max + 1;
}


std::stack<int> dfs()
{
	std::stack<int> ret_stack;
	std::stack<int> iter_stack;
	std::vector<char> state = std::vector<char>(normal.size(), WHITE);

	for (int v = 0; v < normal.size(); v++)
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
				continue;
		}
	}

	std::vector<std::vector<int>> sccs;
	std::vector<int> scc_map = std::vector<int>(normal.size());

	//from now on is second dfs run
	//reuse state for visited unvisited
	//reuse iter_stack for this dfs
	while (!ret_stack.empty())
	{
		int v = spop(ret_stack);
		if (state[v] == VISITED)
			continue;

		state[v] = VISITED;
		bool is_scc = true;

		for (int cur : transposed[v])
		{
			if (state[cur] == VISITED)
				continue;

			iter_stack.push(cur);
			is_scc = false;
		}

		if (!is_scc)
			continue;

		std::vector<int> scc = std::vector<int>(iter_stack.size());
		while (!iter_stack.empty())
		{
			int popped = spop(iter_stack);
			scc.push_back(popped);
			scc_map[popped] = sccs.size();
		}

		sccs.push_back(scc);
	}

	//now must convert sccs to single nodes
	//sccs holds all scc node arrays
	//both stacks are empty
	state.clear(); //free some more memory
	transposed.clear(); //moar memory

	std::vector<std::vector<int>> dag = std::vector<std::vector<int>>(sccs.size());

	//TODO: Replace with better
	for (auto scc : sccs)
	{
		std::vector<int> conns;

		for (int node : scc)
		{
			for (int conn : normal[node])
			{
				int translated = scc_map[conn];
				if (!contains(conns, translated))
					conns.push_back(translated);
			}
		}

		dag.push_back(conns);
	}

	//now the clean graph should be in dag
	
}
