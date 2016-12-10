<b>Extruder Test Rig Firmware (Marlin)</b>

Requirements:
- ROBO Electronics Board
- Z Axis Assembly
- Extruder Assembly
- XMIN limit switch
- ZMAX limit switch

Procedure:
- Trigger XMIN limit switch to start "Extruder Test"
- Trigger ZMAX limit switch to move Z Axis down 50mm

Modes of Operation:
- QuickTest Mode - Test without Z Probe Deviation
- FullTest Mode - Test with Z Probe Deviation

Hex Files:
- QuickTest.hex - ExtruderTestRig firmware in QuickTest Mode
- FullTest.hex - ExtruderTestRig firmware in FullTest Mode

Extra Terminal Commands:
- m1000 'Enable Quick Test Mode'
- m1001 'Enable Full Test Mode'

Loop Function:  <br>

if ((READ(X_MIN_PIN))==1 && (testcode == 0) && !(card.sdprinting))<br>
     { <br>
       delay(1000); <br>
       SERIAL_PROTOCOLLN("ROBO Z Axis Test Start"); <br>
       gcode_M302(); <br>
       gcode_G92(); <br>
       gcode_M302(); <br>
       current_position[E_AXIS] = 0; <br>
       destination[E_AXIS] = 120; <br>
       line_to_destination(); <br>
       current_position[E_AXIS] = 0; <br>
       delay(100); <br>
       gcode_M106(); <br>
       delay(100); <br>
       gcode_M118(); <br>
       delay(100); <br>
       if (fulltest == 1) <br>
       { <br>
       gcode_M48(); <br>
       delay(100); <br>
       } <br>
       gcode_M106(); <br>
       delay(100); <br>
       gcode_M18_M84(); <br>
       testcode=0; <br>
       SERIAL_PROTOCOLLN("ROBO Z Axis Test Done"); <br>
      } <br>
  if ((READ(Z_MAX_PIN))==1 && (testcode == 0) && !(card.sdprinting)) <br>
     { <br>
       delay(1000); <br>
       SERIAL_PROTOCOLLN("ROBO Z Axis Move"); <br>
       gcode_G92(); <br>
       feedrate_multiplier = 100; <br>
       current_position[Z_AXIS] = 0; <br>
       destination[Z_AXIS] = 50; <br>
       line_to_destination(); <br>
       delay(1000); <br>
       gcode_M18_M84(); <br>
       testcode=0; <br>
      } <br>
