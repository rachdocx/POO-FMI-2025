#include <iostream>
#include <cstring>

#include <vector>
#include <map>
#include <string>
#include <deque>

#include <stdexcept>
#include <limits>
#include <fstream>

#include <unistd.h>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>
#include <ctime>
#include <iomanip>
//am folosit si alocare dinamica si stl si vectori statici
//nu am folosit list, am ales sa folosesc alt stl, adica dequeue ca are mai mult sens pentru proiect

//run with clang++ -std=c++17 -o GestiuneaSTP GestiuneaSTP.cpp
//command+f pentru: "polimforism", "CLASA ABSTRACTA", "virtual", "diamant"
using namespace std;
using namespace chrono;

void addMinutesAndDisplay(const int minutesToAdd) {
    auto now = system_clock::now();
    auto future_time = now + minutes(minutesToAdd);
    time_t future_c = system_clock::to_time_t(future_time);
    cout << put_time(localtime( & future_c), "%H:%M:%S");
}

const int MAX_LENGHT = 256;
atomic < bool > stopLoop(false);
void checkForExit() {
    string input;
    while (true) {
        cin >> input;
        if (input == "exit") { //nu eu am scris asta dar n are legatura cu poo
            stopLoop = true;
            break;
        }
    }
}
class Station {
    float distance; //distanta fata de urmatoarea statie
    string station_name;
    bool change;

    const int station_id;
    static int id_generator;
    public:
        Station(float distance = 1, const string station_name = "N/A", bool change = false): station_id(id_generator++) {
            if(distance <= 0)
              throw invalid_argument("Distance must be positive in station" + station_name);
            this -> distance = distance;
            this -> station_name = station_name;
            this -> change = change;
        }
    //constructor de copiere
    Station(const Station & station): station_id(id_generator++) {
            this -> distance = station.distance;
            this -> station_name = station.station_name;
            this -> change = station.change;
        }
        ~Station() {
            // delete[] station_name;
        }
    float getDistance() {
        return this -> distance;
    }
    string getStationName() {
        return this -> station_name;
    }
    friend istream & operator >> (istream & in, Station & station) {
        string temp_name;
        in >> station.distance >> temp_name >> station.change;
        station.station_name = temp_name;
        if(station.distance <= 0)
          throw invalid_argument("Distance must be positive in station " + station.station_name); //invalid_argument error
        return in;
    }

    friend ostream & operator << (ostream & out,
        const Station & station) {
        out << station.distance << " " << station.station_name << " " << station.change;
        return out;
    }

    Station & operator = (const Station & station) {
        if (this != & station) {
            this -> distance = station.distance;
            this -> station_name = station.station_name;
            this -> change = station.change;
        }
        return * this;
    }
    void showStation() const {
        cout << distance << " " << station_name << " " << change << " " << station_id << endl;
    }
};

class Line {
    deque < Station > stations;
    float length;
    int no_of_stations;
    string line_name;
    public:
        Line(int no_of_stations = 0, float length = 0, const string line_name = "N/A") {
            this -> length = length;
            this -> line_name = line_name;
            this -> no_of_stations = no_of_stations;
        }

        ~Line() {
            stations.clear();
            //se dezaloca singura memoria right dar tot am fct ceva
        }

    Line & operator = (const Line & line) {
        if (this != & line) {
            this -> length = line.length;
            this -> line_name = line.line_name;
            this -> no_of_stations = line.no_of_stations;
            this -> stations = line.stations;
        }
        return * this;
    }
    int getNoOfStations() {
        return this -> no_of_stations;
    }
    Line(const Line & line) {
        this -> length = line.length;
        this -> line_name = line.line_name;
        this -> no_of_stations = line.no_of_stations;
        stations = line.stations;
    }

    void addStationBack(const Station & station) {
        stations.push_back(station);
        no_of_stations++;
    }

    void addStationFront(const Station & station) {
        stations.push_front(station);
        no_of_stations++;
    }
    void deleteStationBack() {
        stations.pop_back();
        no_of_stations--;
    }
    void deleteStationFront() {
        stations.pop_front();
        no_of_stations--;
    }
    void printLine() const {
        for (size_t i = 0; i < stations.size(); i++) {
            cout << stations[i];
            cout << endl;
        }
    }
    //overloading the '+' operator
    string getLineName() {
        return line_name;
    }
    float getStationDistance(const string station_name) {
        for (auto i : stations) {
            if (station_name == i.getStationName()){
                return i.getDistance();
            }
        }
        return 0;
    }
    //basically getStation()
    Station & operator[](int index) {
        if(index < 0 || index >= no_of_stations)
          throw invalid_argument("Index out of bounds"); //invalid_argument
        return stations[index];
    }
    float getLineLenght(int index) {
        return length;
    }
    Station getStationBack() {
        return stations.back();
    }
    Station getStationFront() {
        return stations.front();
    }
    string getDirection() {
        return stations.at(stations.size() - 1).getStationName();
    }
    void showStations(int i) {
        cout << stations.at(i).getStationName();
    }
    Station getStation(int i) {

        return stations[i];
    }
    friend Line operator + (Line & a, Station b) {
        a.length += b.getDistance();
        return a;
    }
    friend Line operator + (Station & b, Line & a) {
        return a + b;
    }
    //overloading the '-' operator
    friend Line operator - (Line & a, Station b) {
        a.length -= b.getDistance();
        return a;
    }
    friend istream & operator >> (istream & in, Line & line) {
        in >> line.no_of_stations >> line.line_name >> line.length;
        line.stations.clear();
        for (int i = 0; i < line.no_of_stations; i++) {
            Station station;
            in >> station;
            line.stations.push_back(station);

        }
        return in;
    }
    friend ostream & operator << (ostream & out,
        const Line & line) {
        out << line.no_of_stations << ' ' << line.line_name << ' ' << line.length << endl;
        for (int i = 0; i < line.no_of_stations; i++) {
            out << line.stations[i] << endl;
        }
        return out;
    }
};
class System {
    string system_name;
    float trip_price;
    vector < Line > lines;
    string file_path;
    public:
        System(const string file_path = "N/A", const string system_name = "N/A", float trip_price = 0) {
            this -> system_name = system_name;
            this -> file_path = file_path;
            this -> trip_price = trip_price;
        }
        System(const System & system) {
            this -> file_path = system.file_path ;
            this -> system_name = system.system_name ;
            this -> trip_price = system.trip_price;
            this -> lines = system.lines;
        }
        ~System() {
//            if (this -> system_name != nullptr) {
//                delete[] this -> system_name;
//            }
//            if (this -> file_path != nullptr) {
//                delete[] this -> file_path;
//            }
        }
    void addLine(const string name) {
        Line line(0, 0, name);
        lines.push_back(line);
    }
    Line getLine(string name) {
        for (int i = 0; i < lines.size(); i++) {
            if (name == lines[i].getLineName()){
                cout << lines[i];
                return lines[i];
            }
        }
        return Line();
    }
    void deleteLine(const string name) {
        for (int i = 0; i < lines.size(); i++) {
            if (name == lines[i].getLineName()) {
                lines.erase(lines.begin() + i);
                break;
            }
        }
    }
    void addToLineBack(const Station & station, const string name) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].getLineName() == name) {
                lines[i].addStationBack(station);
                //lines[i] = lines[i] + station;
                return;
            }
        }
    }
    void deleteFromLineBack(const string name) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].getLineName() == name) {
                Station temp = lines[i].getStationBack();
                lines[i].deleteStationBack();
                //lines[i] = lines[i] - temp;
                return;
            }
        }
    }
    void deleteFromLineFront(const string name) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].getLineName() == name) {
                Station temp = lines[i].getStationFront();
                lines[i].deleteStationFront();
                //lines[i] = lines[i] - temp;
                return;
            }
        }
    }
    void addToLineFront(const Station & station, const string name) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].getLineName() == name) {
                lines[i].addStationFront(station);
                //lines[i] = lines[i] + station;
                return;
            }
        }
    }
    Line & operator[](const int i) {
        return lines[i];
    }
    int getLineLenght1(const string name) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].getLineName() == name) {
                return lines[i].getLineLenght(i);
            }
        }
        return 0;
    }
    System & operator = (const System & system) {
          if (this != &system) {
            this -> file_path = system.file_path;
            this -> system_name = system.system_name;
            this -> trip_price = system.trip_price;
            this->lines = system.lines;
          }
            return * this;
    }
    friend ostream & operator << (ostream & out,
        const System & system) {
        out << system.system_name << ' ' << system.trip_price << endl;
        for (int i = 0; i < system.lines.size(); i++) {
            out << system.lines[i] << endl;
        }
        return out;
    }
    friend istream & operator >> (istream & in, System & system) {
        string temp_name;
        in >> temp_name >> system.trip_price;
        system.system_name = temp_name;
        system.lines.clear();
        Line temp;
        while (in >> temp) {
            system.lines.push_back(temp);
        }
        if (in.eof() && !in.bad()) {
            in.clear();
        }
        return in;
    }
    //preturile doar se scumpesc nu si scad, evident lol
    System & operator++() {
        trip_price++;
        return * this;
    }
    const string getName() {
        return this -> system_name;
    }

    System operator++(int) {
        System temp = * this;
        trip_price++;
        return temp;
    }
    void loadSystem() {
        ifstream f;
        try{
          f.open(file_path);
          if (!f.is_open()) {
            throw runtime_error("Error opening file" + file_path);
          }
          f >> *this;
          if(f.fail()){
            throw runtime_error("Error reading file" + file_path);
          }
          f.close();
        }
        catch(const runtime_error & e) {
            cout << e.what() << endl;
            if(f.is_open()){
              f.close();
            }
        }
    }
    void saveSystem() {
        ofstream f;
        try{
          f.open(file_path);
          if (!f.is_open()) {
            throw runtime_error("Error opening file" + file_path);
          }
          f << *this;
          if(f.fail()){
            throw runtime_error("Error writing file" + file_path);
          }
          f.close();
        }
        catch(const runtime_error & e) {
          cout << e.what() << endl;
          if(f.is_open()){
            f.close();
          }
        }

    }
};

//CLASA ABSTRACTA care va defini urmatoarele clase
class PublicTransport {
    protected:
        string PublicTransport_name;
        float average_spped;
        string assigned_line;
        int max_capacity;
        int actual_capacity;
    public:
    PublicTransport(const string PublicTransport_name = "N/A", int average_speed = -1, const string assigned_line = "N/A", int max_capacity = -1, int actual_capacity = 0) {
        this -> PublicTransport_name = PublicTransport_name;
        this -> average_spped = average_speed;
        this -> assigned_line = assigned_line;
        this -> max_capacity = max_capacity;
        this -> actual_capacity = actual_capacity;
    }
    PublicTransport(const PublicTransport & PublicTransport) {
        this -> PublicTransport_name = PublicTransport.PublicTransport_name;
        this -> average_spped = PublicTransport.average_spped;
        this -> assigned_line = PublicTransport.assigned_line;
        this -> max_capacity = PublicTransport.max_capacity;
        this -> actual_capacity = PublicTransport.actual_capacity;
    }

    //destructor virtual ca sa pot sterge corect in array ul de la depou
    virtual PublicTransport* clone() const = 0;

    virtual~PublicTransport() {
//        delete[] PublicTransport_name;
//        delete[] assigned_line;
    }
    //operator = virtual
    virtual PublicTransport & operator = (const PublicTransport & other) {
        if (this != & other) {
            PublicTransport_name = other.PublicTransport_name;
            assigned_line = other.assigned_line;
            average_spped = other.average_spped;
            max_capacity = other.max_capacity;
            actual_capacity = other.actual_capacity;
        }
        return * this;
    }

    //nu cred ca mai are vreun sens dupa ce am facut o abstracta dar sure
    //  PublicTransport & operator = (const PublicTransport & PublicTransport) {
    //    if (this != & PublicTransport) {
    //      if (this -> PublicTransport_name != nullptr) {
    //        delete[] this -> PublicTransport_name;
    //      }
    //      this -> PublicTransport_name = new char[strlen(PublicTransport.PublicTransport_name) + 1];
    //      strcpy(this -> PublicTransport_name, PublicTransport.PublicTransport_name);
    //
    //      if (this -> assigned_line != nullptr) {
    //        delete[] this -> assigned_line;
    //      }
    //      this -> assigned_line = new char[strlen(PublicTransport.assigned_line) + 1];
    //      strcpy(this -> assigned_line, PublicTransport.assigned_line);
    //
    //      this -> average_spped = PublicTransport.average_spped;
    //      this -> max_capacity = PublicTransport.max_capacity;
    //      this -> actual_capacity = PublicTransport.actual_capacity;
    //    }
    //    return * this;
    //  }
    //+ virtual dar pe pointeri
    virtual PublicTransport * operator + (int passengers) const = 0;
    //  friend PublicTransport operator + (const PublicTransport & t, int passengers) {
    //    PublicTransport temp = t;
    //    temp.actual_capacity += passengers;
    //    return temp;
    //  }
    //+ cu int comutativ
    //  friend PublicTransport operator + (int passengers,
    //    const PublicTransport & t) {
    //    return t + passengers;
    //  }
    //- nu int
    //- virtuali pe pointeri
    virtual PublicTransport * operator - (int passengers) const = 0;
    //  friend PublicTransport operator - (const PublicTransport & t, int passengers) {
    //    PublicTransport temp = t;
    //    temp.actual_capacity -= passengers;
    //    if (temp.actual_capacity < 0) temp.actual_capacity = 0;
    //    return temp;
    //  }

    //checks if 2 PublicTransports are on the same line
    bool operator == (const PublicTransport & other) const {
        return this -> assigned_line == other.assigned_line;
    }
    //allows adding an integer to a PublicTransport(to add passangers)
    //  PublicTransport operator + (int passengers) {
    //    PublicTransport temp = * this;
    //    temp.actual_capacity += passengers;
    //    return temp;
    //  }
    friend ostream & operator << (ostream & out, const PublicTransport & PublicTransport) {
        out << PublicTransport.PublicTransport_name << " ";
        out << PublicTransport.assigned_line << " ";
        out << PublicTransport.average_spped << " ";
        out << PublicTransport.max_capacity << " ";
        out << PublicTransport.actual_capacity << " ";
        return out;
    }
    friend istream & operator >> (istream & in, PublicTransport & PublicTransport) {
        in >> PublicTransport.PublicTransport_name >> PublicTransport.assigned_line >> PublicTransport.average_spped >> PublicTransport.max_capacity >> PublicTransport.actual_capacity;
        return in;
    }
    void changeLine(string line) {
        assigned_line = line;
    }
    float getTime(Station station) {
        return station.getDistance() * 100 / this -> average_spped;
        //nu este timpul real sau corect, este doar un exemplu la scala 1 minut = o secunda
    }
    string getName() {
        return this -> PublicTransport_name;
    }
    //operator de incrementare ++ (prefixat și postfixat)
    PublicTransport & operator++() {
        if (actual_capacity < max_capacity)
            actual_capacity++;
        return * this;
    }
    virtual PublicTransport * operator++(int) = 0;
    //  PublicTransport operator++(int) {
    //    PublicTransport temp = * this;
    //    if (actual_capacity < max_capacity)
    //      actual_capacity++;
    //    return temp;
    //  }
    //operator de decrementare
    PublicTransport & operator--() {
        if (actual_capacity > 0)
            actual_capacity--;
        return * this;
    }
    virtual PublicTransport * operator--(int) = 0;
    //  PublicTransport operator--(int) {
    //    PublicTransport temp = * this;
    //    if (actual_capacity > 0)
    //      actual_capacity--;
    //    return temp;
    // operator de comparatie
    bool operator < (const PublicTransport & other) const {
        return this -> average_spped < other.average_spped;
    }
    bool operator > (const PublicTransport & other) const {
        return this -> average_spped > other.average_spped;
    }
    //urmeaza cea mai oribila si glitch uita functie din lume
    void setTren(System & system) {
       Line temp_line;
    try {
        temp_line = system.getLine(this->assigned_line);
        if (temp_line.getLineName() == "N/A" || temp_line.getNoOfStations() == 0) {

            throw runtime_error("Line " + this->assigned_line + " not found or is empty in the system for vehicle " + this->PublicTransport_name + ". Cannot simulate ride.");
        }
    } catch (const exception& e) {
        cout << "Error initializing line for simulation: " << e.what() << endl;
        return;
    }

    thread inputThread(checkForExit);
    stopLoop = false;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dista(0, (this->max_capacity > 0 ? (this->max_capacity -1) : 0) / 5 + (this->max_capacity == 0));

    int global_smth = 0;

    try {
        while (!stopLoop) {
            cout << "The " << this->getType() << "'s (" << this->getName() <<") direction is: " << temp_line.getDirection() << endl;
            cout << endl;
            for (int i = 0; i < temp_line.getNoOfStations() - 1; i++) {
                if (stopLoop) break;
                PublicTransport* newPublicTransportState = nullptr;
                try {
                    int randomNumber = dista(gen);
                    newPublicTransportState = (*this) + randomNumber;
                    if (!newPublicTransportState) throw std::bad_alloc();
                    *this = *newPublicTransportState;
                    delete newPublicTransportState;
                    newPublicTransportState = nullptr;

                    int smth = getTime(temp_line[i]);
                    global_smth += smth + 1;
                    cout << "The " << this->getType() << " " << this->getName() << " is leaving: ";
                    temp_line.showStations(i);
                    cout << " and it will arrive at: ";
                    temp_line.showStations(i + 1);
                    cout << " in " << smth << " minutes at exactly: ";
                    addMinutesAndDisplay(global_smth);
                    cout << ". The " << this->getType() << " capacity is: " << this->actual_capacity << "/" << this->max_capacity << endl;

                    this_thread::sleep_for(chrono::seconds(smth));

                    if (stopLoop) break;

                    uniform_int_distribution<> distb_leave(0, this->actual_capacity > 0 ? this->actual_capacity : 0);
                    int randomNumber2_leave = distb_leave(gen);
                    newPublicTransportState = (*this) - randomNumber2_leave;
                    if (!newPublicTransportState)
                      throw bad_alloc();
                    *this = *newPublicTransportState;
                    delete newPublicTransportState;
                    newPublicTransportState = nullptr;

                } catch (const bad_alloc& e) {
                    cout << "Memory allocation error during ride simulation: " << e.what() << endl;
                    delete newPublicTransportState;
                    if (inputThread.joinable())
                      inputThread.join();
                    return;
                } catch (const std::out_of_range& e_line) {
                     cout << "Line access error during ride simulation: " << e_line.what() << endl;
                     delete newPublicTransportState;
                     if (inputThread.joinable())
                       inputThread.join();
                     return;
                } catch (const std::exception& e_sim) {
                    cout << "Simulation step error: " << e_sim.what() << endl;
                    delete newPublicTransportState;
                    if (inputThread.joinable())
                      inputThread.join();
                    return;
                }
            }
            if (stopLoop) break;

            cout << "The " << this->getType() << " " << this->getName() << " arrived at: ";
            temp_line.showStations(temp_line.getNoOfStations() - 1);
            cout << " and is returning to: ";
            temp_line.showStations(0);
            cout << endl << endl << "The " << this->getType() << " " << this->getName() << " direction is: ";
            temp_line.showStations(0);
            cout << endl << endl;

            for (int i = temp_line.getNoOfStations() - 1; i > 0; i--) {
                if (stopLoop) break;
                PublicTransport* newPublicTransportState = nullptr;
                try {
                    int randomNumber = dista(gen);
                    newPublicTransportState = (*this) + randomNumber;
                    if (!newPublicTransportState)
                      throw bad_alloc();
                    *this = *newPublicTransportState;
                    delete newPublicTransportState;
                    newPublicTransportState = nullptr;

                    int smth = getTime(temp_line[i - 1]);
                    global_smth += smth + 1;
                    cout << "The " << this->getType() << " " << this->getName() << " is leaving: ";
                    temp_line.showStations(i);
                    cout << " and it will arrive at: ";
                    temp_line.showStations(i - 1);
                    cout << " in " << smth << " minutes at exactly ";
                    addMinutesAndDisplay(global_smth);
                    cout << ". The " << this->getType() << " capacity is: " << this->actual_capacity << "/" << this->max_capacity << endl;

                    this_thread::sleep_for(chrono::seconds(smth));

                    if (stopLoop) break;

                    uniform_int_distribution<> distb_leave(0, this->actual_capacity > 0 ? this->actual_capacity : 0);
                    int randomNumber2_leave = distb_leave(gen);
                    newPublicTransportState = (*this) - randomNumber2_leave;
                    if (!newPublicTransportState)
                      throw bad_alloc();
                    *this = *newPublicTransportState;
                    delete newPublicTransportState;
                    newPublicTransportState = nullptr;

                } catch (const bad_alloc& e) {
                    cout << "Memory allocation error during ride simulation (return trip): " << e.what() << endl;
                    delete newPublicTransportState;
                    if (inputThread.joinable())
                      inputThread.join();
                    return;
                } catch (const out_of_range& e_line) {
                     cout << "Line access error during ride simulation (return trip): " << e_line.what() << endl;
                     delete newPublicTransportState;
                     if (inputThread.joinable())
                       inputThread.join();
                     return;
                } catch (const exception& e_sim) {
                    cout << "Simulation step error (return trip): " << e_sim.what() << endl;
                    delete newPublicTransportState;
                    if (inputThread.joinable())
                      inputThread.join();
                    return;
                }
            }
             if (stopLoop) break;

            cout << "The " << this->getType() << " " << this->getName() << " arrived at: ";
            temp_line.showStations(0);
            cout << " and is returning to: ";
            temp_line.showStations(temp_line.getNoOfStations() - 1);
            cout << endl << endl;
        }
    } catch (const exception& e_outer) {
        cerr << "Outer simulation loop error: " << e_outer.what() << endl;
    }

    stopLoop = false;
    if (inputThread.joinable()) {
        inputThread.join();
    }
    }
    //metoda pur virtuala pentru clasa abstracta
    virtual const string getType() const = 0;
};

class Metro: public PublicTransport {
    bool is_electric;
    int no_of_wagons;
    public:
      //constructor cu/fara parametrii
        Metro(const char * name = "N/A", int speed = -1,
            const char * line = "N/A",
                int max_cap = -1,
                int actual_cap = 0,
                bool is_electric = false,
                int no_of_wagons = -1): PublicTransport(name, speed, line, max_cap, actual_cap) {
            this -> is_electric = is_electric;
            this -> no_of_wagons = no_of_wagons;
        }
    //constructor de copiere
    Metro(const Metro & orig): PublicTransport(orig) {
        this -> is_electric = orig.is_electric;
        this -> no_of_wagons = orig.no_of_wagons;
    }
    ~Metro() {
    }
    Metro* clone() const {
            return new Metro(*this);
        }
    PublicTransport & operator = (const PublicTransport & other) {
        if (this != & other) {
            const Metro * m = dynamic_cast <
                const Metro * > ( & other);
            if (m) {
                PublicTransport::operator = ( * m); // Apelează operatorul virtual din clasa de bază
                this -> is_electric = m -> is_electric;
                this -> no_of_wagons = m -> no_of_wagons;
            }
        }
        return * this;
    }

    friend istream & operator >> (istream & os, Metro & metro) {
        os >> (PublicTransport & ) metro;
        os >> metro.is_electric;
        os >> metro.no_of_wagons;
        return os;
    }

    friend ostream & operator << (ostream & os, const Metro & metro) {
        os << (PublicTransport & ) metro;
        os << metro.is_electric << " ";
        os << metro.no_of_wagons << " ";
        return os;
    }

    //void print() {
    //  cout << static_cast < PublicTransport > ( * this) << endl;
    //  cout << is_electric << endl;
    //  cout << no_of_wagons << endl;
    //}

    const string getType() const {
        return "Metro";
    }

    PublicTransport * operator++(int) {
        Metro * m = new Metro( * this);
        if (actual_capacity < max_capacity)
            actual_capacity++;
        return m;
    }

    PublicTransport * operator--(int) {
        Metro * m = new Metro( * this);
        if (actual_capacity > 0)
            actual_capacity--;
        return m;
    }

    PublicTransport * operator + (int passengers) const {
        Metro * m = new Metro( * this);
        m -> actual_capacity += passengers;
        return m;
    }

    PublicTransport * operator - (int passengers) const {
        Metro * m = new Metro( * this);
        m -> actual_capacity -= passengers;
        if (m -> actual_capacity < 0)
            m -> actual_capacity = 0;
        return m;
    }
};
class Bus: public PublicTransport {
    protected: bool is_electric;
    string registration_number;
    bool low_floor; //acces pentru persoane cu dizabilitati
    bool doors[3]; //starea de deschidere/inchidere a fiecarei usi
    public: Bus(const string name = "N/A", int speed = -1, const string line = "N/A", int max_cap = -1, int actual_cap = 0, bool is_electric = false, const string reg_nr = "N/A", bool low_floor = true): PublicTransport(name, speed, line, max_cap, actual_cap),
    is_electric(is_electric),
    low_floor(low_floor) {
        registration_number = reg_nr;
        doors[0] = doors[1] = doors[2] = false;
    }
    ~Bus() {
        //delete[] registration_number;
    }
    Bus* clone() const {
        return new Bus(*this);
    }
    PublicTransport & operator = (const PublicTransport & other) {
        if (this != & other) {
            const Bus * b = dynamic_cast <
                const Bus * > ( & other);
            if (b) {
                PublicTransport::operator = ( * b);
                is_electric = b -> is_electric;
                registration_number = b -> registration_number;
                low_floor = b -> low_floor;
                for (int i = 0; i < 3; ++i) {
                    doors[i] = b -> doors[i];
                }
            }
        }
        return * this;
    }

    friend istream & operator >> (istream & os, Bus & bus) {
        os >> (PublicTransport & ) bus;
        os >> bus.is_electric;
        string temp;
        os >> temp;
        bus.registration_number = temp;
        os >> bus.low_floor;
        return os;
    }

    friend ostream & operator << (ostream & os,
        const Bus & bus) {
        os << (PublicTransport & ) bus;
        os << bus.is_electric << " ";
        os << bus.registration_number << " ";
        os << bus.low_floor << " ";
        return os;
    }

    void switchLowFloor() {
        cout << "Low Floor currently ";
        if (low_floor) {
            cout << "up\n";
        } else
            cout << "down\n";
        char input[50];
        cout << "Would you like to change the low floor?(yes or no)\n";
        cin >> input;
        if (strcmp(input, "yes") == 0) {
            low_floor = !low_floor;
        } else if (strcmp(input, "no") == 0) {
            return;
        } else
            cout << "invalid input\n";
    }

    const string getType() const {
        return "Bus";
    }

    PublicTransport * operator++(int) {
        Bus * m = new Bus( * this);
        if (actual_capacity < max_capacity)
            actual_capacity++;
        return m;
    }

    PublicTransport * operator--(int) {
        Bus * m = new Bus( * this);
        if (actual_capacity > 0)
            actual_capacity--;
        return m;
    }

    PublicTransport * operator + (int passengers) const {
        Bus * b = new Bus( * this);
        b -> actual_capacity += passengers;
        if (b -> actual_capacity > b -> max_capacity)
            b -> actual_capacity = b -> max_capacity;
        return b;
    }

    PublicTransport * operator - (int passengers) const {
        Bus * b = new Bus( * this);
        b -> actual_capacity -= passengers;
        if (b -> actual_capacity < 0)
            b -> actual_capacity = 0;
        return b;
    }
};
class Tram: public PublicTransport {
    protected:
        bool is_electric;
        int no_of_USBports;
    friend class Trolleybus; //!!
    public: Tram(const string name = "N/A", int speed = -1, const string line = "N/A", int max_cap = -1, int actual_cap = 0, bool is_electric = 1, int no_of_USBports = 0): PublicTransport(name, speed, line, max_cap, actual_cap),
    is_electric(is_electric),
    no_of_USBports(no_of_USBports) {}
    Tram(const Tram & orig): PublicTransport(orig) {
            if (this != & orig) {
                PublicTransport::operator = (orig);
                this -> is_electric = orig.is_electric;
                this -> no_of_USBports = orig.no_of_USBports;
            }
        }
    ~Tram() {}
    Tram* clone() const  {
        return new Tram(*this);
    }
    PublicTransport & operator = (const PublicTransport & other) {
        if (this != & other) {
            const Tram * t = dynamic_cast <
                const Tram * > ( & other);
            if (t) {
                PublicTransport::operator = ( * t);
                is_electric = t -> is_electric;
                no_of_USBports = t -> no_of_USBports;
            }
        }
        return * this;
    }
    friend istream & operator >> (istream & os, Tram & tram) {
        os >> (PublicTransport & ) tram;
        os >> tram.is_electric;
        os >> tram.no_of_USBports;
        return os;
    }
    friend ostream & operator << (ostream & os,
        const Tram & tram) {
        os << (PublicTransport & ) tram;
        os << tram.is_electric << " ";
        os << tram.no_of_USBports << " ";
        return os;
    }
    const string getType() const {
        return "Tram";
    }

    PublicTransport * operator + (int passengers) const {
        Tram * t = new Tram( * this);
        t -> actual_capacity += passengers;
        if (t -> actual_capacity > t -> max_capacity)
            t -> actual_capacity = t -> max_capacity;
        return t;
    }
    PublicTransport * operator--(int) {
        Tram * m = new Tram( * this);
        if (actual_capacity > 0)
            actual_capacity--;
        return m;
    }

    PublicTransport * operator++(int) {
        Tram * m = new Tram( * this);
        if (actual_capacity < max_capacity)
            actual_capacity++;
        return m;
    }

    PublicTransport * operator - (int passengers) const {
        Tram * t = new Tram( * this);
        t -> actual_capacity -= passengers;
        if (t -> actual_capacity < 0)
            t -> actual_capacity = 0;
        return t;
    }

};
//mostenirea diamant
class Trolleybus: public Bus, public Tram {
    bool is_connected;
    public:
        Trolleybus(
            const string name = "N/A", int speed = -1,
            const string line = "N/A",
            int max_cap = -1,
            int actual_cap = 0,
            bool is_electric_bus = true,
            const string reg_nr = "N/A",
            bool low_floor = true,
            bool is_electric_tram = true,
            int no_usb_ports = 0,
            bool is_connected = true
        ):
        Bus(name, speed, line, max_cap, actual_cap, is_electric_bus, reg_nr, low_floor),
        Tram(name, speed, line, max_cap, actual_cap, is_electric_tram, no_usb_ports),
        is_connected(is_connected) {}
    ~Trolleybus() {
      //nimic explicit ca bus si tram se distrug corect singure
    }
    Trolleybus* clone() const {
            return new Trolleybus(*this);
        }
    //polimorfism cu lista de initializai
    //downacast
    Trolleybus(const Trolleybus & other): Bus(static_cast <const Bus & > (other)),Tram(static_cast <const Tram & > (other)),is_connected(other.is_connected) {}
    PublicTransport & operator = (const PublicTransport & other) {
        // alegem ramura Bus
        if (static_cast <const PublicTransport * > (static_cast <const Bus * > (this)) != & other) {
          //polimorfism
            const Trolleybus * t = dynamic_cast <const Trolleybus * > ( & other);
            if (t) {
                Bus::operator = ( * t); // copiem partea de bus si public transport
                Tram::no_of_USBports = t -> Tram::no_of_USBports; // partea din tram
                is_connected = t -> is_connected; // propriu
            }
        }
        //polimorfism
        return * static_cast < Bus * > (this); //returnam un bus (???)
    }

    void changePlugStatus() {
        cout << "The Trolleybus is ";
        if (is_connected)
            cout << "connected to the head wires ";
        else
            cout << "disconnected from the head wires ";
        cout << "\nWould you like to change the status?(yes/no): ";
        char op[6];
        cin >> op;
        if (op[0] == 'y')
            is_connected = !is_connected;
        else if (op[0] == 'n')
            return;
        else
            cout << "Invalid input!\n";
    }
    const string getType() const {
        return "Trolleybus";
    }
    // operator+ cu int
    PublicTransport * operator + (int passengers) const {
        Trolleybus * tb = new Trolleybus( * this);
        tb -> Bus::actual_capacity += passengers;
        if (tb -> Bus::actual_capacity > tb -> Bus::max_capacity)
            tb -> Bus::actual_capacity = tb -> Bus::max_capacity;
            return static_cast<PublicTransport*>(static_cast<Bus*>(tb));
    }

    // operator- cu int
    PublicTransport * operator - (int passengers) const {
        Trolleybus * tb = new Trolleybus( * this);
        tb -> Bus::actual_capacity -= passengers;
        if (tb -> Bus::actual_capacity < 0)
            tb -> Bus::actual_capacity = 0;
            return static_cast<PublicTransport*>(static_cast<Bus*>(tb));
    }

    // postfix ++
    PublicTransport * operator++(int) {
        Trolleybus * tb = new Trolleybus( * this);
        if (tb -> Bus::actual_capacity < tb -> Bus::max_capacity)
            tb -> Bus::actual_capacity++;
            return static_cast<PublicTransport*>(static_cast<Bus*>(tb));

    }
    // postfix --
    PublicTransport * operator--(int) {
        Trolleybus * tb = new Trolleybus( * this);
        if (tb -> Bus::actual_capacity > 0)
            tb -> Bus::actual_capacity--;
            return static_cast<PublicTransport*>(static_cast<Bus*>(tb));

    }
    //overload istream
    friend istream & operator >> (istream & in, Trolleybus & t) {
        in >> static_cast < Bus & > (t); // citeste publictransport si bus prin bus
        in >> t.Tram::no_of_USBports; // citeste partea din tram
        in >> t.is_connected; // specific trolleybus
        return in;
    }
    //overload ostream
    friend ostream & operator << (ostream & out,
        const Trolleybus & t) {
        out << static_cast <
            const Bus & > (t); // scrie publicTransport si bus
        out << t.Tram::no_of_USBports << " "; // scrie partea din tram
        out << t.is_connected << " "; // scrie partea din trolleybus
        return out;
    }
};

class Depot {
    string depot_name;
    string file_path;
    //vector < PublicTransport * > transports; //vectori de pointers la clasa de baza ale mijloacelor de transport in comun
  map <string, PublicTransport * > transports;
    public:
        Depot(const string file_path = "N/A", const string name = "N/A") {
            this -> file_path = file_path;
            depot_name = name;
        }
        ~Depot() {
//            delete[] depot_name;
//            delete[] file_path;
            for(auto const& i: transports) {
              delete i.second;
            }
            transports.clear();
        }

        Depot(const Depot & other) {
            file_path = other.file_path;
            depot_name = other.depot_name;
            for(auto const& i: other.transports) {
              if(i.second) {
                transports[i.first] = i.second->clone();
              }
            }
        }
        void changeLine(const string name, const string line_name) {
//            for (int i = 0; i < transports.size(); i++) {
//                if (transports[i] -> getName() == name) {
//                    transports[i] -> changeLine(line_name);
//                    return;
//                }
//            }
            auto i = transports.find(name);
            if (i != transports.end()) {
              PublicTransport* v_ptr = i->second;
              if(v_ptr != nullptr) {
                v_ptr->changeLine(line_name);
              }
            }
        }
    void deleteVehicle(string nume) {
//        for (int i = 0; i < transports.size(); i++) {
//            if (transports[i] -> getName() == nume) {
//                transports.erase(transports.begin() + i);
//            }
//        }
          auto i = transports.find(nume);
          if (i != transports.end()) {
            PublicTransport* v_ptr = i->second;
            if(v_ptr != nullptr) {
              delete v_ptr;
            }
            transports.erase(i);
          }
    }
    Depot & operator = (const Depot & other) {
        if (this != & other) {
          for (auto const& i: this->transports) {
            delete i.second;
          }
          this->transports.clear();
          file_path = other.file_path;
          depot_name = other.depot_name;
          for(auto const& i: other.transports) {
            if(i.second != nullptr) {
              PublicTransport* temp = i.second->clone(); //de rezolvat copiere superficiala candva daca mai ai timp
              this->transports[i.first] = temp;
            }
            else
              this->transports[i.first] = nullptr;
          }
//            for (int i = 0; i < (int) transports.size(); ++i) {
//                delete transports[i];
//            }
//            transports.clear();
//            depot_name = other.depot_name;
//            for (int i = 0; i < (int) other.transports.size(); ++i) {
//                transports.push_back(other.transports[i]);
//            }
        }
        return * this;
    }

    void add(PublicTransport * t) {
        auto i = transports.find(t->getName());
        if (i != transports.end()) {
          if(i->second != nullptr && i->second != t) {
            delete i->second;
          }
        }
        transports[t->getName()] = t;
    }

    int size() const {
        return transports.size();
    }

    PublicTransport * operator[](int index) {
      if (index < 0 || index >= size())
        return nullptr;
      int nr = 0;
      for (auto const& i: transports) {  //complet ineficient
        if(nr == index)
          return i.second;
        nr++;
      }
      return nullptr;
        //return transports[index];
    }
    PublicTransport * getTrain(string nume) {
      auto i = transports.find(nume);
      if (i != transports.end()) {
        return i->second;
      }
      else
        return nullptr;
//        for (int i = 0; i < transports.size(); i++) {
//            if (transports[i] -> getName() == nume) {
//                return transports[i];
//            }
//        }
//        return nullptr;
    }
    friend ostream & operator << (ostream & out, const Depot & depot) {
        out << depot.depot_name << '\n';
        out << depot.transports.size() << '\n';
        for (auto const& i: depot.transports) {
        PublicTransport* temp = i.second;
        if(Trolleybus* tb = dynamic_cast<Trolleybus*>(temp)) {
          out<<"Trolleybus "<<*tb<<'\n';
        }
        else if(Metro * m = dynamic_cast<Metro*>(temp)) {
          out<<"Metro "<<*m<<'\n';
        }
        else if(Bus* b = dynamic_cast<Bus*>(temp)) {
          out<<"Bus "<<*b<<'\n';
        }
        else if(Tram * t = dynamic_cast<Tram*>(temp)) {
          out<<"Tram "<<*t<<'\n';
        }
        else
          cout<<"Unknown type!\n";
        }
//        for (int i = 0; i < depot.transports.size(); ++i) {
//            if (Metro * m = dynamic_cast < Metro * > (depot.transports[i])) {
//                out << "Metro " << * m << endl;
//            }
//            //trolleybus peste clasele derivate ca altfel nu intra niciodata aici
//            else if (Trolleybus * tb = dynamic_cast < Trolleybus * > (depot.transports[i])) {
//                out << "Trolleybus " << * tb << endl;
//            } else if (Bus * b = dynamic_cast < Bus * > (depot.transports[i])) {
//                out << "Bus " << * b << endl;
//            } else if (Tram * t = dynamic_cast < Tram * > (depot.transports[i])) {
//                out << "Tram " << * t << endl;
//            } else {
//                out << * depot.transports[i] << " UnknownType\n";
//            }
//        }

        return out;

    }
    friend istream & operator >> (istream & in, Depot & depot) {
        string name;
        int n;
        in >> name;
        depot.depot_name = name;
        in >> n;
        for(int i = 0; i < n; i++) {
          string type;
          in >> type;
          PublicTransport* temp = nullptr;
          if(type == "Metro"){
            Metro* m = new Metro();
            in >> *m;
            temp = m;
          }
          else if(type == "Bus"){
            Bus* b = new Bus();
            in >> *b;
            temp = b;
          }
          else if(type == "Tram"){
            Tram* t = new Tram();
            in >> *t;
            temp = t;
          }
          else if(type == "Trolleybus"){
            Trolleybus* t = new Trolleybus();
            in >> *t;
            temp = static_cast <Bus*> (t);
          }
          if(temp != nullptr) {
            depot.transports[temp->getName()] = temp;
          }

        }
//        for (int i = 0; i < n; ++i) {
//            string type;
//            in >> type;
//            if (type == "Metro") {
//                Metro * m = new Metro();
//                in >> * m;
//                depot.transports.push_back(m);
//            } else if (type == "Bus") {
//                Bus * b = new Bus();
//                in >> * b;
//                depot.transports.push_back(b);
//            } else if (type == "Tram") {
//                Tram * tr = new Tram();
//                in >> * tr;
//                depot.transports.push_back(tr);
//            } else if (type == "Trolleybus") {
//                Trolleybus * t = new Trolleybus();
//                in >> * t;
//                depot.transports.push_back(static_cast < Bus * > (t));
//            }
//        }
        return in;
    }
    void addTransport(PublicTransport * t) {
        //transports.push_back(t);
        transports[t->getName()] = t;
    }
    string getName() const {
        return depot_name;
    }
    string getFileName() const {
        return file_path;
    }
    void loadDepot() {
        ifstream f;
        try{
          f.open(file_path);
          if(f.fail()) {
            throw runtime_error("Error opening file " + file_path);
          }
        f >> * this;
        if(f.fail() && !f.eof()) {
          throw runtime_error("Error opening file " + file_path);
        }
            f.close();
        }
        catch(const runtime_error & e) {
            cout<<e.what()<<endl;
            if(f.is_open()) {
            f.close();
            }
        }
    }
    void saveDepot() {
        ofstream f;
        try{
          f.open(file_path);
          if(f.fail()) {
            throw runtime_error("Error opening file " + file_path);
          }
            f << * this;
            if(f.fail()) {
              throw runtime_error("Error saving data to file " + file_path);
            }
            f.close();
        }
        catch(const runtime_error & e) {
          cout<<e.what()<<endl;
          if(f.is_open()) {
            f.close();
          }
        }
    }
};
int Station::id_generator = 0;
int option, option1, option2;
float average_spped_global;
string global_station_name, global_line_name, global_system_name;
string global_PublicTransport_name, global_depot_name;
string global_pb_type;
//pentru comutativitatea virtuala +
inline PublicTransport * operator + (int passengers, const PublicTransport & pt) {
    return pt + passengers;
}
//pentru comutativitatea virtuala -
inline PublicTransport * operator - (int passengers, const PublicTransport & pt) {
    return pt - passengers;
}

template <class T>
class Object {
    T generic_object;
    string file_path;
    public:

      Object(string file_path = "N/A", T generic_object = T()){
        this->generic_object = generic_object;
        this->file_path = file_path;
      }
      ~Object(){

      }
      Object(const Object<T> &other){
        generic_object = other.generic_object;
        file_path = other.file_path;
      }
      Object<T> & operator = (const Object<T> &other){
        generic_object = other.generic_object;
        file_path = other.file_path;
        return *this;
      }
    void loadObject() {
          ifstream f;
          try{
              f.open(file_path);
              if (!f.is_open()) {
                  throw runtime_error("Error opening file" + file_path);
              }
              f >> generic_object;
              if(f.fail()){
                  throw runtime_error("Error reading file" + file_path);
              }
              f.close();
          }
          catch(const runtime_error & e) {
              cout << e.what() << endl;
              if(f.is_open()){
                  f.close();
              }
          }
      }
    void saveObject() {
          ofstream f;
          try{
              f.open(file_path);
              if (!f.is_open()) {
                  throw runtime_error("Error opening file" + file_path);
              }
              f << generic_object;
              if(f.fail()){
                  throw runtime_error("Error writing file" + file_path);
              }
              f.close();
          }
          catch(const runtime_error & e) {
              cout << e.what() << endl;
              if(f.is_open()){
                  f.close();
              }
          }

      }
    T& getGenericObject() {
          return generic_object;
      }
    const T& getGenericObject() const {
          return generic_object;
      }
    const string& getFilePath() const {
          return file_path;
      }
    void setFilePath(const string& path) {
          file_path = path;
      }

    friend istream& operator >> (istream& is, Object<T> &obj) {
      is >> obj.file_path;
      return is;
    }
    friend ostream& operator << (ostream& os, Object<T> &obj) {
        os << obj.file_path;
        return os;
    }
};


class Singleton{
  private:
    static Singleton * instance;
    vector< Object <System> > &systems;
    vector< Object < Depot> > &depots;


    Singleton(vector<Object <System>>&s,vector<Object <Depot> >&d):systems(s),depots(d){}
    //prima metoda template
    template<class vehicle_type>
    vehicle_type * createAndRead(const string message){
        cout<<message<<'\n';
        vehicle_type * vehicle = new vehicle_type();
        cin >> *vehicle;
        return vehicle;
    }
    void mainMenu() {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Manage Systems\n";
        cout << "2. Manage Vehicles\n";
        cout << "3. See and Manage Schedule\n";
        cout << "0. Exit program\n";
        cout << "Enter option: ";
    }
    void systemMenuPrint(){
        cout << "\n===== SYSTEM MANAGEMENT =====\n";
        cout << "1. Show all Systems\n";
        cout << "2. Show a specific System\n";
        cout << "3. Add a Station\n";
        cout << "4. Add a line\n";
        cout << "5. Remove a Station\n";
        cout << "6. Remove a line\n";
        cout << "0. Return to main menu\n";
        cout << "Enter option: ";
    }
    void vehicleMenuPrint(){
        cout << "\n===== VEHICLE MANAGEMENT =====\n";
        cout << "1. Show all Depots\n";
        cout << "2. Add a vehicle\n";
        cout << "3. To show a specific depot\n";
        cout << "4. To assign other line to a vehicle\n";
        cout << "5. To delete a vehicle\n";
        cout << "!6. To focus on a Vehicle's ride\n";
        cout << "7. Check and edit low floor of a bus or trolley\n";
        cout << "8. Check and edit connection to overhead wires\n";
        cout << "0. Return to main menu\n";
        cout << "Enter option: ";
    }
    void scheduleMenuPrint(){
      cout << "\n===== SCHEDULE MANAGEMENT =====\n";
      cout << "1. Show all Schedules\n";
      cout << "2. Show a specific Schedule\n";
    }
    void systemsMenu(){
      int systemOption = -1;
                    do {
                        systemMenuPrint();
                        try{
                        cin >> systemOption;
                        if(systemOption < 0 || systemOption >6) {
                          throw logic_error("Invalid option!");
                        }
                        if(cin.fail()) {
                          cin.clear();
                          cin.ignore();
                          throw ios_base::failure("Invalid input!");
                        }
                        switch (systemOption) {
                        case 0:
                            cout << "Returning to main menu...\n";
                            break;

                        case 1: {
                            cout << "\nAvailable Systems:\n";
                            for (int i = 0; i < systems.size(); ++i) {
                                cout << systems[i].getGenericObject().getName() << endl;
                            }
                            break;
                        }

                        case 2: {
                          try{
                            cout << "Enter System name: ";
                            string global_station_name;
                            cin >> global_station_name;

                            int i;
                            for (i = 0; i < systems.size(); ++i) {
                                if (systems[i].getGenericObject().getName() == global_station_name) {
                                    cout << systems[i].getGenericObject() << endl;
                                    break;
                                }
                            }
                            if (i == systems.size()) {
                                throw runtime_error("System does not exist");
                            }

                          }
                          catch(const runtime_error & e) {
                                cout<<e.what()<<endl;
                          }
                            break;
                        }

                        case 3: {
                            string global_system_name;
                            string global_line_name;

                            cout << "Enter System name to edit: ";
                            cin >> global_system_name;
                            cout << "Enter Line name to edit: ";
                            cin >> global_line_name;

                            Station station;
                            cout << "Enter Station distance, name and boolean value for change:\n";
                            try{
                                cin >> station;
                                int addOption,i;
                                cout << "1. Add to Back\n2. Add to Front\nEnter option: ";
                                cin >> addOption;
                                try{
                                    if(addOption != 1 && addOption != 2) {
                                        throw runtime_error("Invalid option for position");
                                    }
                                    for (i = 0; i < systems.size(); ++i) {

                                        if (systems[i].getGenericObject().getName() == global_system_name){
                                            if (addOption == 1) {
                                                systems[i].getGenericObject().addToLineBack(station, global_line_name);
                                                cout << "Station added to the back of the line.\n";
                                            } else if (addOption == 2) {
                                                systems[i].getGenericObject().addToLineFront(station, global_line_name); //de facut try and catch dupa ce fac dequeue
                                                cout << "Station added to the front of the line.\n";
                                            } else {
                                                cout << "Invalid option.\n";
                                            }
                                            break;
                                        }
                                    }
                                    if(i == systems.size()) {
                                        throw runtime_error("Invalid option for system");
                                    }
                                }
                                catch(const runtime_error & e) {
                                    cout<<e.what()<<endl;

                                }
                            }
                            catch(const invalid_argument & e) {
                              cout<<e.what()<<endl;
                            }
                            break;
                        }
                        case 4: {
                            cout << " Enter the System name: ";
                            cin >> global_system_name;
                            cout << "Enter line name: ";
                            cin >> global_line_name;
                            try{
                              int i;
                                for (i = 0; i < systems.size(); ++i) {
                                    if (systems[i].getGenericObject().getName() == global_system_name) {
                                        systems[i].getGenericObject().addLine(global_line_name);
                                        break;
                                    }
                                }
                                if(i == systems.size()) {
                                  throw runtime_error("Invalid option for SYSTEM");
                                }
                            } catch(const runtime_error & e) {
                              cout<<e.what()<<endl;
                            }
                            break;
                        }
                        case 5: {
                            cout << "Insert the System's name you're trying to edit: ";
                            cin >> global_system_name;
                            cout << "Insert Line's name you're trying to edit: ";
                            cin >> global_line_name;
                            cout << "You can only delete the front or back of the line.\n";
                            cout << "Enter option: ";
                            cin >> option2;
                            try{
                              if(option2 != 1 && option2 != 2) {
                                throw runtime_error("Invalid option for position");
                              }
                              int i;
                                for (i = 0; i < systems.size(); ++i) {
                                    if (systems[i].getGenericObject().getName() == global_system_name) {
                                        if (option2 == 1) {
                                            systems[i].getGenericObject().deleteFromLineBack(global_line_name);
                                        } else if (option2 == 2) {
                                            systems[i].getGenericObject().deleteFromLineFront(global_line_name);
                                        } else
                                            cout << "Invalid option.\n";
                                        break;
                                    }

                                }
                               if(i == systems.size()) {
                                 throw runtime_error("Invalid option for SYSTEM");
                               }
                            }catch(const runtime_error & e) {
                              cout<<e.what()<<endl;
                            }
                            break;
                        }
                        case 6: {
                            cout << "Insert System's name you're trying to edit: ";
                            cin >> global_system_name;
                            cout << "Insert Line's name you're trying to delete: ";
                            cin >> global_line_name;
                            try{
                                int i;
                                for (i = 0; i < systems.size(); ++i) {
                                    if (systems[i].getGenericObject().getName() == global_system_name) {
                                        systems[i].getGenericObject().deleteLine(global_line_name);
                                        break;
                                    }
                                }
                                if(i == systems.size()) {
                                  throw runtime_error("Invalid option for SYSTEM");
                                }
                            }
                            catch(const runtime_error & e) {
                              cout<<e.what()<<endl;

                            }
                            break;
                        }
                        case 7: {
                            cout << "Insert System's name you're trying to edit the ride's price: ";
                            cin >> global_system_name;
                            try{
                              int i;
                                for (i = 0; i < systems.size(); ++i) {
                                    if (systems[i].getGenericObject().getName() == global_system_name){
                                        systems[i].getGenericObject()++;
                                        break;
                                    }

                                }
                                if(i == systems.size()) {
                                  throw runtime_error("Invalid option for SYSTEM");
                                }
                            }
                            catch(const runtime_error & e) {
                              cout<<e.what()<<endl;
                            }
                            break;
                        }
                        default:
                            cout << "Invalid option. Please try again.\n";
                            break;
                        }
                        }catch(const logic_error & e) {
                          cout<<e.what()<<endl;

                          systemOption = -1;
                        }
                        catch(const ios_base::failure & e) {
                          cout<<e.what()<<endl;

                          systemOption = -1;
                        }

                    } while (systemOption != 0);
    }
    void depotsMenu(){
      int vehicleOption = -1;
                do {
                    vehicleMenuPrint();
                    try
                     {
                    cin >> vehicleOption;
                    if(vehicleOption < 0 || vehicleOption > 8) {
                      throw invalid_argument ("Invalid option.");
                    }
                    if(cin.fail()) {
                      cin.clear();
                      cin.ignore();
                      throw ios_base::failure ("Invalid option.");
                    }
                    switch (vehicleOption) {
                    case 0:
                        cout << "Returning to main menu...\n";
                        break;

                    case 1:
                        cout << '\n';
                        for (int i = 0; i < depots.size(); ++i) {
                            cout << depots[i].getGenericObject().getName() << "\n";
                        }
                        break;

                    case 2: {
                        cout << "Select a Depot: ";
                        cin >> global_depot_name;
                        cout << "Select vehicle type: ";
                        cin >> global_pb_type;
                        try{
                          if(global_pb_type != "Metro" && global_pb_type != "Bus" && global_pb_type != "Tram" && global_pb_type != "Trolleybus")
                            throw logic_error (global_pb_type + " is not a valid public transport vehicle type.");
                        int i;
                        for (i = 0; i < depots.size(); ++i) {
                            if (depots[i].getGenericObject().getName() == global_depot_name) {
                                if (global_pb_type == "Metro") {
                                    //polimorfism
                                    depots[i].getGenericObject().add(createAndRead<Metro>("Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), and Number of Wagons\n"));
//                                    cout << "Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), and Number of Wagons\n";
//                                    Metro * m = new Metro();
//                                    cin >> * m;
//                                    depots[i].add(m);

                                } else if (global_pb_type == "Bus") {
                                    //polimorfism
                                    depots[i].getGenericObject().add(createAndRead<Bus>("Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Registration Number, and Low Floor Status\n"));
//                                    cout << "Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Registration Number, and Low Floor Status\n";
//                                    Bus * b = new Bus();
//                                    cin >> * b;
//                                    depots[i].add(b);

                                } else if (global_pb_type == "Tram") {
                                    //polimorfism
                                    depots[i].getGenericObject().add(createAndRead<Tram>("Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Number of USB ports\n"));
//                                    cout << "Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Number of USB ports\n";
//                                    Tram * t = new Tram();
//                                    cin >> * t;
//                                    depots[i].add(t);

                                } else if (global_pb_type == "Trolleybus") {
                                    //polimorfism
                                    depots[i].getGenericObject().add(static_cast<Bus *>(createAndRead<Trolleybus>("Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Registration Number, and Low Floor Status, Number of USB ports, Plugged Status\n")));
//                                    cout << "Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Registration Number, and Low Floor Status, Number of USB ports, Plugged Status\n";
//                                    Trolleybus * t = new Trolleybus();
//                                    cin >> * t;
//                                    //downcast
//                                    depots[i].add(static_cast < Bus * > (t));

                                } else
                                    cout << "Invalid option.\n";

                                break;
                            }

                        }
                        if(i == depots.size()) {
                          throw runtime_error(global_depot_name + " depot does not exist!");
                        }
                        }catch(const logic_error & e) {
                            cout<<e.what()<<endl;
                        }
                        catch(const runtime_error & e) {
                          cout<<e.what()<<endl;
                        }
                        break;
                    }
                    case 3: {
                        cout << "Select a depot: ";
                        cin >> global_depot_name;
                        try{
                          int i;
                        for (i = 0; i < depots.size(); ++i) {
                            if (depots[i].getGenericObject().getName() == global_depot_name) {
                                cout << depots[i].getGenericObject() << "\n";
                                break;
                            }
                            if(i == depots.size()) {
                              throw runtime_error (global_depot_name + " depot does not exist!");
                            }
                        }
                        }catch(const runtime_error & e) {
                          cout<<e.what()<<endl;
                        }

                        break;
                    }
                    case 4: {
                        cout << "Insert a Depot: ";
                        cin >> global_depot_name;
                        cout << "Insert vehicle name: ";
                        cin >> global_pb_type;
                        cout << "Insert a new line: ";
                        cin >> global_line_name;
                        try{
                        for (int i = 0; i < depots.size(); ++i) {
                            if (depots[i].getGenericObject().getName() == global_depot_name){
                                depots[i].getGenericObject().changeLine(global_pb_type, global_line_name);
                            }
                          if( i == depots.size()) {
                            throw runtime_error (global_depot_name + " depot does not exist!");
                          }
                        }
                        }catch(const runtime_error & e) {
                          cout<<e.what()<<endl;
                        }
                        break;
                    }
                    case 5: {
                        cout << "Insert a depot: ";
                        cin >> global_depot_name;
                        cout << "Insert a vehicle: ";
                        cin >> global_pb_type;
                        try{
                          int i;
                        for (i = 0; i < depots.size(); ++i) {
                            if (depots[i].getGenericObject().getName() == global_depot_name) {
                                depots[i].getGenericObject().deleteVehicle(global_pb_type);
                                break;
                            }
                         if(i == depots.size()) {
                           throw runtime_error (global_depot_name + " depot does not exist!");
                         }
                        }}catch(const runtime_error & e) {
                          cout<<e.what()<<endl;
                        }
                        break;
                    }
                    case 6: {
                        //cea mai mare prajeala care exista
                        cout << "Insert System: ";
                        cin >> global_system_name;
                        cout << "Insert a depot: ";
                        cin >> global_depot_name;
                        cout << "Insert a vehicle: ";
                        cin >> global_pb_type;
                        try{
                          int i;
                        for (i = 0; i < depots.size(); ++i) {
                            if (depots[i].getGenericObject().getName() == global_depot_name) {
                                //polimorfism
                                PublicTransport * temp_pb = depots[i].getGenericObject().getTrain(global_pb_type);
                                for (int j = 0; j < systems.size(); ++j) {
                                    if (systems[j].getGenericObject().getName() == global_system_name) {
                                        temp_pb -> setTren(systems[j].getGenericObject());
                                    }
                                }
                                delete temp_pb;
                            }
                        }
                        if(i == depots.size()) {
                          throw runtime_error (global_depot_name + " depot does not exist!");
                        }
                        }catch(const runtime_error & e) {
                          cout<<e.what()<<endl;
                        }
                        break;
                    }
                    case 7: {
                        cout << "Insert a public transport name: ";
                        cin >> global_pb_type;
                        bool ok = false, ok1 = false;
                        int i;
                        try{
                        for (i = 0; i < depots.size(); ++i) {
                            for (int j = 0; j < depots[i].getGenericObject().size(); ++j) {

                                if (depots[i].getGenericObject()[j] -> getName() == global_pb_type) {
                                  try{


                                  if(depots[i].getGenericObject()[j]-> getType() != "Bus" && depots[i].getGenericObject()[j] -> getType()!= "Trolleybus") {
                                    throw logic_error ( "\n" + global_pb_type + " doesn't support low floor.\n");
                                  }
                                    //if (depots[i][j] -> getType() == "Bus" || depots[i][j] -> getType() == "Trolleybus") {
                                        //polimorfism
                                        Bus * b = dynamic_cast < Bus * > (depots[i].getGenericObject()[j]); //downcast de la trolleybus->bus
                                        if(b != nullptr)
                                            b->switchLowFloor();

                                   // } else
                                      //  cout << "This vehicle doesn't suppport low floor.\n";
                                    ok = true;
                                    }catch(const logic_error & e) {
                                      ok = true;
                                      cout<<e.what()<<endl;
                                    }
                                }

                            }
                        }//nu i am mai rasp de la 2 aokk
                            if(ok == false)
                              throw runtime_error (global_pb_type + " could not be found!");
                        }//this is so boring ts pmo
                        catch(const runtime_error & e)  {
                          cout<<e.what()<<endl;
                        }
                        break;
                    }
                    case 8: {
                        cout << "Insert a trolleybus name: ";
                        cin >> global_pb_type;
                        bool ok = false;
                        try{
                          int i;
                        for (i = 0; i < depots.size(); ++i) {
                            for (int j = 0; j < depots[i].getGenericObject().size(); ++j) {
                                if (depots[i].getGenericObject()[j] -> getName() == global_pb_type) {
                                   try{
                                     if (depots[i].getGenericObject()[j] -> getType() != "Trolleybus")
                                       throw runtime_error ( "\n" + global_pb_type + "is not a Trolleybus, only Trolleybuses are connected to wires");

                                        //polimorfism
                                        Trolleybus * t = dynamic_cast < Trolleybus * > (depots[i].getGenericObject()[j]);
                                        t -> changePlugStatus();

                                    }catch(const runtime_error & e) {
                                      ok = true;
                                      cout<<e.what()<<endl;
                                    }
                                    ok = true;
                                }
                            }
                        }
                        if(ok == false) {
                            throw runtime_error (global_pb_type + " could not be found!");
                        }
                        }catch(const runtime_error & e) {
                          ok = true;
                          cout<<e.what()<<endl;
                        }
                        break;
                    }
                    default:
                        cout << "Invalid option. Please try again.\n";
                        break;
                    }
                    }
                    catch(invalid_argument &e) {
                      cout << e.what() << "\n";
                      vehicleOption = -1;
                    }
                    catch(ios_base::failure &e) {
                      cout << e.what() << "\n";
                      vehicleOption = -1;
                    }

                } while (vehicleOption != 0);
    }
    public:
        Singleton(const Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;

        static Singleton * getInstance(vector < Object <System>> &systems, vector< Object<Depot>> &depots) {
            if(instance == nullptr){
              instance = new Singleton (systems, depots);
            }
            return instance;
        }

        void run(){

          option = -1;
          do{
            try{
              mainMenu();
              cin>>option;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore();
                    throw ios_base::failure("Option failure!");
                }
                if(option != 1 && option != 2 && option != 0) {
                    throw invalid_argument("Not an option!");
                }
                switch(option){
                  case 0:{
                    cout<<"Exiting program...\n";
                    break;
                  }
                    case 1:{
                      systemsMenu();
                      break;
                    }
                      case 2:{
                        depotsMenu();
                        break;
                    }
                }

            }
              catch(ios_base::failure& e){
                  cout<<e.what()<<"\n";
                  option = -1;
              }
              catch(invalid_argument& e){
                  cout<<e.what()<<"\n";
                  option = -1;
              }
          }while(option != 0);
        }
    static void removeInstance(){
      delete instance;
      instance = nullptr;
    }
};
Singleton * Singleton::instance = nullptr;

int main() {
    //  Bus a("autobuzu", 50, "M1", 100, 50, true, "B123", true);
    //  PublicTransport* result = a + 20;
    //  cout << *dynamic_cast<Bus*>(result);
    //  delete result;

    vector < Object <System> > systems_wrapper;
    Object<System> temp;
    ifstream f("config_systems.txt");
    while(f>>temp){ systems_wrapper.push_back(temp); }
//    systems_wrapper.push_back(Object <System>("system_metro.txt"));
//    systems_wrapper.push_back(Object <System>("system_bus_tram_trolley.txt"));
    //systems.push_back(System("system_cfr.txt"));
    for (int i = 0; i < systems_wrapper.size(); ++i) {
        systems_wrapper[i].loadObject();
    }
    vector < Object <Depot> > depots_wrapper;
    ifstream g("config_depots.txt");
    Object<Depot> temp1;
    while(g>>temp1){depots_wrapper.push_back(temp1);}
//    depots_wrapper.push_back(Object <Depot>("depoul_militari.txt"));
//    depots_wrapper.push_back(Object <Depot>("depoul_berceni.txt"));
    //depots.push_back(Depot("depoul_garadenord.txt"));
    for (int i = 0; i < depots_wrapper.size(); ++i) {
        depots_wrapper[i].loadObject();
    }



    Singleton * MENIU = Singleton::getInstance(systems_wrapper, depots_wrapper);
    MENIU -> run();



    for (int i = 0; i < systems_wrapper.size(); ++i) {
        systems_wrapper[i].saveObject();
    }
    for (int i = 0; i < depots_wrapper.size(); ++i) {
        depots_wrapper[i].saveObject();
    }


    cout << "All data saved. Program terminated.\n";

    return 0;
}