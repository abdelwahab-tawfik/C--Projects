# 🏦 Bank Admin System (C++)

A console-based **Bank Administration System** built using **C++** as part of **Course 8** in the Programming Advices roadmap.

This project simulates a simple banking system with **clients management, users management, authentication, permissions, and transactions**, all using **file handling**.

---

## 🚀 Features

### 🔐 Authentication System
- Login using **username & password**
- User data stored in `users.txt`
- Permission-based access control

### 👤 Client Management
- Add new clients
- Update client information
- Delete clients
- Search for clients
- Display all clients
- Client data stored in `clients.txt`

### 💰 Transactions
- Deposit money
- Withdraw money
- View total balances of all clients

### 👥 Users Management (Admin Only)
- Add new users
- Update user information
- Delete users
- Search for users
- Assign specific permissions to each user

---

## 🛂 Permissions System

Each user has permissions represented using **bitwise flags**, such as:
- View clients
- Add clients
- Delete clients
- Update clients
- Perform transactions
- Manage users

Admin users can have **full access**.

---

## 🗂️ File Structure

BankAdminSystem/
│
├── main.cpp
├── clients.txt // Stores clients data
├── users.txt // Stores users data
├── README.md
---

## 🛠️ Technologies Used

- **C++**
- **File Handling (fstream)**
- **Structs**
- **Enums**
- **Vectors**
- **Bitwise Operations**
- **Console UI**

## ▶️ How to Run

1. Compile the project using any C++ compiler:
   ```bash
   g++ main.cpp -o BankAdminSystem

2.. Run the executable:  BankAdminSystem

3.. Login using a valid username and password from users.txt.

📚 Learning Outcomes
Practical usage of file-based databases

Implementing authentication & authorization

Using bitwise permissions

Structuring a large C++ console application

Handling real-world CRUD operations

⚠️ Notes
- This project is for learning purposes
- Data is stored in text files (no database)
- .exe files are intentionally excluded from the repository

👨‍💻 Author

Abdelwahab Tawfik
- Learning C++ & Software Development
- Following Programming Advices Roadmap
🔗 GitHub: [https://github.com/abdelwahab-tawfik]
🔗 LinkedIn: [https://www.linkedin.com/in/abdelwahab-mohammed]

⭐ If you like this project, feel free to star the repository
