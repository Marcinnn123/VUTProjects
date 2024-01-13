#include <simlib.h>
#include <random>
#include <iostream>
#include <chrono>
#include <getopt.h>

class Truck : public Process {
public:
    void Behavior();
private:
    Store* s;
};

class Load : public Process {
public:
    void Behavior();
private:
    Store* s;
};

const int DEFAULT_NUM_CARS = 310;
unsigned long int NUM_CARS = 700;
const double CAR_CAPACITY = 70.0;  // packages
double TOTAL_MATERIAL = 70000.0; // packages
const double MATERIAL = TOTAL_MATERIAL;
const double WORKING_HOURS = 8.0; // in hours 
double materialCarried = 0.0;
int successRate = 0;
double avg = 0.0;
int notDelivered = 0;


Store* S;

class Timer : public Event {
    Process* Id;
public:
    Timer(Process* p, double tm) : Id(p) {
        Activate(Time + tm);
    }
    void Behavior() {
        delete Id;
        PrintEnd();
        Cancel();
    }

    void PrintEnd() {
        std::cout << "-----------------------" << std::endl;
        std::cout << "End of working hours" << std::endl;
    }
};

void Load::Behavior() {
    double t = Time;
    Event* timeout = new Timer(this, WORKING_HOURS);
    while (TOTAL_MATERIAL > 0) {
        Enter(*S, 1);
        TOTAL_MATERIAL -= CAR_CAPACITY;
        (new Truck())->Activate();
    }
    delete timeout;
    std::cout << "All packages delivered in: " << Time - t << std::endl;
    successRate += 10;
	avg += Time - t;
}

void Truck::Behavior() {
    double wait_time;

    if (Random() < (NUM_CARS - DEFAULT_NUM_CARS) * 100) {
        wait_time = Normal(7.8, 0.2);
        Wait(wait_time);
    } else {
        wait_time = Normal(7.5, 0.4);
        Wait(wait_time); 
    }
    materialCarried += CAR_CAPACITY;

    Leave(*S, 1);
 
}

int main(int argc, char* argv[]) {

    int material = TOTAL_MATERIAL;
    int opt;
    while ((opt = getopt(argc, argv, "c:p:")) != -1) {
        switch (opt) {
        case 'c':
            NUM_CARS = std::atoi(optarg);
            break;
        case 'p':
            TOTAL_MATERIAL = std::atoi(optarg);
            material = TOTAL_MATERIAL;
            break;
        }
    }

	std::cout << "------------------------------------------------------------\n"
		<< "START OF SIMULATION\n"
		<< "------------------------------------------------------------\n"
		<< "Running experiment with " << NUM_CARS << " cars.\n"
		<< "Running experiment with " << TOTAL_MATERIAL << " packages.\n"
		<< "------------------------------------------------------------\n"
		<< std::endl;

    S = new Store("Loading car", NUM_CARS);

    for (int i = 1; i <= 10; i++) {
		std::cout << "----- SIMULATION " << i << "\n";
        Init(0, 24);

        (new Load())->Activate();
        Run();

        if (TOTAL_MATERIAL < 0) {
            TOTAL_MATERIAL = 0.0;
        }
        std::cout << "Packages left: " << TOTAL_MATERIAL << std::endl;
        std::cout << "Delivered: " << materialCarried << std::endl;
		notDelivered += TOTAL_MATERIAL;
		TOTAL_MATERIAL = material;
        materialCarried = 0.0;
		

		std::cout<<"----- END OF SIMULATION "<<i<<std::endl<<std::endl;
    }
	
	int help = (100 - successRate)/10;
	avg = (avg+(8*help))/10;

	std::cout << "------------------------------------------------------------\n"
		<< "SIMULATION FINISHED WITH "<<successRate<<"%."<<std::endl
		<< "AVERAGE TIME WAS "<<avg<<" hours."<<std::endl
		<< "ON AVERGE, THE DRIVERS DID NOT DELIVER "<<notDelivered/10<<" PACKAGES"<<std::endl
		<< "------------------------------------------------------------\n";
	
    delete S; 
    return 0;
}
