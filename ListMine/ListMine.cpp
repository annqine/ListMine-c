#include <iostream>
#include <string>

typedef void* LISTPOSITION;

template <class T>
class List
{
protected:
    class Node
    {
    protected:
        T data;
        Node* next;
        Node* prev;

    public:
        friend class List;
        Node(T data, Node* prev = nullptr, Node* next = nullptr) : data(data), prev(prev), next(next) {};
        T& getData() { return data; }
        Node* getNext() { return next; }
        Node* getPrev() { return prev; }

        ~Node() {};
    };
protected:
    Node* head;
    Node* tail;
    size_t count;

public:
    List() : head(nullptr), tail(nullptr), count(0) {}
    ~List() { RemoveAll(); }
    size_t getCount() { return count; }

    LISTPOSITION getHead() { return (LISTPOSITION)head; }
    LISTPOSITION getTail() { return (LISTPOSITION)tail; }

    T& getNext(LISTPOSITION& t) {
        T& data = ((Node*)t)->getData();
        t = ((Node*)t)->getNext();
        return data;
    }

    void AddTail(T d) {
        Node* t = new Node(d, tail);
        if (tail) tail->next = t;
        tail = t;
        if (!head) head = t;
        count++;
    }

    void RemoveAll() {
        Node* t = head;
        Node* n;
        while (t) {
            n = t->getNext();
            delete t;
            t = n;
        }
        head = tail = nullptr;
        count = 0;
    }

    LISTPOSITION Find(T d) {
        Node* t = head;
        while (t) {
            if (t->getData() == d) {
                return (LISTPOSITION)t;
            }
            t = t->getNext();
        }
        return nullptr;
    }
    void RemoveAt(LISTPOSITION pos) {
        Node* current = (Node*)pos;
        Node* prevNode = current->getPrev();
        Node* nextNode = current->getNext();

        if (prevNode) {
            prevNode->next = nextNode;
        }
        else {
            head = nextNode;
        }

        if (nextNode) {
            nextNode->prev = prevNode;
        }
        else {
            tail = prevNode;
        }

        delete current;
        count--;
    }

};

class Bus
{
public:
    int number;
    std::string driverName;
    int routeNumber;
    bool onRoute; // true - на маршруті, false - в парку

    Bus(int num, const std::string& name, int route) : number(num), driverName(name), routeNumber(route), onRoute(false) {}

    operator LISTPOSITION() const {
        return (LISTPOSITION)this;
    }

    int GetBusNumber() const { return number; }

    void ToggleRouteStatus() {
        onRoute = !onRoute;
    }
};

class BusPark : public List<Bus>
{
public:

    LISTPOSITION FindBus(int busNumber) {
        LISTPOSITION pos = getHead();
        while (pos) {
            Bus& bus = *((Bus*)pos);
            std::cout << "Checking bus: " << bus.number << std::endl;
            if (bus.number == busNumber) {
                std::cout << "Bus found!" << std::endl;
                return pos;
            }
            pos = ((Node*)pos)->getNext();
        }
        std::cout << "Bus not found!" << std::endl;
        return nullptr;
    }



    void BusLeavesPark(int busNumber) {
        LISTPOSITION pos = FindBus(busNumber);
        if (pos) {
            Bus& bus = *((Bus*)pos);
            if (!bus.onRoute) {
                std::cout << "Bus " << bus.number << " is already in the park.\n";
            }
            else {
                bus.onRoute = false;
                std::cout << "Bus " << bus.number << " left the park.\n";
            }
        }
        else {
            std::cout << "Bus with number " << busNumber << " not found.\n";
        }
    }

    void BusEntersPark(int busNumber) {
        LISTPOSITION pos = FindBus(busNumber);
        if (pos) {
            Bus& bus = *((Bus*)pos);
            if (bus.onRoute) {
                std::cout << "Bus " << bus.number << " is already on route.\n";
            }
            else {
                bus.onRoute = true;
                std::cout << "Bus " << bus.number << " entered the park.\n";
            }
        }
        else {
            std::cout << "Bus with number " << busNumber << " not found.\n";
        }
    }

    void PrintBusesOnRoute() {
        LISTPOSITION pos = getHead();
        std::cout << "Buses on route:\n";
        while (pos) {
            Bus& bus = *((Bus*)pos);
            if (bus.onRoute) {
                std::cout << "Bus number: " << bus.number << ", Driver: " << bus.driverName << ", Route: " << bus.routeNumber << "\n";
            }
            pos = ((Node*)pos)->getNext();
        }
    }

    void PrintBusesInPark() {
        LISTPOSITION pos = getHead();
        std::cout << "Buses in the park:\n";
        while (pos) {
            Bus& bus = *((Bus*)pos);
            if (!bus.onRoute) {
                std::cout << "Bus number: " << bus.number << ", Driver: " << bus.driverName << ", Route: " << bus.routeNumber << "\n";
            }
            pos = ((Node*)pos)->getNext();
        }
    }

    void RemoveBus(LISTPOSITION pos) {
        if (pos) {
            RemoveAt(pos);
            std::cout << "Bus has been removed.\n";
        }
        else {
            std::cout << "Bus not found.\n";
        }
    }
};

#include <iostream>

void ShowMainMenu();
void ShowBusParkMenu(BusPark& busPark);

int main() {
    BusPark busPark;

    // Изначально добавим 5 автобусов
    busPark.AddTail(Bus(1, "John Doe", 101));
    busPark.AddTail(Bus(2, "Jane Smith", 102));
    busPark.AddTail(Bus(3, "Bob Johnson", 103));
    busPark.AddTail(Bus(4, "Alice Brown", 104));
    busPark.AddTail(Bus(5, "Charlie White", 105));

    int choice;
    do {
        ShowMainMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            ShowBusParkMenu(busPark);
            break;
        case 2:
            // Другие возможные пункты меню
            std::cout << "Exiting the program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please enter a valid option.\n";
            break;
        }

    } while (choice != 2);

    return 0;
}

void ShowMainMenu() {
    std::cout << "=== Main Menu ===\n";
    std::cout << "1. Bus Park Menu\n";
    std::cout << "2. Exit\n";
}

void ShowBusParkMenu(BusPark& busPark) {
    int choice = 0;
    LISTPOSITION pos = nullptr;

    do {
        std::cout << "\n=== Bus Park Menu ===\n";
        std::cout << "1. Print Buses in the Park\n";
        std::cout << "2. Print Buses on Route\n";
        std::cout << "3. Bus Leaves Park\n";
        std::cout << "4. Bus Enters Park\n";
        std::cout << "5. Add Bus\n";
        std::cout << "6. Remove Bus\n";
        std::cout << "7. Change Route Status\n";
        std::cout << "8. Back to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::string newDriverName;

        switch (choice) {
        case 1:
            busPark.PrintBusesInPark();
            break;
        case 2:
            busPark.PrintBusesOnRoute();
            break;
        case 3: {
            int leavingBusNumber;
            std::cout << "Enter the bus number leaving the park: ";
            std::cin >> leavingBusNumber;
            LISTPOSITION pos = busPark.Find(Bus(leavingBusNumber, "", 0));
            if (pos) {
                Bus& leavingBus = *((Bus*)pos);
                busPark.BusLeavesPark(leavingBus.number);
            }
            else {
                std::cout << "Bus not found.\n";
            }
            break;
        }
        case 4: {
            int enteringBusNumber;
            std::cout << "Enter the bus number entering the park: ";
            std::cin >> enteringBusNumber;
            LISTPOSITION pos = busPark.FindBus(enteringBusNumber);
            if (pos) {
                Bus& enteringBus = *((Bus*)pos);
                busPark.BusEntersPark(enteringBus.GetBusNumber());
            }
            else {
                std::cout << "Bus not found.\n";
            }
            break;
        }
        case 5: {
            int newBusNumber, newRouteNumber;
            std::cout << "Enter details for the new bus:\n";
            std::cout << "Bus Number: ";
            std::cin >> newBusNumber;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Driver Name: ";
            std::getline(std::cin, newDriverName);

            std::cout << "Route Number: ";

            while (!(std::cin >> newRouteNumber)) {
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                std::cout << "Invalid input. Enter a numeric value for Route Number: ";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            busPark.AddTail(Bus(newBusNumber, newDriverName, newRouteNumber));
            std::cout << "Bus added successfully.\n";
            break;
        }
        case 6: {
            int removeBusNumber;
            std::cout << "Enter the bus number to remove: ";
            std::cin >> removeBusNumber;
            LISTPOSITION pos = busPark.FindBus(removeBusNumber);
            if (pos) {
                busPark.RemoveBus(pos);
                std::cout << "Bus removed successfully.\n";
            }
            else {
                std::cout << "Bus not found.\n";
            }
            break;
        }

        case 7: {
            int changeRouteBusNumber;
            std::cout << "Enter the bus number to change route status: ";
            std::cin >> changeRouteBusNumber;
            LISTPOSITION pos = busPark.FindBus(changeRouteBusNumber);
            if (pos) {
                Bus& bus = *((Bus*)pos);
                bus.onRoute = !bus.onRoute;
                std::cout << "Route status changed successfully.\n";
            }
            else {
                std::cout << "Bus not found.\n";
            }
            break;
        }
        case 8:
            std::cout << "Returning to Main Menu.\n";
            break;
        default:
            std::cout << "Invalid choice. Please enter a valid option.\n";
            break;
        }

    } while (choice != 8);
}
