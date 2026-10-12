#include <cstdio>
#include <iostream>

using namespace std;

const int MAX_N = 500000;

// Custom min-heap implementation
struct MinHeap {
    int arr[MAX_N + 5];
    int size;
    
    MinHeap() : size(0) {}
    
    void push(int x) {
        arr[++size] = x;
        int i = size;
        while (i > 1 && arr[i] < arr[i / 2]) {
            int temp = arr[i];
            arr[i] = arr[i / 2];
            arr[i / 2] = temp;
            i /= 2;
        }
    }
    
    int top() {
        return arr[1];
    }
    
    void pop() {
        arr[1] = arr[size--];
        int i = 1;
        while (i * 2 <= size) {
            int child = i * 2;
            if (child + 1 <= size && arr[child + 1] < arr[child]) {
                child++;
            }
            if (arr[i] <= arr[child]) break;
            int temp = arr[i];
            arr[i] = arr[child];
            arr[child] = temp;
            i = child;
        }
    }
    
    bool empty() {
        return size == 0;
    }
};

// Custom max-heap implementation
struct MaxHeap {
    int arr[MAX_N + 5];
    int size;
    
    MaxHeap() : size(0) {}
    
    void push(int x) {
        arr[++size] = x;
        int i = size;
        while (i > 1 && arr[i] > arr[i / 2]) {
            int temp = arr[i];
            arr[i] = arr[i / 2];
            arr[i / 2] = temp;
            i /= 2;
        }
    }
    
    int top() {
        return arr[1];
    }
    
    void pop() {
        arr[1] = arr[size--];
        int i = 1;
        while (i * 2 <= size) {
            int child = i * 2;
            if (child + 1 <= size && arr[child + 1] > arr[child]) {
                child++;
            }
            if (arr[i] >= arr[child]) break;
            int temp = arr[i];
            arr[i] = arr[child];
            arr[child] = temp;
            i = child;
        }
    }
    
    bool empty() {
        return size == 0;
    }
};

class ElevatorSystem {
private:
    int current_floor;
    bool direction_up;  // true for up, false for down
    
    // Min-heap for requests above current floor
    MinHeap up_requests;
    // Max-heap for requests below current floor  
    MaxHeap down_requests;
    
    // Track active requests - since we need to handle up to 5e5 operations
    // and floor values can be up to 1e9, we'll use a simple approach
    // with two arrays to store all requests and mark them as active/inactive
    int all_requests[MAX_N + 5];
    int request_count;
    bool request_active[MAX_N + 5];
    
    // Find if a request is still active
    bool is_active(int floor) {
        for (int i = 0; i < request_count; i++) {
            if (all_requests[i] == floor && request_active[i]) {
                return true;
            }
        }
        return false;
    }
    
    // Mark request as inactive
    void set_inactive(int floor) {
        for (int i = 0; i < request_count; i++) {
            if (all_requests[i] == floor && request_active[i]) {
                request_active[i] = false;
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
        int count = 0;
        for (int i = 0; i < request_count; i++) {
            if (request_active[i]) count++;
        }
        return count;
    }
    
public:
    ElevatorSystem() : current_floor(0), direction_up(true), request_count(0) {
        for (int i = 0; i < MAX_N + 5; i++) {
            request_active[i] = false;
        }
    }
    
    void add_request(int floor) {
        all_requests[request_count] = floor;
        request_active[request_count] = true;
        request_count++;
        
        if (floor > current_floor) {
            up_requests.push(floor);
        } else {
            down_requests.push(floor);
        }
    }
    
    void cancel_request(int floor) {
        set_inactive(floor);
        // Lazy deletion - actual cleanup happens during heap operations
    }
    
    void action() {
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
                int target = up_requests.top();
                up_requests.pop();
                set_inactive(target);
                current_floor = target;
            } else if (!down_requests.empty()) {
                // No up requests, reverse direction
                direction_up = false;
                int target = down_requests.top();
                down_requests.pop();
                set_inactive(target);
                current_floor = target;
            }
        } else {
            // Try to go down first
            if (!down_requests.empty()) {
                int target = down_requests.top();
                down_requests.pop();
                set_inactive(target);
                current_floor = target;
            } else if (!up_requests.empty()) {
                // No down requests, reverse direction
                direction_up = true;
                int target = up_requests.top();
                up_requests.pop();
                set_inactive(target);
                current_floor = target;
            }
        }
    }
    
    int locate() const {
        return current_floor;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    
    ElevatorSystem elevator;
    char command[20];
    
    for (int i = 0; i < n; i++) {
        scanf("%s", command);
        
        if (command[0] == 'a' && command[1] == 'c') {  // action
            elevator.action();
        } else if (command[0] == 'a' && command[1] == 'd') {  // add
            int floor;
            scanf("%d", &floor);
            elevator.add_request(floor);
        } else if (command[0] == 'c') {  // cancel
            int floor;
            scanf("%d", &floor);
            elevator.cancel_request(floor);
        } else if (command[0] == 'l') {  // locate
            printf("%d\n", elevator.locate());
        }
    }
    
    return 0;
}