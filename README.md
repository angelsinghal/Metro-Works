Delhi Metro System Simulation

Project Overview

This project simulates a metro rail system, allowing users to book tickets, check fares, view metro maps, and recharge their metro cards. It also includes an administrative interface for adding stations and connections between them.

Features

User Features:

Book a Ticket: Find the shortest path between two stations and calculate the fare.

Recharge Card: Add balance to a metro card.

View Card Balance: Check the current balance of the metro card.

Metro Station Map: Display all stations and their connections.

Calculate Fare: Get the fare for traveling between two stations.

Admin Features:

Add Metro Station: Add a new station to the system.

Add Connection Between Stations: Define routes and fares between stations.

View Metro Map: Display the entire metro system.

Technologies Used

C++ for implementing the metro system logic.

Dijkstra's Algorithm for finding the shortest path between stations.

Depth First Search (DFS) for finding all possible paths between two stations.

Priority Queue for efficient shortest path computation.

How to Run the Program

Compile the program using a C++ compiler (e.g., g++):

g++ metro_system.cpp -o metro_system

Run the compiled executable:

./metro_system

Follow the on-screen instructions to interact with the metro system.

User Guide

Login Options:

User Login: Enter username and navigate the metro system.

Admin Login: Enter admin credentials (admin/password) to manage the metro system.

Example Commands:

Book a ticket from "Rajiv Chowk" to "Kashmere Gate".

Recharge metro card with a specific amount.

View metro station map.

Add a new station (Admin only).

Future Improvements

Graphical Interface for better user interaction.

Database Integration to store user balances and travel history.

Real-time Traffic Data to optimize travel routes.

Contributors

Angel Singhal (9922103062)

License

This project is for educational purposes only.

