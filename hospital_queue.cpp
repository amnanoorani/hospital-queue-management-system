#include <iostream>
#include <string>
using namespace std;

const int MAX = 100;
const int NUM_DOCTORS = 3;

int getInt(string message, int low, int high) {
    int x;
    while (true) {
        cout << message;
        if (cin >> x && x >= low && x <= high) {
            return x;
        }
        cout << "Wrong input, try again.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

class Patient {
private:
    int token;
    string name;
    int age;
    int urgency;
    int arrival;

public:
    Patient() {
        token = 0;
        name = "";
        age = 0;
        urgency = 3;
        arrival = 0;
    }

    Patient(int token, string name, int age, int urgency, int arrival) {
        this->token = token;
        this->name = name;
        this->age = age;
        this->urgency = urgency;
        this->arrival = arrival;
    }

    int getToken() { return token; }
    string getName() { return name; }
    int getAge() { return age; }
    int getUrgency() { return urgency; }
    int getArrival() { return arrival; }

    string urgencyName() {
        if (urgency == 1) return "Critical";
        if (urgency == 2) return "Serious";
        return "Normal";
    }

    int treatTime() {
        if (urgency == 1) return 30;
        if (urgency == 2) return 20;
        return 10;
    }
};

class PriorityQueue {
private:
    Patient items[MAX];
    int count;

public:
    PriorityQueue() {
        count = 0;
    }

    bool isEmpty() { return count == 0; }
    bool isFull() { return count == MAX; }
    int size() { return count; }

    // keeps the queue sorted by urgency, same urgency stays in arrival order
    void enqueue(Patient p) {
        int pos = count;
        while (pos > 0 && items[pos - 1].getUrgency() > p.getUrgency()) {
            items[pos] = items[pos - 1];
            pos--;
        }
        items[pos] = p;
        count++;
    }

    Patient dequeue() {
        Patient first = items[0];
        for (int i = 1; i < count; i++) {
            items[i - 1] = items[i];
        }
        count--;
        return first;
    }

    void display(int now) {
        if (isEmpty()) {
            cout << "No patient is waiting.\n";
            return;
        }
        cout << "Token\tName\tAge\tUrgency\t\tWaiting\n";
        for (int i = 0; i < count; i++) {
            cout << items[i].getToken() << "\t" << items[i].getName() << "\t"
                 << items[i].getAge() << "\t" << items[i].urgencyName() << "\t"
                 << now - items[i].getArrival() << " min\n";
        }
    }
};

class Doctor {
private:
    string name;
    bool busy;
    int timeLeft;
    int seen;

public:
    Doctor() {
        name = "";
        busy = false;
        timeLeft = 0;
        seen = 0;
    }

    Doctor(string name) {
        this->name = name;
        busy = false;
        timeLeft = 0;
        seen = 0;
    }

    string getName() { return name; }
    bool isBusy() { return busy; }
    int getSeen() { return seen; }

    void startPatient(int minutes) {
        busy = true;
        timeLeft = minutes;
        seen++;
    }

    void passMinute() {
        if (busy) {
            timeLeft--;
            if (timeLeft == 0) {
                busy = false;
            }
        }
    }

    void show() {
        cout << "Dr. " << name << ": ";
        if (busy) {
            cout << "busy, " << timeLeft << " min left";
        } else {
            cout << "free";
        }
        cout << " (patients seen: " << seen << ")\n";
    }
};

class Hospital {
private:
    PriorityQueue waiting;
    Doctor doctors[NUM_DOCTORS];
    int now;
    int nextToken;
    int treated;
    int totalWait;
    int maxWait;

    int findFreeDoctor() {
        int best = -1;
        for (int i = 0; i < NUM_DOCTORS; i++) {
            if (!doctors[i].isBusy()) {
                if (best == -1 || doctors[i].getSeen() < doctors[best].getSeen()) {
                    best = i;
                }
            }
        }
        return best;
    }

    void assignDoctors() {
        while (!waiting.isEmpty()) {
            int d = findFreeDoctor();
            if (d == -1) {
                return;
            }
            Patient p = waiting.dequeue();
            int wait = now - p.getArrival();

            doctors[d].startPatient(p.treatTime());
            treated++;
            totalWait += wait;
            if (wait > maxWait) {
                maxWait = wait;
            }

            cout << "Token " << p.getToken() << " (" << p.getName() << ", "
                 << p.urgencyName() << ") goes to Dr. " << doctors[d].getName()
                 << ", waited " << wait << " min\n";
        }
    }

public:
    Hospital() {
        doctors[0] = Doctor("Ahmed");
        doctors[1] = Doctor("Sana");
        doctors[2] = Doctor("Bilal");
        now = 0;
        nextToken = 1;
        treated = 0;
        totalWait = 0;
        maxWait = 0;
    }

    void addPatient() {
        if (waiting.isFull()) {
            cout << "Queue is full.\n";
            return;
        }
        string name;
        cout << "Name: ";
        cin >> name;
        int age = getInt("Age: ", 0, 120);
        cout << "1 = Critical, 2 = Serious, 3 = Normal\n";
        int urgency = getInt("Urgency: ", 1, 3);

        Patient p(nextToken++, name, age, urgency, now);
        waiting.enqueue(p);
        cout << "Token " << p.getToken() << " given to " << name << "\n";
        assignDoctors();
    }

    void passTime() {
        int minutes = getInt("Minutes to pass: ", 1, 480);
        for (int m = 0; m < minutes; m++) {
            now++;
            for (int i = 0; i < NUM_DOCTORS; i++) {
                doctors[i].passMinute();
            }
            assignDoctors();
        }
        cout << "Time is now " << now << " min\n";
    }

    void showWaiting() {
        waiting.display(now);
    }

    void showDoctors() {
        for (int i = 0; i < NUM_DOCTORS; i++) {
            doctors[i].show();
        }
    }

    void showStats() {
        cout << "Time: " << now << " min\n";
        cout << "Patients waiting: " << waiting.size() << "\n";
        cout << "Patients treated: " << treated << "\n";
        if (treated > 0) {
            cout << "Average wait: " << (double)totalWait / treated << " min\n";
            cout << "Longest wait: " << maxWait << " min\n";
        }
    }
};

int main() {
    Hospital hospital;
    int choice;

    do {
        cout << "\n--- Hospital Queue System ---\n";
        cout << "1. Add patient\n";
        cout << "2. Show waiting patients\n";
        cout << "3. Show doctors\n";
        cout << "4. Pass time\n";
        cout << "5. Statistics\n";
        cout << "0. Exit\n";
        choice = getInt("Enter choice: ", 0, 5);

        if (choice == 1) hospital.addPatient();
        else if (choice == 2) hospital.showWaiting();
        else if (choice == 3) hospital.showDoctors();
        else if (choice == 4) hospital.passTime();
        else if (choice == 5) hospital.showStats();
    } while (choice != 0);

    cout << "Goodbye\n";
    return 0;
}
