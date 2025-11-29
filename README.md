# FinOptima - Personal Finance Allocation Tool

## Team Members
- SAMANTHA W (23PW25)  
- SHREE SINDHIYA HARSHINI K (23PW28)  

## Overview
FinOptima is a terminal-based personal finance allocation tool implemented in C. It helps users distribute their income efficiently across various savings goals and expenses. The program uses a **Genetic Algorithm (GA)** to determine an optimal allocation strategy, prioritizing financial items based on user-defined importance while respecting a fixed budget.

## Features
- Allocate income to savings goals and expenses optimally using GA  
- Prioritize items based on user-defined importance (1-10)  
- Supports dynamic input of multiple savings goals and expense categories  
- Highlights funded vs. not funded items in color-coded output  
- Provides a summary showing total allocations and remaining budget  
- Flexible and reusable: can handle varying number of items and income  

## Program Flow

1.Input total income
2.Enter number of savings goals and details (name, target amount, importance)
3.Enter number of expenses and details (name, amount, importance)
4.The progam runs a genetic algorithm over multiple generations to find the best allocation
5.Displays:
  - Funded vs. not funded savings goals
  - Funded vs. not funded expenses
  - Summary of total allocated amounts and remaining budget

## Notes

- Fitness of a solution is determined by the sum of importance of funded items.
- Solutions exceeding the total income are penalized and not considered.
- The program uses roulette wheel selection, crossover, and mutation in its GA.

### Created as a C project for practicing genetic algorithms, optimization, and menu-driven console applications.
