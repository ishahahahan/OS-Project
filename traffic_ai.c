#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100

typedef struct
{
    int capacity;
    int initialTraffic;
    int vehicleNumber[3]; // 0: normal, 1: ambulance, 2: fire engine
    int incoming[MAX_NODES];
    int outgoing[MAX_NODES];
    int accessToken;
} WaitNode;

typedef struct
{
    char lastAccess;
    double priorityOfWaitNode[MAX_NODES];
} ControllerNode;


double calculatePriority(WaitNode *waitNodes, int numNodes, int node)
{
    double priority = 0.0;
    for (int i = 0; i < 3; i++)
    {
        priority += (i + 1) * waitNodes[node].vehicleNumber[i];
    }
    return (priority / waitNodes[node].capacity)*100;
}

void allocateTokens(WaitNode *waitNodes, ControllerNode *controllerNodes, int numNodes)
{
    
        if (controllerNodes[0].priorityOfWaitNode[0] > 0)
        {
            int highestPriorityNode = 0;
            for (int j = 1; j < numNodes; j++)
            {
                if(j == 1)  continue;
                if (controllerNodes[0].priorityOfWaitNode[j] > controllerNodes[0].priorityOfWaitNode[highestPriorityNode])
                {
                    highestPriorityNode = j;
                }
            }
            waitNodes[highestPriorityNode].accessToken = 1;
            controllerNodes[0].lastAccess = highestPriorityNode;
            printf("%d", highestPriorityNode);
        }
  
}

void simulateTrafficFlow(int adjacencyMatrix[MAX_NODES][MAX_NODES], int numNodes, WaitNode *waitNodes, ControllerNode *controllerNodes)
{
    for (int i = 0; i < numNodes; i++)
    {
        if(i == 1)  continue; // Skip the empty node
        for (int j = 0; j < numNodes; j++)
        {
            if (i != 1 && j != 1 && adjacencyMatrix[i][j] && waitNodes[i].accessToken)
            {
                // Move vehicles from node i to node j
                int numVehicles = waitNodes[i].initialTraffic < waitNodes[j].capacity ? waitNodes[i].initialTraffic : waitNodes[j].capacity;
                waitNodes[i].initialTraffic -= numVehicles;
                waitNodes[j].initialTraffic += numVehicles;
                waitNodes[i].accessToken = 0;
            }
        }
    }
}


int main()
{
    // Initialize waitNodes and controllerNodes here
    // Then simulate traffic flow and handle token allocation

    int numNodes = 4;

    WaitNode waitNodes[MAX_NODES] = {
        {10, 5, {3, 1, 1}, {0, 0, 0, 0}, {1, 0, 0, 0}, 0},
        {0, 0, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, 0},
        {12, 2, {2, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, 0},
        {8, 0, {0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, 0}};

    int adjacencyMatrix[MAX_NODES][MAX_NODES] = {
        {0, 1, 0, 0},
        {1, 0, 1, 1},
        {0, 1, 0, 0},
        {0, 1, 0, 0}};

    ControllerNode controllerNodes[MAX_NODES];
    controllerNodes[0].lastAccess = -1;
    for (int j = 0; j < numNodes; j++)
    {
        if(j == 1)  continue;
        controllerNodes[0].priorityOfWaitNode[j] = calculatePriority(waitNodes, numNodes, j);
    }

    for (int i = 0; i < numNodes; i++)
    {
        printf("%f\n", controllerNodes[0].priorityOfWaitNode[i]);
    }

    allocateTokens(waitNodes, controllerNodes, numNodes);
    simulateTrafficFlow(adjacencyMatrix, numNodes, waitNodes, controllerNodes);

    for (int i = 0; i < numNodes; i++)
    {
        printf("Node %d: Capacity: %d, Initial Traffic: %d, Normal Vehicles: %d, Ambulance Vehicles: %d, Fire Engine Vehicles: %d\n", i, waitNodes[i].capacity, waitNodes[i].initialTraffic, waitNodes[i].vehicleNumber[0], waitNodes[i].vehicleNumber[1], waitNodes[i].vehicleNumber[2]);
    }

    return 0;
}
