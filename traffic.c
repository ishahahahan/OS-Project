#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 20
#define MAX_VEHICLES 100

// Structure to represent a traffic node
typedef struct {
    char node_id; // ID of the node
    char lastAccess[];
} TrafficNode;

// Structure to represent a wait node
typedef struct {
    char node_id; // ID of the node
    int capacity; // Capacity of the wait node
    int vehicles; // Number of vehicles currently present
} WaitNode;

// Structure to represent a vehicle
typedef struct {
    int destination; // Destination node of the vehicle
    int time_to_destination; // Time taken for the vehicle to reach its destination
} Vehicle;

// Function to allocate access tokens to wait nodes from traffic nodes
void allocate_tokens(TrafficNode* traffic_nodes, WaitNode* wait_nodes, int num_nodes) {
    // Simplified token allocation logic
    for (int i = 0; i < num_nodes; i++) {
        if (traffic_nodes[i].vehicles > 0 && wait_nodes[i].vehicles < wait_nodes[i].capacity) {
            int tokens_allocated = traffic_nodes[i].vehicles; // Allocating all vehicles from traffic node
            wait_nodes[i].vehicles += tokens_allocated; // Move vehicles to wait node
            traffic_nodes[i].vehicles = 0; // Reset vehicles count at traffic node
        }
    }
}

// Function to simulate vehicle movement and calculate metrics
void move_vehicles(WaitNode* wait_nodes, Vehicle* vehicles, int* num_vehicles) {
    // Simplified vehicle movement logic
    for (int i = 0; i < *num_vehicles; i++) {
        // Assuming vehicles move at constant speed
        vehicles[i].time_to_destination--;

        // Remove vehicles that reached their destination
        if (vehicles[i].time_to_destination == 0) {
            wait_nodes[vehicles[i].destination].vehicles--;
            // Remove vehicle from the list
            for (int j = i; j < (*num_vehicles - 1); j++) {
                vehicles[j] = vehicles[j + 1];
            }
            (*num_vehicles)--;
            i--; // Adjust index after removing vehicle
        }
    }
}

// Function to calculate average wait time at nodes
double calculate_average_wait_time(WaitNode* wait_nodes, int num_nodes) {
    int total_wait_time = 0;
    int total_allocations = 0;

    // Calculate total wait time and total allocations
    for (int i = 0; i < num_nodes; i++) {
        total_wait_time += wait_nodes[i].vehicles;
        total_allocations += wait_nodes[i].capacity;
    }

    // Calculate average wait time
    if (total_allocations > 0) {
        return (double) total_wait_time / total_allocations;
    } else {
        return 0.0; // Return 0 if there are no allocations to prevent division by zero
    }
}

// Function to calculate average token allocation time for each node
double calculate_average_token_allocation_time(int* token_allocation_times, int num_nodes) {
    double total_time = 0;

    for (int i = 0; i < num_nodes; i++) {
        total_time += token_allocation_times[i];
    }

    return total_time / num_nodes;
}

// Function to calculate time to destination for the first vehicle for each node
int calculate_time_to_destination_first_vehicle(Vehicle* vehicles, int num_vehicles, int destination) {
    int min_time = INT_MAX;

    for (int i = 0; i < num_vehicles; i++) {
        if (vehicles[i].destination == destination && vehicles[i].time_to_destination < min_time) {
            min_time = vehicles[i].time_to_destination;
        }
    }

    return min_time;
}

// Function to calculate time to destination for the last vehicle for each node
int calculate_time_to_destination_last_vehicle(Vehicle* vehicles, int num_vehicles, int destination) {
    int max_time = 0;

    for (int i = 0; i < num_vehicles; i++) {
        if (vehicles[i].destination == destination && vehicles[i].time_to_destination > max_time) {
            max_time = vehicles[i].time_to_destination;
        }
    }

    return max_time;
}

// Function to calculate average throughput for each node
double calculate_average_throughput(WaitNode* wait_nodes, int num_nodes, int time_elapsed) {
    int total_vehicles_processed = 0;

    // Calculate total vehicles processed
    for (int i = 0; i < num_nodes; i++) {
        total_vehicles_processed += (wait_nodes[i].capacity - wait_nodes[i].vehicles);
    }

    // Calculate average throughput
    return (double) total_vehicles_processed / time_elapsed * 60; // Convert to vehicles per minute
}

// Function to calculate average traffic density for each node
double calculate_average_traffic_density(WaitNode* wait_nodes, int num_nodes) {
    int total_filled_capacity = 0;

    // Calculate total filled capacity
    for (int i = 0; i < num_nodes; i++) {
        total_filled_capacity += wait_nodes[i].vehicles;
    }

    // Calculate average traffic density
    return (double) total_filled_capacity / (num_nodes * wait_nodes[0].capacity) * 100; // Convert to percentage
}

int main() {
    FILE *fp;
    fp = fopen("traffic_input.txt", "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    int num_nodes;
    fscanf(fp, "%d%*c", &num_nodes);

    WaitNode* wait_nodes = (WaitNode*) malloc(num_nodes*sizeof(WaitNode));
    char controller_node;
    char* dest_nodes = (char*)malloc(num_nodes * sizeof(char));
    int num_vehicles;

    // Read adjacency matrix to connect edges of the nodes
    int adjacency_matrix[MAX_NODES][MAX_NODES];
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            fscanf(fp, "%d", &adjacency_matrix[i][j]);
        }
    }

    // Read wait node capacities
    for (int i = 0; i < num_nodes; i++) {
        char node_name;
        int capacity;
        fscanf(fp, " %c: %d%*c", &node_name, &capacity);
        wait_nodes[i].node_id = node_name;
        wait_nodes[i].capacity = capacity;
        wait_nodes[i].vehicles = 0;
    }

    TrafficNode traffic_node;
    // Read traffic controller node 
    fscanf(fp, " %c%*c", &controller_node);
    traffic_node.node_id = controller_node;

    // Read initial traffic allocation
    for (int i = 0; i < num_nodes/2; i++) {
        char node_name;
        int vehicles;
        fscanf(fp, " %c: %d%*c", &node_name, &vehicles);
        for(int j = 0; j < num_nodes; j++){
            if(node_name == wait_nodes[j].node_id){
                wait_nodes[j].vehicles = vehicles;
                break;
            }
        }
    }

    // Read destination nodes
    for (int i = 0; i < num_nodes/2; i++) {
        fscanf(fp, " %c%*c", &dest_nodes[i]);
    }

    fclose(fp);

    // Simulation
    int time_elapsed = 0;
    int num_processed_vehicles = 0;
    Vehicle vehicles[MAX_VEHICLES];
    int token_allocation_times[MAX_NODES] = {0}; // To store token allocation time for each node

    // Assuming max time to destination is 10 minutes
    while (time_elapsed < 10 * 60) { // 10 minutes in seconds
        // Token allocation phase
        allocate_tokens(traffic_nodes, wait_nodes, num_nodes);

        // Vehicle movement phase
        move_vehicles(wait_nodes, vehicles, &num_processed_vehicles);

        // Update time to destination for remaining vehicles
        for (int i = 0; i < num_processed_vehicles; i++) {
            vehicles[i].time_to_destination--;
        }

        // Increment token allocation time for each node that has traffic
        for (int i = 0; i < num_nodes; i++) {
            if (traffic_nodes[i].vehicles > 0) {
                token_allocation_times[i]++;
            }
        }

        // Increment time elapsed
        time_elapsed++;
    }

    // Display metrics for each node
    printf("Average Wait Time at Nodes:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %c: %.1f seconds\n", 'A' + i, calculate_average_wait_time(wait_nodes + i, 1));
    }

    printf("Average Token Allocation Time for Each Node:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %c: %.2f seconds\n", 'A' + i, (double) token_allocation_times[i] / (10 * 60)); // Convert to seconds
    }

    printf("Time to Destination for First Vehicle for Each Node:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %c: %d minutes\n", 'A' + i, calculate_time_to_destination_first_vehicle(vehicles, num_processed_vehicles, i) / 60); // Convert to minutes
    }

    printf("Time to Destination for Last Vehicle for Each Node:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %c: %d minutes\n", 'A' + i, calculate_time_to_destination_last_vehicle(vehicles, num_processed_vehicles, i) / 60); // Convert to minutes
    }

    printf("Average Throughput:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %c: %.0f vehicles per minute\n", 'A' + i, calculate_average_throughput(wait_nodes + i, 1, time_elapsed));
    }

    printf("Average Traffic Density:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %c: %.0f%%\n", 'A' + i, calculate_average_traffic_density(wait_nodes + i, 1));
    }

    return 0;
}
