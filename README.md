# ESP32 Device Bridge

A template project demonstrating ESP32C6 development with ESP-IDF framework.


## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

This project provides a foundation for developing applications with the ESP32C6 microcontroller. It demonstrates best practices for ESP-IDF projects and serves as a starting point for various applications.


## Features

- ESP32 hardware integration
- ESP-IDF framework implementation
- Modular component structure
- Comprehensive documentation
- Automated testing support

## Requirements

### Hardware
- ESP32 series development board
- USB cable for flashing

### Software
- ESP-IDF framework (version >= 5.x)
- Python 3.7+
- CMake 3.5+
- Ninja build system
- Git

## Setup Instructions

1. Install ESP-IDF prerequisites:
   ```bash
   # On Linux/macOS
   ./install.sh
   
   # On Windows
   install.bat
   ```

2. Clone the repository:
   ```bash
   git clone https://github.com/[username]/[repository].git
   cd [repository]
   ```

3. Set target and configure:
   ```bash
   idf.py set-target esp32c6
   idf.py menuconfig
   ```

## Usage

Build and flash the application:
```bash
idf.py build flash monitor
```

Clean the build directory:
```bash
idf.py clean
```

## Contributing

Contributions are welcome! Please submit pull requests against the main branch. Ensure all tests pass before submitting.

### Code Style Guidelines

- Follow ESP-IDF coding standards
- Use consistent indentation (4 spaces)
- Document all public functions
- Keep functions focused and short

## License

Licensed under Apache License 2.0. See LICENSE file for details.


## Project Structure

```plaintext
project_name/
├── main/
│   ├── CMakeLists.txt
│   ├── Kconfig
│   ├── app_main.c
│   ├── nvs_utils.c
│   ├── nvs_utils.h
│   ├── webui.c
│   ├── webui.h
│   ├── wifi.c
│   ├── wifi.h
│   ├── zigbee.c
│   └── zigbee.h
├── html/
│   ├── style.css
│   ├── index.html
│   └── wifi_config.html
├── README.md
├── CMakeLists.txt
└── sdkconfig.defaults
```
