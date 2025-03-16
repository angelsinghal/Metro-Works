#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <string>
#include <bits/stdc++.h>

using namespace std;

// Structure to represent an edge in the graph (station connection)
struct Edge
{
    int destination; // Index of the neighboring station
    int weight;      // Distance between stations
};

// Structure to represent a vertex in the graph (station)
struct Station
{
    int id;
    string name;
    vector<Edge> edges;
};

class MetroSystem
{
public:
    vector<Station> stations_;

    // Function to find all paths between source and destination using Depth First Search
    void findAllPathsDFS(const vector<Station> &graph, int current, int destination, vector<bool> &visited, vector<int> &path, vector<vector<int>> &allPaths) const
    {
        visited[current] = true;
        path.push_back(current);

        if (current == destination)
        {
            allPaths.push_back(path);
        }
        else
        {
            for (const auto &edge : graph[current].edges)
            {
                int next = edge.destination;
                if (!visited[next])
                {
                    findAllPathsDFS(graph, next, destination, visited, path, allPaths);
                }
            }
        }

        visited[current] = false;
        path.pop_back();
    }

    // Function to display the shortest path
    void displayShortestPath(const vector<int> &shortestPath) const
    {
        cout << "Shortest Path (minimum stations): ";
        for (int i = 0; i < shortestPath.size(); ++i)
        {
            cout << shortestPath[i];
            if (i < shortestPath.size() - 1)
                cout << " -> ";
        }
        cout << endl;
    }

    void addStation(const string &name)
    {
        int newId = stations_.size(); // New station's ID is the current size of the vector
        stations_.push_back({newId, name});
    }

    // Function to add a connection (edge) between two stations
    void addConnection(int from, int to, int weight)
    {
        stations_[from].edges.push_back({to, weight});
        // Assuming metro lines are bidirectional, add reverse direction as well
        stations_[to].edges.push_back({from, weight});
    }

    // Function to display the metro station map
    void displayMetroMap() const
    {
        cout << "Metro Station Map:\n";

        for (const auto &station : stations_)
        {
            cout << "Station: " << station.name << ", ID: " << station.id << ", Connections: ";
            if (station.edges.empty())
            {
                cout << "None";
            }
            else
            {
                for (const auto &edge : station.edges)
                {
                    cout << "(" << stations_[edge.destination].name << ", Fare: $" << edge.weight << ") ";
                }
            }
            cout << endl;
        }
    }

    // Function to find the shortest path using Dijkstra's algorithm
    vector<int> findShortestPath(const vector<Station> &graph, int source, int destination, vector<vector<int>> &allPaths) const
    {
        int n = graph.size();
        vector<int> distance(n, numeric_limits<int>::max()); // Initialize distances to infinity
        vector<int> previous(n, -1);                         // Initialize previous nodes to -1 (undefined)

        distance[source] = 0; // Distance from source to source is 0

        // Priority queue to store vertices based on their distance from the source
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, source});

        while (!pq.empty())
        {
            int u = pq.top().second;
            int dist = pq.top().first;
            pq.pop();

            // If the current distance is greater than the calculated distance, skip
            if (dist > distance[u])
                continue;

            // Iterate through neighbors of the current vertex
            for (const auto &edge : graph[u].edges)
            {
                int v = edge.destination;
                int weight = edge.weight;

                // Relaxation step: if the new distance is shorter, update it and update the previous node
                if (distance[u] + weight < distance[v])
                {
                    distance[v] = distance[u] + weight;
                    previous[v] = u;
                    pq.push({distance[v], v});
                }
            }
        }

        // Reconstruct the shortest path from source to destination
        vector<int> shortestPath;
        for (int v = destination; v != -1; v = previous[v])
        {
            shortestPath.push_back(v);
        }
        reverse(shortestPath.begin(), shortestPath.end());

        // Find all paths from source to destination
        vector<bool> visited(n, false);
        vector<int> path;
        findAllPathsDFS(graph, source, destination, visited, path, allPaths);

        return shortestPath;
    }

    // Function to calculate the shortest path and fare between two stations
    double calculateFare(const string &sourceName, const string &destinationName) const
    {
        int sourceIndex = findStationIndex(sourceName);
        int destinationIndex = findStationIndex(destinationName);

        if (sourceIndex == -1 || destinationIndex == -1)
        {
            cout << "Error: Source or destination station not found." << endl;
            return -1.0; // Return a negative value to indicate error
        }

        // Find the shortest path using Dijkstra's algorithm
        vector<vector<int>> allPaths;
        vector<int> shortestPath = findShortestPath(stations_, sourceIndex, destinationIndex, allPaths);

        if (shortestPath.empty() || shortestPath.size() == 1)
        {
            cout << "Error: No route available between source and destination." << endl;
            return -1.0;
        }

        // Calculate fare based on the sum of edge weights
        double fare = 0;
        for (int i = 0; i < shortestPath.size() - 1; ++i)
        {
            int currentStation = shortestPath[i];
            int nextStation = shortestPath[i + 1];
            for (const auto &edge : stations_[currentStation].edges)
            {
                if (edge.destination == nextStation)
                {
                    fare += edge.weight;
                    break;
                }
            }
        }

        return fare;
    }

    // Function to display all possible paths
    void displayAllPaths(const vector<vector<int>> &allPaths) const
    {
        cout << "All Possible Paths:\n";
        for (const auto &path : allPaths)
        {
            for (int i = 0; i < path.size(); ++i)
            {
                cout << stations_[path[i]].name;
                if (i < path.size() - 1)
                    cout << " -> ";
            }
            cout << endl;
        }
    }

    // Function to find the station index by its name
    int findStationIndex(const string &name) const
    {
        string lowercaseName = name;
        transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), ::tolower);

        for (int i = 0; i < stations_.size(); ++i)
        {
            string lowercaseStationName = stations_[i].name;
            transform(lowercaseStationName.begin(), lowercaseStationName.end(), lowercaseStationName.begin(), ::tolower);

            if (lowercaseStationName == lowercaseName)
            {
                return i;
            }
        }
        return -1; // No match found
    }
};

class Admin
{
public:
    // Functionality for an admin to add a new metro station
    void addMetroStation(MetroSystem &metroSystem, const string &stationName)
    {
        metroSystem.addStation(stationName);
        cout << "Station '" << stationName << "' added to the metro system.\n";
    }

    // Functionality for an admin to add a connection (edge) between two existing stations
    void addConnection(MetroSystem &metroSystem, int from, int to, int weight)
    {
        metroSystem.addConnection(from, to, weight);
        cout << "Connection added between stations " << metroSystem.stations_[from].name
             << " and " << metroSystem.stations_[to].name << " with weight " << weight << ".\n";
    }
};

class User
{
public:
    string username;
    double cardBalance; // Card balance of the user

public:
    // Constructor
    User(string uname, double balance) : username(uname), cardBalance(balance) {}

    // Function to recharge the card with a specified amount
    void rechargeCard(double amount)
    {
        if (amount <= 0)
        {
            cout << "Error: Invalid recharge amount. Please enter a positive value." << endl;
            return;
        }

        cardBalance += amount;
        cout << "Card successfully recharged with $" << amount << ". Current balance: $" << cardBalance << endl;
    }

    double getCardBalance() const
    {
        return cardBalance;
    }

    // Function to book a ticket based on source and destination stations
    bool bookTicket(const MetroSystem &metroSystem, const string &source, const string &destination)
    {
        int sourceIndex = metroSystem.findStationIndex(source);
        int destinationIndex = metroSystem.findStationIndex(destination);

        if (sourceIndex == -1 || destinationIndex == -1)
        {
            cout << "Error: Source or destination station not found." << endl;
            return false;
        }

        if (sourceIndex == destinationIndex)
        {
            cout << "Error: Source and destination stations are the same." << endl;
            return false;
        }

        // Find the shortest path and calculate fare
        vector<vector<int>> allPaths;
        vector<int> shortestPath = metroSystem.findShortestPath(metroSystem.stations_, sourceIndex, destinationIndex, allPaths);

        // Display shortest path
        cout << "Shortest Path from " << source << " to " << destination << ":" << endl;
        metroSystem.displayShortestPath(shortestPath);

        // Calculate fare
        double fare = metroSystem.calculateFare(source, destination);

        // Check if the user has sufficient balance
        if (fare < 0)
        {
            return false; // Error occurred, return false
        }

        if (cardBalance < fare)
        {
            cout << "Error: Insufficient balance. Please recharge your card." << endl;
            return false;
        }

        // Deduct fare from card balance
        cardBalance -= fare;
        cout << "Ticket successfully booked from " << source << " to " << destination << ". Fare deducted: $" << fare << ". Remaining balance: $" << cardBalance << endl;
        return true;
    }
};

void userMenu(const string &username, const MetroSystem &metroSystem)
{
    User user(username, 50.0);

    int choice;
    do
    {
        cout << "\nMetro System Menu:\n";
        cout << "1. Book Ticket\n";
        cout << "2. Recharge Card\n";
        cout << "3. View Card Balance\n";
        cout << "4. Metro Station Map\n";
        cout << "5. Calculate Fare\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string source, destination;
            cout << "Enter source station: ";
            cin.ignore();
            getline(cin, source);
            cout << "Enter destination station: ";
            getline(cin, destination);
            user.bookTicket(metroSystem, source, destination);
            break;
        }
        case 2:
        {
            double amount;
            cout << "Enter recharge amount: $";
            cin >> amount;
            user.rechargeCard(amount);
            break;
        }
        case 3:
        {
            double balance = user.getCardBalance();
            cout << "Current card balance for " << username << ": $" << balance << endl;
            break;
        }
        case 4:
        {
            metroSystem.displayMetroMap();
            break;
        }
        case 5:
        {
            string source, destination;
            cout << "Enter source station: ";
            cin.ignore();
            getline(cin, source);
            cout << "Enter destination station: ";
            getline(cin, destination);
            double fare = metroSystem.calculateFare(source, destination);
            cout << endl
                 << "Fare is:" << fare << endl;
            break;
        }
        case 6:
        {
            cout << "Exiting the system...\n";
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);
}

void welcomeWindow(MetroSystem &metroSystem)
{
    cout << "\nWelcome to the Delhi Metro System!\n";
    cout << "1. Login as User\n";
    cout << "2. Login as Admin\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;

    if (choice == 1)
    {
        string username;
        cout << "Enter username: ";
        cin >> username;
        cout << "Welcome back, " << username << "!\n";
        userMenu(username, metroSystem);
    }
    else if (choice == 2)
    {
        string adminUsername, adminPassword;
        cout << "Enter admin username: ";
        cin >> adminUsername;
        cout << "Enter admin password: ";
        cin >> adminPassword;

        if (adminUsername == "admin" && adminPassword == "password")
        {
            cout << "Admin login successful!\n";
            Admin admin;

            int adminChoice;
            do
            {
                cout << "\nAdmin Menu:\n";
                cout << "1. Add Metro Station\n";
                cout << "2. Add Connection between Stations\n";
                cout << "3. Display Metro Map\n";
                cout << "4. Exit Admin Menu\n";
                cout << "Enter your choice: ";
                cin >> adminChoice;

                switch (adminChoice)
                {
                case 1:
                {
                    string stationName;
                    cout << "Enter the name of the new station: ";
                    cin.ignore();
                    getline(cin, stationName);
                    admin.addMetroStation(metroSystem, stationName);
                    break;
                }
                case 2:
                {
                    int from, to, weight;
                    cout << "Enter the ID of the source station: ";
                    cin >> from;
                    cout << "Enter the ID of the destination station: ";
                    cin >> to;
                    cout << "Enter the weight (fare) of the connection: ";
                    cin >> weight;
                    admin.addConnection(metroSystem, from, to, weight);
                    break;
                }
                case 3:
                    metroSystem.displayMetroMap();
                    break;
                case 4:
                    cout << "Exiting Admin Menu...\n";
                    welcomeWindow(metroSystem);
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (adminChoice != 4);
        }
        else
        {
            cout << "Invalid username or password. Please try again.\n";
            welcomeWindow(metroSystem);
        }
    }
    else if (choice == 3)
    {
        cout << "Exiting the system...\n";
    }
    else
    {
        cout << "Invalid choice. Please try again.\n";
        welcomeWindow(metroSystem);
    }
}

void welcomeScreen()
{
    system("cls");
    
    cout<<"\t\t"<<" _  _      _              _        _         _ "<<endl;
    cout<<"\t\t"<<"|  \\/  | _| |_ _ _ __   \\ \\      / /_  _ _| | ___"<<endl;
    cout<<"\t\t"<<"| |\\/| |/ _ \\ _| '/ _ \\   \\ \\ /\\ / / _ \\| '| |/ / _|"<<endl;
    cout<<"\t\t"<<"| |  | |  _/ || | | () |   \\ V  V / () | |  |   <\\__ \\ " <<endl;
    cout<<"\t\t"<<"||  ||\\_|\\_||  \\_/     \\/\\/ \\_/||  ||\\\\__/"<<endl;

    cout << "Press any key to continue...\n";

    // Wait for user input before proceeding
    cin.ignore(); // clear space from buffer
    cin.get();    // read single character and waits till user gives input
    system("cls");
}

int main()
{
    system("cls");
    cout << "\n\n\n\n\n--------JAYPEE INSTITUE OF INFORMATION AND TECHNOLOGY--------------\n\n\n";
    cout << "GROUP MEMBERS:\n";
    cout << "Angel Singhal 9922103062\n";
    cout << "Press any key to continue...\n";
    cin.ignore();
    cin.get();
    system("cls");

    welcomeScreen();


   // Define station names from Delhi Metro
    const vector<string> stationNames = {
        "Shahid Sthal", "ITO", "Kashmere Gate", "Chandni Chowk",
        "Rajiv Chowk", "Barakhamba Road", "Karol Bagh", "Haqiabad",
        "Vishwavidyalaya", "Pragati Maidan", "Noida Electronic City", "Majlis Park", "Mundka"};

    // Create metro system
    MetroSystem metroSystem;

    // Populate metro system with stations
    for (int i = 0; i < stationNames.size(); ++i)
    {
        metroSystem.addStation(stationNames[i]);
    }

    // Define connections (edges) between stations
    // Note: Ensure bidirectional connections are added for each edge

    // Shahid Sthal connections
    metroSystem.addConnection(0, 1, 10); // Shahid Sthal -> ITO (fare: 10)
    metroSystem.addConnection(0, 2, 15); // Shahid Sthal -> Kashmere Gate (fare: 15)
    metroSystem.addConnection(0, 11, 25); // Shahid Sthal -> Majlis Park (fare: 25)
    metroSystem.addConnection(0, 12, 30); // Shahid Sthal -> Mundka (fare: 30)

    // ITO connections
    metroSystem.addConnection(1, 0, 10); // ITO -> Shahid Sthal (fare: 10) - bidirectional
    metroSystem.addConnection(1, 2, 5);  // ITO -> Kashmere Gate (fare: 5)
    metroSystem.addConnection(1, 3, 8);  // ITO -> Chandni Chowk (fare: 8)

    // Kashmere Gate connections
    metroSystem.addConnection(2, 0, 15); // Kashmere Gate -> Shahid Sthal (fare: 15) - bidirectional
    metroSystem.addConnection(2, 1, 5);  // Kashmere Gate -> ITO (fare: 5)
    metroSystem.addConnection(2, 3, 12); // Kashmere Gate -> Chandni Chowk (fare: 12)
    metroSystem.addConnection(2, 11, 20); // Kashmere Gate -> Majlis Park (fare: 20)
    metroSystem.addConnection(2, 12, 25); // Kashmere Gate -> Mundka (fare: 25)

    // NoidaElectronic City connections
    metroSystem.addConnection(3, 0, 20); // Noida Electronic City -> Shahid Sthal (fare: 20) - bidirectional
    metroSystem.addConnection(3, 2, 14); // Noida Electronic City -> Kashmere Gate (fare: 14)
    metroSystem.addConnection(3, 4, 13); // Noida Electronic City -> Chandni Chowk (fare: 13)

    // Majlis Park connections
    metroSystem.addConnection(11, 0, 25); // Majlis Park -> Shahid Sthal (fare: 25) - bidirectional
    metroSystem.addConnection(11, 2, 20); // Majlis Park -> Kashmere Gate (fare: 20)
    metroSystem.addConnection(11, 12, 10); // Majlis Park -> Mundka (fare: 10)

    // Mundka connections
    metroSystem.addConnection(12, 0, 30); // Mundka -> Shahid Sthal (fare: 30) - bidirectional
    metroSystem.addConnection(12, 2, 25); // Mundka -> Kashmere Gate (fare: 25)
    metroSystem.addConnection(12, 11, 10); // Mundka -> Majlis Park (fare: 10)

    // Create user
    welcomeWindow(metroSystem);

    return 0;
}
