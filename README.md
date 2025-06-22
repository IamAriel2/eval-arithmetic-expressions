# Arithmetic Expressions Evaluator

This project is a C program that reads a mathematical expression involving complex numbers from an input file, evaluates the expression, and writes the result to an output file.

## Features

- Supports basic arithmetic operations on complex numbers: addition, subtraction, multiplication, division, and exponentiation.
- Handles expressions with brackets for proper order of operations.
- Reads input from a file (`input.txt`) and writes the result to a file (`output.txt`).

## File Structure

- `main.c`: The main source code file containing the implementation of the expression evaluator.
- `input.txt`: The input file containing the mathematical expression to be evaluated.
- `output.txt`: The output file where the result of the evaluated expression is written.

## How to Use

1. **Prepare the Input File**: Write the mathematical expression you want to evaluate in the `input.txt` file. For example:
    ```txt
    (3+5*i)*(2-4*i)
    ```

2. **Run the Program**: Execute the compiled program. It will read the expression from `input.txt`, evaluate it, and write the result to `output.txt`.
    ```sh
    ./main
    ```

3. **Check the Output**: Open the `output.txt` file to see the result of the evaluated expression.

## Example

Given the input expression in `input.txt`:
```txt
(3+5i)*(2-4i)
```

The output in `output.txt` will be:
```txt
26.000 - 2.000i
```
