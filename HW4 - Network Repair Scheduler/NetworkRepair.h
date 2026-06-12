//
//  NetworkRepair.h
//  cs300 hw4
//
//  Created by nehir ceylan on 5/8/26.
//
#ifndef NETWORK_REPAIR_H
#define NETWORK_REPAIR_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct RepairRequest {
    string id;
    int u;
    int v;
    int urgency;
    int cost;
};

class DisjointSet {
private:
    vector<int> parent;
    vector<int> sz;
    int components;

public:
    DisjointSet(int n);
    int find(int x);
    bool unite(int a, int b);
    bool connected(int a, int b);
    int componentSize(int x);
    int componentCount();
};

class BinaryHeap {
private:
    vector<RepairRequest> heap;
    unordered_map<string, int> position;
    bool higherPriority(const RepairRequest& a, const RepairRequest& b);
    void heapifyUp(int index);
    void heapifyDown(int index);
    void swapNodes(int i, int j);

public:
    bool empty();
    bool contains(const string& id);
    void insert(const RepairRequest& r);
    RepairRequest extractTop();
    void update(const string& id, int urgency, int cost);
    vector<RepairRequest> getAll();
};

class NetworkRepair {
private:
    DisjointSet* dsu;
    BinaryHeap heap;
    unordered_set<string> allRepairIds;
    long long totalCost;
    static bool sortPriority(const RepairRequest& a, const RepairRequest& b);
    void sortPendingRepairs(vector<RepairRequest>& repairs);

public:
    NetworkRepair();

    void loadNetwork(const string& filename);
    void processOperations(const string& filename);
    void addRepair(const string& id, int u, int v, int urgency, int cost);
    void updateRepair(const string& id, int urgency, int cost);
    void printPending();
    void repairNext();
    void connected(int u, int v);
    void componentSize(int u);
    void componentCount();
    void printTotalCost();
};

#endif
