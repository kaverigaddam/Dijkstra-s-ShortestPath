#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

const int INFINITY = std::numeric_limits<int>::max();

class CityNetwork {
private:
    std::vector<std::string> cities;
    std::vector<std::vector<int>> distances;

public:
    CityNetwork(const std::vector<std::string>& cityNames) : cities(cityNames) {
        int size = cities.size();
        distances.resize(size, std::vector<int>(size, INFINITY));
        for (int i = 0; i < size; ++i) {
            distances[i][i] = 0;
        }
    }

    void addRoad(const std::string& from, const std::string& to, int distance) {
        int fromIndex = findCityIndex(from);
        int toIndex = findCityIndex(to);
        if (fromIndex != -1 && toIndex != -1) {
            distances[fromIndex][toIndex] = distance;
            distances[toIndex][fromIndex] = distance;
        }
    }

    void displayNetwork() {
        std::cout << "City Network:\n";
        for (const auto& city : cities) {
            std::cout << city << ": ";
            for (int j = 0; j < cities.size(); ++j) {
                if (distances[findCityIndex(city)][j] < INFINITY) {
                    std::cout << cities[j] << "(" << distances[findCityIndex(city)][j] << ") ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    std::pair<std::vector<std::string>, int> findShortestRoute(const std::string& start, const std::string& end) {
        std::vector<int> shortestDistance(cities.size(), INFINITY);
        std::vector<int> previousCity(cities.size(), -1);
        std::vector<bool> visited(cities.size(), false);

        int startIndex = findCityIndex(start);
        int endIndex = findCityIndex(end);
        shortestDistance[startIndex] = 0;

        for (int i = 0; i < cities.size() - 1; ++i) {
            int currentCity = findNearestUnvisitedCity(shortestDistance, visited);
            if (currentCity == -1) break;
            
            visited[currentCity] = true;
            
            for (int neighborCity = 0; neighborCity < cities.size(); ++neighborCity) {
                if (!visited[neighborCity] && 
                    distances[currentCity][neighborCity] < INFINITY &&
                    shortestDistance[currentCity] + distances[currentCity][neighborCity] < shortestDistance[neighborCity]) {
                    shortestDistance[neighborCity] = shortestDistance[currentCity] + distances[currentCity][neighborCity];
                    previousCity[neighborCity] = currentCity;
                }
            }
        }

        std::vector<std::string> route;
        for (int at = endIndex; at != -1; at = previousCity[at]) {
            route.push_back(cities[at]);
        }
        std::reverse(route.begin(), route.end());

        return {route, shortestDistance[endIndex]};
    }

private:
    int findCityIndex(const std::string& cityName) {
        auto it = std::find(cities.begin(), cities.end(), cityName);
        return (it != cities.end()) ? std::distance(cities.begin(), it) : -1;
    }

    int findNearestUnvisitedCity(const std::vector<int>& distances, const std::vector<bool>& visited) {
        int minDistance = INFINITY;
        int minIndex = -1;
        for (int i = 0; i < cities.size(); ++i) {
            if (!visited[i] && distances[i] < minDistance) {
                minDistance = distances[i];
                minIndex = i;
            }
        }
        return minIndex;
    }
};

int main() {
    std::vector<std::string> cities = {"NewYork", "LosAngeles", "Chicago", "Houston", "Phoenix", "Philadelphia"};
    CityNetwork network(cities);

    network.addRoad("NewYork", "Chicago", 800);
    network.addRoad("NewYork", "Houston", 1600);
    network.addRoad("LosAngeles", "Houston", 1500);
    network.addRoad("LosAngeles", "Phoenix", 400);
    network.addRoad("Chicago", "Houston", 1000);
    network.addRoad("Chicago", "Phoenix", 1700);
    network.addRoad("Houston", "Phoenix", 1200);
    network.addRoad("Houston", "Philadelphia", 1500);
    network.addRoad("Phoenix", "Philadelphia", 2100);

    network.displayNetwork();

    std::string startCity, endCity;
    std::cout << "Enter start city: ";
    std::cin >> startCity;
    std::cout << "Enter destination city: ";
    std::cin >> endCity;

    auto [route, totalDistance] = network.findShortestRoute(startCity, endCity);

    std::cout << "Shortest route from " << startCity << " to " << endCity << ":\n";
    for (const auto& city : route) {
        std::cout << city << " -> ";
    }
    std::cout << "\nTotal distance: " << totalDistance << " miles\n";

    return 0;
}
