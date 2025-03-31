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
    cout << put_time(localtime(&future_c), "%H:%M:%S");
}
const int MAX_LENGHT=256;
atomic<bool> stopLoop(false);
void checkForExit() {
    string input;
    while (true) {
        cin >> input;
        if (input == "exit") {   //nu eu am scris asta dar n are legatura cu poo
            stopLoop = true;
            break;
        }
    }
}
class Station{
  float distance; //distanta fata de urmatoarea statie
  char* station_name;
  bool change;

  const int station_id;
  static int id_generator;
  //nu prea merge treaba cu generatul de id uri ptr ca de-a lunugul proiecteului mai
	//generez statii temporare auxiliare si se baga peste id-ul celor care exista pe magistrale
  public:
    Station(float distance = 0, const char* station_name = "N/A", bool change = false) : station_id(id_generator++){
        this->distance = distance;
        this->station_name = new char[strlen(station_name) + 1];
        strcpy(this->station_name, station_name);
        this->change = change;
    }
	//constructor de copiere
    Station(const Station& station) : station_id(id_generator++) {
        this->distance = station.distance;
        this->station_name = new char[strlen(station.station_name) + 1];
        strcpy(this->station_name, station.station_name);
        this->change = station.change;
    }
    ~Station(){
      delete[] station_name;
    }
    float getDistance(){
      return this->distance;
    }
	char* getStationName(){
          return this->station_name;
	}
	friend istream& operator>>(istream& in, Station& station){
  		char temp_name[100];
  		in >> station.distance >> temp_name >> station.change;
  		delete[] station.station_name;
  		station.station_name = new char[strlen(temp_name) + 1];
  		strcpy(station.station_name, temp_name);
  		return in;
	}

    friend ostream& operator<<(ostream& out, const Station& station){
      out << station.distance << " " << station.station_name << " " << station.change;
      return out;
    }

    Station& operator=(const Station& station){
      if(this != &station) {
        this->distance = station.distance;
        delete[] station_name;
        this->station_name = new char[strlen(station.station_name) + 1];
        strcpy(this->station_name, station.station_name);
        this->change = station.change;
      }
      return *this;
    }
    void showStation () const {
      cout << distance << " " << station_name << " " << change << " " << station_id << endl;
    }
};

class Line{
    vector<Station> stations;
    float length;
    int no_of_stations;
    char line_name[50]; //aici am folosit char normal
    public:
      Line(int no_of_stations = 0, float length = 0, const char* line_name = "N/A"){
        this->length = length;
        strcpy(this->line_name, line_name);
        this->no_of_stations = no_of_stations;
      }

      ~Line(){
        stations.clear();
        //se dezaloca singura memoria right dar tot am fct ceva
      }

      Line& operator=(const Line& line){
        if(this != &line) {
          this->length = line.length;
          strcpy(this->line_name, line.line_name);
          this->no_of_stations = line.no_of_stations;
          for(int i = 0; i < line.no_of_stations; i++){
            this->stations.push_back(line.stations[i]);
            this->no_of_stations++;
          }
        }
        return *this;
      }
      int getNoOfStations(){
        return this->no_of_stations;
      }
      Line(const Line& line){
        this->length = line.length;
        strcpy(this->line_name, line.line_name);
        this->no_of_stations = line.no_of_stations;
        stations = line.stations;
      }

      void addStationBack(const Station& station){
        stations.push_back(station);
		no_of_stations++;
      }

      void addStationFront(const Station& station){
        stations.insert(stations.begin(), station);
        no_of_stations++;
      }
	  void deleteStationBack(){
            stations.pop_back();
            no_of_stations--;
	  }
      void deleteStationFront(){
        stations.erase(stations.begin());
        no_of_stations--;
      }
      void printLine() const {
        for(size_t i = 0; i < stations.size(); i++){
          cout<<stations[i];
          cout<<endl;
        }
      }
    //overloading the '+' operator
      char* getLineName(){
        return line_name;
      }
     float getStationDistance(char* station_name){
       for(int i = 0; i < stations.size(); i++){
         if(strcmp(station_name, stations[i].getStationName()) == 0){
           return stations[i].getDistance();
         }
       }
       return 0;
     }
//basically getStation()
  Station& operator[](int index) {
    return stations[index];
      }
  float getLineLenght(int index){
    return length;
  }
     Station getStationBack(){
       return stations.back();
     }
     Station getStationFront(){
       return stations.front();
     }
     char* getDirection(){
       return stations[stations.size()-1].getStationName();
     }
     void showStations (int i){
       cout<<stations[i].getStationName();
     }
  Station getStation(int i){
        return stations[i];
      }
	friend Line operator+(Line &a, Station b){
          a.length += b.getDistance();
          return a;
	}
    friend Line operator+(Station& b, Line& a) {
    	return a + b;
	}
    //overloading the '-' operator
    friend Line operator-(Line &a, Station b){
      a.length -= b.getDistance();
      return a;
    }
     friend istream& operator>>(istream& in, Line& line){
		in>>line.no_of_stations>>line.line_name>>line.length;
        line.stations.clear();
        for(int i = 0; i < line.no_of_stations; i++){
          Station station;
          in>>station;
          line.stations.push_back(station);

        }
        return in;
     }
     friend ostream& operator<<(ostream& out, const Line& line){
       out<<line.no_of_stations<<' '<<line.line_name<<' '<<line.length<<endl;
       for(int i = 0; i < line.no_of_stations; i++){
         out<<line.stations[i]<<endl;
       }
       return out;
     }
};
class System{
  char *system_name;
  float trip_price;
  vector<Line> lines;
public:
System(const char *system_name="N/A", float trip_price = 0){
    this->system_name = new char[strlen(system_name) + 1];
    strcpy(this->system_name, system_name);
    this->trip_price = trip_price;
}
  System(const System& system){
    if(this->system_name != nullptr){
      delete[] this->system_name;
    }
    this->system_name = new char[strlen(system.system_name) + 1];
    strcpy(this->system_name, system.system_name);
    this->trip_price = system.trip_price;
  }
  ~System(){
    if(this->system_name != nullptr){
      delete[] this->system_name;
    }
  }
  void addLine(const char name[50]){
    Line line(0, 0, name);
    lines.push_back(line);
  }
  Line getLine(char* name){
    for(int i = 0; i < lines.size(); i++){
      if(strcmp(name, lines[i].getLineName()) == 0){
          cout<<lines[i];
          return lines[i];
      }
    }
    return Line();
  }
  void deleteLine(const char name[]){
    for(int i = 0; i < lines.size(); i++){
      if(!strcmp(name, lines[i].getLineName())){
        lines.erase(lines.begin() + i);
        break;
      }
    }
  }
  void addToLineBack(const Station& station, char name[50]){
    for(int i = 0; i < lines.size(); i++){
      if(!strcmp(lines[i].getLineName(), name)){
        lines[i].addStationBack(station);
       	lines[i] = lines[i] + station;
        return;
      }
    }
  }
  void deleteFromLineBack(const char name[50]){
    for(int i = 0; i < lines.size(); i++){
      if(!strcmp(lines[i].getLineName(), name)){
        Station temp = lines[i].getStationBack();
        lines[i].deleteStationBack();
        lines[i] = lines[i] - temp;
        return;
      }
    }
  }
  void deleteFromLineFront(char name[50]){
    for(int i = 0; i < lines.size(); i++){
      if(!strcmp(lines[i].getLineName(), name)){
        Station temp = lines[i].getStationFront();
        lines[i].deleteStationFront();
        lines[i] = lines[i] - temp;
        return;
      }
    }
  }
  void addToLineFront(const Station& station, char name[50]){
    for(int i = 0; i < lines.size(); i++){
      if(!strcmp(lines[i].getLineName(), name)){
        lines[i].addStationFront(station);
        lines[i] = lines[i] + station;
        return;
      }
    }
  }
  Line& operator[](const int i) {
    return lines[i];
}
	int getLineLenght1(char name[]){
          for(int i = 0; i < lines.size(); i++){
            if(!strcmp(lines[i].getLineName(), name)){
              return lines[i].getLineLenght(i);
            }
          }
       return 0;
	}
  System& operator=(const System& system){
    if(this->system_name != nullptr){
      delete[] this->system_name;
    }
    this->system_name = new char[strlen(system.system_name) + 1];
    strcpy(this->system_name, system.system_name);
    this->trip_price = system.trip_price;
    return *this;
  }
  friend ostream& operator<<(ostream& out, const System& system){
	out<<system.system_name<<' '<<system.trip_price<<endl;
    for(int i = 0; i < system.lines.size(); i++){
      out<<system.lines[i]<<endl;
    }
    return out;
  }
  friend istream& operator>>(istream& in, System& system){
    if(system.system_name != nullptr) {
        delete[] system.system_name;
    }
    char temp_name[100];
    in>>temp_name>>system.trip_price;
    system.system_name = new char[strlen(temp_name) + 1];
    strcpy(system.system_name, temp_name);
	system.lines.clear();
    Line temp;
    while(in>>temp){
      system.lines.push_back(temp);
    }
    return in;
  }
  //preturile doar se scumpesc nu si scad, evident lol
  System &operator++() {
        trip_price++;
        return *this;
    }

    System operator++(int) {
        System temp = *this;
        trip_price++;
        return temp;
    }
	void loadSystem(){
    	ifstream f("system.txt");
    	f >> *this;
        f.clear();
    	f.close();
	}
	void saveSystem(){
    	ofstream f("system.txt");
        f << *this;
        f.clear();
        f.close();
	}
};
class PublicTransport{
  protected:
  char *PublicTransport_name;
  float average_spped;
  char* assigned_line;
  int max_capacity;
  int actual_capacity;
  public:
    PublicTransport(const char* PublicTransport_name = "N/A", int average_speed=-1,const char* assigned_line= "N/A", int max_capacity=-1, int actual_capacity=0){
		this->PublicTransport_name = new char[strlen(PublicTransport_name) + 1];
        strcpy(this->PublicTransport_name, PublicTransport_name);
        this->average_spped = average_speed;
        this->assigned_line = new char[strlen(assigned_line) + 1];
        strcpy(this->assigned_line, assigned_line);
        this->max_capacity = max_capacity;
        this->actual_capacity = actual_capacity;
    }
    PublicTransport(const PublicTransport& PublicTransport){
      this->PublicTransport_name = new char[strlen(PublicTransport.PublicTransport_name) + 1];
      strcpy(this->PublicTransport_name, PublicTransport.PublicTransport_name);
      this->average_spped = PublicTransport.average_spped;
      this->assigned_line = new char[strlen(PublicTransport.assigned_line) + 1];
      strcpy(this->assigned_line, PublicTransport.assigned_line);
      this->max_capacity = PublicTransport.max_capacity;
      this->actual_capacity = PublicTransport.actual_capacity;
    }
    ~PublicTransport(){
      delete[] PublicTransport_name;
      delete[] assigned_line;
    }
   PublicTransport &operator=(const PublicTransport& PublicTransport){
  if(this != &PublicTransport) {
    if(this->PublicTransport_name != nullptr){
      delete[] this->PublicTransport_name;
    }
    this->PublicTransport_name = new char[strlen(PublicTransport.PublicTransport_name) + 1];
    strcpy(this->PublicTransport_name, PublicTransport.PublicTransport_name);

    if(this->assigned_line != nullptr){
      delete[] this->assigned_line;
    }
    this->assigned_line = new char[strlen(PublicTransport.assigned_line) + 1];
    strcpy(this->assigned_line, PublicTransport.assigned_line);

    this->average_spped = PublicTransport.average_spped;
    this->max_capacity = PublicTransport.max_capacity;
    this->actual_capacity = PublicTransport.actual_capacity;
  }
  return *this;
}
friend PublicTransport operator+(const PublicTransport& t, int passengers) {
    PublicTransport temp = t;
    temp.actual_capacity += passengers;
    return temp;
}
//+ cu int comutativ
friend PublicTransport operator+(int passengers, const PublicTransport& t) {
    return t + passengers;
}
//- nu int
friend PublicTransport operator-(const PublicTransport& t, int passengers) {
    PublicTransport temp = t;
    temp.actual_capacity -= passengers;
    if (temp.actual_capacity < 0) temp.actual_capacity = 0;
    return temp;
}

//checks if 2 PublicTransports are on the same line
  bool operator==(const PublicTransport& other) const {
      return strcmp(this->assigned_line, other.assigned_line) == 0;
    }
 //allows adding an integer to a PublicTransport(to add passangers)
  PublicTransport operator+(int passengers) {
      PublicTransport temp = *this;
      temp.actual_capacity += passengers;
      return temp;
    }
    friend ostream& operator<<(ostream& out, const PublicTransport& PublicTransport){
      out<<PublicTransport.PublicTransport_name << " ";
      out<<PublicTransport.assigned_line << " ";
      out<<PublicTransport.average_spped << " " ;
      out<<PublicTransport.max_capacity << " ";
      out<<PublicTransport.actual_capacity << " ";
      return out;
    }
    friend istream& operator>>(istream& in, PublicTransport& PublicTransport){
      in>>PublicTransport.PublicTransport_name>>PublicTransport.assigned_line>>PublicTransport.average_spped>>PublicTransport.max_capacity>>PublicTransport.actual_capacity;
      return in;
    }
    void changeLine(char* line){
      strcpy(assigned_line, line);
    }
    float getTime(Station station)
    {
      return station.getDistance() * 100 / this->average_spped;
      //nu este timpul real sau corect, este doar un exemplu la scala 1 minut = o secunda
    }
    char* getName(){
      return this->PublicTransport_name;
    }
    //operator de incrementare ++ (prefixat și postfixat)
    PublicTransport &operator++() {
        if (actual_capacity < max_capacity)
            actual_capacity++;
        return *this;
    }

    PublicTransport operator++(int) {
        PublicTransport temp = *this;
        if (actual_capacity < max_capacity)
            actual_capacity++;
        return temp;
    }
    //operator de decrementare
    PublicTransport &operator--() {
        if (actual_capacity > 0)
            actual_capacity--;
        return *this;
    }

    PublicTransport operator--(int) {
        PublicTransport temp = *this;
        if (actual_capacity > 0)
            actual_capacity--;
        return temp;
    }// operator de comparatie
    bool operator<(const PublicTransport &other) const {
      return this->average_spped < other.average_spped;
    }
    bool operator>(const PublicTransport &other) const {
      return this->average_spped > other.average_spped;
    }
    //urmeaza cea mai oribila si glitch uita functie din lume
    void setTren(System &system){
        Line temp_line = system.getLine(this->assigned_line);
        thread inputThread(checkForExit);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dista(0, (this->max_capacity-1)/5);
        int randomNumber = dista(gen);
        uniform_int_distribution<> distb(0, randomNumber);
        int randomNumber2 = distb(gen);
        int global_smth = 0;
        while(!stopLoop){
          cout<<"The PublicTransport's direction is: "<<temp_line.getDirection()<<endl;
          cout<<endl;
          for(int i = 0; i < temp_line.getNoOfStations()-1; i++){
            this->actual_capacity += randomNumber;
            randomNumber = dista(gen);
            int smth = getTime(temp_line[i]);
            global_smth += smth+1;
            cout << "The PublicTransport is leaving: ";
            temp_line.showStations(i);
            cout << " and it will arrive at: ";
            temp_line.showStations(i+1);
            cout << " in "<< getTime(temp_line[i]) << " minutes at exactly: ";
            addMinutesAndDisplay(global_smth);
            cout << ". PublicTransport capacity is: " << this->actual_capacity<<endl;
            sleep(smth);
            if(stopLoop){
              inputThread.join();
              return;
            }
            uniform_int_distribution<> distb(0, this->actual_capacity);
            randomNumber2 = distb(gen);
            this->actual_capacity -= randomNumber2;

          }
          cout<<"The PublicTransport arrived at: ";
          temp_line.showStations(temp_line.getNoOfStations()-1);
          cout<<" and is returning to: ";
          temp_line.showStations(0);
          cout<<endl;
          cout<<endl<<"The PublicTransport's direction is: ";
          temp_line.showStations(0);
          cout<<endl<<endl;
          for(int i = temp_line.getNoOfStations()-1; i >0 ; i--){
            int smth = getTime(temp_line[i-1]);
            global_smth+=smth+1;
            cout<<"The PublicTransport is leaving: ";
            temp_line.showStations(i);
            cout<<" and it will arrive at: ";
            temp_line.showStations(i-1);
            cout<<" in "<< getTime(temp_line[i-1])<<" minutes at exactly ";
            addMinutesAndDisplay(global_smth);
            cout<<endl;
            sleep(smth);
            if(stopLoop){
              inputThread.join();
              return;
            }
          }
          cout<<"The PublicTransport arrived at: ";
          temp_line.showStations(0);
          cout<<" and is returning to: ";
          temp_line.showStations(temp_line.getNoOfStations()-1);
          cout<<endl<<endl;

        }
        stopLoop = false;
        inputThread.join();
    }
};
class Metro : public PublicTransport{
  bool is_electric;
  int no_of_wagons;
  public:
    Metro() : PublicTransport(), is_electric(true), no_of_wagons(1) {}
    friend istream& operator<<(ostream& os, const Metro& metro){
      os>>static_cast<PublicTransport&>(metro);
      os>>is_electric;
      os>>no_of_wagons;
      return os;
    }
    void print(){
      cout << static_cast<PublicTransport>(*this)<<endl;
      cout << is_electric<<endl;
      cout << no_of_wagons<<endl;
    }
};
class Depot{
  char *depot_name;
  PublicTransport* PublicTransports;
  int no_of_PublicTransports;
  public:
    Depot(const char* depot_name = "N/A" , int no_of_PublicTransports = 0){
      this->depot_name = new char[strlen(depot_name) + 1];
      strcpy(this->depot_name, depot_name);
      this->no_of_PublicTransports = no_of_PublicTransports;
      PublicTransports = new PublicTransport[MAX_LENGHT];
    }
    ~Depot(){
      delete[] PublicTransports;
      delete [] depot_name;
    }
    Depot(const Depot& depot){
      if (depot.depot_name != nullptr){
        delete[] depot_name;
      }
      if (depot.PublicTransports != nullptr){
        delete[] depot.PublicTransports;
      }
      this->depot_name = new char[strlen(depot.depot_name) + 1];
      strcpy(this->depot_name, depot.depot_name);
      this->no_of_PublicTransports = depot.no_of_PublicTransports;
      PublicTransports = new PublicTransport[MAX_LENGHT];
    }
    PublicTransport& operator[](int index) {
      return PublicTransports[index];
    }
    PublicTransport getFastestPublicTransport() {
      PublicTransport temp_PublicTransport;
      for (int i = 0; i < no_of_PublicTransports-1; i++) {
        if (PublicTransports[i]>PublicTransports[i+1]){
          temp_PublicTransport = PublicTransports[i];
        }
      }
      return temp_PublicTransport;
    }
    friend ostream& operator<<(ostream& out, const Depot& depot){
      out<<depot.depot_name << " "<<depot.no_of_PublicTransports<<" "<<endl;
      for (int i = 0; i < depot.no_of_PublicTransports; i++) {
        out<<depot.PublicTransports[i]<<endl;
      }
      return out;
    }
    friend istream& operator>>(istream& in, Depot& depot){
      in>>depot.depot_name>>depot.no_of_PublicTransports;
      for (int i = 0; i < depot.no_of_PublicTransports; i++) {
        in>>depot.PublicTransports[i];
      }
      return in;
    }
    void changeLine(char name[50], char line_name[50]){
      for (int i = 0; i < no_of_PublicTransports; i++) {
        if(strcmp(PublicTransports[i].getName(), name) == 0){
          PublicTransports[i].changeLine(line_name);
          return;
        }
      }
    }
    PublicTransport getPublicTransport(char nume[]){
      for(int i = 0; i < no_of_PublicTransports; i++){
        if(strcmp(PublicTransports[i].getName(), nume) == 0){
          return PublicTransports[i];
        }
      }
      return PublicTransport();
    }
    void deletePublicTransport(char nume[]){
      for(int i = 0; i < no_of_PublicTransports; i++){
        if(strcmp(PublicTransports[i].getName(), nume) == 0){
          for(int j = i; j < no_of_PublicTransports-1; j++){
            PublicTransports[j] = PublicTransports[j+1];
          }
        }
      }
      no_of_PublicTransports--;
    }
    void addPublicTransport(PublicTransport PublicTransport){
      PublicTransports[this->no_of_PublicTransports] = PublicTransport;
      this->no_of_PublicTransports++;
    }
    void loadDepot(){
      ifstream f("depot.txt");
      f>>*this;
      f.clear();
      f.close();
    }
    void saveDepot(){
      ofstream f("depot.txt");
      f<<*this;
      f.clear();
      f.close();
    }
};
int Station::id_generator = 0;
int option, option1, option2;
float average_spped_global;
char global_station_name[50], global_line_name[50], global_PublicTransport_name[50];
int main() {
  Metro metro;
  metro.print();
  /*System metrorex;
  Depot depoul1;
  metrorex.loadSystem();
  depoul1.loadDepot();
  cout<<"1. For managing the Lines"<<endl;
  cout<<"2. For managing the PublicTransports"<<endl;
  cout<<"0 To exit the program"<<endl;
  cin>>option;
  while(option != 0){
    switch(option){
      case 1:{
        cout<<"1. For showing the System"<<endl;
        cout<<"2. For adding a station"<<endl;
        cout<<"3. For adding a line"<<endl;
        cout<<"4. For deleting a station"<<endl;
        cout<<"5. For deleting a line"<<endl;
        cout<<"6. Increase the Ride Price by one"<<endl;
        cout<<"7. Print one Line's Lenght"<<endl;
        cout<<"0 to go back"<<endl;
        cin>>option1;
        while(option1 != 0){
        	switch(option1){
                  case 1:{
                    cout<<metrorex;
                    break;
                  }
                  case 2:{
                    cout<<"Insert the Line where you're looking to add the Station"<<endl;
                    cin>>global_line_name;
                    Station station;
                    cout<<"Insert the Station distace, name and boolean value for change"<<endl;
                    cin>>station;
                    cout<<"Insert 1 for push_back, 2 for push_front"<<endl;
                    cin>>option2;
                    if(option2 == 1){
                      metrorex.addToLineBack(station,global_line_name);
                    }
                    else if(option2 == 2){
                      metrorex.addToLineFront(station,global_line_name);
                    }
                    break;
               		//de adaugat else optiune gresita
                  }
                  case 3:{
                    cout<<"Insert Line's name"<<endl;
                    cin>>global_line_name;
                    metrorex.addLine(global_line_name);
                    break;
                  }
                  case 4:{
                    cout<<"Attention! In a subway system you can only delete the first or last station!"<<endl;
                    cout<<"Insert the Line's name"<<endl;
 					cin>>global_line_name;
                    cout<<"1. For pop_back, 2. For pop_front"<<endl;
 					cin>>option2;
                    if(option2 == 1){
                      metrorex.deleteFromLineBack(global_line_name);
                    }
                    else if(option2 == 2){
                      metrorex.deleteFromLineFront(global_line_name);
                    }
                    break;
                  }
                  case 5:{
                    cout<<"Insert Line's name"<<endl;
                    cin>>global_line_name;
                    metrorex.deleteLine(global_line_name);
                    break;
                  }
                  case 6:{
                    metrorex++;
                    break;
                  }
                  case 7:{
                    cout<<"Insert Line's name"<<endl;
                    cin>>global_line_name;
                    cout << metrorex.getLineLenght1(global_line_name)<<endl;
                  }
        	}
     		cout<<"1. For showing the System"<<endl;
        	cout<<"2. For adding a station"<<endl;
        	cout<<"3. For adding a line"<<endl;
            cout<<"4. For deleting a station"<<endl;
            cout<<"5. For deleting a line"<<endl;
        	cout<<"6. Increase the Ride Price by one"<<endl;
        	cout<<"0 To go back"<<endl;
            cin>>option1;
        }
        break;
      }
      case 2:{
			cout<<"1. For showing the PublicTransports"<<endl;
            cout<<"2. To assign other line to a PublicTransport"<<endl;
            cout<<"3. To add a PublicTransport in the Depot"<<endl;
            cout<<"!4. To focus on a PublicTransport's ride"<<endl;
            cout<<"5. To delete a PublicTransport from the Depot!"<<endl;
            cout<<"6. To return the fastest PublicTransport in the Depot"<<endl;
            cout<<"0. For going back"<<endl;
            cin>>option1;
            while(option1 != 0){
              switch(option1){
                case 1:{
					cout<<depoul1;
                    break;
                }
                case 2:{
                  cout<<"Insert the PublicTransport you are looking for"<<endl;
                  cin>>global_PublicTransport_name;
                  cout<<"Insert the Line name"<<endl;
                  cin>>global_line_name;
                  depoul1.changeLine(global_PublicTransport_name,global_line_name);
                  break;
                }
                case 3:{
                  cout<<"Insert PublicTransport's name, assigned Line's name, Average Speed and Maximum Capacity and Curent Capacity"<<endl;
                  PublicTransport temp_PublicTransport;
				  cin>>temp_PublicTransport;
                  depoul1.addPublicTransport(temp_PublicTransport);
                  break;
                }
                case 4:{
                  cout<<"Insert name of the PublicTransport you are looking for"<<endl;
                  cin>>global_PublicTransport_name;
                  PublicTransport temp_PublicTransport = depoul1.getPublicTransport(global_PublicTransport_name);
                  //cout<<temp_PublicTransport;
                  stopLoop = false;
                  temp_PublicTransport.setTren(metrorex);
                  break;
                }
                case 5:{
                  cin>>global_PublicTransport_name;
                  depoul1.deletePublicTransport(global_PublicTransport_name);
                  break;
                }
                case 6:{
                  cout<<depoul1.getFastestPublicTransport()<<endl;
                }
              }
      		cout<<"1. For showing the PublicTransports"<<endl;
            cout<<"2. To assign other line to a PublicTransport"<<endl;
            cout<<"3. To add a PublicTransport in the Depot"<<endl;
            cout<<"!4. To focus on a PublicTransport's ride"<<endl;
            cout<<"5. To delete a PublicTransport from the Depot!"<<endl;
            cout<<"6. To return the fastest PublicTransport in the Depot"<<endl;
            cout<<"0. For going back"<<endl;
            cin>>option1;
            }
        break;
      }
    }
   	cout<<"1. For managing the Lines"<<endl;
  	cout<<"2. For managing the PublicTransports"<<endl;
  	cout<<"0 To exit the program"<<endl;
    cin>>option;
  }
  metrorex.saveSystem();
  depoul1.saveDepot();*/
  return 0;
}