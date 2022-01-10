#pragma once

namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		vector<T_NodeType*> path;
		vector<NodeRecord> openList;
		vector<NodeRecord> closedList;
		NodeRecord currentRecord;

		NodeRecord startRecord;
	
		startRecord.pNode = pStartNode;
		startRecord.pConnection = nullptr;
		startRecord.estimatedTotalCost = GetHeuristicCost(pStartNode,pGoalNode);

		openList.push_back(startRecord);
		
		currentRecord = openList.front();
		while (!openList.empty())
		{
			
				 //std::min_element || find || find_if
			NodeRecord rec =*std::min_element(openList.begin(), openList.end(), [](NodeRecord& a , NodeRecord& b)
				{
					return a.estimatedTotalCost < b.estimatedTotalCost;
				});
			currentRecord = rec;
			
			if (currentRecord.pNode->GetIndex() == pGoalNode->GetIndex())
			{
				break;
			}
			else
			{
				for (auto con : m_pGraph->GetNodeConnections(currentRecord.pNode))
				{
					bool IsOnClosed{ false };
					//currentRecord.pConnection = con;
					T_NodeType* pNextNode = m_pGraph->GetNode(con->GetTo());
					// calculated g-Cost ( cost from start to current node )
					auto gCost = currentRecord.costSoFar + con->GetCost();
					// 2.D

					for (int i = 0; i < closedList.size(); i++)
					{
						// check if any of those connections is already on the closed list 
						if (closedList[i].pNode == pNextNode)
						{
							IsOnClosed = true;
							// check the closedList(pNode) and if a connection to the connections EndNode already exist in the closed list
							if (closedList[i].pConnection != nullptr)
							{
								if (closedList[i].pConnection->GetTo() == con->GetTo())
								{

									// if existing connection is cheaper continue 
									if (gCost >= closedList[i].costSoFar)
									{
										continue;
									}
									// else remove it from the closed list so we can add the cheaper one later
									else
									{
										closedList.erase(std::remove(closedList.begin(), closedList.end(), closedList[i]), closedList.end());

									}
								}
							}
							
						}

					}
					// 2.E
					// if 2.D check fails (node is not on closed) then check if any of those connections exist on the openList
					if (!IsOnClosed)
					{
						for (int i = 0; i < openList.size(); i++)
						{
							// if it exist on the openList
							if (openList[i].pNode == pNextNode)
							{
								// check if a connection to the EndNode already exist in openlist
								if (openList[i].pConnection->GetTo() == con->GetTo())
								{
									// check if the existing connection is cheaper , if so continue
									if (gCost >= openList[i].costSoFar)
									{
										continue;
									}
									// else remove it from the openlist so it can be replaced
									else
									{
										openList.erase(std::remove(openList.begin(), openList.end(), openList[i]), openList.end());
									}
								}
							}
						}
					}

					NodeRecord newRec;
					newRec.pNode = pNextNode;
					newRec.pConnection = con;
					newRec.costSoFar = gCost;
					newRec.estimatedTotalCost = gCost + GetHeuristicCost(newRec.pNode, pGoalNode) ;
					// 2.F

					openList.push_back(newRec);
					////2.G
					


				}
				openList.erase(std::remove(openList.begin(), openList.end(), currentRecord), openList.end());

				closedList.push_back(currentRecord);
			}
		
		
			//openList.clear();
		}
	
		
		T_NodeType* pCurrentNode = currentRecord.pNode;
		
		while (pCurrentNode != pStartNode)
		{
			
			
			path.push_back(pCurrentNode);
			auto inClosed = std::find_if(closedList.begin(), closedList.end(), [currentRecord](NodeRecord& a)
				{
					return a.pNode->GetIndex() == currentRecord.pConnection->GetFrom();
				}
			);
			if (inClosed->pNode != nullptr)
			{
				currentRecord = *inClosed;
			}
			pCurrentNode = currentRecord.pNode;
			
				

			
		}
		path.push_back(pStartNode);
		// reversing the path
		std::reverse(path.begin(), path.end());

		
		return path;
		//return std::vector<T_NodeType*>{};
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}