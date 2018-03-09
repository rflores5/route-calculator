Calculating travel routes in South Bend using Dijkstra's Algorithm

This program is from the command line through the route.cpp file. When it is run it takes a starting and ending address in South Bend as input and outputs a street-by-street driving directions to get to the ending address.
 
For example:

$ ./route 
Starting address: 620 W Washington St 
Ending address: 4477 Progress Dr 
0.250207 miles on W Washington St 
0.837454 miles on Laporte Ave 
0.0397415 miles on Wilber St 
1.76114 miles on Lincoln Way W 
0.0729842 miles on Maplewood Ave 
0.400587 miles on Commerce Dr 
0.0442461 miles on Progress Dr

All of the street information that is used can be found in the southbend.v2.map file. The street infromation is then translated to edges in a graph represented in an adjacency list. Dijkstra's algorithm is used on this street graph to find the shortest path between the start and end address. 

Note: The files measure.c, random.txt, and test1/2/3/4/5a/5b are all used for testing purposes and are not necessary in running this program. 

Each edge for the graph was a struct that contained the starting node, streetname, distance (weight), and ending node. (url not used)

Below are some notes about the performance originally written for a reviewer.

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
