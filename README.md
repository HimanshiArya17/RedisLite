# RedisLite - In-Memory Key Value Store in C++

RedisLite is a simplified implementation of Redis built completely in C++. The project demonstrates the use of fundamental data structures and algorithms by implementing an in-memory key-value database with LRU eviction, TTL support, persistence, and command-based interaction.

## Features

- Key-Value Storage
- LRU (Least Recently Used) Cache
- TTL (Time To Live) Support
- Automatic Expired Key Removal using Priority Queue
- Persistent Storage using File I/O
- Command History
- Cache Statistics
- Rename Keys
- Append to Existing Values
- Increment / Decrement Integer Values
- Display All Active Keys
- Save and Load Database

---

## Commands Supported

| Command | Description |
|----------|-------------|
| SET key value | Insert or update a key |
| GET key | Retrieve value |
| DELETE key | Delete a key |
| EXIST key | Check if key exists |
| DISPLAY | Display all keys |
| SETEX key ttl value | Store key with expiry |
| APPEND key value | Append text to value |
| INCR key | Increment integer value |
| DECR key | Decrement integer value |
| RENAME old new | Rename key |
| KEYS | Display all active keys |
| SIZE | Number of active keys |
| SAVE | Save database to file |
| LOAD | Load database from file |
| HISTORY | Display recent commands |
| STATS | Display cache statistics |
| CLEAR | Remove all keys |

---

## Data Structures Used

- Hash Map (`unordered_map`)
- Doubly Linked List
- Priority Queue (Min Heap)
- Vector
- File Streams

---

## Project Architecture

```
             User Commands
                    |
                    ▼
            Command Parser
                    |
        -------------------------
        |                       |
        ▼                       ▼
    Hash Map              Priority Queue
        |                       |
        ▼                       ▼
 Doubly Linked List     Expired Key Removal
        |
        ▼
   LRU Cache Management
        |
        ▼
   File Persistence
```

---

## LRU Cache

The cache stores recently accessed keys at the front of the doubly linked list.

When capacity is exceeded:

- Least Recently Used key is removed.
- Hash map is updated.
- Eviction statistics are maintained.

---

## TTL Support

Keys can be assigned an expiry time using:

```
SETEX key ttl value
```

Expired keys are removed efficiently using a Priority Queue (Min Heap).

---

## Persistence

Database contents can be stored and restored using:

```
SAVE
LOAD
```

Data is stored in a text file (`memory.txt`).

---

## Statistics

RedisLite tracks:

- Cache Hits
- Cache Misses
- Expired Keys
- Evicted Keys

---

## Time Complexity

| Operation | Complexity |
|------------|------------|
| GET | O(1) |
| SET | O(1) |
| DELETE | O(1) |
| LRU Update | O(1) |
| SETEX | O(log n) |
| Remove Expired | O(log n) |

---

## Technologies

- C++
- STL
- Object-Oriented Programming
- File Handling
- Data Structures & Algorithms

---

## Future Improvements

- Multi-threading
- Network Socket Support
- Publish / Subscribe
- Transactions
- Multiple Databases
- Snapshot Compression

---

## Learning Outcomes

This project helped me understand:

- Hash Maps
- Linked Lists
- Priority Queues
- LRU Cache Design
- Memory Management
- File Persistence
- Object-Oriented Design
