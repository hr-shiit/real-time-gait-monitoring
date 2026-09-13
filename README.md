# Real Time GAIT Monitoring and Disorder Detection System

**Bangalore Institute of Technology**  
Institution of Rajya Vokkaligara Sangha | An Autonomous Institution under VTU  
Department of Electrical and Electronics Engineering  
Major Project (BEE586) · Project ID **B-02**

An affordable, portable wearable system for continuous gait monitoring and disorder detection. It uses an ESP32, multi-site inertial sensors, and plantar pressure sensors, with BLE telemetry and an offline dual-head neural network for gait-phase and activity classification on a companion mobile app.

## Project team

| Role | Name | USN |
| --- | --- | --- |
| Student | Harshit Srivastav | 1BI23EE020 |
| Student | Pratyush Rout | 1BI23EE041 |
| Student | Sameekshya Ranjan | 1BI23EE050 |
| Student | Tejasvi K | 1BI23EE060 |
| Guide | Prof. Nandini N, Assistant Professor | — |

**Batch contact:** 9975797761

## Introduction

Traditional clinical gait analysis is expensive and limited to specialized labs with optical motion capture, which makes continuous monitoring and early disorder detection hard to access. This project addresses that gap with a wearable built from standard microcontrollers, multi-site IMUs, and plantar pressure sensors. Wireless telemetry and an offline neural-network classification pipeline bring gait metrics and classification results to a smartphone without clinical infrastructure.

## Objectives

- Engineer an intelligent classification model that can discern gait-phase progression and activity type, covering both physiological and pathological locomotion.
- Design a dual-head neural network that classifies walking phases and detects movement disorders from multi-sensor inputs (slow rhythm patterns and fast impact events).
- Optimise the pipeline for embedded deployment: buffer ESP32 sensor telemetry, send it over BLE, preprocess it, and run a trained dual-head CNN for mobile-app output.
- Architect the wearable hardware: ESP32 with multi-site MPU-6050 IMUs, FSR402 pressure sensors, and a TP4056-managed LiPo power subsystem.

## Methodology

1. **Sensing and power** — Five MPU-6050 IMUs (I2C multiplexer) and six FSR402 foot sensors (LM358 conditioning), powered by a 3.7 V LiPo and coordinated by an ESP32.
2. **Model training** — A Kaggle gait dataset is filtered, processed with Extended Kalman Filtering (EKF), and normalized to train a dual-head CNN, then compiled to 8-bit quantized TFLite INT8.
3. **Preprocessing and inference** — Buffered BLE data is filtered, EKF-processed, normalized, and evaluated with the TFLite model for gait-phase and activity classification.
4. **Wireless monitoring** — Classification results, gait phases, and alerts stream to a companion mobile app for real-time visualization.

## Firmware in this repository

| Path | Purpose |
| --- | --- |
| `Major_Test.ino` | Multi-IMU (MPU-6050) fusion over a TCA9548A multiplexer; streams pitch/roll for five sensors |
| `FInal_Test/FInal_Test.ino` | Combined IMU + FSR heel/ball pressure stream from ESP32 |
| `Fsr_Test/Fsr_Test.ino` | Processing sketch for single-foot heel/ball pressure visualization |

## Hardware

- ESP32 microcontroller
- TCA9548A I2C multiplexer
- Five MPU-6050 IMUs
- FSR402 plantar pressure sensors (heel/ball in the current sketches; six FSRs in the full system)
- LM358 op-amp conditioning
- TP4056 LiPo charging / 3.7 V LiPo battery

## Results

The wearable hardware rig was fabricated and worn comfortably with the sensor network. Multi-site inertial and plantar pressure sensors captured motion telemetry during testing. The dual-head model was trained on a gait dataset for off-device phase and movement-pattern identification. Later iterations moved toward live walking analysis, supporting real-time locomotion tracking as a low-cost option for clinical and everyday gait diagnostics.

## Acknowledgement

We sincerely thank our guide **Prof. Nandini N**, Assistant Professor, Department of Electrical and Electronics Engineering, B.I.T, for constant guidance, support, and encouragement throughout the Major Project.

We thank Major Project Coordinators **Dr. H. Suresh**, Associate Professor, and **Prof. Avinash S**, Assistant Professor, Department of EEE, B.I.T.

We are grateful to **Dr. P. Pramila**, Professor and Head, Department of EEE, B.I.T, and **Dr. Vijaya Prakash A M**, Principal, B.I.T, for their support.

## Additional information

Major Project (BEE586), Department of Electrical and Electronics Engineering, Bangalore Institute of Technology.  
College: [www.bit-bangalore.edu.in](https://www.bit-bangalore.edu.in)
