# ASA Projects — Análise e Síntese de Algoritmos

Projects developed for the **Análise e Síntese de Algoritmos** (Design and Analysis of Algorithms) course at **Instituto Superior Técnico (IST)**.

---

## 📂 Projects Overview

### 🔹 Project 1 — Protein Chain Energy Maximization
* **Problem**: Find the optimal sequence to remove amino acids from a protein chain to maximize total energy released (based on adjacent potentials and biochemical affinity).
* **Approach**: **Dynamic Programming** ($O(n^3)$ interval DP, similar to matrix chain multiplication).
* **Language**: C++

---

### 🔹 Project 2 — Delivery Route Allocation in DAGs
* **Problem**: Determine delivery assignments for a fleet of trucks across a road network with no cycles (Directed Acyclic Graph). Truck assignment depends on the total number of distinct paths between intersections.
* **Approach**: **Graph Theory**, **Topological Sorting**, and **Dynamic Programming** to count paths between all node pairs efficiently.
* **Language**: C++

---

### 🔹 Project 3 — Football Championship Elimination
* **Problem**: Given the current match results in a round-robin tournament, determine the minimum number of remaining games each team must win to still be able to win the championship (or `-1` if impossible).
* **Approach**: Modeled and solved as a **Linear Programming (LP)** problem.
* **Tech Stack**: Python with the **PuLP** library (GLPK solver).

---

## 💻 Tech Stack
* **C++11** (`-O3`)
* **Python 3** (PuLP, GLPK)
