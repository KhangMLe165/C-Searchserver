# ThreadServe — Multithreaded HTTP Server in C++

ThreadServe is a low-level, multi-threaded web server built completely from scratch in **C++**, designed to explore the fundamentals of **network programming**, **concurrency**, and **HTTP protocol implementation**.

This project uses **POSIX sockets** for TCP communication and **pthreads** for handling multiple simultaneous client connections, providing a lightweight but functional HTTP/1.0 server capable of serving static content.

---

## 🔧 Tech Stack

- **C++**
- **POSIX Sockets** (`<sys/socket.h>`, `<arpa/inet.h>`)
- **pthreads** (POSIX threading library)
- Linux/Unix development environment

---

## ⚙️ Features

- ✅ Handles multiple concurrent client connections via threading  
- ✅ Implements a basic **HTTP/1.0 request-response** protocol  
- ✅ Serves static files (HTML, CSS, images, etc.) from a `/public` directory  
- ✅ Clean socket setup with robust error handling  
- ✅ Graceful shutdown and connection cleanup  
- ✅ Modular server design for extensibility  

---

## 🏗️ Architecture Overview

ThreadServe follows a simple but scalable server architecture:

- **Listener Thread**
  - Creates and binds a server socket
  - Listens for incoming TCP connections
  - Accepts new clients continuously

- **Worker Threads**
  - Each connection is delegated to a worker thread
  - Threads parse incoming HTTP requests
  - Static resources are located and returned with proper headers
  - Connections close cleanly after request completion

This design enables the server to support multiple clients simultaneously while keeping the core implementation lightweight.

---

## 📂 Static File Serving

All static content is hosted inside the `/public` folder.

Example:

```
public/
 ├── index.html
 ├── styles.css
 └── images/
```

Requests such as:

```
GET /index.html HTTP/1.0
```

Return the corresponding file with the appropriate HTTP response.

---

## 🚀 Getting Started

### Build

```bash
make
```

### Run

```bash
./threadserve <port> <public_directory>
```

Example:

```bash
./threadserve 8080 ./public
```

Then open your browser at:

```
http://localhost:8080
```

---

## 🧠 Learning Outcomes

This project provided hands-on experience with:

- Low-level socket programming and TCP communication  
- Thread-based concurrency using pthreads  
- HTTP request parsing and response construction  
- Server-side file I/O and resource management  
- Building real systems infrastructure from first principles  

---

## 🚀 Future Improvements

Potential extensions include:

- Support for HTTP/1.1 persistent connections  
- Thread pool optimization instead of per-connection threads  
- MIME type detection for broader file support  
- Logging and request analytics  
- Dynamic routing and backend functionality  

---

## 📌 Skills Demonstrated

- C++ Systems Programming  
- POSIX Networking & Sockets  
- Multithreading with pthreads  
- HTTP Protocol Fundamentals  
- Concurrent Server Architecture  
- Debugging with Valgrind/GDB  

---

## 📜 License

This project is for educational purposes and personal learning.
