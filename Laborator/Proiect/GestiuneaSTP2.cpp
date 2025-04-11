#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>
#include <ctime>
#include <iomanip>
 //am folosit si alocare dinamica si stl si vectori statici
//run with clang++ -std=c++17 -o GestiuneaSTP GestiuneaSTP.cpp
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
  char * station_name;
  bool change;

  const int station_id;
  static int id_generator;
  //nu prea merge treaba cu generatul de id uri ptr ca de-a lunugul proiecteului mai
  //generez statii temporare auxiliare si se baga peste id-ul celor care exista pe magistrale
  public:
    Station(float distance = 0,
      const char * station_name = "N/A", bool change = false): station_id(id_generator++) {
      this -> distance = distance;
      this -> station_name = new char[strlen(station_name) + 1];
      strcpy(this -> station_name, station_name);
      this -> change = change;
    }
  //constructor de copiere
  Station(const Station & station): station_id(id_generator++) {
      this -> distance = station.distance;
      this -> station_name = new char[strlen(station.station_name) + 1];
      strcpy(this -> station_name, station.station_name);
      this -> change = station.change;
    }
    ~Station() {
      delete[] station_name;
    }
  float getDistance() {
    return this -> distance;
  }
  char * getStationName() {
    return this -> station_name;
  }
  friend istream & operator >> (istream & in, Station & station) {
    char temp_name[100];
    in >> station.distance >> temp_name >> station.change;
    delete[] station.station_name;
    station.station_name = new char[strlen(temp_name) + 1];
    strcpy(station.station_name, temp_name);
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
      delete[] station_name;
      this -> station_name = new char[strlen(station.station_name) + 1];
      strcpy(this -> station_name, station.station_name);
      this -> change = station.change;
    }
    return * this;
  }
  void showStation() const {
    cout << distance << " " << station_name << " " << change << " " << station_id << endl;
  }
};

class Line {
  vector < Station > stations;
  float length;
  int no_of_stations;
  char line_name[50]; //aici am folosit char normal
  public:
    Line(int no_of_stations = 0, float length = 0,
      const char * line_name = "N/A") {
      this -> length = length;
      strcpy(this -> line_name, line_name);
      this -> no_of_stations = no_of_stations;
    }

    ~Line() {
      stations.clear();
      //se dezaloca singura memoria right dar tot am fct ceva
    }

  Line & operator = (const Line & line) {
    if (this != & line) {
      this -> length = line.length;
      strcpy(this -> line_name, line.line_name);
      this -> no_of_stations = line.no_of_stations;
      for (int i = 0; i < line.no_of_stations; i++) {
        this -> stations.push_back(line.stations[i]);
        this -> no_of_stations++;
      }
    }
    return * this;
  }
  int getNoOfStations() {
    return this -> no_of_stations;
  }
  Line(const Line & line) {
    this -> length = line.length;
    strcpy(this -> line_name, line.line_name);
    this -> no_of_stations = line.no_of_stations;
    stations = line.stations;
  }

  void addStationBack(const Station & station) {
    stations.push_back(station);
    no_of_stations++;
  }

  void addStationFront(const Station & station) {
    stations.insert(stations.begin(), station);
    no_of_stations++;
  }
  void deleteStationBack() {
    stations.pop_back();
    no_of_stations--;
  }
  void deleteStationFront() {
    stations.erase(stations.begin());
    no_of_stations--;
  }
  void printLine() const {
    for (size_t i = 0; i < stations.size(); i++) {
      cout << stations[i];
      cout << endl;
    }
  }
  //overloading the '+' operator
  char * getLineName() {
    return line_name;
  }
  float getStationDistance(char * station_name) {
    for (int i = 0; i < stations.size(); i++) {
      if (strcmp(station_name, stations[i].getStationName()) == 0) {
        return stations[i].getDistance();
      }
    }
    return 0;
  }
  //basically getStation()
  Station & operator[](int index) {
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
  char * getDirection() {
    return stations[stations.size() - 1].getStationName();
  }
  void showStations(int i) {
    cout << stations[i].getStationName();
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
  char * system_name;
  float trip_price;
  vector < Line > lines;
  char * file_path;
  public:
    System(const char * file_path = "N/A", const char * system_name = "N/A", float trip_price = 0) {
      this -> system_name = new char[strlen(system_name) + 1];
      strcpy(this -> system_name, system_name);
      this -> file_path = new char[strlen(file_path) + 1];
      strcpy(this -> file_path, file_path);
      this -> trip_price = trip_price;
    }
  System(const System & system) {
      this -> system_name = new char[strlen(system.system_name) + 1];
      this -> file_path = new char[strlen(system.file_path) + 1];
      strcpy(this -> file_path, system.file_path);
      strcpy(this -> system_name, system.system_name);
      this -> trip_price = system.trip_price;
      this -> lines = system.lines;
    }
    ~System() {
      if (this -> system_name != nullptr) {
        delete[] this -> system_name;
      }
      if (this -> file_path != nullptr) {
        delete[] this -> file_path;
      }
    }
  void addLine(const char name[50]) {
    Line line(0, 0, name);
    lines.push_back(line);
  }
  Line getLine(char * name) {
    for (int i = 0; i < lines.size(); i++) {
      if (strcmp(name, lines[i].getLineName()) == 0) {
        cout << lines[i];
        return lines[i];
      }
    }
    return Line();
  }
  void deleteLine(const char name[]) {
    for (int i = 0; i < lines.size(); i++) {
      if (!strcmp(name, lines[i].getLineName())) {
        lines.erase(lines.begin() + i);
        break;
      }
    }
  }
  void addToLineBack(const Station & station, char name[50]) {
    for (int i = 0; i < lines.size(); i++) {
      if (!strcmp(lines[i].getLineName(), name)) {
        lines[i].addStationBack(station);
        //lines[i] = lines[i] + station;
        return;
      }
    }
  }
  void deleteFromLineBack(const char name[50]) {
    for (int i = 0; i < lines.size(); i++) {
      if (!strcmp(lines[i].getLineName(), name)) {
        Station temp = lines[i].getStationBack();
        lines[i].deleteStationBack();
        //lines[i] = lines[i] - temp;
        return;
      }
    }
  }
  void deleteFromLineFront(char name[50]) {
    for (int i = 0; i < lines.size(); i++) {
      if (!strcmp(lines[i].getLineName(), name)) {
        Station temp = lines[i].getStationFront();
        lines[i].deleteStationFront();
        //lines[i] = lines[i] - temp;
        return;
      }
    }
  }
  void addToLineFront(const Station & station, char name[50]) {
    for (int i = 0; i < lines.size(); i++) {
      if (!strcmp(lines[i].getLineName(), name)) {
        lines[i].addStationFront(station);
        //lines[i] = lines[i] + station;
        return;
      }
    }
  }
  Line & operator[](const int i) {
    return lines[i];
  }
  int getLineLenght1(char name[]) {
    for (int i = 0; i < lines.size(); i++) {
      if (!strcmp(lines[i].getLineName(), name)) {
        return lines[i].getLineLenght(i);
      }
    }
    return 0;
  }
  System & operator = (const System & system) {
    if (this -> system_name != nullptr) {
      delete[] this -> system_name;
    }
    this -> system_name = new char[strlen(system.system_name) + 1];
    if (this -> file_path != nullptr) {
      delete[] this -> file_path;
    }
    this -> file_path = new char[strlen(system.file_path) + 1];
    strcpy(this -> file_path, system.file_path);
    strcpy(this -> system_name, system.system_name);
    this -> trip_price = system.trip_price;
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
    if (system.system_name != nullptr) {
      delete[] system.system_name;
    }
    char temp_name[100];
    in >> temp_name >> system.trip_price;
    system.system_name = new char[strlen(temp_name) + 1];
    strcpy(system.system_name, temp_name);
    system.lines.clear();
    Line temp;
    while (in >> temp) {
      system.lines.push_back(temp);
    }
    return in;
  }
  //preturile doar se scumpesc nu si scad, evident lol
  System & operator++() {
    trip_price++;
    return * this;
  }
  const char * getName(){
    return this->system_name;
  }

  System operator++(int) {
    System temp = * this;
    trip_price++;
    return temp;
  }
  void loadSystem() {
    ifstream f(file_path);
    f >> * this;
    f.clear();
    f.close();
  }
  void saveSystem() {
    ofstream f(file_path);
    f << * this;
    f.clear();
    f.close();
  }
};

//clasa abstracta care va defini urmatoarele clase
class PublicTransport {
  protected: char * PublicTransport_name;
  float average_spped;
  char * assigned_line;
  int max_capacity;
  int actual_capacity;
  public: PublicTransport(const char * PublicTransport_name = "N/A", int average_speed = -1,
    const char * assigned_line = "N/A",
      int max_capacity = -1,
      int actual_capacity = 0) {
    this -> PublicTransport_name = new char[strlen(PublicTransport_name) + 1];
    strcpy(this -> PublicTransport_name, PublicTransport_name);
    this -> average_spped = average_speed;
    this -> assigned_line = new char[strlen(assigned_line) + 1];
    strcpy(this -> assigned_line, assigned_line);
    this -> max_capacity = max_capacity;
    this -> actual_capacity = actual_capacity;
  }
  PublicTransport(const PublicTransport & PublicTransport) {
    this -> PublicTransport_name = new char[strlen(PublicTransport.PublicTransport_name) + 1];
    strcpy(this -> PublicTransport_name, PublicTransport.PublicTransport_name);
    this -> average_spped = PublicTransport.average_spped;
    this -> assigned_line = new char[strlen(PublicTransport.assigned_line) + 1];
    strcpy(this -> assigned_line, PublicTransport.assigned_line);
    this -> max_capacity = PublicTransport.max_capacity;
    this -> actual_capacity = PublicTransport.actual_capacity;
  }

  //destructor virtual ca sa pot sterge corect in array ul de la depou
  virtual~PublicTransport() {
    delete[] PublicTransport_name;
    delete[] assigned_line;
  }

  //nu cred ca mai are vreun sens dupa ce am facut o abstracta dar sure
  PublicTransport & operator = (const PublicTransport & PublicTransport) {
    if (this != & PublicTransport) {
      if (this -> PublicTransport_name != nullptr) {
        delete[] this -> PublicTransport_name;
      }
      this -> PublicTransport_name = new char[strlen(PublicTransport.PublicTransport_name) + 1];
      strcpy(this -> PublicTransport_name, PublicTransport.PublicTransport_name);

      if (this -> assigned_line != nullptr) {
        delete[] this -> assigned_line;
      }
      this -> assigned_line = new char[strlen(PublicTransport.assigned_line) + 1];
      strcpy(this -> assigned_line, PublicTransport.assigned_line);

      this -> average_spped = PublicTransport.average_spped;
      this -> max_capacity = PublicTransport.max_capacity;
      this -> actual_capacity = PublicTransport.actual_capacity;
    }
    return * this;
  }
  virtual PublicTransport* operator+(int passengers) const = 0;
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
  virtual PublicTransport* operator-(int passengers) const = 0;
//  friend PublicTransport operator - (const PublicTransport & t, int passengers) {
//    PublicTransport temp = t;
//    temp.actual_capacity -= passengers;
//    if (temp.actual_capacity < 0) temp.actual_capacity = 0;
//    return temp;
//  }

  //checks if 2 PublicTransports are on the same line
  bool operator == (const PublicTransport & other) const {
    return strcmp(this -> assigned_line, other.assigned_line) == 0;
  }
  //allows adding an integer to a PublicTransport(to add passangers)
//  PublicTransport operator + (int passengers) {
//    PublicTransport temp = * this;
//    temp.actual_capacity += passengers;
//    return temp;
//  }
  friend ostream & operator << (ostream & out,
    const PublicTransport & PublicTransport) {
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
  void changeLine(char * line) {
    strcpy(assigned_line, line);
  }
  float getTime(Station station) {
    return station.getDistance() * 100 / this -> average_spped;
    //nu este timpul real sau corect, este doar un exemplu la scala 1 minut = o secunda
  }
  char * getName() {
    return this -> PublicTransport_name;
  }
  //operator de incrementare ++ (prefixat și postfixat)
  PublicTransport & operator++() {
    if (actual_capacity < max_capacity)
      actual_capacity++;
    return * this;
  }
  virtual PublicTransport* operator++(int) = 0;
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
  virtual PublicTransport* operator--(int) = 0;
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
    Line temp_line = system.getLine(this -> assigned_line);
    thread inputThread(checkForExit);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution < > dista(0, (this -> max_capacity - 1) / 5);
    int randomNumber = dista(gen);
    uniform_int_distribution < > distb(0, randomNumber);
    int randomNumber2 = distb(gen);
    int global_smth = 0;
    while (!stopLoop) {
      cout << "The " << this -> getType() << "'s direction is: " << temp_line.getDirection() << endl;
      cout << endl;
      for (int i = 0; i < temp_line.getNoOfStations() - 1; i++) {
        this -> actual_capacity += randomNumber;
        randomNumber = dista(gen);
        int smth = getTime(temp_line[i]);
        global_smth += smth + 1;
        cout << "The "<<this->getType()<<" "<<this->getName()<<" is leaving: ";
        temp_line.showStations(i);
        cout << " and it will arrive at: ";
        temp_line.showStations(i + 1);
        cout << " in " << getTime(temp_line[i]) << " minutes at exactly: ";
        addMinutesAndDisplay(global_smth);
        cout <<" .The "<<this->getType()<<" capacity is: " << this -> actual_capacity << endl;
        sleep(smth);
        if (stopLoop) {
          inputThread.join();
          return;
        }
        uniform_int_distribution < > distb(0, this -> actual_capacity);
        randomNumber2 = distb(gen);
        this -> actual_capacity -= randomNumber2;

      }
      cout << "The" << this -> getType() << " arrived at: ";
      temp_line.showStations(temp_line.getNoOfStations() - 1);
      cout << " and is returning to: ";
      temp_line.showStations(0);
      cout << endl;
      cout << endl << "The "<<this->getType()<<" "<<this->getName()<<" direction is: ";
      temp_line.showStations(0);
      cout << endl << endl;
      for (int i = temp_line.getNoOfStations() - 1; i > 0; i--) {
        this -> actual_capacity += randomNumber;
        randomNumber = dista(gen);
        int smth = getTime(temp_line[i - 1]);
        global_smth += smth + 1;
        cout << "The "<<this->getType()<<" is leaving: ";
        temp_line.showStations(i);
        cout << " and it will arrive at: ";
        temp_line.showStations(i - 1);
        cout << " in " << getTime(temp_line[i - 1]) << " minutes at exactly ";
        addMinutesAndDisplay(global_smth);
        cout <<" .The "<<this->getType()<<" capacity is: " << this -> actual_capacity << endl;
        sleep(smth);
        if (stopLoop) {
          inputThread.join();
          return;
        }
        uniform_int_distribution < > distb(0, this -> actual_capacity);
        randomNumber2 = distb(gen);
        this -> actual_capacity -= randomNumber2;
      }
      cout << "The "<<this->getType()<<" arrived at: ";
      temp_line.showStations(0);
      cout << " and is returning to: ";
      temp_line.showStations(temp_line.getNoOfStations() - 1);
      cout << endl << endl;

    }
    stopLoop = false;
    inputThread.join();
  }
  //metoda pur virtuala pentru clasa abstracta
  virtual const char * getType() const = 0;
};

class Metro: public PublicTransport {
  bool is_electric;
  int no_of_wagons;
  public:
    Metro(const char * name = "N/A", int speed = -1,
      const char * line = "N/A",
        int max_cap = -1,
        int actual_cap = 0,
        bool is_electric = false,
        int no_of_wagons = -1): PublicTransport(name, speed, line, max_cap, actual_cap) {
      this -> is_electric = is_electric;
      this -> no_of_wagons = no_of_wagons;
    }
  Metro(const Metro & orig): PublicTransport(orig) {
    this -> is_electric = orig.is_electric;
    this -> no_of_wagons = orig.no_of_wagons;
  }
  Metro & operator = (const Metro & other) {
    if (this != & other) {
      PublicTransport::operator = (other); // atribuim partea din clasa de bază
      this -> is_electric = other.is_electric;
      this -> no_of_wagons = other.no_of_wagons;
    }
    return * this;
  }
  friend istream & operator >> (istream & os, Metro & metro) {
    os >> (PublicTransport & ) metro;
    os >> metro.is_electric;
    os >> metro.no_of_wagons;
    return os;
  }
  friend ostream & operator << (ostream & os,
    const Metro & metro) {
    os << (PublicTransport & ) metro;
    os  << metro.is_electric << " ";
    os << metro.no_of_wagons << " ";
    return os;
  }
  //void print() {
  //  cout << static_cast < PublicTransport > ( * this) << endl;
  //  cout << is_electric << endl;
  //  cout << no_of_wagons << endl;
  //}
  const char * getType() const {
    return "Metro";
  }
  PublicTransport* operator++(int) {
      Metro* m = new Metro(*this);
      if (actual_capacity < max_capacity)
        actual_capacity++;
      return m;
    }
  PublicTransport* operator--(int) {
      Metro* m = new Metro(*this);
      if (actual_capacity > 0)
        actual_capacity--;
      return m;
    }

  PublicTransport* operator+(int passengers) const {
      Metro* m = new Metro(*this);
      m->actual_capacity += passengers;
      return m;
    }
  PublicTransport* operator-(int passengers) const {
      Metro* m = new Metro(*this);
      m->actual_capacity -= passengers;
      if (m->actual_capacity < 0)
        m->actual_capacity = 0;
      return m;
    }

};
class Bus: public PublicTransport {
  bool is_electric;
  char * registration_number;
  bool low_floor; //acces pentru persoane cu dizabilitati
  bool doors[3]; //starea de deschidere/inchidere a fiecarei usi
  public:
    Bus(const char * name = "N/A", int speed = -1,
      const char * line = "N/A",
        int max_cap = -1,
        int actual_cap = 0,
        bool is_electric = false,
        const char * reg_nr = "N/A",
          bool low_floor = true): PublicTransport(name, speed, line, max_cap, actual_cap), is_electric(is_electric), low_floor(low_floor) {
      registration_number = new char[strlen(reg_nr) + 1];
      strcpy(registration_number, reg_nr);
      doors[0] = doors[1] = doors[2] = false;
    }
    ~Bus() {
      delete[] registration_number;
    }
  friend istream & operator >> (istream & os, Bus & bus) {
    os >> (PublicTransport & ) bus;
    os >> bus.is_electric;
    char temp[100];
    os >> temp;
    delete[] bus.registration_number;
    bus.registration_number = new char[strlen(temp) + 1];
    strcpy(bus.registration_number, temp);
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
    cout<<"Low Floor currently ";
    if(low_floor) {
      cout<< "up\n";
    }
    else
      cout<< "down\n";
    char input[50];
    cout<<"Would you like to change the low floor?(yes or no)\n";
    cin>>input;
    if(strcmp(input, "yes")==0) {
      low_floor = !low_floor;
    }
    else if(strcmp(input, "no")==0) {
      return;
    }
    else
      cout<<"invalid input\n";
  }

  const char * getType() const {
    return "Bus";
  }
  PublicTransport* operator++(int) {
      Bus* m = new Bus(*this);
      if (actual_capacity < max_capacity)
        actual_capacity++;
      return m;
    }
  PublicTransport* operator--(int) {
      Bus* m = new Bus(*this);
      if (actual_capacity > 0)
        actual_capacity--;
      return m;
    }

  PublicTransport* operator+(int passengers) const {
      Bus* b = new Bus(*this);
      b->actual_capacity += passengers;
      if (b->actual_capacity > b->max_capacity)
        b->actual_capacity = b->max_capacity;
      return b;
    }
  PublicTransport* operator-(int passengers) const {
      Bus* b = new Bus(*this);
      b->actual_capacity -= passengers;
      if (b->actual_capacity < 0)
        b->actual_capacity = 0;
      return b;
    }


};
class Tram: public PublicTransport {
  bool is_electric;
  int no_of_USBports;
  friend class Trolleybus; //!!
  public:
    Tram(const char * name = "N/A", int speed = -1,
      const char * line = "N/A",
        int max_cap = -1,
        int actual_cap = 0,
        bool is_electric = 1,
        int no_of_USBports = 0): PublicTransport(name, speed, line, max_cap, actual_cap), is_electric(is_electric), no_of_USBports(no_of_USBports) {}
  Tram(const Tram & orig): PublicTransport(orig) {
      if (this != & orig) {
        PublicTransport::operator = (orig);
        this -> is_electric = orig.is_electric;
        this -> no_of_USBports = orig.no_of_USBports;
      }
    }
    ~Tram() {}
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
    os  << tram.no_of_USBports << " ";
    return os;
  }
  const char * getType() const {
    return "Tram";
  }
  PublicTransport* operator+(int passengers) const {
      Tram* t = new Tram(*this);
      t->actual_capacity += passengers;
      if (t->actual_capacity > t->max_capacity)
        t->actual_capacity = t->max_capacity;
      return t;
    }
  PublicTransport* operator--(int) {
      Tram* m = new Tram(*this);
      if (actual_capacity > 0)
        actual_capacity--;
      return m;
    }

  PublicTransport* operator++(int) {
      Tram* m = new Tram(*this);
      if (actual_capacity < max_capacity)
        actual_capacity++;
      return m;
    }

  PublicTransport* operator-(int passengers) const {
      Tram* t = new Tram(*this);
      t->actual_capacity -= passengers;
      if (t->actual_capacity < 0)
        t->actual_capacity = 0;
      return t;
    }

};
//mostenirea diamant
class Trolleybus : public Bus, public Tram {
    bool is_connected;

public:
    Trolleybus(
        const char* name = "N/A", int speed = -1,
        const char* line = "N/A", int max_cap = -1,
        int actual_cap = 0,
        bool is_electric_bus = true,
        const char* reg_nr = "N/A",
        bool low_floor = true,
        bool is_electric_tram = true,
        int no_usb_ports = 0,
        bool is_connected = true
    ) :
        Bus(name, speed, line, max_cap, actual_cap, is_electric_bus, reg_nr, low_floor),
        Tram(name, speed, line, max_cap, actual_cap, is_electric_tram, no_usb_ports),
        is_connected(is_connected)
    {}
  Trolleybus(const Trolleybus& other)
      : Bus(static_cast<const Bus&>(other)),
        Tram(static_cast<const Tram&>(other)),
        is_connected(other.is_connected) {}

    const char* getType() const {
        return "Trolleybus";
    }

    // operator+ cu int
    PublicTransport* operator+(int passengers) const {
        Trolleybus* tb = new Trolleybus(*this);
        tb->Bus::actual_capacity += passengers;
        if (tb->Bus::actual_capacity > tb->Bus::max_capacity)
            tb->Bus::actual_capacity = tb->Bus::max_capacity;
      return static_cast<PublicTransport*>(static_cast<Bus*>(tb));
    }

    // operator- cu int
    PublicTransport* operator-(int passengers) const {
        Trolleybus* tb = new Trolleybus(*this);
        tb->Bus::actual_capacity -= passengers;
        if (tb->Bus::actual_capacity < 0)
            tb->Bus::actual_capacity = 0;
      return static_cast<PublicTransport*>(static_cast<Bus*>(tb));
    }

    // postfix ++
    PublicTransport* operator++(int) {
        Trolleybus* tb = new Trolleybus(*this);
        if (tb->Bus::actual_capacity < tb->Bus::max_capacity)
            tb->Bus::actual_capacity++;
      return static_cast<PublicTransport*>(static_cast<Bus*>(tb));
    }

    // postfix --
    PublicTransport* operator--(int) {
        Trolleybus* tb = new Trolleybus(*this);
        if (tb->Bus::actual_capacity > 0)
            tb->Bus::actual_capacity--;
      return static_cast<PublicTransport*>(static_cast<Bus*>(tb));
    }

    // suprascriere istream
    friend istream& operator>>(istream& in, Trolleybus& t) {
        in >> static_cast<Bus&>(t);           // citește PublicTransport și Bus
        in >> t.Tram::no_of_USBports;         // citește partea din Tram
        in >> t.is_connected;                 // specific Trolleybus
        return in;
    }

    // suprascriere ostream
    friend ostream& operator<<(ostream& out, const Trolleybus& t) {
        out << static_cast<const Bus&>(t);          // scrie PublicTransport și Bus
        out << t.Tram::no_of_USBports << " ";       // scrie partea din Tram
        out << t.is_connected << " ";               // scrie partea din Trolleybus
        return out;
    }
};

class Depot {
  char * depot_name;
  char * file_path;
  vector < PublicTransport * > transports; //vectori de pointers la clasa de baza ale mijloacelor de transport in comun

  public:
    Depot(const char* file_path, const char * name = "N/A") {
      this->file_path = new char[strlen(file_path) + 1];
      strcpy(this->file_path, file_path);
      depot_name = new char[strlen(name) + 1];
      strcpy(depot_name, name);
    }

    ~Depot() {
      delete[] depot_name;
      delete[] file_path;
      for (int i = 0; i < (int) transports.size(); ++i) {
        delete transports[i];
      }
      transports.clear();
    }

  Depot(const Depot & other) {
    depot_name = new char[strlen(other.depot_name) + 1];
    file_path = new char[strlen(other.file_path) + 1];
    strcpy(file_path, other.file_path);
    strcpy(depot_name, other.depot_name);

    for (int i = 0; i < (int) other.transports.size(); ++i) {
      transports.push_back(other.transports[i]);
    }
  }
void changeLine(char name[50], char line_name[50]){
      for (int i = 0; i < transports.size(); i++) {
        if(strcmp(transports[i]->getName(), name) == 0){
          transports[i]->changeLine(line_name);
          return;
        }
      }
    }
   void deleteVehicle(char nume[]){
      for(int i = 0; i < transports.size(); i++){
        if(strcmp(transports[i]->getName(), nume) == 0){
			transports.erase(transports.begin() + i);
        }
      }
    }
  Depot & operator = (const Depot & other) {
    if (this != & other) {
      delete[] depot_name;
      for (int i = 0; i < (int) transports.size(); ++i) {
        delete transports[i];
      }
      transports.clear();

      depot_name = new char[strlen(other.depot_name) + 1];
      strcpy(depot_name, other.depot_name);

      for (int i = 0; i < (int) other.transports.size(); ++i) {
        transports.push_back(other.transports[i]);
      }
    }
    return * this;
  }

  void add(PublicTransport * t) {
    transports.push_back(t);
  }

  int size() const {
    return transports.size();
  }

  PublicTransport * operator[](int index) {
    return transports[index];
  }
   PublicTransport * getTrain(char nume[]){
      for(int i = 0; i < transports.size(); i++){
        if(strcmp(transports[i]->getName(), nume) == 0){
          return transports[i];
        }
      }
      return nullptr;
    }
  friend ostream & operator << (ostream & out,
    const Depot & depot) {
    out << depot.depot_name << '\n';
    out << depot.transports.size() << '\n';

    for (int i = 0; i < depot.transports.size(); ++i) {

      if (Metro * m = dynamic_cast < Metro * > (depot.transports[i])) {
        out << "Metro " << * m << endl;
      }
      //trolleybus peste clasele derivate ca altfel nu intra niciodata aici
      else if (Trolleybus * tb = dynamic_cast < Trolleybus * > (depot.transports[i])) {
        out << "Trolleybus " << * tb << endl;
      } else if (Bus * b = dynamic_cast < Bus * > (depot.transports[i])) {
        out << "Bus " << * b << endl;
      } else if (Tram * t = dynamic_cast < Tram * > (depot.transports[i])) {
        out << "Tram " << * t << endl;
      } else {
        out << * depot.transports[i] << " UnknownType\n";
      }
    }
    return out;
  }
  friend istream & operator >> (istream & in, Depot & depot) {
    char name[100];
    int n;
    in >> name;
    delete[] depot.depot_name;
    depot.depot_name = new char[strlen(name) + 1];
    strcpy(depot.depot_name, name);

    in >> n;
    for (int i = 0; i < n; ++i) {
      string type;
      in >> type;

      if (type == "Metro") {
        Metro * m = new Metro();
        in >> * m;
        depot.transports.push_back(m);
      } else if (type == "Bus") {
        Bus * b = new Bus();
        in >> * b;
        depot.transports.push_back(b);
      } else if (type == "Tram") {
        Tram * tr = new Tram();
        in >> * tr;
        depot.transports.push_back(tr);
      } else if (type == "Trolleybus") {
        Trolleybus * t = new Trolleybus();
        in >> * t;
        depot.transports.push_back(static_cast<Bus*>(t));
      }
    }
    return in;
  }
  void addTransport(PublicTransport * t) {
     transports.push_back(t);
  }
  char * getName() const {
    return depot_name;
  }
  char * getFileName() const {
    return file_path;
  }

  void loadDepot() {
    ifstream f(file_path);
    f>>*this;
    f.close();
    f.clear();
  }
  void saveDepot() {
    ofstream f(file_path);
    f<<*this;
    f.close();
    f.clear();
  }
};


int Station::id_generator = 0;
int option, option1, option2;
float average_spped_global;
char global_station_name[50], global_line_name[50], global_PublicTransport_name[50], global_system_name[50],global_depot_name[50];
char global_pb_type[50];
//pentru comutativitatea virtuala +
inline PublicTransport* operator+(int passengers, const PublicTransport& pt) {
  return pt + passengers;
}
//pentru comutativitatea virtuala -
inline PublicTransport* operator-(int passengers, const PublicTransport& pt) {
  return pt - passengers;
}

int main() {

  vector<System> systems;
  systems.push_back(System("system_metro.txt"));
  systems.push_back(System("system_bus_tram_trolley.txt"));
  //systems.push_back(System("system_cfr.txt"));
  for (int i = 0; i < systems.size(); ++i) {
    systems[i].loadSystem();
  }

  vector<Depot> depots;
  depots.push_back(Depot("depoul_militari.txt"));
  depots.push_back(Depot("depoul_berceni.txt"));
  //depots.push_back(Depot("depoul_garadenord.txt"));
  for (int i = 0; i < depots.size(); ++i) {
  	depots[i].loadDepot();
  }
  int option = -1;
  do {
    cout << "\n===== MAIN MENU =====\n";
    cout << "1. Manage Systems\n";
    cout << "2. Manage Vehicles\n";
    cout << "0. Exit program\n";
    cout << "Enter option: ";
    cin >> option;

    switch (option) {
      case 0:
        cout << "Exiting program...\n";
        break;

      case 1: {
        int systemOption = -1;
        do {
          cout << "\n===== SYSTEM MANAGEMENT =====\n";
          cout << "1. Show all Systems\n";
          cout << "2. Show a specific System\n";
          cout << "3. Add a Station\n";
          cout << "4. Add a line\n";
          cout << "5. Remove a Station\n";
          cout << "6. Remove a line\n";
          cout << "0. Return to main menu\n";
          cout << "Enter option: ";
          cin >> systemOption;

          switch (systemOption) {
            case 0:
              cout << "Returning to main menu...\n";
              break;

            case 1: {
              cout << "\nAvailable Systems:\n";
              for (int i = 0; i < systems.size(); ++i) {
                cout << systems[i].getName() << endl;
              }
              break;
            }

            case 2: {
              cout << "Enter System name: ";
              char global_station_name[50];
              cin >> global_station_name;

              int i;
              for (i = 0; i < systems.size(); ++i) {
                if (strcmp(systems[i].getName(), global_station_name) == 0) {
                  cout << systems[i] << endl;
                  break;
                }
              }
              if (i == systems.size()) {
                cout << global_station_name << " not found!" << endl;
              }
              break;
            }

            case 3: {
              char global_system_name[50];
              char global_line_name[50];

              cout << "Enter System name to edit: ";
              cin >> global_system_name;
              cout << "Enter Line name to edit: ";
              cin >> global_line_name;

              Station station;
              cout << "Enter Station distance, name and boolean value for change:\n";
              cin >> station;

              int addOption;
              cout << "1. Add to Back\n2. Add to Front\nEnter option: ";
              cin >> addOption;

              for (int i = 0; i < systems.size(); ++i) {
                if (strcmp(systems[i].getName(), global_system_name) == 0) {
                  if (addOption == 1) {
                    systems[i].addToLineBack(station, global_line_name);
                    cout << "Station added to the back of the line.\n";
                  } else if (addOption == 2) {
                    systems[i].addToLineFront(station, global_line_name);
                    cout << "Station added to the front of the line.\n";
                  } else {
                    cout << "Invalid option.\n";
                  }
                  break;
                }
              }
              break;
            }
			case 4:{
            	cout<<" Enter the System name: ";
                cin >> global_system_name;
                cout << "Enter line name: ";
                cin >> global_line_name;
                for (int i = 0; i < systems.size(); ++i) {
                  if (strcmp(systems[i].getName(), global_system_name) == 0) {
                    systems[i].addLine(global_line_name);
                    break;
                  }

                }
                break;
			}
            case 5:{
            	cout<<"Insert the System's name you're trying to edit: ";
                cin >> global_system_name;
                cout <<"Insert Line's name you're trying to edit: ";
                cin >> global_line_name;
                cout << "You can only delete the front or back of the line.\n";
                cout << "Enter option: ";
                cin >> option2;
                for (int i = 0; i < systems.size(); ++i) {
                  if (strcmp(systems[i].getName(), global_system_name) == 0) {
                  	if (option2 == 1) {
                          systems[i].deleteFromLineBack(global_line_name);
                  	}
                    else if (option2 == 2) {
                          systems[i].deleteFromLineFront(global_line_name);
                    }
                    else
                      cout << "Invalid option.\n";
                    break;
                  }

                }
                break;
            }
            case 6:{
              cout<<"Insert System's name you're trying to edit: ";
              cin >> global_system_name;
              cout <<"Insert Line's name you're trying to delete: ";
              cin >> global_line_name;
              for (int i = 0; i < systems.size(); ++i) {
                if (strcmp(systems[i].getName(), global_system_name) == 0) {
					systems[i].deleteLine(global_line_name);
                    break;
                }
              }
              break;
            }
            case 7:{
           		cout<<"Insert System's name you're trying to edit the ride's price: ";
                cin >> global_system_name;
                for (int i = 0; i < systems.size(); ++i) {
                  if (strcmp(systems[i].getName(), global_system_name) == 0) {
                    systems[i]++;
                    break;
                  }

                }
				break;
            }
            default:
              cout << "Invalid option. Please try again.\n";
              break;
          }
        } while (systemOption != 0);
        break;
      }

      case 2: {
        int vehicleOption = -1;
        do {
          cout << "\n===== VEHICLE MANAGEMENT =====\n";
          cout << "1. Show all Depots\n";
          cout << "2. Add a vehicle\n";
          cout << "3. To show a specific depot\n";
          cout << "4. To assign other line to a vehicle\n";
          cout << "5. To delete a vehicle\n";
          cout << "!6. To focus on a Vehicle's ride\n";
          cout << "7. Check and edit low floor of a bus or trolley\n";
          cout << "0. Return to main menu\n";
          cout << "Enter option: ";
          cin >> vehicleOption;

          switch (vehicleOption) {
            case 0:
              cout << "Returning to main menu...\n";
              break;

            case 1:
              cout<<'\n';
			  for (int i = 0; i < depots.size(); ++i) {
              	cout << depots[i].getName() << "\n";
			  }
              break;

            case 2:{
              cout << "Select a Depot: ";
              cin >> global_depot_name;
              cout << "Select vehicle type: ";
              cin >> global_pb_type;
              for (int i = 0; i < depots.size(); ++i) {
                if (strcmp(depots[i].getName(), global_depot_name) == 0) {
                	if(strcmp(global_pb_type, "Metro") == 0) {
                        //polimorfism
                        cout<<"Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), and Number of Wagons\n";
               	 		Metro * m = new Metro();
                		cin>> *m;
                		depots[i].add(m);

              		}
                    else if(strcmp(global_pb_type, "Bus") == 0) {
                        //polimorfism
                        cout<<"Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Registration Number, and Low Floor Status\n";
                     	Bus * b = new Bus();
                        cin>>*b;
                        depots[i].add(b);


                    }
                    else if(strcmp(global_pb_type, "Tram") == 0) {
                      //polimorfism
                      cout<<"Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Number of USB ports\n";
                    	Tram * t = new Tram();
                        cin>>*t;
                        depots[i].add(t);

                    }
                    else if(strcmp(global_pb_type, "Trolleybus") == 0) {
                      //polimorfism
                      cout<<"Insert Name, Assigned Line, Average Speed, Maximum Capacity, Actual Capacity, Electric?(1/0), Registration Number, and Low Floor Status, Number of USB ports, Plugged Status\n";
                      Trolleybus * t = new Trolleybus();
                      cin>>*t;
                      depots[i].add(static_cast<Bus*>(t));

                    }
                    else
                      cout << "Invalid option.\n";
                   	break;
                }
              }

              break;
			}
            case 3:{
              cout<<"Select a depot: ";
              cin>>global_depot_name;
              for (int i = 0; i < depots.size(); ++i) {
                if (strcmp(depots[i].getName(), global_depot_name) == 0) {
                  cout<<depots[i]<<"\n";
                  break;
                }
              }
              break;
            }
            case 4:{
            	cout<<"Insert a Depot: ";
                cin>>global_depot_name;
                cout <<"Insert vehicle name: ";
                cin>>global_pb_type;
                cout << "Insert a new line: ";
                cin>>global_line_name;
                for (int i = 0; i < depots.size(); ++i) {
                  if (strcmp(depots[i].getName(), global_depot_name) == 0) {
                    depots[i].changeLine(global_pb_type, global_line_name);
                  }
                }
            }
            case 5:{
              cout<<"Insert a depot: ";
              cin>>global_depot_name;
              cout<<"Insert a vehicle: ";
              cin>>global_pb_type;
              for (int i = 0; i < depots.size(); ++i) {
                if (strcmp(depots[i].getName(), global_depot_name) == 0) {
                  depots[i].deleteVehicle(global_pb_type);
                  break;
                }
              }
            }
            case 6:{
			  //cea mai mare prajeala care exista
              cout<<"Insert System: ";
              cin>>global_system_name;
              cout<<"Insert a depot: ";
              cin>>global_depot_name;
              cout<<"Insert a vehicle: ";
              cin>>global_pb_type;
			  for (int i = 0; i < depots.size(); ++i) {
              	if (strcmp(depots[i].getName(), global_depot_name) == 0) {
                  //polimorfism
                  PublicTransport * temp_pb = depots[i].getTrain(global_pb_type);
                  for(int j = 0; j < systems.size(); ++j) {
                    if (strcmp(systems[j].getName(), global_system_name) == 0) {
                      temp_pb->setTren(systems[j]);
                    }
                  }
              	  delete temp_pb;
              	}
			  }
              break;
            }
            case 7:{
                cout<<"Insert a public transport name: ";
                cin>>global_pb_type;
                int i;
                for ( i = 0; i < depots.size(); ++i) {
                  for (int j = 0; j < systems.size(); ++j) {
                    if(strcmp(depots[i][j]->getName(), global_pb_type) == 0) {
                      if(strcmp(depots[i][j]->getType(), "Bus") == 0 || strcmp(depots[i][j]->getType(), "Tram") == 0) {
                        //polimorfism
                        Bus * b = dynamic_cast<Bus*>(depots[i][j]);
                        b->switchLowFloor();
                        delete b;
                      }
                      else
                        cout<<"This vehicle doesn't suppport low floor.\n";
                      break;
                    }

                  }
                }
                if(i == depots.size())
                  cout<<"The vehicle could not be found.\n";
                break;
              }

            default:
              cout << "Invalid option. Please try again.\n";
              break;
          }
        } while (vehicleOption != 0);
        break;
      }

      default:
        cout << "Invalid option. Please try again.\n";
        break;
    }
  } while (option != 0);

  // Save systems and depot before exiting
  for (int i = 0; i < systems.size(); ++i) {
    systems[i].saveSystem();
  }
  for (int i = 0; i < depots.size(); ++i) {
    depots[i].saveDepot();
  }

  cout << "All data saved. Program terminated.\n";
  return 0;
}