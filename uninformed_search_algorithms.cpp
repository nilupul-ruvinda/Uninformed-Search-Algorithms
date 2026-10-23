#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>

using namespace std;

struct measurements
{
	double executionTime;
	int nodesExpanded;
	int stackSize;
	bool goalFound;
};

vector<vector<int>> generateLargeGraph(int total_nodes, int branches)
{
	vector<vector<int>> graph(total_nodes);

	for (int i = 0; i < total_nodes; i++)
	{
		for (int j = 1; j <= branches; j++)
		{
			int child = branches * i + j;

			if (child < total_nodes)
			{
				graph[i].push_back(child);
			}
			else
			{
				break;
			}
		}
	}
	return graph;
}

void BFS(const vector<vector<int>> &graph, int start, int goal, measurements &m)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	m.nodesExpanded = 0;
	m.stackSize = 0;
	m.goalFound = false;

	vector<bool> visited(graph.size(), false);
	vector<int> parent(graph.size(), -1);
	queue<int> q;
	q.push(start);
	visited[start] = true;
	while (!q.empty())
	{
		if ((int)q.size() > m.stackSize)
		{
			m.stackSize = (int)q.size();
		}

		int current = q.front();
		q.pop();

		m.nodesExpanded++;

		cout << "Expanded: " << int(0 + current) << endl;
		if (current == goal)
		{
			m.goalFound = true;
			break;
		}

		for (int next : graph[current])
		{
			if (!visited[next])
			{
				visited[next] = true;
				parent[next] = current;
				q.push(next);
			}
		}
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	m.executionTime = std::chrono::duration<double, milli>(endTime - startTime).count();

	// Print path
	vector<int> path;
	for (int v = goal; v != -1; v = parent[v])
		path.push_back(v);
	cout << "\nPath: ";
	for (int i = path.size() - 1; i >= 0; i--)
		cout << int(0 + path[i]) << " ";
	cout << endl;
}

void DFS(const vector<vector<int>> &graph, int start, int goal, measurements &m)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	m.nodesExpanded = 0;
	m.stackSize = 0;
	m.goalFound = false;

	vector<bool> visited(graph.size(), false);
	vector<int> parent(graph.size(), -1);
	stack<int> s;
	s.push(start);
	while (!s.empty())
	{
		if ((int)s.size() > m.stackSize)
		{
			m.stackSize = (int)s.size();
		}

		int current = s.top();
		s.pop();

		if (visited[current])
			continue;
		visited[current] = true;
		m.nodesExpanded++;

		cout << "Expanded: " << int(0 + current) << endl;
		if (current == goal)
		{
			m.goalFound = true;
			break;
		}

		// Add children to stack
		// Reverse order so that leftmost child is processed first
		for (int i = graph[current].size() - 1; i >= 0; i--)
		{
			int next = graph[current][i];
			if (!visited[next])
			{
				parent[next] = current;
				s.push(next);
			}
		}
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	m.executionTime = std::chrono::duration<double, milli>(endTime - startTime).count();

	// Print path
	vector<int> path;
	for (int v = goal; v != -1; v = parent[v])
		path.push_back(v);
	cout << "\nPath: ";
	for (int i = path.size() - 1; i >= 0; i--)
		cout << int(0 + path[i]) << " ";
	cout << endl;
}

bool DLS(const vector<vector<int>> &graph,
		 int current,
		 int goal,
		 int depthLimit,
		 vector<bool> &visited,
		 vector<int> &path,
		 measurements &m,
		 int currentDepth = 0)
{
	if (currentDepth + 1 > m.stackSize)
	{
		m.stackSize = currentDepth + 1;
	}

	cout << "Expanded: "
		 << int(0 + current)
		 << " (depth " << depthLimit << ")" << endl;
	m.nodesExpanded++;

	if (current == goal)
	{
		m.goalFound = true;
		path.push_back(current);
		return true;
	}
	if (depthLimit == 0)
		return false;

	visited[current] = true;
	for (int next : graph[current])

	{
		if (!visited[next])

		{
			if (DLS(graph,
					next,
					goal,
					depthLimit - 1,
					visited,
					path, m, currentDepth + 1))

			{
				path.push_back(current);
				return true;
			}
		}
	}
	visited[current] = false;
	return false;
}

bool IDS(const vector<vector<int>> &graph,
		 int start,
		 int goal,
		 int maxDepth, measurements &m)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	m.nodesExpanded = 0;
	m.stackSize = 0;
	m.goalFound = false;

	bool success = false;

	for (int depth = 0; depth <= maxDepth; depth++)

	{
		cout << "\nSearching with depth limit = " << depth << endl;
		vector<bool> visited(graph.size(), false);
		vector<int> path;

		measurements iterationMetrics;
		iterationMetrics.nodesExpanded = 0;
		iterationMetrics.stackSize = 0;
		iterationMetrics.goalFound = false;

		if (DLS(graph,
				start,
				goal,
				depth,
				visited,
				path, iterationMetrics))

		{
			m.nodesExpanded += iterationMetrics.nodesExpanded;
			if (iterationMetrics.stackSize > m.stackSize)
			{
				m.stackSize = iterationMetrics.stackSize;
			}
			m.goalFound = true;
			success = true;

			cout << "\nPath: ";
			for (int i = path.size() - 1; i >= 0; i--)
				cout << int(0 + path[i]) << " ";
			cout << endl;
			break;
		}
		m.nodesExpanded += iterationMetrics.nodesExpanded;
		if (iterationMetrics.stackSize > m.stackSize)
		{
			m.stackSize = iterationMetrics.stackSize;
		}
	}
	if (!success)
	{
		cout << "\nGoal not found." << endl;
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	m.executionTime = std::chrono::duration<double, milli>(endTime - startTime).count();
	return success;
}

int main()
{
	freopen("exp_4_imbalanced.txt", "w", stdout);
	vector<vector<int>> graph = generateLargeGraph(1000, 2);

	measurements bfs;
	measurements dfs;
	measurements dls;
	measurements ids;

	cout << "\nBFS" << endl;
	BFS(graph, 0, 900, bfs);

	cout << "\nExecution Time: " << bfs.executionTime << " ms" << endl;
	cout << "Nodes Expanded: " << bfs.nodesExpanded << endl;
	cout << "Maximum Queue Size: " << bfs.stackSize << endl;
	cout << "Goal Found: " << (bfs.goalFound ? "Yes" : "No") << endl;

	cout << "\nDFS" << endl;
	DFS(graph, 0, 900, dfs);

	cout << "\nExecution Time: " << dfs.executionTime << " ms" << endl;
	cout << "Nodes Expanded: " << dfs.nodesExpanded << endl;
	cout << "Maximum Stack Size: " << dfs.stackSize << endl;
	cout << "Goal Found: " << (dfs.goalFound ? "Yes" : "No") << endl;

	int start = 0;
	int goal = 900;
	int depthLimit = 10;

	vector<bool> visited(graph.size(), false);
	vector<int> path;

	cout << "\nDLS" << endl;
	dls.nodesExpanded = 0;
	dls.stackSize = 0;
	dls.goalFound = false;

	auto startTime = std::chrono::high_resolution_clock::now();
	bool found = DLS(graph,
					 start,
					 goal,
					 depthLimit,
					 visited,
					 path, dls);
	auto endTime = std::chrono::high_resolution_clock::now();
	dls.executionTime = std::chrono::duration<double, milli>(endTime - startTime).count();

	if (found)
	{
		cout << "\nPath: ";
		for (int i = path.size() - 1; i >= 0; i--)
			cout << int(0 + path[i]) << " ";
		cout << endl;
	}
	else
	{
		cout << "\nGoal not found within depth limit." << endl;
	}

	cout << "\nExecution Time: " << dls.executionTime << " ms" << endl;
	cout << "Nodes Expanded: " << dls.nodesExpanded << endl;
	cout << "Maximum Stack Size: " << dls.stackSize << endl;
	cout << "Goal Found: " << (dls.goalFound ? "Yes" : "No") << endl;

	cout << "\nIDS" << endl;
	IDS(graph, start, goal, 10, ids);

	cout << "\nExecution Time: " << ids.executionTime << " ms" << endl;
	cout << "Nodes Expanded: " << ids.nodesExpanded << endl;
	cout << "Maximum Stack Size: " << ids.stackSize << endl;
	cout << "Goal Found: " << (ids.goalFound ? "Yes" : "No") << endl;

	return 0;
}