Programming Assignment 5
========================

_Your description here_

Each edge for the graph was a struct that contained the starting node, streetname, distance (weight), and ending node. (url not used)

My adjacency list I used was an unordered_map<int,vector<struct edge>>. The int used as the key was the starting node for 
a street segment. The vector contained all the egdes that shared that starting node. 
When testing test5b, it took over 70 seconds. One of the main reasons is because I used a vector within the map for my adjancency list. 
When recostructing the path, I had to look up all of the streetnames and distances of the street segments, which took linear time.
If I redid this asssignment I would have used a hash table because it has constant look up time. 

In my Dijkstra's algorithm, I used a priority queue to store all of the nodes that had not been visited yet.
I used the style from class that stores each node as a struct with an overloaded comparison operator. 
I also used an unordered_set to store all of the nodes that have been visted.

Because my path reconstruction started at target to the source, I added each direction to temporary stack which has
Last in First Out capabilities. Then I popped the top off and added it the steps vector so that the first direction
was now first. This processs also contributed to my test5b taking so long.

_Grader comments here_
