#include <cstdio>
#include <iostream>

const int MAX_N = 500000;

// Custom min-heap implementation
struct MinHeap {
    int arr[MAX_N + 5];
    int size;
    
    void init() {
        size = 0;
    }
    
    void push(int x) {
        int i, j, temp;
        arr[++size] = x;
        i = size;
        j = size / 2;
        while (j > 0 && arr[i] < arr[j]) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i = j;
            j = i / 2;
        }
    }
    
    int top() {
        return arr[1];
    }
    
    void pop() {
        int i, j, child, temp;
        arr[1] = arr[size];
        size--;
        i = 1;
        while (i * 2 <= size) {
            child = i * 2;
            if (child + 1 <= size && arr[child + 1] < arr[child]) {
                child++;
            }
            if (arr[i] <= arr[child]) break;
            temp = arr[i];
            arr[i] = arr[child];
            arr[child] = temp;
            i = child;
        }
    }
    
    int empty() {
        return size == 0;
    }
};

// Custom max-heap implementation
struct MaxHeap {
    int arr[MAX_N + 5];
    int size;
    
    void init() {
        size = 0;
    }
    
    void push(int x) {
        int i, j, temp;
        arr[++size] = x;
        i = size;
        j = size / 2;
        while (j > 0 && arr[i] > arr[j]) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i = j;
            j = i / 2;
        }
    }
    
    int top() {
        return arr[1];
    }
    
    void pop() {
        int i, j, child, temp;
        arr[1] = arr[size];
        size--;
        i = 1;
        while (i * 2 <= size) {
            child = i * 2;
            if (child + 1 <= size && arr[child + 1] > arr[child]) {
                child++;
            }
            if (arr[i] >= arr[child]) break;
            temp = arr[i];
            arr[i] = arr[child];
            arr[child] = temp;
            i = child;
        }
    }
    
    int empty() {
        return size == 0;
    }
};

struct ElevatorSystem {
    int current_floor;
    int direction_up;  // 1 for up, 0 for down
    
    // Min-heap for requests above current floor
    struct MinHeap up_requests;
    // Max-heap for requests below current floor  
    struct MaxHeap down_requests;
    
    // Track active requests
    int all_requests[MAX_N + 5];
    int request_count;
    int request_active[MAX_N + 5];
    
    // Find if a request is still active
    int is_active(int floor) {
        int i;
        for (i = 0; i < request_count; i++) {
            if (all_requests[i] == floor && request_active[i]) {
                return 1;
            }
        }
        return 0;
    }
    
    // Mark request as inactive
    void set_inactive(int floor) {
        int i;
        for (i = 0; i < request_count; i++) {
            if (all_requests[i] == floor && request_active[i]) {
                request_active[i] = 0;
                break;
            }
        }
    }
    
    // Clean up invalid elements from heap
    void cleanup_up_heap() {
        while (!up_requests.empty() && !is_active(up_requests.top())) {
            up_requests.pop();
        }
    }
    
    void cleanup_down_heap() {
        while (!down_requests.empty() && !is_active(down_requests.top())) {
            down_requests.pop();
        }
    }
    
    // Count active requests
    int count_active() {
        int i, count;
        count = 0;
        for (i = 0; i < request_count; i++) {
            if (request_active[i]) count++;
        }
        return count;
    }
    
    void init() {
        int i;
        current_floor = 0;
        direction_up = 1;
        request_count = 0;
        up_requests.init();
        down_requests.init();
        for (i = 0; i < MAX_N + 5; i++) {
            request_active[i] = 0;
        }
    }
    
    void add_request(int floor) {
        all_requests[request_count] = floor;
        request_active[request_count] = 1;
        request_count++;
        
        if (floor > current_floor) {
            up_requests.push(floor);
        } else {
            down_requests.push(floor);
        }
    }
    
    void cancel_request(int floor) {
        set_inactive(floor);
    }
    
    void action() {
        int target;
        
        // Clean up both heaps first
        cleanup_up_heap();
        cleanup_down_heap();
        
        if (count_active() == 0) {
            // No requests, elevator stays still
            return;
        }
        
        if (direction_up) {
            // Try to go up first
            if (!up_requests.empty()) {
                target = up_requests.top();
                up_requests.pop();
                set_inactive(target);
                current_floor = target;
            } else if (!down_requests.empty()) {
                // No up requests, reverse direction
                direction_up = 0;
                target = down_requests.top();
                down_requests.pop();
                set_inactive(target);
                current_floor = target;
            }
        } else {
            // Try to go down first
            if (!down_requests.empty()) {
                target = down_requests.top();
                down_requests.pop();
                set_inactive(target);
                current_floor = target;
            } else if (!up_requests.empty()) {
                // No down requests, reverse direction
                direction_up = 1;
                target = up_requests.top();
                up_requests.pop();
                set_inactive(target);
                current_floor = target;
            }
        }
    }
    
    int locate() {
        return current_floor;
    }
};

int main() {
    int n, i, floor;
    char command[20];
    struct ElevatorSystem elevator;
    
    scanf("%d", &n);
    elevator.init();
    
    for (i = 0; i < n; i++) {
        scanf("%s", command);
        
        if (command[0] == 'a' && command[1] == 'c') {  // action
            elevator.action();
        } else if (command[0] == 'a' && command[1] == 'd') {  // add
            scanf("%d", &floor);
            elevator.add_request(floor);
        } else if (command[0] == 'c') {  // cancel
            scanf("%d", &floor);
            elevator.cancel_request(floor);
        } else if (command[0] == 'l') {  // locate
            printf("%d\n", elevator.locate());
        }
    }
    
    return 0;
}