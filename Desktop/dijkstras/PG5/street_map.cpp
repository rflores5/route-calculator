#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <stack>
#include "street_map.hpp"

using namespace std;

//some of this code came from the PG4 solution

street_map::street_map (const string &filename) {	//reading in the map data and adding it to adjacency list
	string line;
	fstream infile(filename);
	string name;
	mapData.clear();
	while(getline(infile,line)){
		istringstream iss(line);
		string code;
		iss >> code >> ws;
		if(code == "N:"){
			getline(iss,name);		//street name line
		}
		else if(code == "E:"){			//street segments witout addresses
			int startnode, endnode;
			float distance;
			string url;
			iss >> startnode >> endnode >> distance >> url;
			edge e1(name,startnode,endnode,distance,url);		//edges in graph are stored as structs 
			edge e2(name,endnode,startnode,distance,url);
			if(adjList.find(startnode) != adjList.end()){
				adjList[startnode].push_back(e1);
			}
			else{
				adjList[startnode] = vector<edge>();
				adjList[startnode].push_back(e1);	
			}
			if(adjList.find(endnode) != adjList.end()){		//reverse order edges added to create undirected graph
				adjList[endnode].push_back(e2);	
			}
			else{
				adjList[endnode] = vector<edge>();
				adjList[endnode].push_back(e2);
			}
			
		}
		else if(code == "R:"){						//street segments with adresses
			int parity,start,end,startnode,endnode;
			float distance;
			string url;
			iss >> parity >> start >> end >> startnode >> endnode >> distance >> url;	
			side s(name,parity);
			if(mapData.find(s) != mapData.end()){
				mapData[s].push_back(segment(start,end,startnode,endnode,distance,url));	//mapData from PG4
			}
			else{
				mapData[s] = vector<segment>();
				mapData[s].push_back(segment(start,end,startnode,endnode,distance,url));

			}

			edge e1(name,startnode,endnode,distance,url);
			edge e2(name,endnode,startnode,distance,url);
                        if(adjList.find(startnode) != adjList.end()){
                                adjList[startnode].push_back(e1);
                        }
                        else{
                                adjList[startnode] = vector<edge>();
                                adjList[startnode].push_back(e1);
                        }
			if(adjList.find(endnode) != adjList.end()){
                                adjList[endnode].push_back(e2);
                        }
                        else{
                                adjList[endnode] = vector<edge>();
                                adjList[endnode].push_back(e2);
                        }

			
		}
		else{
			cout << "|" <<code<<"|"<<endl;
			throw runtime_error("bad file format");

		}		 
	}

	for(auto side_segment : mapData){					//sort street segments if they are not in order
		sort(side_segment.second.begin(),side_segment.second.end());
	}
}


int searchSegments(vector<struct segment> &vec, int &target, int first, int size){      //binary search to search for segment within a vector

        int middle;
        if (size==0){
                return -1;
        }

        else {
                middle = first + size/2;
                //struct segment s = vec[middle];
                if (vec[middle].start <= target && target <= vec[middle].end){
                        return middle;
                }
                else if (target < vec[middle].start){
                        return searchSegments(vec,target,first,size/2);
                }
                else {
                        return searchSegments(vec,target,middle+1,(size-1)/2);
                }
        }

}


bool street_map::geocode(const string &address, int &u, int &v, float &pos) const {	//determining the position of an address on a street segment
	istringstream iss(address);
	int number;
	string name;
	iss >> number >> ws;
	getline(iss,name);
	auto pair_it = mapData.find(side(name,number%2));

	if(pair_it == mapData.end()){
		return false;
	}
	auto segs = pair_it->second;

        int i = searchSegments(segs, number, 0, segs.size());
	if(i < 0){
		return false;
	}

	u = segs[i].startnode;
	v = segs[i].endnode;
	float num = (number-segs[i].start);
	float den = (segs[i].end-segs[i].start+2);
	pos = (segs[i].distance)*(num/den);
	
	return true;
}

//some of the following code was inspired from an example from the class bitbucket examples
struct entry{				//node representation used in priority queue in dijkstra's algrorithm		
	int startnode;
	float distance;
	bool operator<(const struct entry &other) const {return distance > other.distance; }
};

bool street_map::route3(int source, int target, float &distance) const {
	
	unordered_set<int> marked;		//set keeping track of which nodes have been visited
	priority_queue<entry> frontier;		//priority queue keeping track of nodes that have not been visited
	frontier.push(entry{source,0});
	float dist = 0;

	while(frontier.size() > 0){		//dijkstra's algorithm
		auto top = frontier.top();
		frontier.pop();
		if(marked.count(top.startnode)) continue;
		marked.insert(top.startnode);

		if(top.startnode == target){	//target has been found
			distance = top.distance;
			return true;
		}
	
		auto it = adjList.find(top.startnode);
		if (it != adjList.end()){
			for(auto v : it->second){
				frontier.push(entry{v.endnode,v.distance+top.distance});
			}
		}
	}	
	return false;
}

bool street_map::route4(int su, int sv, float spos, int tu, int tv, float tpos, float &distance) const {

	if (su==tu && sv==tv){			//if source and target are on the same street segment
		distance = tpos-spos;
	}

	unordered_set<int> marked;
	priority_queue<entry> frontier; //distance,node
        float dist = 0;
	auto s = adjList.find(su)->second;
	auto t = adjList.find(tu)->second;
	auto vec1 = s.begin();
	auto vec2 = t.begin();
	bool b1 = false;
	bool b2 = false;
	float s1;
	float t1;

	while (b1==false || b2==false){		//search for the name and distance of the source and target street segments
		if((*vec1).endnode == sv){
			s1 = (*vec1).distance;
			vec1 = s.begin();
			b1 = true;
		}
		if((*vec2).endnode == tv){
			t1 = (*vec2).distance;
			vec2 = t.begin();
			b2 = true;
		}			
		vec1++;
		vec2++;
	}
	int source = -2;			//hallucinating source and target nodes
	int target = -1;
	frontier.push(entry{source,0});		//source and street segment of source added to frontier
	frontier.push(entry{su,spos});
	frontier.push(entry{sv,s1-spos});

        while(frontier.size() > 0){		
                auto top = frontier.top();
                frontier.pop();
                if(marked.count(top.startnode)) continue;
                marked.insert(top.startnode);
		
		if(top.startnode == tu){				//if current node is on target street, add distance of target
			frontier.push(entry{target,top.distance+tpos});
		}
		if(top.startnode == tv){
			frontier.push(entry{target,top.distance+t1-tpos});
		}
                if(top.startnode == target){
                        distance = top.distance;
                        return true;
                }
                auto it = adjList.find(top.startnode);
                if (it != adjList.end()){
                        for(auto v : it->second){			
                                frontier.push(entry{v.endnode,v.distance+top.distance});
                        }
                }
        }
        return false;


}

struct node{					//new node representionation keeping track of previous node
	int startnode;
	float distance;
	int previous_node;
	bool operator<(const struct node &other) const {return distance > other.distance; }

};

bool street_map::route(int su, int sv, float spos, int tu, int tv, float tpos, vector<pair<string, float>> &steps) const {
  steps.clear();

		//frontierentry: node, distance, previous node

 	float distance;
	

        unordered_set<int> marked;	
       	unordered_map<int,node> path;				//path hash table keeps track of nodes visited with previous node to recreate path
	priority_queue<node> frontier; //distance,node
        auto vec1 = adjList.find(su)->second;
        auto vec2 = adjList.find(tu)->second;
	string sourcestreet;
	string targetstreet;					//find streetname and distance of source and target street segments
        float s1;
        float t1;
       	for(int i = 0;i < vec1.size();i++){
                if(vec1[i].endnode == sv){
                        s1 = vec1[i].distance;
			sourcestreet = vec1[i].streetName;
			break;
                }
	}
	for(int j = 0;j < vec2.size();j++){
                if(vec2[j].endnode == tv){
                        t1 = vec2[j].distance;
			targetstreet = vec2[j].streetName;
			break;
                }
        }

        int source = -2;				//hallucinating source and target nodes
        int target = -1;
        frontier.push(node{source,0,source});
        frontier.push(node{su,spos,source});
        frontier.push(node{sv,s1-spos,source});
        while(frontier.size() > 0){
                auto top = frontier.top();
                frontier.pop();
                if(marked.count(top.startnode)) continue;
                marked.insert(top.startnode);			
		path.insert(make_pair(top.startnode,top));	//any node visited added to path so directions can be determined
	
                if(top.startnode == tu){
                        frontier.push(node{target,top.distance+tpos,top.startnode});
                }
                else if(top.startnode == tv){
                        frontier.push(node{target,top.distance+t1-tpos,top.startnode});
                }
                else if(top.startnode == target){		//target has been found
                        distance = top.distance;	
   			break;

	        }
                auto it = adjList.find(top.startnode);
                if (it != adjList.end()){
                        for(auto v : it->second){
                                frontier.push(node{v.endnode,v.distance+top.distance,top.startnode});
                        }
                }
        }
 	if(frontier.size()==0) return false;	//target was not found

	bool finished = false;
	stack<pair<string,float>> temp;		//stack used for LIFO because path recronstruction starts at target (end)
	string streetName = "";
	float length;
	int curr = target;		//current node starts at target
	while(!finished){		//reconstructing path by starting with target and following each previous node until source

		//node	
		int next = (path.find(curr)->second).previous_node; //next node in path reconstruction

		if (su==tu && sv==tv){	//if source and target are on the same street segment
			streetName = targetstreet;
			length = tpos - spos;
			temp.push(make_pair(streetName,length));	//only one direction in this case
			finished = true;
		} 
		else if(curr==target){	//when current node is target which is a hallucinated node
			streetName = targetstreet;
			if (next == tu) length = tpos; 
			else if (next == tv)  length = t1 -tpos;
		}
		else if (next==source){	//when the next node is source (last) which is a hallucinated node
    			streetName = sourcestreet;
			if (curr = su) length = spos;
			else if (curr = sv) length = s1 - spos;
			temp.push(make_pair(streetName,length));	//push final direction into stack 
			finished = true;				//last direction, path reconstruction is finished
                }

		else{
			auto got = adjList.find(next);			//all other cases, street name and length need to be searched in adjacency list
			auto vec = got->second;
			for(int i = vec.size()-1; i >=0; i--){
				if(vec[i].endnode == curr){
					streetName = vec[i].streetName;
					length = vec[i].distance;
					break;
				}					
			}
		}
		if(!finished) temp.push(make_pair(streetName,length));	//directions are added to stack, which will be in reverse order 
		curr = next;
	}

	int n = temp.size();
	for(int j = 0; j < n; j++){		//directions are taken off stack and put into steps vector in correct order
		auto curr = temp.top();		//travel distances of segments on same street are added together
		temp.pop();
		if(steps.size()==0){		
			steps.push_back(curr);
			continue;
		}
		if(curr.first != steps.back().first){
			steps.push_back(curr);
		}
		else{
			steps.back().second+=curr.second;
		}
	}
	return true;
}

