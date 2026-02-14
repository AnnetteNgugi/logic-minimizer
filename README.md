# Logic Minimizer (C++)

## 📌 Description
This is a modular C++ console application that generates a Sum of Products (SOP) expression from a truth table.

## ✅ Features
- Supports 2–4 variables
- Clean file structure
- Beginner friendly
- Uses std:: (no global namespace pollution)

## 📁 Project Structure
input → handles user input  
minimizer → generates SOP terms  
output → prints the result  

## ⚙️ Compile
g++ main.cpp input/Input.cpp minimizer/Minimizer.cpp output/Printer.cpp -o logic

## ▶️ Run
./logic

## 🧪 Example
Input:
2  
0 1 1 1  

Output:
F = A'B + AB' + AB

## 🚀 Future Improvements
- Real Karnaugh Map grouping
- Boolean simplification
- K-map visual display
