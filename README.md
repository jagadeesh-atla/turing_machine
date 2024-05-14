# Turing Machine Simulator

[Wikipedia](https://en.wikipedia.org/wiki/Turing_machine)

## Build and Run

```bash
gcc -o tm utm.c parse.c main.c

./tm.exe file.txt
```

## Syntax for Machine Encoding

```
;; Double Semi-colons indicates Comments

;; These are keywords should be capital and in new-line
STATES
;; List all states separated by " "(space),
;; can spread multiple lines

FINAL STATES
;; List all states separated by " "(space),
;; can spread multiple lines and should be in STATES

SYMBOLS
;; List all symbols separated by " "(space),
;; can spread multiple lines and should be a single character

BLANK
;; A single symbol

INITIAL STATE
;; A single state

TAPE
;; Initial tape

TRANSITIONS
;; <current state> <current symbol> <new symbol> <direction> <new state>
;; <direction> : > | < | = ;; means right, left or stay
;; '*' can be used as a wildcard in <current symbol> or <current state> to match any character or state.
;; '*' can be used in <new symbol> or <new state> to mean 'no change'.
;; Note: Make sure wildcards should be at last for particular state, because it may alter desired output.
```

## Example

### Simple Incrementer

[file link](examples\binary_incremnter.txt)

```

STATES
Inc Halt

SYMBOLS
0 1
_

BLANK
_

INITIAL STATE
Inc

FINAL STATES
Halt

TAPE
110110

TRANSITIONS
Inc 0 1 > Halt
Inc 1 0 > Inc

```

**Output** :

```

1 : Inc [1] 1 0 1 1 0
2 : Inc 0 [1] 0 1 1 0
3 : Inc 0 0 [0] 1 1 0
4 : Halt 0 0 1 [1] 1 0

=--------------------------------------------=
[REPORT]:
        INITIAL TAPE: 110110
        No. of steps: 4
        FINAL TAPE  : 001110
=--------------------------------------------=

```

For more examples check examples folder.
