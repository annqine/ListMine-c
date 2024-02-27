#include <iostream>

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
};

class BusPark : public List<Bus>
{
public:
    void BusLeavesPark(int busNumber) {
        LISTPOSITION pos = Find(Bus(busNumber, "", 0));
        if (pos) {
            Bus& bus = *((Bus*)pos);
            bus.onRoute = true;
            std::cout << "Autobus " << bus.number << " left the park.\n";
        }
        else {
            std::cout << "Bus with number " << busNumber << " not found.\n";
        }
    }

    void BusEntersPark(int busNumber) {
        LISTPOSITION pos = Find(Bus(busNumber, "", 0));
        if (pos) {
            Bus& bus = *((Bus*)pos);
            bus.onRoute = false;
            std::cout << "Autobus " << bus.number << " entered the park.\n";
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
};

int main() {
    BusPark busPark;

    // Додаємо тестові дані - три автобуси в парку
    busPark.AddTail(Bus(1, "John Doe", 101));
    busPark.AddTail(Bus(2, "Jane Smith", 102));
    busPark.AddTail(Bus(3, "Bob Johnson", 103));

    // Покажемо інформацію про всі автобуси (початковий стан)
    busPark.PrintBusesInPark();
    busPark.PrintBusesOnRoute();

    // Автобус номер 2 виїжджає з парку
    busPark.BusLeavesPark(2);

    // Автобус номер 1 виїжджає з парку
    busPark.BusLeavesPark(1);

    // Покажемо інформацію про автобуси після виїзду
    busPark.PrintBusesInPark();
    busPark.PrintBusesOnRoute();

    // Автобус номер 3 повертається в парк
    busPark.BusEntersPark(3);

    // Покажемо інформацію про автобуси після в'їзду
    busPark.PrintBusesInPark();
    busPark.PrintBusesOnRoute();

    return 0;
}
