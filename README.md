# Logic Minimizer (K-Map) (C++)

## 📌 Description
A modular C++ console application that simplifies Boolean functions (2–4 variables) using Karnaugh-map logic and outputs the minimal SOP expression with grouping explanation.

## ✅ Features
- Accepts full truth table input
- Displays the corresponding K-map (2×2, 2×4, 4×4)
- Generates minimal SOP expression
- Shows grouping details (group size, covered minterms, term produced)
- Clean multi-file C++ structure

## 📁 Project Structure
input/ → user input
minimizer/ → simplification logic
output/ → K-map + result display

## ⚙️ Compile
g++ main.cpp input/input.cpp minimizer/minimizer.cpp output/output.cpp -o logic

## ▶️ Run
./logic

## 🧪 Example
# Input:
1. Enter number of variables (2–4)
2. Enter 2ⁿ output values in binary order
Example for 3 variables:
0 0 1 1 0 0 1 1

## Requirements
- C++17 compatible compiler
- g++

## 🚀 Future Improvements
- Don’t-care conditions
- POS form output
- Minterm input mode
- GUI version