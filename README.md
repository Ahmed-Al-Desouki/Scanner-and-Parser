# Scanner-and-Parser
# Scanner & Parser Projects Collection

## 📋 Project Overview

This repository showcases a series of **Scanner (Lexer) and Parser** implementations developed as part of an educational journey in compiler design and lexical analysis. The projects focus on building finite automata-based scanners that process input source code or text files to identify tokens, validate syntax, and generate structured output. Each iteration demonstrates progressive enhancements in code architecture, error handling, and feature integration, reflecting iterative development practices.

The core workflow involves:
- **Input Processing**: Reading from structured input files containing source code or tokens.
- **Tokenization**: Using deterministic finite automata (DFA) to recognize lexical tokens.
- **Validation**: Checking for syntactic validity and reporting errors.
- **Output Generation**: Producing detailed reports in output files, indicating validity status, token lists, and parsing trees (where applicable).

These projects serve as practical examples of compiler front-end components, suitable for academic study, prototyping, or integration into larger compiler toolchains.

## 🛠️ Key Components & File Structure

The projects utilize a modular file-based approach for configuration, input, and output management. Common files across implementations include:

### Core Files:
- **Automaton Definition File**: Contains the DFA transition table, states, and acceptance criteria (e.g., `automaton.txt` or `dfa_config.xml`).
- **Tokens File**: Defines lexical tokens, their patterns (regular expressions), and precedence rules (e.g., `tokens.def`).
- **Input File**: Source code or test strings to be scanned/parsed (e.g., `input.txt` or `source.code`).
- **Transformation File**: Rules for state transitions, symbol mappings, and optimizations (e.g., `transformations.rules`).
- **Error Handling Config**: Specifications for error recovery and reporting (e.g., `errors.cfg`).
- **Output File**: Generated results including token streams, parse trees, validity status, and diagnostics (e.g., `output.txt` or `parse_report.json`).

### Project Variants:
Multiple implementations are included, each building on the previous:
1. **Basic Scanner**: Core DFA-based tokenization with simple validity checks.
2. **Enhanced Scanner**: Added support for nested structures, lookahead, and basic error recovery.
3. **Parser Integration**: Combines scanner output with recursive descent or LL(1) parsing for syntax analysis.
4. **Advanced Version**: Incorporates symbol tables, semantic actions, and AST (Abstract Syntax Tree) generation.

Each variant is housed in a dedicated subdirectory (e.g., `v1_basic/`, `v2_enhanced/`) with its own set of configuration files.

## 🎯 Technical Approach

### Scanner Implementation:
- **Finite Automata Engine**: Built using state transition matrices or graph-based representations for efficient token recognition.
- **Lexical Analysis Phases**:
  1. **Preprocessing**: Remove comments, whitespace normalization.
  2. **Token Matching**: Traverse DFA states based on input characters.
  3. **Backtracking**: Handle ambiguities with longest-match rules.
  4. **Validation**: Ensure complete input consumption and no dangling states.
- **Multiple Techniques Explored**:
  - Direct-coded DFA for performance.
  - Table-driven transitions for flexibility.
  - Regex-to-DFA conversion utilities in later versions.

### Parser Integration (Select Variants):
- **Syntax Analysis**: Top-down parsing with predictive tables or recursive descent.
- **Error Reporting**: Contextual diagnostics with line/column references.
- **Output Formats**: Human-readable reports, JSON for automation, or DOT files for visualization.

All implementations are written in **Python** (primary) with some C++ prototypes for performance benchmarking. Dependencies are minimal, relying on standard libraries like `re` for regex support and `graphviz` for optional visualization.

## 🚀 Getting Started

### Prerequisites:
- Python 3.8+ (for Python-based implementations)
- Graphviz (optional, for AST visualization)
- No external databases or servers required—pure file-based execution.

### Setup & Execution:
1. **Clone the Repository**:
