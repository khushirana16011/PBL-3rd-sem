
#include <iostream>
#include <climits>
using namespace std;

const int MAX = 10;

class Location {
public:
    int id;
    char name;

    Location(int i, char n) {
        id = i;
        name = n;
    }
};

class Route {
public:
    int distance;
    int path[MAX];
    int count;

    Route() {
        distance = INT_MAX;
        count = 0;
    }

    bool operator<(const Route &r) const {
        return distance < r.distance;
    }
};

class RoutePlanner {
public:
    virtual void displayInfo() {
        cout << "Basic Route Planner\n";
    }

    virtual ~RoutePlanner() {}
};

class Graph : public RoutePlanner {
    struct Node {
        int dest;
        int weight;
        Node *next;
    };

    Node *head[MAX];
    int n;

public:
    Graph(int vertices) {
        n = vertices;

        for (int i = 0; i < n; i++)
            head[i] = NULL;
    }

    void displayInfo() override {
        cout << "Route Optimization System\n";
        cout << "Shortest Route using Dijkstra\n";
    }

    void addRoad(int u, int v, int w) {
        Node *p = new Node;
        p->dest = v;
        p->weight = w;
        p->next = head[u];
        head[u] = p;

        Node *q = new Node;
        q->dest = u;
        q->weight = w;
        q->next = head[v];
        head[v] = q;
    }

    Route findShortestPath(int src, int dest) {
        int dist[MAX], parent[MAX], visited[MAX] = {0};

        for (int i = 0; i < n; i++) {
            dist[i] = INT_MAX;
            parent[i] = -1;
        }

        dist[src] = 0;

        for (int i = 0; i < n; i++) {
            int u = -1;
            Route best;

            for (int j = 0; j < n; j++) {
                Route current;
                current.distance = dist[j];

                if (!visited[j] && current < best) {
                    best = current;
                    u = j;
                }
            }

            if (u == -1)
                break;

            visited[u] = 1;
            Node *p = head[u];

            while (p != NULL) {
                int v = p->dest;
                int w = p->weight;

                if (!visited[v] && dist[u] != INT_MAX &&
                    dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                }

                p = p->next;
            }
        }

        Route r;
        r.distance = dist[dest];

        if (dist[dest] == INT_MAX)
            return r;

        int temp[MAX], count = 0;
        int curr = dest;

        while (curr != -1) {
            temp[count++] = curr;
            curr = parent[curr];
        }

        for (int i = count - 1; i >= 0; i--)
            r.path[r.count++] = temp[i];

        return r;
    }

    void findShortestPath(Location src, Location dest) {
        Route r = findShortestPath(src.id, dest.id);
        displayRoute(r);
    }

    void displayRoute(Route r) {
        if (r.distance == INT_MAX) {
            cout << "No route available!\n";
            return;
        }

        cout << "Shortest route: ";

        for (int i = 0; i < r.count; i++) {
            cout << char('A' + r.path[i]);

            if (i < r.count - 1)
                cout << " -> ";
        }

        cout << "\nTotal distance: "
             << r.distance << " km\n";
    }

    ~Graph() {
        for (int i = 0; i < n; i++) {
            Node *p = head[i];

            while (p != NULL) {
                Node *temp = p;
                p = p->next;
                delete temp;
            }
        }
    }
};

int main() {
    Graph g(6);

    RoutePlanner *planner = &g;
    planner->displayInfo();

    Location A(0, 'A'), B(1, 'B'), C(2, 'C');
    Location D(3, 'D'), E(4, 'E'), F(5, 'F');

    g.addRoad(0, 1, 2);
    g.addRoad(0, 2, 4);
    g.addRoad(1, 2, 1);
    g.addRoad(1, 3, 3);
    g.addRoad(2, 4, 2);
    g.addRoad(3, 5, 2);
    g.addRoad(4, 5, 3);

    int src, dest;

    cout << "\nLocations: 0=A, 1=B, 2=C, "
         << "3=D, 4=E, 5=F\n";

    cout << "Enter source: ";
    cin >> src;

    cout << "Enter destination: ";
    cin >> dest;

    if (src < 0 || src >= 6 ||
        dest < 0 || dest >= 6) {
        cout << "Invalid location!\n";
        return 0;
    }

    g.findShortestPath(
        Location(src, char('A' + src)),
        Location(dest, char('A' + dest))
    );

    return 0;
}
