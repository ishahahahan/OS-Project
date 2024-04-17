#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VEHICLE_TYPES 3 
#define MAX_NODES 10

typedef struct {
    char vehicleType; 
    int vehicleNumber; 
    int destination; 
    int source; 
    int time; 
} VehicleNode;

typedef struct {
    char nodeID;
    int capacity; 
    int initialTraffic; 
    VehicleNode vehicle[MAX_VEHICLE_TYPES];
    bool accessToken; 
} WaitNode;

typedef struct {
    char nodeID;
    char lastAccess;
    char priorityOfWaitNode[MAX_NODES]; 
    int priorityOfWaitNodeIndex;
} TrafficNode;

int main(){

    int num_nodes = 4;
    int num_traffic_nodes = 1;
    
    WaitNode* waitNode = (WaitNode*) malloc (num_nodes * sizeof(WaitNode));
    TrafficNode* trafficNode = (TrafficNode*) malloc (num_traffic_nodes * sizeof(TrafficNode));
    
    int adjacencyMatrix[MAX_NODES][MAX_NODES] = {
        {0, 1, 0, 0},
        {1, 0, 1, 1},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    };

    for(int i = 0; i < num_nodes; i++){
        char alpha = 'A' + i;
        waitNode[i].nodeID = alpha;
        printf("Capacity of node %c: ", waitNode[i].nodeID);
        scanf("%d", &waitNode[i].capacity);
        if(waitNode[i].capacity == 0){
            trafficNode[0].nodeID = waitNode[i].nodeID;
        }
        printf("Initial traffic of node %c: ", waitNode[i].nodeID);
        scanf("%d", &waitNode[i].initialTraffic);
        waitNode[i].accessToken = false;
    }

    // The wait node whose capacity is 0 is the traffic node
    for(int i = 0; i < num_nodes; i++){
        if(waitNode[i].capacity == 0){
            trafficNode[0].nodeID = waitNode[i].nodeID;
        }
    }

    for(int i = 0; i < num_nodes; i++){
        printf("Node %c\n", waitNode[i].nodeID);
        for(int j = 0; j < num_nodes; j++){
            if(adjacencyMatrix[i][j] == 1){
                printf("Node %c is connected to node %c\n", waitNode[i].nodeID, waitNode[j].nodeID);
            }
        }
    }

    // Given the capacity of the nodes, randomly assign the vehicles to the nodes
    


    
}