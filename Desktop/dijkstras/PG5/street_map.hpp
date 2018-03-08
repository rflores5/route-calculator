#ifndef STREET_MAP_HPP
#define STREET_MAP_HPP
#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <utility>
using namespace std;

struct side{			//struct representing side of the street
	string streetName;
	int parity;
	bool operator==(side const& other) const{ return streetName == other.streetName && parity == other.parity;}
	side(const string &streetName, int parity) : streetName(streetName),parity(parity) {}
};

struct segment{			//struct representing street segment
	int start;
	int end;
	string URL;
	float distance;
	int startnode;
	int endnode;
	segment(int start,int end,int startnode, int endnode, float distance, string &URL) : start(start), end(end),startnode(startnode),endnode(endnode),distance(distance),URL(URL) {}
	bool operator<(const segment &other) const { return end < other.end; }
};



namespace std{			//modifying hasher for structs as key
	template<>
	struct hash<side>{
		hash<string> string_hasher;
		size_t operator() (side const& s) const {
			return string_hasher(s.streetName) ^ s.parity;
		}
	};
}

struct edge{			//struct representing edge in a graph
	string streetName;
	int startnode;
	int endnode;
	float distance;
	string url;
	edge(string streetName,int startnode,int endnode,float distance, string url) : streetName(streetName),startnode(startnode),endnode(endnode),distance(distance), url(url) {}
};


class street_map {
public:
  // Constructor.
  // `filename` is the pathname of a file with the format described in pg4.pdf.

  explicit street_map (const std::string &filename);

  // Geocodes an address.
  // Input argument:
  //   `address` is an address of the form "1417 E Wayne St"
  // Return value:
  //   `true` if address is found, `false` if address is not found
  // Output arguments:
  //   `u` is the starting node of the edge on which the location lies
  //   `v` is the ending node of the edge
  //   `pos` is how far along the edge the location lies

  bool geocode(const std::string &address, int &u, int &v, float &pos) const;

  // Three functions for finding the shortest route.

  // Input arguments:
  //   `source` is the source node
  //   `target` is the target node
  // Output argument:
  //   `distance` is the shortest distance between them

  bool route3(int source, int target, float &distance) const;

  // Input arguments:
  //   `su`, `sv` is the edge on which the source lies
  //   `spos`     is how far along the edge the source lies
  //   `tu`, `tv` is the edge on which the source lies
  //   `tpos`     is how far along the edge the source lies
  // Output argument:
  //   `distance` is the shortest distance between them

  bool route4(int su, int sv, float spos, int tu, int tv, float tpos, float &distance) const;

  // Input arguments:
  //   `su`, `sv` is the edge on which the source lies
  //   `spos`     is how far along the edge the source lies
  //   `tu`, `tv` is the edge on which the source lies
  //   `tpos`     is how far along the edge the source lies
  // Output argument:
  //   `steps`    is a sequence of driving directions;
  //              each member is a canonical street name and a distance.
  //              The original contents (if any) are cleared out.

  bool route(int su, int sv, float spos, int tu, int tv, float tpos, std::vector<std::pair<std::string, float>> &steps) const;

private:
	unordered_map<struct side, vector<struct segment>> mapData;	//Map info used for geocoding
	unordered_map<int, vector<struct edge>> adjList;		//adjacency list


};

#endif
