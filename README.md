# Gift Card Engine

A small full-stack app for tracking gift cards: balances, expiry dates, redemption planning, and expiry-risk analysis. Built as a C++ backend (CLI + REST API) with a React frontend.

## Features

- Add, edit, and remove gift cards (title, balance, expiry date, associated companies)
- Track expiry: flag cards that are expired or expiring soon
- **Redemption planning** — order cards by expiry date or by balance to decide what to spend first
- **Risk analysis** — surface cards at risk of expiring and the total balance at stake
- Simple flat-file persistence (`cards.txt`), no external database required
- Usable either as an interactive CLI or through a REST API + web UI

## Project structure

```
backend/
  include/        Headers (GiftCard, Wallet, FileRepository, RedemptionPlanner, RiskAnalyzer, ValidationUtils, ...)
  src/
    main.cpp          CLI entry point
    server_main.cpp   REST API server entry point
    GCApp.cpp, CommandParser.cpp   CLI application layer
    GiftCard.cpp, Wallet.cpp, FileRepository.cpp
    RedemptionPlanner.cpp, RiskAnalyzer.cpp, ValidationUtils.cpp
  tests/          GoogleTest unit tests
  CMakeLists.txt

frontend/
  src/
    App.jsx, api.js
    components/    AddCard, EditCard, CardList, CardForm
```

## Backend — build & run

### Prerequisites

- CMake 3.20+, Ninja, and a C++17 compiler.
- **Windows:** install [MSYS2](https://www.msys2.org/), then from the **MSYS2 UCRT64** terminal (Start menu → "MSYS2 UCRT64", not plain PowerShell/cmd) run:
  ```bash
  pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
  ```
  Build **and** run the binaries from that same MSYS2 UCRT64 terminal (or a bash terminal configured to it in your editor). Plain Windows PowerShell/cmd doesn't have `C:\msys64\ucrt64\bin` on PATH by default — running the built `.exe` from there fails immediately with a DLL-not-found crash (exit code `-1073741511`), since runtime DLLs like `libstdc++-6.dll` live in that folder.
- **macOS:** `brew install cmake ninja` (Xcode command line tools provide the compiler).
- **Linux:** `sudo apt install cmake ninja-build g++` (or your distro's equivalent).

Dependencies (GoogleTest, cpp-httplib, nlohmann/json) are fetched automatically via CMake's `FetchContent`.

```bash
cd backend
cmake -B build -G Ninja
cmake --build build
```

This produces three targets directly inside `backend/build/`:

| Target       | Description                                  |
|--------------|-----------------------------------------------|
| `cli`        | Interactive command-line app                  |
| `server`     | REST API server (listens on `localhost:8080`) |
| `run_tests`  | GoogleTest unit test suite                    |

Run them from `backend/build` (not `backend/`):

```bash
cd backend/build
./cli          # interactive CLI
./server       # REST API on http://localhost:8080
./run_tests    # unit tests
```

> **Troubleshooting:** if `./server` (or `./cli`) exits instantly with no output, it's almost always a missing-DLL issue from running outside the MSYS2 shell (see Prerequisites above) — reopen the MSYS2 UCRT64 terminal and re-run from there.

### REST API

All endpoints return/consume JSON. Card data is persisted to `cards.txt` after each write.

| Method | Path               | Description                                  |
|--------|--------------------|-----------------------------------------------|
| GET    | `/api/cards`       | List all cards                                |
| POST   | `/api/cards`       | Create a card (`title`, `balance`, `expiryDate` as `DD-MM-YYYY`, `companies`) |
| PUT    | `/api/cards/:id`   | Update a card's title, balance, expiry, or companies |
| DELETE | `/api/cards/:id`   | Remove a card                                 |
| GET    | `/api/cards/risk`  | Cards at risk of expiring + total balance at risk |
| GET    | `/api/cards/plan`  | Redemption plan, ordered `?by=expiry` (default) or `?by=balance` |

## Frontend — build & run

Requires Node.js.

```bash
cd frontend
npm install
npm run dev       # start dev server (expects the backend running on :8080)
npm run build     # production build
npm run lint      # ESLint
```

## Running tests

```bash
cd backend/build
./run_tests
```

## Tech stack

- **Backend:** C++17, CMake, [cpp-httplib](https://github.com/yhirose/cpp-httplib), [nlohmann/json](https://github.com/nlohmann/json), [GoogleTest](https://github.com/google/googletest)
- **Frontend:** React 19, Vite, ESLint
