# Multistage-Aircraft-Drone-Detection N Response System
Project: Multi-Stage Drone and Aircraft Detection & Response System

The objective of this project is to simulate an intelligent embedded system capable of continuously monitoring its surroundings, detecting aerial targets, tracking them, and initiating an appropriate response based on different threat conditions.

 🟢 1. Idle Stage (Safe Environment)

The primary radar servo continuously sweeps from 0° to 180°, scanning the surrounding area using both an Ultrasonic Sensor and a Sound Sensor. During this stage, a blinking "green LED" indicates that the environment is safe and no threat has been detected.

 🟡 2. Detection Stage (Threat Alert Mode)

When the system identifies a potential target, it switches into "Threat Alert Mode".

Case 1 – Standard Target
If an aircraft or drone enters the ultrasonic detection range and its acoustic signature is also detected, the system classifies it as a potential threat. The "yellow LED" begins blinking while the buzzer produces intermittent warning beeps. At the same time, a secondary servo-mounted laser locks onto the target and tracks its movement.

Case 2 – Low-Altitude Stealth Drone
Low-flying drones can sometimes evade reliable ultrasonic detection. To overcome this limitation, the sound sensor continuously monitors engine and propeller noise. Once the predefined acoustic threshold is reached, the system enters "Threat Alert Mode". If the sound intensity or duration exceeds a higher threshold, the system immediately transitions to the response stage.

 🔴 3. Response Stage (Target Engagement)

When the detected target enters the inner response zone—or exceeds the acoustic threshold—the system initiates its final response sequence.

■The red LED flashes rapidly.
■A continuous siren indicates a critical threat.
■ The servo-mounted laser points directly at the target to simulate neutralization.
■ After the engagement sequence is complete, the targeting servo automatically returns to its home position, allowing the system to resume scanning.

🛠️ Technologies & Components:

● Arduino UNO
● Ultrasonic Sensor
● Sound Sensor
● Dual Servo Motors
● Laser Module
●Multi-Color LEDs
●Buzzer/Siren
●Processing IDE

This project has been an incredible learning experience in embedded systems, sensor fusion, servo control, real-time decision-making, and hardware-software integration.Also its shows the raw power of tiny microcontroller.There is still plenty to improve by adding more sensors,AI in it.
Thanks for your time
