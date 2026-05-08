# Robotic Arm Controller (PCA9685) 🦾

An optimized, highly responsive 3-DOF robotic arm controller built with Arduino and the PCA9685 PWM Servo Driver. This project was specifically developed for competitive robotics (Maze Solving & Payload Manipulation) to drastically reduce operation time.

## 🚀 Key Features

* **Time Optimization:** Replaced slow, manual joint-by-joint control with **3 Pre-defined Macros/Modes**.
* **Simultaneous Movement:** The arm smoothly transitions the Base, Shoulder, and Gripper to target angles concurrently with a single command.
* **Manual Override:** Retains precise single-degree tuning for dynamic adjustments on the field.
* **Hardware:** - Arduino (Nano/Uno/Mega)
  - PCA9685 16-Channel 12-bit PWM Controller
  - Bluetooth Module (HC-05/HC-06)

## 🎮 Pre-defined Modes

1. **Mode 1 (Target Grab):** Prepares the arm at specific angles `(Base: 130°, Shoulder: 35°)` to pick up the payload.
2. **Mode 2 (Attack/Travel):** Lowers the profile of the arm `(Base: 0°, Shoulder: 55°)` for safe and fast navigation across the track.
3. **Mode 3 (High Dumbbell):** Elevates the payload `(Base: 130°, Shoulder: 170°)` to drop it into elevated targets.

## 🛠️ Usage

Send the following commands via Bluetooth Serial (Baud Rate: 9600):
- Send `1`, `2`, or `3` to trigger the pre-defined modes.
- Send `T`/`X` for Shoulder adjustment, `U`/`D` for Base adjustment, and `G`/`H` for Gripper control.
