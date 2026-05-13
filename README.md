# Unified Multi-Restaurant Ordering and Secure Delivery Authentication System

This project is a C++ based implementation of a logistics and delivery system designed to handle complex multi-restaurant orders while ensuring secure authentication between customers, couriers, and vendors. It was developed as an Object-Oriented Programming (OOP) Lab project.

## 📋 Table of Contents
- [Project Overview](#-project-overview)
- [Key Features](#-key-features)
- [Technical Implementation](#-technical-implementation)
- [System Workflow](#-system-workflow)
- [Getting Started](#-getting-started)
- [Contributors](#-contributors)

---

## 🚀 Project Overview
Traditional food delivery platforms often require separate orders for different restaurants, leading to multiple delivery fees and fragmented logistics. This system introduces:
- **Unified Sourcing:** Allows customers to bundle orders from multiple locations.
- **Optimized Routing:** Dynamic assignment of couriers to multi-stop paths.
- **Secure Authentication:** A state-driven mechanism to verify delivery and prevent disputes.

---

## ✨ Key Features
- **Multi-Restaurant Cart:** Order from up to 10 restaurants in a single transaction.
- **Courier Management:** Tracks availability and busy-states of delivery personnel.
- **Order Life-cycle Tracking:** Real-time status updates: `PENDING` ➔ `ROUTED` ➔ `PICKED_UP` ➔ `DELIVERED`.
- **Security Protocols:** Requires specific actor authentication for "Pickup" and "Drop-off" events to facilitate fair refunds and accountability.

---

## 🛠 Technical Implementation
The system is built in **C++** and demonstrates core **Object-Oriented Programming (OOP)** principles:

- **Inheritance:** Uses an abstract base class `IActor` which is inherited by specific classes like `Courier`.
- **Encapsulation:** Protects sensitive data within `Order`, `Restaurant`, and `System` classes, using getters and setters for controlled access.
- **Abstraction:** Complex routing algorithms and state transitions are abstracted away from the end-user via a clean Command Line Interface (CLI).
- **Static Arrays & Fixed Memory:** Optimized for performance using fixed-size buffers (`MAX=50`) for data management.

---

## 🔄 System Workflow
1. **Initialization:** The system administrator adds Restaurants and Couriers.
2. **Placement:** A Customer ID is entered along with IDs for the desired restaurants.
3. **Routing:** The `runRouting()` engine checks for available couriers and maps the delivery.
4. **Authentication:** - Courier updates status to `PICKUP`.
   - Customer provides `DROPOFF_CONFIRMED`.
5. **Completion:** The system logs the final delivery and releases the courier back to the available pool.

---

## 🚦 Getting Started

### Prerequisites
* A C++ compiler (GCC, Clang, or MSVC)
* Terminal or Command Prompt access

### Execution
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/yourusername/multi-restaurant-ordering.git](https://github.com/yourusername/multi-restaurant-ordering.git)
