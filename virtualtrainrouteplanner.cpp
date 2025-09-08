#include <iostream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

struct Station {
    string name;
    Station* prev;
    Station* next;
    Station(const string &n) : name(n), prev(nullptr), next(nullptr) {}
};

class Route {
private:
    Station* head;
    Station* tail;
    Station* current;     // pointer to simulate train position
    bool isCircular;

public:
    Route() : head(nullptr), tail(nullptr), current(nullptr), isCircular(false) {}

    ~Route() {
        clear();
    }

    void clear() {
        if (!head) return;
        // If circular, break circle first
        if (isCircular && tail) {
            tail->next = nullptr;
            head->prev = nullptr;
        }
        Station* ptr = head;
        while (ptr) {
            Station* nxt = ptr->next;
            delete ptr;
            ptr = nxt;
        }
        head = tail = current = nullptr;
    }

    bool empty() const {
        return head == nullptr;
    }

    void setCircular(bool c) {
        isCircular = c;
        if (!head) return;
        if (isCircular) {
            head->prev = tail;
            tail->next = head;
        } else {
            // break the links
            head->prev = nullptr;
            tail->next = nullptr;
        }
    }

    void addStationEnd(const string &name) {
        Station* node = new Station(name);
        if (!head) {
            head = tail = node;
            if (isCircular) {
                head->next = head->prev = head;
            }
        } else {
            if (isCircular) {
                // tail -> node -> head, maintain circular links
                node->prev = tail;
                node->next = head;
                tail->next = node;
                head->prev = node;
                tail = node;
            } else {
                node->prev = tail;
                tail->next = node;
                tail = node;
            }
        }
        if (!current) current = head; // set current if first station added
    }

    void addStationBeginning(const string &name) {
        Station* node = new Station(name);
        if (!head) {
            head = tail = node;
            if (isCircular) {
                head->next = head->prev = head;
            }
        } else {
            if (isCircular) {
                node->next = head;
                node->prev = tail;
                head->prev = node;
                tail->next = node;
                head = node;
            } else {
                node->next = head;
                head->prev = node;
                head = node;
            }
        }
        if (!current) current = head;
    }

    bool insertAfter(const string &afterName, const string &name) {
        Station* p = findStationPtr(afterName);
        if (!p) return false;
        Station* node = new Station(name);
        if (isCircular && p == tail) {
            // inserting after tail: new tail
            node->prev = tail;
            node->next = head;
            tail->next = node;
            head->prev = node;
            tail = node;
        } else if (!isCircular && p == tail) {
            // normal tail insertion
            node->prev = tail;
            tail->next = node;
            tail = node;
        } else {
            // middle insertion
            Station* nxt = p->next;
            node->next = nxt;
            node->prev = p;
            p->next = node;
            if (nxt) nxt->prev = node;
        }
        return true;
    }

    bool removeStation(const string &name) {
        Station* p = findStationPtr(name);
        if (!p) return false;

        if (p == head && p == tail) {
            // only one node
            delete p;
            head = tail = current = nullptr;
            return true;
        }

        if (isCircular) {
            // update neighbors
            p->prev->next = p->next;
            p->next->prev = p->prev;
            if (p == head) head = p->next;
            if (p == tail) tail = p->prev;
        } else {
            if (p->prev) p->prev->next = p->next;
            if (p->next) p->next->prev = p->prev;
            if (p == head) head = p->next;
            if (p == tail) tail = p->prev;
        }

        // move current if it pointed to deleted node
        if (current == p) {
            current = p->next ? p->next : head;
        }

        delete p;
        return true;
    }

    Station* findStationPtr(const string &name) const {
        if (!head) return nullptr;
        Station* ptr = head;
        if (isCircular) {
            do {
                if (ptr->name == name) return ptr;
                ptr = ptr->next;
            } while (ptr != head);
            return nullptr;
        } else {
            while (ptr) {
                if (ptr->name == name) return ptr;
                ptr = ptr->next;
            }
            return nullptr;
        }
    }

    bool findStation(const string &name) const {
        return findStationPtr(name) != nullptr;
    }

    void displayForward() const {
        if (!head) {
            cout << "[Empty route]\n";
            return;
        }
        cout << "Route (forward): ";
        Station* ptr = head;
        if (isCircular) {
            bool first = true;
            do {
                cout << (first ? "" : " -> ") << ptr->name;
                first = false;
                ptr = ptr->next;
            } while (ptr != head);
        } else {
            while (ptr) {
                cout << ptr->name;
                if (ptr->next) cout << " -> ";
                ptr = ptr->next;
            }
        }
        cout << "\n";
    }

    void displayBackward() const {
        if (!tail) {
            cout << "[Empty route]\n";
            return;
        }
        cout << "Route (backward): ";
        Station* ptr = tail;
        if (isCircular) {
            bool first = true;
            do {
                cout << (first ? "" : " -> ") << ptr->name;
                first = false;
                ptr = ptr->prev;
            } while (ptr != tail);
        } else {
            while (ptr) {
                cout << ptr->name;
                if (ptr->prev) cout << " -> ";
                ptr = ptr->prev;
            }
        }
        cout << "\n";
    }

    void setCurrentAt(const string &name) {
        Station* p = findStationPtr(name);
        if (!p) {
            cout << "Station '" << name << "' not found.\n";
            return;
        }
        current = p;
        cout << "Current position set to '" << current->name << "'.\n";
    }

    void resetCurrentToHead() {
        current = head;
        if (current) cout << "Current position set to head: " << current->name << "\n";
        else cout << "Route is empty.\n";
    }

    void moveNext(int steps = 1) {
        if (!current) {
            cout << "No current station set.\n";
            return;
        }
        for (int i = 0; i < steps; ++i) {
            if (current->next) current = current->next;
            else {
                cout << "Reached end of linear route, cannot move next further.\n";
                return;
            }
        }
        cout << "Now at: " << current->name << "\n";
    }

    void movePrev(int steps = 1) {
        if (!current) {
            cout << "No current station set.\n";
            return;
        }
        for (int i = 0; i < steps; ++i) {
            if (current->prev) current = current->prev;
            else {
                cout << "Reached start of linear route, cannot move previous further.\n";
                return;
            }
        }
        cout << "Now at: " << current->name << "\n";
    }

    void showCurrent() const {
        if (!current) cout << "No current station set.\n";
        else cout << "Current station: " << current->name << "\n";
    }

    void displayDetailed() const {
        cout << "Route details:\n";
        cout << (isCircular ? "Type: Circular\n" : "Type: Linear\n");
        displayForward();
        displayBackward();
        if (current) cout << "Current station: " << current->name << "\n";
    }

    // Utility to create a sample route quickly
    void createSampleRoute() {
        clear();
        addStationEnd("StationA");
        addStationEnd("StationB");
        addStationEnd("StationC");
        addStationEnd("StationD");
        addStationEnd("StationE");
        resetCurrentToHead();
        cout << "Sample route created (A->B->C->D->E).\n";
    }

    // Simulate travel between two named stations (linear calculation)
    void travelBetween(const string &from, const string &to) {
        Station* f = findStationPtr(from);
        Station* t = findStationPtr(to);
        if (!f || !t) {
            cout << "One or both stations not found.\n";
            return;
        }
        // If circular route, we can decide shortest path in steps (both directions)
        if (isCircular) {
            // measure steps forward
            int forwardSteps = 0;
            Station* p = f;
            while (p->name != t->name) {
                p = p->next; forwardSteps++;
                if (p == f) break; // safety
            }
            // measure steps backward
            int backwardSteps = 0;
            p = f;
            while (p->name != t->name) {
                p = p->prev; backwardSteps++;
                if (p == f) break;
            }
            cout << "Travel from " << from << " to " << to << " (circular): choose ";
            if (forwardSteps <= backwardSteps) {
                cout << "forward (" << forwardSteps << " stops).\n";
            } else {
                cout << "backward (" << backwardSteps << " stops).\n";
            }
        } else {
            // linear: find direction if possible
            // try forward
            int forwardSteps = 0;
            Station* p = f;
            while (p && p->name != t->name) {
                p = p->next; forwardSteps++;
            }
            // try backward
            int backwardSteps = 0;
            p = f;
            while (p && p->name != t->name) {
                p = p->prev; backwardSteps++;
            }
            if (p == nullptr) {
                cout << "No path from " << from << " to " << to << " in linear route.\n";
            } else {
                if (forwardSteps < backwardSteps) {
                    cout << "Travel forward " << forwardSteps << " stops from " << from << " to " << to << ".\n";
                } else {
                    cout << "Travel backward " << backwardSteps << " stops from " << from << " to " << to << ".\n";
                }
            }
        }
    }
};

// Helper to read full line after numeric input
void readLineAfterInt() {
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Route route;
    int choice;
    cout << "=== Virtual Train Route Planner ===\n";

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Create sample route (A->B->C->D->E)\n";
        cout << "2. Add station at end\n";
        cout << "3. Add station at beginning\n";
        cout << "4. Insert station after a station\n";
        cout << "5. Remove station\n";
        cout << "6. Display route (forward)\n";
        cout << "7. Display route (backward)\n";
        cout << "8. Toggle circular/linear route\n";
        cout << "9. Set current station\n";
        cout << "10. Reset current to head\n";
        cout << "11. Move current next (steps)\n";
        cout << "12. Move current previous (steps)\n";
        cout << "13. Show current station\n";
        cout << "14. Search station\n";
        cout << "15. Travel between two stations (suggest direction)\n";
        cout << "16. Route details\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            readLineAfterInt();
            cout << "Invalid input. Try again.\n";
            continue;
        }
        readLineAfterInt();

        if (choice == 0) break;

        string a, b;
        int steps;
        switch (choice) {
            case 1:
                route.createSampleRoute();
                break;
            case 2:
                cout << "Station name to add at end: ";
                getline(cin, a);
                route.addStationEnd(a);
                cout << "Added at end.\n";
                break;
            case 3:
                cout << "Station name to add at beginning: ";
                getline(cin, a);
                route.addStationBeginning(a);
                cout << "Added at beginning.\n";
                break;
            case 4:
                cout << "Insert after which station? ";
                getline(cin, a);
                cout << "New station name: ";
                getline(cin, b);
                if (route.insertAfter(a, b)) cout << "Inserted '" << b << "' after '" << a << "'.\n";
                else cout << "Station '" << a << "' not found.\n";
                break;
            case 5:
                cout << "Station name to remove: ";
                getline(cin, a);
                if (route.removeStation(a)) cout << "Removed '" << a << "'.\n";
                else cout << "Station '" << a << "' not found.\n";
                break;
            case 6:
                route.displayForward();
                break;
            case 7:
                route.displayBackward();
                break;
            case 8:
                cout << "Enter 1 for Circular route, 0 for Linear: ";
                int c; cin >> c; readLineAfterInt();
                route.setCircular(c!=0);
                cout << "Route type updated.\n";
                break;
            case 9:
                cout << "Set current to station name: ";
                getline(cin, a);
                route.setCurrentAt(a);
                break;
            case 10:
                route.resetCurrentToHead();
                break;
            case 11:
                cout << "Steps to move forward: ";
                cin >> steps; readLineAfterInt();
                route.moveNext(steps);
                break;
            case 12:
                cout << "Steps to move backward: ";
                cin >> steps; readLineAfterInt();
                route.movePrev(steps);
                break;
            case 13:
                route.showCurrent();
                break;
            case 14:
                cout << "Station name to search: ";
                getline(cin, a);
                if (route.findStation(a)) cout << "Found station '" << a << "'.\n";
                else cout << "Station '" << a << "' not found.\n";
                break;
            case 15:
                cout << "From station: ";
                getline(cin, a);
                cout << "To station: ";
                getline(cin, b);
                route.travelBetween(a, b);
                break;
            case 16:
                route.displayDetailed();
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }

    cout << "Exiting planner. Bye!\n";
    return 0;
}
