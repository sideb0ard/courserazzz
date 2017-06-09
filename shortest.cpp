#include <iostream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

typedef pair<string, int> destcost;
typedef vector<destcost> destinations;
typedef unordered_map<string, destinations> sourceToDestinations;

typedef tuple<string, string, int> edge;

constexpr int INF = 1000000; // magic number

const sourceToDestinations graph{{"S", {{"A", 4}, {"B", 3}, {"D", 7}}},
                                 {"A", {{"C", 1}}},
                                 {"B", {{"S", 3}, {"D", 4}}},
                                 {"C", {{"E", 1}, {"D", 3}}},
                                 {"D", {{"E", 1}, {"F", 5}, {"T", 3}}},
                                 {"E", {{"G", 2}, {"T", 4}}},
                                 {"F", {}},
                                 {"G", {{"E", 2}, {"T", 3}}},
                                 {"T", {{"F", 5}}}};

vector<edge> pathsToExplore;
unordered_map<string, int> DestinationDistances;

const string destinationTarget = "T";

tuple<string, int> findNextNodeMinDistance()
{
    string minNode{""};
    int minDistance = INF;
    for (auto &d : pathsToExplore) {
        int dist = get<2>(d);
        if (dist < minDistance) {
            minDistance = dist;
            minNode = get<1>(d);
        }
    }
    if (minDistance != INF) {
        cout << minNode << " has lowest distance of " << minDistance << endl;
        return make_tuple(minNode, minDistance);
    }
    else {
        cout << "No more minimum distances -- all are infinite" << endl;
        exit(1);
    }
}

int getMyCurrentDistance(string node)
{
    auto me = DestinationDistances.find(node);
    if (me != DestinationDistances.end()) {
        return me->second;
    }
    else {
        cout << "Errr - nae number, mate!\n";
        return INF;
    }
}

bool shouldUpdatePathsToExplore(string node, int weight)
{
    for (auto &n : pathsToExplore) {
        string nnode = get<1>(n);
        int nweight = get<2>(n);
        if (node.compare(nnode) == 0 && weight >= nweight)
            return false;
    }
    return true;
}

void removeFromPathsToExplore(string node)
{
    for (int i = pathsToExplore.size() - 1; i >= 0; --i) {
        edge e = pathsToExplore[i];
        string nnode = get<1>(e);
        if (node.compare(nnode) == 0) {
            pathsToExplore.erase(pathsToExplore.begin() + i);
        }
    }
}

void printDestinationDistances()
{
    for (auto &i : DestinationDistances) {
        cout << i.first << " : " << i.second << endl;
    }
}

void djikstra(string curNode)
{
    cout << "\nLooking at " << curNode << endl;
    int myDist = getMyCurrentDistance(curNode);
    if (myDist == INF) {
        cout << "INF!\n";
        exit(1);
    }
    cout << "my current distance is " << myDist << endl;

    auto nodesFound = graph.find(curNode);
    if (nodesFound != graph.end()) { // i have nodes i can reach
        destinations currentlyReachable = nodesFound->second;
        for (auto &d : currentlyReachable) {
            auto neighborIsInDestinationDistances =
                DestinationDistances.find(d.first);
            if (neighborIsInDestinationDistances !=
                DestinationDistances.end()) {
                cout << "Skippiing " << d.first
                     << " as it's in DestinationDistances" << endl;
            }
            else {
                int tentativeDist = myDist + d.second;
                if (shouldUpdatePathsToExplore(d.first, tentativeDist)) {
                    cout << "Adding neighbor " << d.first
                         << " to pathsToExplore with value " << tentativeDist
                         << endl;
                    removeFromPathsToExplore(d.first);
                    edge e = make_tuple(curNode, d.first, tentativeDist);
                    pathsToExplore.push_back(e);
                }
                else
                    cout << "Nothing to update" << endl;
            }
        }
        // printDestinationDistances();

        auto nextTuple = findNextNodeMinDistance();
        string nextNode = get<0>(nextTuple);
        int nextNodeWeight = get<1>(nextTuple);
        cout << "Next node is " << nextNode << ". Removing from pathsToExplore"
             << endl;
        removeFromPathsToExplore(nextNode);
        DestinationDistances[nextNode] = nextNodeWeight;

        if (DestinationDistances.find(destinationTarget) !=
            DestinationDistances.end()) {
            cout << "Found the Target! Distance is "
                 << DestinationDistances[destinationTarget] << endl;
            printDestinationDistances();
            exit(0);
        }
        djikstra(nextNode);
    }
    else {
        cout << "Couldn't find any nodes for " << curNode << endl;
    }
}

int main()
{
    string startingNode = "S";
    DestinationDistances[startingNode] = 0;
    djikstra(startingNode);
}
