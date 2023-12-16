#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wunknown-pragmas"

/*list connections per person*/
int** connections;

int main()
{
	int i;
	int node_count, edge_count;
	scanf("%d %d", &node_count, &edge_count);

	int *edges;
	connections = malloc(node_count * sizeof(int*));
	edges = malloc(edge_count * 2 * sizeof(int*));

	while (edge_count--)
	{
		int start, end;
		scanf("%d %d", &start, &end);

		/*store edge*/
		edges[edge_count >> 1] = start;
		edges[(edge_count >> 1) + 1] = end;

		/*store count in connections[i]*/
		(*((int*)&connections[start]))++;
	}

	for (i = 0; i < node_count; i++)
		/*after read connections[i] = malloc((connections[i] + 1) * sizeof(int)); //null terminated*/
		connections[i] = malloc((*((int*)&connections[i]) + 1) * sizeof(int));

	return 0;
}
