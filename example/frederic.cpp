#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <map>

const std::string FILE_PATH = "./data/one_million_rows.txt";

struct CityStruct {
  double mininum = 0;
  double maximum = 0;

  double temperature_acc = 0;
  int station_counter = 0;

  std::string city;

  CityStruct() = delete;
  CityStruct(double temp, std::string& city) : city(city) {
    update(temp);
  }

  void update(double temp) {
    if (temp < mininum) mininum = temp;
    if (temp > maximum) maximum = temp;
    temperature_acc += temp;
    station_counter++;
  }

  double computeMean() const {
    return temperature_acc / station_counter;
  }

  void display() const {
    std::cout << to_string();
  }


  std::string to_string() const {
    std::ostringstream oss;
    oss << city << "=" << mininum << "/" << computeMean() << "/" << maximum;
    return oss.str();
  }
};

int main () {
  std::ifstream file(FILE_PATH);

  if(!file.is_open()) {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }

  std::string line;
  double temp;
  std::string city;

  std::map<std::string,CityStruct> stations;

  while(getline(file, line)) {
    std::istringstream stringstream(line);

    getline(stringstream, city, ';');
    stringstream >> temp;

    auto [it, inserted] = stations.try_emplace(city, CityStruct{temp, city});
    if (!inserted) {
      it->second.update(temp);
    }
  }

  std::cout << "{";
  bool first = true;
  for(auto& [key, value]: stations) {
    if(!first) std::cout << ", ";
    value.display();
    first = false;
  }
  std::cout << "}" << std::endl;
}
