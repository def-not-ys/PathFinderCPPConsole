# PathFinderCPPConsole
A C++ console app implementing path finding algorithms   

A path finding app built using [olcConsoleGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine). *courtesy to OneLoneCoder*

This app explores three different path finding algorithms (**A star**, **Breadth First Search**, **Depth First Search**) to find *a* path from start node(green) to end node(red) in a 2D grid.

- Holding down the **shift** key can select the start node position by clicking the node.
- Holding down the **control** key can select the end node position by clicking the node.
- Obstacles (grey) can be added or removed by clicking each node.

To start the algorithem:
- Press **'A'** to start **A star** 
- Press **'B'** to start **BFS** 
- Press **'D'** to start **DFS** 

The path found by the algorithm will be shown as a path (yellow) on the grid.
The shade of the node shows if the node has been explored by the algorithm: 
- bright blue: explored
- dark blue: not explored

Below are some images of the solved configuration. 

### A star
A star guarantees to find the shortest path from star node to end node
![a_star](/images/A_star_solved.JPG)

### BFS
BFS finds *a* path from start node to end node with a wide range of exploration  
![bfs](/images/BFS_solved.JPG)

### BFS
DFS finds *a* path from start node to end node but explores less nodes comparing to BFS
![dfs](/images/DFS_solved.JPG)
