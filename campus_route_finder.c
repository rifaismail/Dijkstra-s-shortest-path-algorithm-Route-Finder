#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_V 10
#define MAX_HEAP 100
#define INF INT_MAX

/* ---------- Graph: adjacency list (linked list of edges) ---------- */
struct Edge {
    int to;
    int weight;
    struct Edge *next;
};

struct Edge *adj[MAX_V];
char *names[MAX_V] = {"Main Gate", "Admin Block", "Library", "IT Block",
                      "Canteen", "Auditorium", "Hostel", "Sports Complex"};
int V = 8;

void addEdge(int u, int v, int w)
{
    struct Edge *e1 = (struct Edge *)malloc(sizeof(struct Edge));
    e1->to = v; e1->weight = w; e1->next = adj[u]; adj[u] = e1;

    struct Edge *e2 = (struct Edge *)malloc(sizeof(struct Edge));
    e2->to = u; e2->weight = w; e2->next = adj[v]; adj[v] = e2;
}

/* ---------- Priority queue: binary min-heap ---------- */
struct HeapNode {
    int dist;
    int vertex;
};

struct HeapNode heap[MAX_HEAP];
int heapSize = 0;

void push(int dist, int vertex)
{
    int i = heapSize++;
    heap[i].dist = dist;
    heap[i].vertex = vertex;
    while (i > 0 && heap[(i - 1) / 2].dist > heap[i].dist) {
        struct HeapNode t = heap[i];
        heap[i] = heap[(i - 1) / 2];
        heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

struct HeapNode pop()
{
    struct HeapNode top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, small = i;
        if (l < heapSize && heap[l].dist < heap[small].dist) small = l;
        if (r < heapSize && heap[r].dist < heap[small].dist) small = r;
        if (small == i) break;
        struct HeapNode t = heap[i];
        heap[i] = heap[small];
        heap[small] = t;
        i = small;
    }
    return top;
}

/* ---------- Dijkstra's algorithm ---------- */
int dist[MAX_V], parent[MAX_V];

void dijkstra(int source)
{
    int i;
    for (i = 0; i < V; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[source] = 0;
    heapSize = 0;
    push(0, source);

    while (heapSize > 0) {
        struct HeapNode cur = pop();
        int u = cur.vertex;
        if (cur.dist > dist[u])
            continue;                       /* skip outdated entry */
        struct Edge *e;
        for (e = adj[u]; e != NULL; e = e->next) {
            if (dist[u] + e->weight < dist[e->to]) {
                dist[e->to] = dist[u] + e->weight;
                parent[e->to] = u;
                push(dist[e->to], e->to);
            }
        }
    }
}

void printPath(int target)
{
    if (parent[target] != -1) {
        printPath(parent[target]);
        printf(" -> ");
    }
    printf("%s", names[target]);
}

void showRoute(int s, int t)
{
    dijkstra(s);
    printf("Route %s -> %s\n", names[s], names[t]);
    printf("  Path    : ");
    printPath(t);
    printf("\n  Distance: %d m\n\n", dist[t]);
}

int main()
{
    int i;
    for (i = 0; i < MAX_V; i++)
        adj[i] = NULL;

    /* 0 Main Gate, 1 Admin Block, 2 Library, 3 IT Block,
       4 Canteen, 5 Auditorium, 6 Hostel, 7 Sports Complex */
    addEdge(0, 1, 150);
    addEdge(0, 2, 300);
    addEdge(1, 2, 120);
    addEdge(1, 3, 250);
    addEdge(2, 3, 100);
    addEdge(2, 4, 200);
    addEdge(3, 5, 180);
    addEdge(4, 6, 140);
    addEdge(4, 5, 160);
    addEdge(5, 7, 220);
    addEdge(6, 7, 260);

    printf("=== CAMPUS ROUTE FINDER (Dijkstra's Algorithm) ===\n\n");

    dijkstra(0);
    printf("Shortest distances from %s:\n", names[0]);
    for (i = 0; i < V; i++)
        printf("  %-16s %d m\n", names[i], dist[i]);
    printf("\n");

    showRoute(0, 6);
    showRoute(1, 7);
    showRoute(2, 5);

    return 0;
}
