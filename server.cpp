#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <cassert>
#include "dijkstra.h"
#include "wdigraph.h"
#include "digraph.h"
#include "serialport.h"

using namespace std;

typedef pair<int, long long> PIL;

struct Point
{
    long long lon;
    long long lat;
};

int numPoints = 0;

/*
    Description: Computes the Manhattan distance between two vertices of graph. 
    Arguments:  const Point& pt1, const Point& pt2
    Returns: long long
*/
long long manhattan(const Point& pt1, const Point& pt2)
{
    return abs(pt1.lon - pt2.lon) + abs(pt1.lat - pt2.lat);
}

/*
    Description: Computes the Manhattan distance between two vertices of graph. 
    Arguments:  const Point& pt1, int lon, int lat
    Returns: long long
*/
long long manhattan(const Point& pt1, int lon, int lat)
{
    return abs(pt1.lon - lon) + abs(pt1.lat - lat);
}

/*
    Description: Parses the text file describing the road network then builds the weighted directed
                graph and stores the coordinates of every vertex.
    Arguments: string filename, WDigraph& graph, unordered_map<int, Point>&points
    Returns: WDigraph* 
*/
WDigraph* read_city_graph_undirected(string filename, WDigraph& graph, unordered_map<int, Point>&points) 
{
    ifstream file(filename);
    string str;
    vector<int> pos;
    Point point;
    if(file.is_open())
    {
        string::size_type sz;
        while(getline(file, str))
        {
            if(str[0] == 'V')
            {
                size_t found = str.find(",");
                size_t found1 = str.find(",", found+1);
                size_t found2 = str.find(",", found1+1);
                string index1s = str.substr(found+1,(found1-1)-(found));
                string lats = str.substr(found1+1, (found2-1)- found1);
                string lons = str.substr(found2+1, (str.length()-1)-found2);
                lats.erase(lats.begin()+2);
                lons.erase(lons.begin()+4);
                int index = stoi(index1s, &sz);
                long long lat = stoi(lats);
                long long lon = stoi(lons);
                lat = lat/10;
                lon = lon /10;
                graph.addVertex(index);
                point.lon = lon;
                point.lat = lat;
                points[index] = point;
                numPoints++;
            }
            else if(str[0] == 'E')
            {
                size_t found = str.find(",");
                size_t found1 = str.find(",", found+1);
                size_t found2 = str.find(",", found1+1);
                string index1s = str.substr(found+1, (found1-1)-(found));
                string index2s = str.substr(found1+1,(found2-1)-(found1));
                int index1 = stoi(index1s, &sz);
                int index2 = stoi(index2s, &sz);
                graph.addEdge(index1, index2, manhattan(points[index1], points[index2]));
            }

        }

    }
    return &graph;
}

/*
    Description: Finds the closest vertex to the one requested.
    Arguments: long long lat, long long lon, unordered_map<int, Point>& points
    Returns: int
*/
int getClosestVertex(long long lat, long long lon, unordered_map<int, Point>&points)
{
    long long minDist = 0;
    int minIndex;
    int i = 0;
    for(auto& e : points)
    {
        long long dist = manhattan(e.second, lon, lat);
        if(dist < minDist || (i == 0))
        {
            minDist = dist;
            minIndex = e.first;
        }
        i++;
    }
    return minIndex;
}

/*
    Description: Converts a long long int to a string (array of chars).
    Arguments: string& myString, long long num, int &size
    Returns: void
*/
void longlongtochar(string& myString, long long num, int &size) 
{
    long long numcpy = num;
    long long temp, temp2, temp3;
    bool neg = false;
    int digit;
    int delim = 0;
    while(num != 0)
    {
      num = num /10;
      size++;
    }
    if(numcpy < 0)
    {
      myString[0] = '-';
      delim = 1;
      numcpy = numcpy * -1;
      neg = true;
    }
    for(int i = size-1; i >= 0; i--)
    {
      temp = (long long)pow(10, i);
      if(temp > 10)
      {
        temp = temp +1;
      }
      temp2 = numcpy / (temp);
      temp3 = numcpy / (((temp) * 10));
      digit = temp2 - (temp3*10);
      myString.push_back(digit + '0');
    }
    if(neg)
    {
      size++;
    }
   
}


int main(int argc, char *argv[])
{

    SerialPort Serial("/dev/ttyACM0");

    WDigraph graph;
    unordered_map<int, Point> points;
    unordered_map<int, PIL> searchtree;


    WDigraph* g = read_city_graph_undirected(argv[1],graph, points);

    list<int> path;
    string acknow = "A\n";
    string line;
    long long addy[4];
    bool bytesarrived = false;
    string arduino;
    while(true)
    {
        if(!bytesarrived)
        {
            line = Serial.readline();
            if(line[0] == 'R')
            {
                // parse request string
                size_t found = line.find(" ");
                size_t found1 = line.find(" ", found+1);
                size_t found2 = line.find(" ", found1+1);
                size_t found3 = line.find(" ", found2+1);
                string lats1 = line.substr(found+1, (found1-1) - found);
                string lons1 = line.substr(found1+1, (found2-1) - found1);
                string lats2 = line.substr(found2+1, (found3-1)-found2);
                string lons2 = line.substr(found3+1, (line.length()-1)-found3);
                long long lat1  = stoi(lats1);
                long long lon1 = stoi(lons1);
                long long lat2  = stoi(lats2);
                long long lon2 = stoi(lons2);
                int vertex1 = getClosestVertex(lat1, lon1, points);
                int vertex2 = getClosestVertex(lat2, lon2, points);
                dijkstra(graph, vertex1, searchtree);     
                bytesarrived = true;
                if (searchtree.find(vertex2) == searchtree.end()) 
                {
                    Serial.writeline("N 0\n");
                    bytesarrived = false;
                    path.clear();
                    searchtree.clear();
                }   
                else 
                {
                    int stepping = vertex2;
                    long long count = 0;
                    string str = "";
                    int size = 0;
                    while (stepping != vertex1)
                    {
                        // build path
                        path.push_front(stepping);
                        count++;
                        // crawl up the search tree one step
                        stepping = searchtree[stepping].first;
                    }
                    longlongtochar(str, count + 1, size);
                    path.push_front(vertex1);
                    Serial.writeline("N ");
                    // If more than 500 waypoints to be stored, no path.
                    if(count + 1 >= 500){
                        Serial.writeline("0");
                        Serial.writeline("\n");
                        bytesarrived = false;
                        path.clear();
                        searchtree.clear();
                    }
                    else{
                        Serial.writeline(str);
                        Serial.writeline("\n");
                    }
                }
            }
        }
        else
        {
            arduino = Serial.readline();
            string line;
            for (auto it : path) {
                line  = "W: "+ to_string(points[it].lat);
                line = line + " ";
                line = line  + to_string(points[it].lon);
                if(arduino[0] == 'A'){
                    Serial.writeline(line);
                    Serial.writeline("\n");
                }
                arduino = Serial.readline();
            }
            Serial.writeline("E");
            Serial.writeline("\n");
            bytesarrived = false;
            path.clear();
            searchtree.clear();
        }
    }
    return 1;
}
