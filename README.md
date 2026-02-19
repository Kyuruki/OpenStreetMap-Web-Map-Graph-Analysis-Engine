# OpenStreetMap Web Map & Graph Analysis Engine

## Overview

OpenStreetMap Web Map & Graph Analysis Engine is a project designed to work with OpenStreetMap data, providing tools for graph-based operations, server-side functionality, and web-based visualization. The project includes a variety of components, such as a server, tests, and a web interface for map visualization.

<img width="3747" height="1945" alt="screenshot" src="https://github.com/user-attachments/assets/35a48bcd-f0d3-42d7-acad-ac5870ab783e" />

## Features

- **Graph Operations**: Includes algorithms for graph traversal and distance calculations.
- **Server**: A lightweight server to handle requests and serve data.
- **Web Interface**: A user-friendly interface for visualizing map data.
- **Testing Suite**: Comprehensive tests to ensure the reliability of the graph algorithms and server functionality.

## Project Structure

```
OpenStreetMap-UIC/
├── application.cpp/.h       # Core application logic
├── dist.cpp/.h              # Distance calculation utilities
├── graph.h                  # Graph data structure and algorithms
├── httplib.h                # Lightweight HTTP server library
├── json.hpp                 # JSON parsing library
├── main.cpp                 # Entry point for the application
├── Makefile                 # Build system configuration
├── osm_server/              # Server-related files
├── osm_tests/               # Test-related files
├── build/                   # Build artifacts
├── data/                    # Sample data files
│   ├── empty.json
│   ├── line.json
│   ├── small_buildings.json
│   └── uic-fa25.osm.json
├── tests/                   # Test cases
│   ├── build_graph_tests.cpp
│   ├── dijkstra_tests.cpp
│   └── graph_tests.cpp
├── www/                     # Web interface files
│   ├── map.html
│   ├── css/
│   │   └── map.css
│   └── js/
│       ├── autocomplete.js
│       ├── map.js
│       └── typeahead.js
```

## Getting Started

### Prerequisites

- **Compiler**: Ensure you have a C++ compiler installed (e.g., GCC, Clang).
- **Make**: Required for building the project.

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/OpenStreetMap-UIC.git
   cd OpenStreetMap-UIC
   ```
2. Build the project:
   - For the main application:
     ```bash
     make osm_main
     ```
   - For the server:
     ```bash
     make osm_server
     ```
   - For the tests:
     ```bash
     make osm_tests
     ```

### Running the Application

- **Server**:
  After building the server, run the executable to start serving requests.
  ```bash
  ./osm_server/osm_server
  ```
- **Tests**:
  Run the test suite to verify functionality.
  ```bash
  ./osm_tests/osm_tests
  ```

### Web Interface

1. Open `www/map.html` in a browser.
2. Interact with the map and visualize data.

## Data Files

The `data/` directory contains sample JSON files for testing and demonstration purposes:

- `empty.json`: An empty dataset.
- `line.json`: A simple line dataset.
- `small_buildings.json`: A dataset with small building data.
- `uic-fa25.osm.json`: A sample OpenStreetMap dataset.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Submit a pull request with a detailed description of your changes.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- [OpenStreetMap](https://www.openstreetmap.org/) for the data.
- [JSON for Modern C++](https://github.com/nlohmann/json) for the JSON library.
- [cpp-httplib](https://github.com/yhirose/cpp-httplib) for the HTTP server library.

---

Feel free to reach out for any questions or issues!
