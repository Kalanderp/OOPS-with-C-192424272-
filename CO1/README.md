# Assignment 1 – CO1: C++ Code and Captured Outputs

This archive accompanies the completed report. It contains one C++ source per code/prediction task, input samples where applicable, captured stdout/stderr, and compile diagnostics. All programs were built in the sandbox with g++ 13.3.0 using C++17. Each source is standalone; the Task 11 sample configuration is in `Task11/fares.cfg`.

## Shared fare assumptions used in the executable programs

- Base fares are 1,000 / 2,000 / 3,000 paise for Inner / Middle / Outer.
- Students and senior citizens receive an assumed 50% concession applied to the base fare first; weekday peak surcharge is applied to the reduced fare.
- Peak windows are [08:00, 10:00) and [17:00, 20:00): 08:00 and 17:00 are peak; 10:00 and 20:00 are off-peak.
- Minimum balance after payment is 5,000 paise (Rs 50).
- Task 12 adds Airport at 6,000 paise; its 10% weekend discount replaces the weekday peak surcharge.
- Task 10's repeated 1,001-paise amount is a synthetic numerical stress case, not a standard fare in the zone table.

## Build and run

From this directory, compile any file with:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/Task06.cpp -o Task06
./Task06
```

Task 10 is measured without optimization (`-O0`) to make the repeated-add loops observable:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 src/Task10.cpp -o Task10
./Task10
```

For stdin examples, use the matching file in `inputs/`, for example:

```bash
./Task08 < inputs/Task08.txt
./Task07_oop < inputs/Task07_oop.txt
```

Task 11 reads `Task11/fares.cfg`; run it from the archive root. Task 4 intentionally produces a `-Wsign-compare` warning for the signed/unsigned comparison under study; its output remains the demonstration's expected result. Other sources compiled without warnings under the listed flags.

## Programs included

| File | Coverage |
|---|---|
| `Task02_constants.cpp` | Typed constants and scoped zone enum demonstration |
| `task3_types_money_demo.cpp` | Sizes, an illustrative 3-million-journey total, and floating-point accumulation error |
| `Task04_conversions.cpp` | Supplied implicit-conversion prediction exercise |
| `Task05_precedence.cpp` | Supplied precedence/increment/division prediction exercise |
| `Task06.cpp` | Completed switch-based fare function, peak boundaries, minimum-balance check |
| `Task07_procedural.cpp` | Procedural fare/card-processing version |
| `Task07_oop.cpp` | Encapsulated `SmartCard` version |
| `Task08.cpp` | Input validation/recovery and formatted journey report |
| `Task09.cpp` | Table-driven tests (including concession and boundary cases) |
| `Task10.cpp` | Integer paise vs repeated `double` accumulation at three scales |
| `Task11_config.cpp` | Validated startup loading of a small fare configuration |
| `Task12.cpp` | Fourth zone, weekend rule, concession, date-scoped two-zone pass |

Captured program outputs and compiler diagnostics are in `outputs/`. Timing values from Task 10 are machine- and run-dependent; rerunning will produce fresh measurements.

## Personalization before submission

Fill the student name, register number, department/section, academic year/semester, and GitHub repository link in the report cover. The repository link and course-specific identity details were not supplied, so they are left blank rather than fabricated.
