//
//  NetworkRepair.cpp
//  cs300 hw4
//
//  Created by nehir ceylan on 5/8/26.
//

#include "NetworkRepair.h"


DisjointSet::DisjointSet(int n) {
    parent.resize(n + 1);
    sz.resize(n + 1, 1);
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }
    components = n;
}

int DisjointSet::find(int x) {
    if (parent[x] == x) {
        return x;
    }
    parent[x] = find(parent[x]);
    return parent[x];
}

bool DisjointSet::unite(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);
    if (rootA == rootB) {
        return false;
    }
    if (sz[rootA] < sz[rootB]) {
        swap(rootA, rootB);
    }
    parent[rootB] = rootA;
    sz[rootA] += sz[rootB];
    components--;
    return true;
}

bool DisjointSet::connected(int a, int b) {
    return find(a) == find(b);
}

int DisjointSet::componentSize(int x) {
    return sz[find(x)];
}

int DisjointSet::componentCount() {
    return components;
}


bool BinaryHeap::higherPriority(const RepairRequest& a,
                                const RepairRequest& b) {
    if (a.urgency != b.urgency) {
        return a.urgency > b.urgency;
    }
    if (a.cost != b.cost) {
        return a.cost < b.cost;
    }
    return a.id < b.id;
}

void BinaryHeap::swapNodes(int i, int j) {
    swap(heap[i], heap[j]);
    position[heap[i].id] = i;
    position[heap[j].id] = j;
}

void BinaryHeap::heapifyUp(int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (higherPriority(heap[index], heap[parentIndex])) {
            swapNodes(index, parentIndex);
            index = parentIndex;
        }
        else {
            break;
        }
    }
}

void BinaryHeap::heapifyDown(int index) {
    int n = heap.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int best = index;
        if (left < n &&
            higherPriority(heap[left], heap[best])) {
            best = left;
        }
        if (right < n &&
            higherPriority(heap[right], heap[best])) {
            best = right;
        }
        if (best != index) {
            swapNodes(index, best);
            index = best;
        }
        else {
            break;
        }
    }
}

bool BinaryHeap::empty() {
    return heap.empty();
}

bool BinaryHeap::contains(const string& id) {
    return position.find(id) != position.end();
}

void BinaryHeap::insert(const RepairRequest& r) {
    heap.push_back(r);
    int index = heap.size() - 1;
    position[r.id] = index;
    heapifyUp(index);
}

RepairRequest BinaryHeap::extractTop() {
    RepairRequest top = heap[0];
    position.erase(top.id);
    if (heap.size() == 1) {
        heap.pop_back();
        return top;
    }
    heap[0] = heap.back();
    position[heap[0].id] = 0;
    heap.pop_back();
    heapifyDown(0);
    return top;
}

void BinaryHeap::update(const string& id, int urgency, int cost) {
    if (!contains(id)) {
        return;
    }

    int index = position[id];
    heap[index].urgency = urgency;
    heap[index].cost = cost;
    heapifyUp(index);
    index = position[id];
    heapifyDown(index);
}

vector<RepairRequest> BinaryHeap::getAll() {
    return heap;
}



bool NetworkRepair::sortPriority(const RepairRequest& a,
                                 const RepairRequest& b) {
    if (a.urgency != b.urgency) {
        return a.urgency > b.urgency;
    }
    if (a.cost != b.cost) {
        return a.cost < b.cost;
    }
    return a.id < b.id;
}

void NetworkRepair::sortPendingRepairs(vector<RepairRequest>& repairs) {
    int n = repairs.size();
    for (int i = 0; i < n; i++) {
        int best = i;
        for (int j = i + 1; j < n; j++) {
            if (sortPriority(repairs[j], repairs[best])) {
                best = j;
            }
        }
        if (best != i) {
            RepairRequest temp = repairs[i];
            repairs[i] = repairs[best];
            repairs[best] = temp;
        }
    }
}

NetworkRepair::NetworkRepair() {
    dsu = nullptr;
    totalCost = 0;
}

void NetworkRepair::loadNetwork(const string& filename) {
    ifstream fin(filename);
    int N, M;
    fin >> N >> M;
    dsu = new DisjointSet(N);
    for (int i = 0; i < M; i++) {
        int u, v;
        fin >> u >> v;
        dsu->unite(u, v);
    }
    fin.close();
}

void NetworkRepair::addRepair(const string& id,
                              int u,
                              int v,
                              int urgency,
                              int cost) {
    if (allRepairIds.count(id)) {
        return;
    }
    allRepairIds.insert(id);
    RepairRequest r;
    r.id = id;
    r.u = u;
    r.v = v;
    r.urgency = urgency;
    r.cost = cost;
    heap.insert(r);
}

void NetworkRepair::updateRepair(const string& id,
                                 int urgency,
                                 int cost) {
    heap.update(id, urgency, cost);
}

void NetworkRepair::printPending() {
    cout << "Pending repairs:" << endl;
    vector<RepairRequest> repairs = heap.getAll();
    if (repairs.empty()) {
        cout << "<Empty>" << endl;
        return;
    }

    sortPendingRepairs(repairs);
    for (const auto& r : repairs) {
        cout << r.id << " "
             << r.u << " "
             << r.v << " urgency="
             << r.urgency << " cost="
             << r.cost << endl;
    }
}

void NetworkRepair::repairNext() {
    if (heap.empty()) {
        cout << "No repairs pending." << endl;
        return;
    }
    RepairRequest r = heap.extractTop();
    if (!dsu->connected(r.u, r.v)) {
        dsu->unite(r.u, r.v);
        totalCost += r.cost;
        cout << "Repaired: " << r.id << endl;
        cout << "Stations: " << r.u << "-" << r.v << endl;
        cout << "Components: "
             << dsu->componentCount() << endl;
    }
    else {
        cout << "Skipped: " << r.id << endl;
        cout << "Stations: " << r.u << "-" << r.v << endl;
        cout << "Components: "
             << dsu->componentCount() << endl;
    }
}

void NetworkRepair::connected(int u, int v) {
    if (dsu->connected(u, v)) {
        cout << "Connected "
             << u << " "
             << v << ": YES" << endl;
    }
    else {
        cout << "Connected "
             << u << " "
             << v << ": NO" << endl;
    }
}

void NetworkRepair::componentSize(int u) {
    cout << "Component size of "
         << u << ": "
         << dsu->componentSize(u) << endl;
}

void NetworkRepair::componentCount() {
    cout << "Component count: "
         << dsu->componentCount() << endl;
}

void NetworkRepair::printTotalCost() {
    cout << "Total cost: "
         << totalCost << endl;
}

void NetworkRepair::processOperations(const string& filename) {
    ifstream fin(filename);
    string line;
    while (getline(fin, line)) {
        if (line.empty()) {
            continue;
        }
        stringstream ss(line);
        string command;
        ss >> command;
        if (command == "ADD_REPAIR") {
            string id;
            int u, v, urgency, cost;
            ss >> id >> u >> v >> urgency >> cost;
            addRepair(id, u, v, urgency, cost);
        }

        else if (command == "UPDATE_REPAIR") {
            string id;
            int urgency, cost;
            ss >> id >> urgency >> cost;
            updateRepair(id, urgency, cost);
        }

        else if (command == "PRINT_PENDING") {
            printPending();
        }
        else if (command == "REPAIR_NEXT") {
            repairNext();
        }
        else if (command == "CONNECTED") {
            int u, v;
            ss >> u >> v;
            connected(u, v);
        }

        else if (command == "COMPONENT_SIZE") {
            int u;
            ss >> u;
            componentSize(u);
        }
        else if (command == "COMPONENT_COUNT") {
            componentCount();
        }
        else if (command == "TOTAL_COST") {
            printTotalCost();
        }
    }
    fin.close();
}
