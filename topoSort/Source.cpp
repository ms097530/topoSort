#include <stdexcept>
#include <stack>
#include <vector>
#include <iostream>

using namespace std;


//assume each node has a Mark attribute
enum Mark { NONE, TEMP, PERM };	//used to determine if there is a cycle (not DAG if there is a cycle)

void visit(int index, vector<int> adj[], Mark * visitArr, stack<int>& storage)
// Perform DFS starting from index. If this node is marked TEMP there is a cycle - cancel operation (throw exception)
{
	if (visitArr[index] == TEMP)
		throw runtime_error("Cycle detected.");
	visitArr[index] = TEMP;

	for (std::vector<int>::iterator it = adj[index].begin(); it != adj[index].end(); ++it)	//recursive call to visit
	{
		if (visitArr[*it] != PERM)
			visit(*it, adj, visitArr, storage);
	}

	visitArr[index] = PERM;
	//add node to storage stack
	storage.push(index);
}

int* topoSort(int V, vector<int> adj[])
// V = num vertices
// adj = adjacency list (in the form of array of vectors)
// visit throws exception if cycle is detected
{

	int* arr = new int[V];			//array to store results
	stack<int> myStack;
	Mark* visitedArr = new Mark[V]{ NONE };		//array to store whether or not a node has been visited
	
	try
	{
		for (int i = 0; i < V; ++i)
			//loop to initiate DFS from all nodes
		{
			if (visitedArr[i] != PERM)		//only starts DFS from nodes that haven't already been visited at some point
			{								//visit will check for loops (TEMP marked nodes getting visited) so don't need to check here
				visit(i, adj, visitedArr, myStack);		//pass in index of node to start DFS from and the array to store nodes
			}
		}
	}
	catch (const exception& e)
	{
		delete[] visitedArr;
		delete[] arr;
		e;				//"use" of e to get rid of compiler warning
		throw;
	}

	for (int i = 0; i < V; ++i)
	{
		arr[i] = myStack.top();
		myStack.pop();
	}

	delete[] visitedArr;
	return arr;
}

int main()
{
	const int SIZE = 6;
	vector<int> adjList[SIZE];
	//adjList[0].push_back(1);
	//adjList[1].push_back(2);
	//adjList[2].push_back(1);
	//adjList[2].push_back(3);
	//adjList[3].push_back(4);
	//adjList[3].push_back(5);
	//adjList[2].push_back(5);
	//adjList[4].push_back(3);
	//adjList[0].push_back(0);

	adjList[5].push_back(0);
	adjList[1].push_back(3);
	adjList[1].push_back(5);
	adjList[0].push_back(5);

	try
	{
		int* arr = topoSort(SIZE, adjList);

		for (int i = 0; i < SIZE; ++i)
			cout << arr[i] << " ";

		cout << endl;

		delete[] arr;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}