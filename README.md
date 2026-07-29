# 🎁 Gift Card Engine

A full-stack application for managing gift cards, tracking balances, monitoring expiration dates, and generating intelligent redemption recommendations.

The project combines a modern **C++17 backend** with a **React + Vite frontend**, exposing the same business logic through both a REST API and an interactive command-line interface.

🌐 **Live Demo:** https://gift-card-engine.vercel.app/

> **Note:** The frontend is deployed publicly on Vercel. The C++ backend currently runs locally and will be deployed in a future release.

---

# ✨ Features

### 💳 Gift Card Management

- Create gift cards
- Edit existing cards
- Delete cards
- Store balances
- Track associated companies

### 📅 Expiration Tracking

- Detect expired cards
- Highlight cards expiring soon
- Calculate total balance at risk

### 🎯 Redemption Planning

Generate smart spending recommendations:

- Sort by earliest expiration
- Sort by highest balance

### 📊 Risk Analysis

Analyze your wallet to discover:

- Expired cards
- Soon-to-expire cards
- Total money at risk

### 💾 Persistence

- Automatic saving
- File-based storage (`cards.txt`)
- No external database required

### 🖥️ Multiple Interfaces

The same backend logic powers:

- REST API
- Interactive CLI
- React Web Application

---

# 🏗️ Architecture

```
                    React + Vite
                         │
                  HTTP / JSON
                         │
                         ▼
                 C++ REST API Server
                  (cpp-httplib)
                         │
        ┌────────────────┼────────────────┐
        │                │                │
        ▼                ▼                ▼
     Wallet       Risk Analyzer    Redemption Planner
        │
        ▼
 File Repository (cards.txt)
```

The project follows a layered architecture that separates:

- Presentation
- Business Logic
- Data Persistence

This allows both the CLI and REST API to reuse exactly the same domain logic.

---

# 📂 Project Structure

```
gift-card-engine/
│
├── backend/
│   ├── include/
│   ├── src/
│   │   ├── main.cpp
│   │   ├── server_main.cpp
│   │   ├── GiftCard.cpp
│   │   ├── Wallet.cpp
│   │   ├── FileRepository.cpp
│   │   ├── RedemptionPlanner.cpp
│   │   ├── RiskAnalyzer.cpp
│   │   └── ValidationUtils.cpp
│   │
│   ├── tests/
│   └── CMakeLists.txt
│
├── frontend/
│   ├── src/
│   ├── components/
│   └── package.json
│
└── README.md
```

---

# 🚀 Live Demo

| Component | Status |
|-----------|--------|
| Frontend | ✅ https://gift-card-engine.vercel.app/ |
| Backend API | ⚠️ Localhost only |

---

# 🛠️ Technology Stack

## Backend

- C++17
- CMake
- Ninja
- cpp-httplib
- nlohmann/json
- GoogleTest

## Frontend

- React 19
- Vite
- JavaScript
- ESLint

---

# ⚙️ Building the Backend

## Requirements

### Windows

Install MSYS2 and the required packages:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc \
          mingw-w64-ucrt-x86_64-cmake \
          mingw-w64-ucrt-x86_64-ninja
```

### macOS

```bash
brew install cmake ninja
```

### Linux

```bash
sudo apt install cmake ninja-build g++
```

---

## Build

```bash
cd backend

cmake -B build -G Ninja

cmake --build build
```

---

# ▶️ Running

## Backend

```bash
cd backend/build

./server
```

Server:

```
http://localhost:8080
```

Available executables:

| Executable | Purpose |
|------------|---------|
| cli | Interactive CLI |
| server | REST API |
| run_tests | Unit Tests |

---

# 🌐 REST API

## Get all cards

```http
GET /api/cards
```

---

## Create a card

```http
POST /api/cards
```

```json
{
  "title": "Amazon",
  "balance": 150,
  "expiryDate": "31-12-2026",
  "companies": [
    "Amazon"
  ]
}
```

---

## Update card

```http
PUT /api/cards/:id
```

---

## Delete card

```http
DELETE /api/cards/:id
```

---

## Risk Analysis

```http
GET /api/cards/risk
```

Returns:

- cards at risk
- total balance at risk

---

## Redemption Plan

```http
GET /api/cards/plan?by=expiry
```

or

```http
GET /api/cards/plan?by=balance
```

---

# 💻 Frontend

Install dependencies:

```bash
cd frontend

npm install
```

Run development server:

```bash
npm run dev
```

Production build:

```bash
npm run build
```

Preview production build:

```bash
npm run preview
```

---

# 🧪 Testing

Run all backend tests:

```bash
cd backend/build

./run_tests
```

The project includes unit tests covering the core business logic using GoogleTest.

---

# 📈 Future Improvements

- SQLite persistence
- Docker support
- Backend cloud deployment
- Authentication
- User accounts
- Search & filtering
- Dashboard analytics
- Categories & tags
- CI/CD with GitHub Actions

---

# 📸 Screenshots

## Dashboard

> Coming soon

## Risk Analysis

> Coming soon

## Redemption Planner

> Coming soon

---

# 🎯 Learning Goals

This project was built to practice and demonstrate:

- Modern C++ development
- Object-Oriented Design
- REST API development
- React frontend development
- Client-server architecture
- Unit testing
- Input validation
- Software architecture
- Clean code principles
- Git & GitHub workflow

---

# 📄 License

This project is intended for educational and portfolio purposes.