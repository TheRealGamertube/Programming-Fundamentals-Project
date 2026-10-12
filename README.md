# 🏦 Bank Account Management System

A secure, console-based **Bank Account Management System** written in **C**. It supports separate **user** and **admin** logins, PIN-protected accounts, transaction logging with timestamps, savings interest, and **encrypted file storage** so personal data is never saved as plain text.

Built as a group project by **Zain (Group Leader)**, **Rayyan** and **Sarim**.

📋 **Project board:** [Trello – Bank Account Management System](https://trello.com/b/FAOHUCAM/bank-account-management-system-%F0%9F%8F%A6)

---

## 📑 Table of Contents

- [Features](#-features)
- [Project Status](#-project-status)
- [Tech Stack](#-tech-stack)
- [Data Storage & Security](#-data-storage--security)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
- [Usage](#-usage)
- [Testing](#-testing)
- [Team](#-team)
- [License](#-license)

---

## ✨ Features

### Core Features
- **Create account**: register a new account with a PIN
- **Deposit & withdraw**: with validation for invalid amounts and insufficient funds
- **View account details**: see complete information for your account
- **Close / delete account safely**: confirmation before removal
- **Fund transfer**: move money between two accounts

### Extra Features
- **Savings interest**: automatically calculates and credits returns on savings accounts
- **Transaction logging**: every deposit, withdrawal and transfer is recorded with a timestamp
- **User mini-statement**: personal transaction history and activity summary
- **Hidden PIN input**: the PIN is masked while typing

### Security & Admin
- **Role authentication**: separate login for users and admins
- **Admin Master PIN**: a special PIN required for admin access
- **Admin master control**: view, manage, edit or close any account
- **Admin global audit**: view transaction histories for all accounts
- **Cipher encryption / decryption**: all data files are saved as ciphertext to protect personal data

---

## 📊 Project Status

| Area | Status |
|------|--------|
| Account creation | ✅ Done |
| Cipher encryption / decryption | 🚧 In progress |
| Setup (structs, file formats, environment) | 📝 Planned |
| Core features (deposit, withdraw, transfer, view, close) | 📝 Planned |
| Extra features (interest, logging, mini-statement, hidden PIN) | 📝 Planned |
| Security & admin (roles, master PIN, control, audit) | 📝 Planned |
| Testing & wrap-up (edge cases, report, demo) | 📝 Planned |

> Live progress is tracked on our [Trello board](https://trello.com/b/FAOHUCAM/bank-account-management-system-%F0%9F%8F%A6).

---

## 🛠 Tech Stack

| Component | Details |
|-----------|---------|
| Language | C |
| Compiler | GCC |
| IDE | VS Code / Dev C++ |
| Storage | File handling (encrypted files) |
| Interface | Command-line / console |
| Version control | Git & GitHub |
| Project management | Trello |

---

## 🔐 Data Storage & Security

- **Structures:** the program is built around two core structs, `Account` and `Transaction`.
- **Files:** accounts and transactions are stored in separate data files.
- **Encryption:** files are written as **ciphertext** using a cipher and decrypted only when loaded into memory, so personal data is not readable in the saved files.
- **PIN protection:** PINs are entered with hidden input (not echoed to the screen).
- **Roles:** users can only access their own account; admins need the **Master PIN** to unlock admin controls.

> ⚠️ The cipher used here is for learning purposes and is not production-grade cryptography.

---

## 📁 Project Structure

```
bank-account-management-system/
├── main.c            # Entry point, login and menus
├── account.c / .h    # Account functions and struct
├── transaction.c / .h# Transaction logging and struct
├── admin.c / .h      # Admin features
├── cipher.c / .h     # Encryption / decryption
├── data/             # Encrypted data files (created at runtime)
└── README.md
```

> Update this to match your actual file layout. A single-file project works too.

---

## 🚀 Getting Started

### Prerequisites
- A C compiler (**GCC**, via MinGW on Windows)
- **VS Code** or **Dev C++** (optional)
- **Git**

### Clone

```bash
git clone https://github.com/TheRealGamertube/Programming-Fundamentals-Project.git
cd Programming-Fundamentals-Project
```

### Compile

```bash
gcc main.c account.c transaction.c admin.c cipher.c -o bank
```

*(Single-file project: `gcc main.c -o bank`)*

### Run

```bash
# Linux / macOS
./bank

# Windows
bank.exe
```

---

## 💻 Usage

1. Run the program and choose **User** or **Admin** login.
2. Enter your account number and PIN (hidden as you type).
3. Pick an action from the menu: deposit, withdraw, transfer, view details, mini-statement, or close account.
4. Admins enter the **Master PIN** to view, edit or close any account and to audit all transactions.
5. Exit the program to save all data securely.

---

## 🧪 Testing

We test edge cases including:
- ❌ Invalid PIN
- ❌ Insufficient funds
- ❌ Missing / non-existent account
- ❌ Invalid amounts and menu choices

---

## 👥 Team

| Name | Role |
|------|------|
| **Zain** | Group Leader |
| **Sarim** | Developer |
| **Zain** | Developer |

Tasks are split among team members on the Trello board.

---

## 📄 License

This project was created for educational purposes. Add a `LICENSE` file (e.g. MIT) if you want to formalize usage.

---

⭐ If you found this project helpful, consider giving the repository a star!
