#pragma once

namespace Elite 
{
	template <class T_NodeType, class T_ConnectionType>
	class BFS
	{
	public:
		BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph);

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);
	private:
		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
	};

	template <class T_NodeType, class T_ConnectionType>
	BFS<T_NodeType, T_ConnectionType>::BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph)
		: m_pGraph(pGraph)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> BFS<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode)
	{
		std::queue<T_NodeType*>openList; //Nodes we need to check
		std::map<T_NodeType*, T_NodeType*>closedList; // already cheked Nodes and History of 

		openList.push(pStartNode);

		while (!openList.empty())
		{
			T_NodeType* pCurrentNode = openList.front(); // taking the node from the openlist
			openList.pop(); // removing that node

			if (pCurrentNode == pDestinationNode)
			{
				break; // found end node -> stop this loop
			}
			for (auto con : m_pGraph->GetNodeConnections(pCurrentNode->GetIndex()))
			{
				T_NodeType* pNextNode = m_pGraph->GetNode(con->GetTo());
				if (closedList.find(pNextNode) == closedList.end())
				{
					// we did not find this node in the closedList
					openList.push(pNextNode);
					closedList[pNextNode] = pCurrentNode;
				}
			}
		}
		//start tracking back
		vector<T_NodeType*> path;
		T_NodeType* pCurrentNode = pDestinationNode;
		
		while (pCurrentNode != pStartNode)
		{
			path.push_back(pCurrentNode);
			pCurrentNode = closedList[pCurrentNode];
		}
		//path.push_back(pStartNode);
		// reversing the path
		std::reverse(path.begin(), path.end());

		return path;
	}
}

