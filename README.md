# Grim Reaper is a slowloris simulator attack

## Introduction

This project is a simple C program designed for educational purposes only. It simulates a slowloris attack, which is a type of denial-of-service attack aimed at keeping a web server connection open and exhausting its resources. The goal is to provide insights into how such attacks work and to promote awareness of the importance of implementing effective defense mechanisms.

**Disclaimer:** Do not use this tool for any malicious activities. The sole purpose of this project is educational, and any misuse is strongly discouraged.

## Table of Contents

- [How it Works](#how-it-works)
- [Usage](#usage)
- [Defense Mechanisms](#defense-mechanisms)
- [Contributing](#contributing)
- [To do](#to-do)

## How it Works

The slowloris simulator sends HTTP requests to a target web server and deliberately keeps the connections open by sending partial requests at regular intervals, preventing the server from serving other legitimate requests. This helps users understand the principles behind slowloris attacks and the potential impact on web server performance.

## Usage

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/pedroferreiraa/grim_reaper
   ```
2. **Compiling:**
   ```bash
   gcc grim_reaper.c -o grim -O2 -std=gnu99
   ```
3. **Run the program**
   ```bash
   ./grim --ip 127.0.0.1 --port 7359
   ./grim --help
   ```   
## Defense Mechanisms

To defend against slowloris attacks, consider implementing the following measures:

1. Limit Concurrent Connections:
   ```
   Configure your web server to limit the number of simultaneous connections from a single IP address.
   ```
3. Timeout Settings:
   ```
   Implement connection timeout settings to automatically close idle connections.
   ```
4. Rate Limiting:
   ```
   Employ rate-limiting mechanisms to restrict the number of requests from a single IP within a specified time frame.
   ```
5. Load Balancers:
   ```
   Use load balancers to distribute traffic evenly and prevent a single point of failure.
   ```
6. Intrusion Prevention Systems (IPS):
   ```
   Deploy IPS to detect and block malicious traffic patterns.
   ```
## Contributing

Contributions to improve the educational value, documentation, or security considerations of this project are welcome. Please open an issue or submit a pull request with your suggestions.

## To do
1. Make more than 1024 connections using fork().
