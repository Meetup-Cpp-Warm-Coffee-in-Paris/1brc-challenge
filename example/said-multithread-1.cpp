#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>
#include <numeric>
#include <chrono>
#include <filesystem>
#include <stdexcept>
#include <queue>
#include <future>

namespace fs = std::filesystem;
const std::string filename = "data/one_million_rows.txt";

template <typename T>
class ThreadSafeQueue {
    std::mutex mutex;
    std::condition_variable cond_var;
    std::queue<T> queue;

public:
    void push(T&& item) {
        {
            std::lock_guard lock(mutex);
            queue.push(item);
        }

        cond_var.notify_one();
    }

    void push(T& item) {
        {
            std::lock_guard lock(mutex);
            queue.push(item);
        }

        cond_var.notify_one();
    }

    T& front() {
        std::unique_lock lock(mutex);
        cond_var.wait(lock, [&] { return !queue.empty(); });
        return queue.front();
    }

    void pop() {
        std::lock_guard lock(mutex);
        queue.pop();
    }
};

// Specialization of ThreadSafeQueue for pairs
template<typename T1, typename T2>
class ThreadSafeQueue<std::pair<T1,T2>> {
    std::mutex mutex;
    std::condition_variable cond_var;
    std::queue<std::pair<T1,T2>> queue;

public:
    void push(std::pair<T1, T2>&& item) {
        {
            std::lock_guard lock(mutex);
            queue.push(item);
        }

        cond_var.notify_one();
    }

    void push(std::pair<T1, T2>& item) {
        {
            std::lock_guard lock(mutex);
            queue.push(item);
        }

        cond_var.notify_one();
    }

    std::pair<T1,T2>& front() {
        std::unique_lock lock(mutex);
        cond_var.wait(lock, [&] { return !queue.empty(); });
        return queue.front();
    }

    void pop() {
        std::lock_guard lock(mutex);
        queue.pop();
    }
};

struct CityStruct {
    double minimum;
    double maximum;

    double temperature_acc{ 0.0 };
    int station_counter{ 0 };

    CityStruct() = delete;
    CityStruct(double temp) :minimum{ temp }, maximum{ temp } {}

    void update(double temp) {
        if (temp < minimum) minimum = temp;
        else if (temp > maximum) maximum = temp;
        temperature_acc += temp;
        ++station_counter;
    }

    double computeMean() const {
        return temperature_acc / station_counter;
    }
};

std::ostream& operator<<(std::ostream& os, const CityStruct& cs) {
    os << cs.minimum << "/" << cs.computeMean() << "/" << cs.maximum;
    return os;
}

// Queue to store read lines
ThreadSafeQueue<std::string> chunksQueue; 

// Queue to stone parsed lines <city, temperature>
ThreadSafeQueue<std::pair<std::string, double>> parsedQueue; 


void producerThread(const fs::path& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }
    std::string chunk;
    while (std::getline(file, chunk)) {
        chunksQueue.push(chunk);
    }

    // Signal end of file by pushing an empty string
    chunksQueue.push("");
}

void parse() {
    while (true) {
        std::string chunk = chunksQueue.front();
        chunksQueue.pop();
        // Exit loop if end of file is reached
        if (chunk.empty()) {
            break;
        }

        // parse chunk
        double temp;
        std::string city;
        std::istringstream stringstream(chunk);
        getline(stringstream, city, ';');
        stringstream >> temp;
        parsedQueue.push(std::make_pair(city, temp));
    }
    parsedQueue.push(std::make_pair(std::string{}, 0.0));
}

void store(std::promise<std::unordered_map<std::string, CityStruct>> result) {
    std::unordered_map<std::string, CityStruct> stations;
    while (true) {
        auto parsedLine = parsedQueue.front();
        parsedQueue.pop();
        if (parsedLine.first.empty()) {
            break;
        }
        auto [it, inserted] = stations.try_emplace(parsedLine.first, CityStruct{ parsedLine.second });
        if (!inserted) {
            it->second.update(parsedLine.second);
        }
    }
    result.set_value(stations);
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    fs::path filepath = fs::current_path() / filename;
    if (!fs::exists(filepath)) {
        std::cerr << "file doesn't exists" << std::endl;
    }

    std::promise<std::unordered_map<std::string, CityStruct>> result;
    std::future<std::unordered_map<std::string, CityStruct>> waitEndOfStore = result.get_future();

    std::thread producer(producerThread, std::ref(filepath));
    std::thread parser(parse);
    std::thread storeThread(store, std::move(result));
    producer.join();
    parser.join();
    std::unordered_map<std::string, CityStruct> stations = waitEndOfStore.get();
    storeThread.join();

    std::map<std::string, CityStruct> sortedStations{ stations.begin(), stations.end() };

    std::cout << "{";
    for (auto it = sortedStations.begin(); it != sortedStations.end();) {
        std::cout << it->first << '=' << it->second;
        if (++it != sortedStations.end()) std::cout << ", ";
    }
    std::cout << "}" << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::cout << "time : " << diff << std::endl;
    return 0;
}