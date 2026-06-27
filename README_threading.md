# Multithreaded Prime & Palindrome Finder

A multi-threaded C++ program that detects prime and palindrome numbers across a numeric range, with both thread-safe and thread-unsafe implementations.

## Overview

The program spawns T worker threads, divides a numeric range across them, and identifies:
- Prime numbers
- Palindrome numbers
- Numbers that are both prime and palindrome

Two versions are provided to demonstrate the effect of race conditions on shared data.

## Versions

| File | Description |
|---|---|
| `safe.cpp` | Thread-safe — uses `std::mutex` to synchronize access to shared variables and lists |
| `unsafe.cpp` | Thread-unsafe — no synchronization; demonstrates race conditions on shared state |

## Input Format

File: `in.txt`

```
<rangeStart> <rangeEnd>
```

## Output

**STDOUT** — printed by each thread as it starts, plus summary totals:
```
ThreadID=0, startNum=700666000, endNum=700666008
...
totalNums=32, numOfPrime=3, numOfPalindrome=1, numOfPalindromicPrime=1
```

**output.txt** — full lists of found numbers:
```
The prime numbers are:
700666007
...
The palindrome numbers are:
...
The palindromicPrime numbers are:
...
```

## Build & Run

```bash
g++ -o safe safe.cpp -lpthread
g++ -o unsafe unsafe.cpp -lpthread

./safe <T>
./unsafe <T>
```

`T` — number of worker threads (e.g. `./safe 4`)  
Ensure `in.txt` is in the same directory.

## Sample

**in.txt**
```
700666000 700666032
```

**Run:** `./safe 4`

**STDOUT:**
```
ThreadID=0, startNum=700666000, endNum=700666008
ThreadID=1, startNum=700666008, endNum=700666016
ThreadID=2, startNum=700666016, endNum=700666024
ThreadID=3, startNum=700666024, endNum=700666032
totalNums=32, numOfPrime=3, numOfPalindrome=1, numOfPalindromicPrime=1
```

## Thread Safety

The safe version uses two mutexes:
- `mtx0` — serializes thread startup print statements
- `mtx1` — guards all writes to shared counters and lists

The unsafe version omits all synchronization, producing non-deterministic results under concurrent access.

## Tech

C++ · `std::thread` · `std::mutex` · `std::lock_guard`

## Course

CPE 473 — Operating Systems  
Computer Engineering Department, Jordan University of Science and Technology
