#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef pair<string, int> destcost; 
typedef vector<destcost> destinations;
typedef tuple<string, string, int> edge;
typedef unordered_map<string, destinations> sourceToDestinations;

const sourceToDestinations graph {{"S", {{"A",4}, {"B",3}, {"D",7}}},
                                  {"A", {{"C", 1}}},
                                  {"B", {{"S",3}, {"D",4}}},
                                  {"C", {{"E",1}, {"D",3}}},
                                  {"D", {{"E",1}, {"F",5}, {"T",3}}},
                                  {"E", {{"G",2},{"T",4}}},
                                  {"F", {}},
                                  {"G", {{"E",2}, {"T",3}}},
                                  {"T", {{"F",5}}} };

unordered_map<string, int> closedSet {{"S", 0}};
vector<edge> openSet;

const string destinationTarget = "T";

tuple<string, int> find_next_node_min_distance()
{
    string minD{""};
    int minDistance = 100;
    for (auto& d : openSet) {
        int dist = get<2>(d);
        if (dist < minDistance)
        {
            minDistance = dist;
            minD = get<1>(d);
        }
    }
    cout << minD << " has lowest distance of " << minDistance << endl;
    return make_tuple(minD, minDistance);
}

int get_my_current_distance(string node)
{
    auto me = closedSet.find(node);
    if (me != closedSet.end())
    {
        return me->second;
    }
    else
        cout << "Errr - nae number, mate!\n";
    return -999;
}

bool should_update_open_set(string node, int weight)
{
    for (auto& n : openSet)
    {
        string nnode = get<1>(n);
        int nweight = get<2>(n);
        if (node.compare(nnode) == 0 && weight > nweight)
            return false;
    }
    return true;
}

void removeFromOpenSet(string node)
{
    for (auto& n : openSet)
    {
        string nnode = get<1>(n);
        if (node.compare(nnode) == 0) {
            for (int i = 0; i < openSet.size(); ++i) {
                if (get<1>(openSet[i]).compare(node) == 0)
                    openSet.erase(openSet.begin() + i);
            }
        }
    }
}

void print_closedSet()
{
    for (auto& i : closedSet)
    {
        cout << i.first << " : " << i.second << endl;
    }
}

void djikstra(string curNode)
{
    cout << "\nLooking at " << curNode << endl;
    int myDist = get_my_current_distance(curNode);
    cout << "my current distance is " << myDist << endl;

    auto found = graph.find(curNode); // all nodes i can reach
    if (found != graph.end()) {
        destinations currentlyReachable = found->second;
        for (auto& d : currentlyReachable) {
            auto neighborIsInClosedSet = closedSet.find(d.first);
            if (neighborIsInClosedSet != closedSet.end()) {
                cout << "Skippiing " << d.first << " as it's in closedSet" << endl;
            }
            else {
                int tentativeDist = myDist + d.second;
                if (should_update_open_set(d.first, tentativeDist))
                {
                    cout << "Adding neighbor " << d.first << " to openSet" << endl;
                    edge e = make_tuple(curNode, d.first, tentativeDist);
                    openSet.push_back(e);
                }
                else
                    cout << "Nothing to update" << endl;
            }
            print_closedSet();
        }


        auto nextTuple = find_next_node_min_distance();
        string nextNode = get<0>(nextTuple);
        int nextNodeWeight = get<1>(nextTuple);
        cout << "Next node is " << nextNode << ". Removing from openset" << endl;
        removeFromOpenSet(nextNode);
        closedSet[nextNode] = nextNodeWeight;

        if (closedSet.find(destinationTarget) != closedSet.end()) {
            cout << "Found the Target! Distance is " << closedSet[destinationTarget] << endl;
            print_closedSet();
            exit(0);
        }
        djikstra(nextNode);
    }
    else {
        cout << "Couldn't find " << curNode << endl;
    }

}

int main()
{
    string startingNode = "S";
    djikstra(startingNode);
}
