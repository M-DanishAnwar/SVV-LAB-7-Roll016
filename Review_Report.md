# Lab 7: Static Code Verification Report

## 1. Review Checklist

| Category | Result issue/OK | defect |
| :--- | :---: | :--- |
| **1- Initialization** | issue | Variable `sum` in `averageAttack()` has no initial value; pointers in `monsters` array are not set to null in constructor. |
| **2- Pointers** | issue | `findMonster` returns null when ID is not found which crashes the program; `m->name` is accessed after calling `delete m`. |
| **3- Arrays/buffers** | issue | Array size is 5 but `main` adds 6 monsters without bounds checking; `cin >> name` has no buffer size limit. |
| **4- Loops** | issue | Loop in `totalMonsterHealth()` uses `<=` instead of `<` and checks one index past the array limit. |
| **5- Arithmetic** | issue | `dmg` is unsigned so the check for less than 0 does not work; risk of division by zero if `monsterCount` is 0. |
| **6- Control flow** | issue | Single `=` used instead of `==` in `findMonster()`; `playerRank()` has no return statement if score is 100 or less. |
| **7- Function calls** | issue | `getMonsterLabel()` returns a reference `&` to a local string variable that gets destroyed on function return. |
| **8- Classes** | issue | `Game copy = game;` causes double-free crash because copy constructor and copy assignment are missing. |

---

## 2. Code Review Report

| Line Number | defect type | why it is problem | proposed fix | severity |
| :---: | :--- | :--- | :--- | :---: |
| **41** | Arrays/buffers | Array only has 5 slots, but main adds 6 monsters so it writes out of bounds and crashes. | Add check: `if (monsterCount >= MAX_MONSTERS) return;` | High |
| **47** | Control flow | Used single `=` instead of `==`, so it overwrites ID and always matches the first monster. | Change to: `if (monsters[i]->id == id)` | High |
| **54** | Pointers | If monster ID is not found, `m` is null, so accessing `m->health` causes a null crash. | Add null check: `if (m == nullptr) return -1;` | High |
| **56** | Arithmetic | `dmg` is an unsigned int, so subtracting it never becomes negative due to unsigned conversion. | Change condition to: `if (m->health <= damage)` | Medium |
| **64** | Loops | Loop runs with `<=`, so it checks index 5 which does not exist. | Change loop to: `i < monsterCount` | Medium |
| **70** | Function calls | Returns reference to local variable `label`, but local variables are wiped from memory on return. | Return by value: change return type to `std::string` | High |
| **77** | Initialization | `sum` is not set to 0, so it calculates the total using garbage data from memory. | Initialize it: `double sum = 0.0;` | Medium |
| **85** | Pointers | `delete m` frees the object first, then line 86 tries to print `m->name` from freed memory. | Print name before calling `delete m;` | High |
| **98** | Control flow | If score is 100 or less (e.g. score 50 from main), function has no return value. | Add default return: `else return 3;` | Low |
| **109** | Pointers | Memory was allocated with `new char[10]`, but deleted using normal `delete` instead of array delete. | Change to: `delete[] name;` | Medium |
