# In-Memory Key-Value Store (C)

A custom, lightweight, in-memory key-value store built from scratch in C. It utilizes a Hash Table with chaining (linked lists) to handle collisions and provides a command-line interface for interacting with the data.

This project demonstrates low-level systems programming concepts, including manual memory management, custom hash functions (djb2), and string parsing.

## 📌 Features
* **Hash Table Implementation:** A fixed-size array of 100 buckets using the **djb2** hash algorithm for fast O(1) average-time lookups.
* **Collision Resolution:** Handles hash collisions via **separate chaining** (singly linked lists).
* **Core Commands:** 
  * `SET <key> <value>` - Insert or update a key.
  * `GET <key>` - Retrieve a value.
  * `DEL <key>` - Delete a key.
  * `KEYS` - List all active keys.
  * `FLUSH` - Clear the entire store.
* **Memory Safety:** Utilizes `malloc` and `strdup` for dynamic allocation, with a fully implemented cleanup function to ensure **zero memory leaks** upon exit.

## 🛠️ Technologies Used
* **Language:** C (C99 or higher)
* **Concepts:** Pointers, Structs, Linked Lists, Hash Functions, Dynamic Memory Allocation, CLI parsing

## 🚀 How to Compile and Run
1. Clone this repository.
2. Compile the code using GCC:
   ```bash
   gcc commands.c -o kvstore

## 📸 Screenshot

