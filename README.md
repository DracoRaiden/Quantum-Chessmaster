# ♟️ Quantum Chessmaster

**Quantum Chessmaster** is a fully-featured, high-performance **C++ chess engine** built as a final project for the *Data Structures & Algorithms* course. With over **5,000 lines of code** and a **modular 10+ file architecture**, this project simulates a complete chess experience including both **Human vs Human** and **Human vs AI** modes. It uses complex data structures, game rule validation, and AI logic to provide a competitive and interactive game environment.

---

## 🎯 Key Features

- 🔄 **Two Gameplay Modes**: Human vs Human & Human vs AI
- 🔍 **Full Move Validation**, including:
  - Check and Checkmate detection
  - En Passant
  - Castling
  - Pawn Promotion
- ♻️ **Undo/Redo Functionality** using stacks and queues
- 📁 **Game History Tracking** using circular double-ended queue (deque)
- 🧠 **AI Engine** with Minimax and priority queue-based evaluation
- 📦 **Modular Structure**: Separated into 10+ source files
- 💾 **File Handling** to save/load game states across sessions
- 🎯 **Clean CLI Interface** with player prompts and instructions

---

## 🛠️ Project Structure

src/
├── Board.cpp / .h            # Chessboard and display logic
├── Piece\*.cpp / .h           # Base class + derived classes for each piece
├── MoveManager.cpp / .h      # makeMove(), updateMove(), and validation
├── AIEngine.cpp / .h         # AI logic using Minimax with priority queue
├── GameManager.cpp / .h      # Turn management and game loop
├── HistoryManager.cpp / .h   # Undo/Redo via stack and deque
├── IOHandler.cpp / .h        # File saving/loading
└── main.cpp                  # Main entry point


## 🧠 Data Structures Used

- ✅ **Stacks & Queues** – Undo and Redo operations
- 🔁 **Circular Deque** – Game history and move tracking
- 🧮 **Priority Queue** – Heuristic evaluation in AI decision-making
- 🧱 **Inheritance & Polymorphism** – Piece hierarchy (King, Queen, etc.)

---

## 💻 How to Run

1. Clone the repository:
   bash
   git clone https://github.com/DracoRaiden/Quantum-Chessmaster.git

2. Navigate to the project directory:

   bash
   cd Quantum-Chessmaster
   
3. Compile the project (e.g., using g++ or any C++17+ compiler):

   bash
   g++ -std=c++17 src/*.cpp -o QuantumChess
   
4. Run the game:
  bash
   ./QuantumChess
   

---

## 📈 What Makes It Special

* ✔️ Over **5000 lines** of code with **robust architecture**
* 💡 Accurate implementation of **complex chess rules**
* 🧠 **Minimax AI** designed using data structures like priority queues
* 🔁 Undo/Redo with error-proof state transitions
* 📂 Full **file persistence** for game continuity

---

## 🚀 Future Enhancements

* 🧠 Add **Alpha-Beta Pruning** to improve AI efficiency
* 🎨 Develop an **interactive GUI** (SFML/Qt/CLI-enhanced)
* 🌐 Add support for **online multiplayer**
* 📜 Export move history in **PGN (Portable Game Notation)** format

---

## 🏅 Team

* **Developer**: [Ammar Saleem](https://github.com/DracoRaiden)
* **Co-Developer**: *[Bilal Khurram](https://github.com/billzcoding)*

---

## 📄 License

This project is developed as an academic submission. For personal and educational use only.

