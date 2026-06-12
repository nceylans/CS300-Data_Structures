//
//  source.cpp
//  cs300 hw4
//
//  Created by nehir ceylan on 5/8/26.
//
#include "NetworkRepair.h"

int main() {
    NetworkRepair system;
    system.loadNetwork("network.txt");
    system.processOperations("operations.txt");
    return 0;
}
