#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

#include "/CPP/lib/olcConsoleGameEngine.h"
#include "/CPP/Data Structure/queue.h"
#include "/CPP/Data Structure/stack.h"


class sNode {
public:
	bool bObstacle = false;
	bool bVisited = false;
	float fGlobalGoal;
	float fLocalGoal;
	int x;
	int y;
	vector<sNode*> vecNeighbours;
	sNode* parent;
};

class PathFinder : public olcConsoleGameEngine {
public:
	PathFinder() {
		m_sAppName = L"Path Finder";
	} 

private:


	sNode* nodes = nullptr;
	int nMapWidth = 16;
	int nMapHeight = 16;

	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;

protected:
	virtual bool OnUserCreate() {
		// create a 2D array of nodes
		nodes = new sNode[nMapWidth * nMapHeight];
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				nodes[y * nMapWidth + x].x = x;
				nodes[y * nMapWidth + x].y = y;
				nodes[y * nMapWidth + x].bObstacle = false;
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].parent = nullptr;
			}
		}

		// create connections 
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				if (y > 0) // don't connect nodes on the top row to north 
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
				if (y < nMapHeight - 1) // don't connect nodes on the bottom row to south
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
				if (x > 0) // don't connect nodes on the left row to west 
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
				if (x < nMapWidth - 1) // don't connect nodes on the right row to east
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);

				//// diagonal connection 
				//if (y > 0 && x > 0)
				//	nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
				//if (y < nMapHeight - 1 && x > 0)
				//	nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
				//if (y > 0 && x < nMapWidth - 1)
				//	nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
				//if (y < nMapHeight - 1 && x < nMapWidth - 1)
				//	nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
			}
		}

		// initilize start and end markers
		nodeStart = &nodes[0];
		nodeEnd = &nodes[nMapWidth * (nMapHeight - 1) + (nMapWidth - 1)];

		return true;
	}

	virtual bool OnUserUpdate(float fElaosedTime) {
		// node size
		int nNodeSize = 9;
		int nNodeBorder = 2;

		// use integer division to nicely get cursor position in node space 
		int nSelectedNodeX = m_mousePosX / nNodeSize;
		int nSelectedNodeY = m_mousePosY / nNodeSize;

		if (m_mouse[0].bReleased) {
			if (nSelectedNodeX >= 0 && nSelectedNodeX < nMapWidth) {
				if (nSelectedNodeY >= 0 && nSelectedNodeY < nMapHeight) {

					if (m_keys[VK_SHIFT].bHeld)
						nodeStart = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
					else if (m_keys[VK_CONTROL].bHeld) 
						nodeEnd = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
					else 
						nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle = !nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle;

					//Solve_AStar();
					//Solve_BFS();
					//Solve_DFS();
				}
			}
		}

		if (m_keys['A'].bHeld)
			Solve_AStar();

		if (m_keys['B'].bHeld)
			Solve_BFS();

		if (m_keys['D'].bHeld)
			Solve_DFS();

		// clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		// draw connection line 
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {

				for (auto n : nodes[y * nMapWidth + x].vecNeighbours)
					DrawLine(x * nNodeSize + nNodeSize / 2, y * nNodeSize + nNodeSize / 2,
						n->x * nNodeSize + nNodeSize / 2, n->y * nNodeSize + nNodeSize / 2,
						PIXEL_SOLID, FG_BLUE);
			}
		}

		// draw nodes on top 
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				// top left corner to bottom right corner 
				Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, 
					(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, 
					PIXEL_HALF, nodes[y * nMapWidth + x].bObstacle? FG_WHITE : FG_BLUE);

				if (nodes[y * nMapWidth + x].bVisited) 
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, 
						(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
						PIXEL_SOLID, FG_BLUE);

				if (&nodes[y * nMapWidth + x] == nodeStart)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,
						(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
						PIXEL_SOLID, FG_GREEN);

				if (&nodes[y * nMapWidth + x] == nodeEnd)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,
						(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
						PIXEL_SOLID, FG_RED);
			}
		}

		// draw path - from end to start follow parent node trail 
		if (nodeEnd != nullptr) {
			sNode* p = nodeEnd;
			while (p->parent != nullptr) {

				DrawLine(p->x * nNodeSize + nNodeSize / 2, p->y * nNodeSize + nNodeSize / 2,
					p->parent->x * nNodeSize + nNodeSize / 2, p->parent->y * nNodeSize + nNodeSize / 2,
					PIXEL_SOLID, FG_YELLOW);

				p = p->parent;
			}
		}
		return true;
	}

	void Solve_AStar() {
		// reset all nodes in the graph
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y * nMapWidth + x].parent = nullptr;
			}
		}

		// calculate distance 
		auto distance = [](sNode* a, sNode* b) {
			return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};

		// calculate heuristic 
		auto heuristic = [distance](sNode* a, sNode* b) {
			return distance(a, b);
		};

		// set up starting conditions 
		sNode* nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

		// working queue
		list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) {

			// sort nodes in the list by global goal in ascending order
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; } );

			// don't process nodes that have been visited 
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			// break if there is no more nodes in the list 
			if (listNotTestedNodes.empty())
				break;

			// explore the node at the front of the list 
			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true;

			// check each neighbour 
			for (auto nodeNeighbour : nodeCurrent->vecNeighbours) {
				// add neighbout to the list if it is not visited and not an obsticle 
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// calculate the neighbouts potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

				// update neighbours stats and set neighbour's parent if application 
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) {
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;
					// update global goal of the neighbour 
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}

		}
		 
	}

	void Solve_Dikjstra() {
		// reset all nodes in the graph
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y * nMapWidth + x].parent = nullptr;
			}
		}

		// calculate distance 
		auto distance = [](sNode* a, sNode* b) {
			return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};

		// calculate heuristic 
		auto heuristic = [distance](sNode* a, sNode* b) {
			return distance(a, b);
		};

		// set up starting conditions 
		sNode* nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

		// working queue
		list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) {

			// sort nodes in the list by global goal in ascending order
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

			// don't process nodes that have been visited 
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			// break if there is no more nodes in the list 
			if (listNotTestedNodes.empty())
				break;

			// explore the node at the front of the list 
			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true;

			// check each neighbour 
			for (auto nodeNeighbour : nodeCurrent->vecNeighbours) {
				// add neighbout to the list if it is not visited and not an obsticle 
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// calculate the neighbouts potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

				// update neighbours stats and set neighbour's parent if application 
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) {
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;
					// update global goal of the neighbour 
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}

		}

	}

	void Solve_BFS() {

		// reset all nodes in the graph
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y * nMapWidth + x].parent = nullptr;
			}
		}

		// working queue
		Queue<sNode*> q;
		sNode* curr = nodeStart;
		curr->bVisited = true;
		q.enqueue(curr);

		while (!q.isEmpty()) {

			curr = q.dequeue();

			if (curr == nodeEnd)
				break;

			for (auto neighbour : curr->vecNeighbours) {

				if (!neighbour->bVisited && !neighbour->bObstacle) {
					q.enqueue(neighbour);
					neighbour->bVisited = true;
					if (neighbour != nodeStart)
						neighbour->parent = curr;
				}
							
			}
		}

	}

	void Solve_DFS() {

		// reset all nodes in the graph
		for (int x = 0; x < nMapWidth; x++) {
			for (int y = 0; y < nMapHeight; y++) {
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y * nMapWidth + x].parent = nullptr;
			}
		}

		// working queue
		Stack<sNode*> s;
		sNode* curr = nodeStart;
		curr->bVisited = true;
		s.push(curr);

		while (!s.isEmpty()) {

			curr = s.pop();

			if (curr == nodeEnd)
				break;

			for (auto neighbour : curr->vecNeighbours) {

				if (!neighbour->bVisited && !neighbour->bObstacle) {
					s.push(neighbour);
					neighbour->bVisited = true;
					if (neighbour != nodeStart)
						neighbour->parent = curr;
				}

			}
		}

	}
};

int main() {
	PathFinder pathfinder;

	pathfinder.ConstructConsole(160, 160, 6, 6);
	pathfinder.Start();

	return 0;
}