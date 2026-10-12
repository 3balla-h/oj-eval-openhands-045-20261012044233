#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

class ElevatorSystem {
private:
    int current_floor;
    bool direction_up;  // true for up, false for down
    
    // Min-heap for requests above current floor
    priority_queue<int, vector<int>, greater<int>> up_requests;
    // Max-heap for requests below current floor  
    priority_queue<int> down_requests;
    
    // Track active requests for efficient cancellation
    unordered_set<int> active_requests;
    
    // Clean up invalid elements from heap
    void cleanup_heap(priority_queue<int, vector<int>, greater<int>>& heap) {
        while (!heap.empty() && active_requests.find(heap.top()) == active_requests.end()) {
            heap.pop();
        }
    }
    
    void cleanup_heap(priority_queue<int>& heap) {
        while (!heap.empty() && active_requests.find(heap.top()) == active_requests.end()) {
            heap.pop();
        }
    }
    
public:
    ElevatorSystem() : current_floor(0), direction_up(true) {}
    
    void add_request(int floor) {
        active_requests.insert(floor);
        if (floor > current_floor) {
            up_requests.push(floor);
        } else {
            down_requests.push(floor);
        }
    }
    
    void cancel_request(int floor) {
        active_requests.erase(floor);
        // Lazy deletion - actual cleanup happens during heap operations
    }
    
    void action() {
        // Clean up both heaps first
        cleanup_heap(up_requests);
        cleanup_heap(down_requests);
        
        if (active_requests.empty()) {
            // No requests, elevator stays still
            return;
        }
        
        if (direction_up) {
            // Try to go up first
            if (!up_requests.empty()) {
                int target = up_requests.top();
                up_requests.pop();
                active_requests.erase(target);
                current_floor = target;
            } else if (!down_requests.empty()) {
                // No up requests, reverse direction
                direction_up = false;
                int target = down_requests.top();
                down_requests.pop();
                active_requests.erase(target);
                current_floor = target;
            }
        } else {
            // Try to go down first
            if (!down_requests.empty()) {
                int target = down_requests.top();
                down_requests.pop();
                active_requests.erase(target);
                current_floor = target;
            } else if (!up_requests.empty()) {
                // No down requests, reverse direction
                direction_up = true;
                int target = up_requests.top();
                up_requests.pop();
                active_requests.erase(target);
                current_floor = target;
            }
        }
    }
    
    int locate() const {
        return current_floor;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    ElevatorSystem elevator;
    string command;
    
    for (int i = 0; i < n; i++) {
        cin >> command;
        
        if (command == "action") {
            elevator.action();
        } else if (command == "add") {
            int floor;
            cin >> floor;
            elevator.add_request(floor);
        } else if (command == "cancel") {
            int floor;
            cin >> floor;
            elevator.cancel_request(floor);
        } else if (command == "locate") {
            cout << elevator.locate() << "\n";
        }
    }
    
    return 0;
}