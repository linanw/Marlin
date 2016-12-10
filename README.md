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

Extra Terminal Commands
- m1000 'Enable Quick Test Mode'
- m1001 'Enable Full Test Mode'
