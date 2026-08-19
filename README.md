# Payroll System 

## Description
This project is a C++ program developed for the CO1409 Programming module.  
The application implements a logistics driver payroll system that reads driver information from files, calculates wages, applies tax deductions, and outputs results.  
It demonstrates the use of core programming concepts such as file handling, data structures, functions, input validation, and error handling.  
The implementation has been completed up to the 70+ grade band requirements.

## Features 
- 📂 Reads driver details from `drivers.txt` (ID, name, rate per mile).
- 🧮 Calculates monthly pay based on miles driven and rate per mile.
- 💰 Income tax calculation (20% above £12,570 annual threshold).
- 📊 Outputs pay before and after tax deductions.
- 📝 Handles multiple pay files (`jan26.txt`, `feb26.txt`, `mar26.txt`).
- ⚠️ Error handling for invalid/missing driver IDs and incomplete records (logged in `errors.txt`).
- 🔄 Continuous file processing until user quits.
- 🧑‍💻 Allows user to input the name of a pay file to process.
- ❌ Displays error messages if a file cannot be found.
- 🗂 Skips incomplete records without modifying driver data.
- 📝 Logs issues (invalid IDs, incomplete entries) into a single `errors.txt` file.
- 🔍 Option to search for a particular driver or view all driver records.
- 📑 Outputs results to files (e.g., `jan26_output.txt`).

## System Requirements
- Language: C++
- Compiler: Any standard C++ compiler Visual Studio
- Input files: drivers.txt, jan26.txt, feb26.txt, mar26.txt, apr26.txt, may26.txt
- Output files: jan26_output.txt, feb26_output.txt, mar26_output.txt, apr26_output.txt, may26_output.txt,, errors.txt

## Usage
1. Place all input files in the project folder.
2. Compile the program using your C++ compiler.
3. Run the executable.
4. Follow prompts to:
   - Enter the name of a pay file (e.g., jan26.txt).
   - Process multiple files in sequence until typing "quit".
   - View driver details and pay summaries.
   - Handle errors gracefully with logs.

## Example Workflow
- Program reads driver details from `drivers.txt`.
- User specifies a pay file (e.g., `jan26.txt`).
- Pay is calculated and displayed:
  - Driver ID, Name, Rate per Mile, Miles Driven
  - Monthly Pay (before tax)
  - Monthly Pay (after tax)
- Results are written to output files.
- Errors are logged in `errors.txt`.

--------------------------------
Developed for CO1409 Programming Coursework (Payroll System)  
University of Lancashire, Academic Year 2025-26
