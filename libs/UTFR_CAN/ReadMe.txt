/* This library contains helpful APIs and data structures
 * to improve the CAN user experience.
 *
 * Contributors: Spencer Ball, 2022
 *
 * University of Toronto Formula Racing
 */


 The plan is to create a unique library version for each MCU (RC Mega, ACM, CAN ISO, Corners).
 Each unique version will contain the message data structures for all messages handled by that MCU.


========== INSTALL THESE LIBRARIES ======================
=========================================================
  --> ArduinoSTL version 1.3.3  (comment out '#define ARDUINOSTL_DEFAULT_CIN_COUT' in ArudinoSTL.cpp to reduce size) 
  --> CAN-BUS Shield version 1.0.0


===== FOR EACH UNIQUE VERSION, EDIT THESE THINGS =======
========================================================

[In UTFR_CAN_versionName.cpp]

  --> CTRL+F replace all "UTFR_CAN" with "UTFR_CAN_VERSIONNAME"

 
[In UTFR_CAN_versionName.h]

  --> CTRL+F replace all "UTFR_CAN" with "UTFR_CAN_VERSIONNAME"

  --> Message field definitions       (in #defines at top of file)
  --> CAN_msgNames_E enumeration      (directly below #defines, above class)

  --> _CAN_msgArray initialization    (in private data definitions, within class)
  --> _CAN_filterArray initialization (in private data definitions, within class)