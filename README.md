# Even More Open PLC

## Project Goals

This project aims to achieve the following objectives:

1. Develop firmware for the ESP32 to interpret Ladder Logic.
2. Provide debugging support for on-board PLC tags.
3. Enable the upload and download of PLC code.

The PLC code will be inspired by Allen-Bradley/Rockwell styled Ladder Logic, though it will not be actual AB code.

### Building and Running

To build the project, use:
```
make
```

To run the project, use:
```
./ladder_logic
```

## Project Rationale

The main limitation with most ESP-based ladder logic systems (e.g., OpenPLC, IoT Ladder Editor) is their reliance on compiling into PLC code or firmware. This is similar to most PLCs or RTUs such as Kingfishers, SCADAPacks, etc., which require a compilation step.

This project aims to create an ESP32 firmware interpreter, allowing logic to be uploaded and downloaded without flashing the firmware. This approach offers several advantages:

- Interpreted languages do not crash your firmware.
- Logic can be changed on the fly.
- Logic can be easily programmed by an external source and transferred to the device.

The project emphasises simplicity, avoiding heavy runtimes like Python2-based GUIs or Java 8 GUIs. A CLI-based runtime or a Docker image with serial port access and a small web server for device configuration will be provided. This will offer a simplified Node-RED-like experience.

## Syntax

All code lines must start with a number. Non-number lines will be skipped or treated as comments. The following example is valid:

```
This represents:
|     var1    var2    var3    |
|-----[ ]-----[/]-----( )-----|
001 XIC(var1) XIO(var2) OTE(var3)
002 END
```

Key points:
- Instructions are typically three letters long.
- Instructions can have any number of parameters.
- Instructions must be separated by a space (` `).
- All variables must be declared before scanning.
- Operations can only be performed on variables of the same type.
- Nested branches are supported.

Instructions are written simialrly to function calls. For example, `ADD(x,y,z)` is equivalent to x + y = z. Ensure that there are no spaces in-between parameters.
- `ADD(x,y,z)` ✅
- `ADD(x, y, z)` ❌

### Example: Branch Testing

```
|     in1                 |
|-----[ ]----+----( )-----|
|     in2    |
|-----[ ]----|
|     in3    |
|-----[ ]----|

001 BST XIC(in1) NXB XIC(in2) NXB XIC(in3) BND OTE(out)
```

### Example: Visualisation

```
tof_acc int 0
tof_dn bool 1
tof_pre int 10000
tof_tt bool 0
onr bool 0

001 ONR(onr) TOF(tof_dn,tof_tt,tof_pre,tof_acc)
002 XIC(tof_dn)
```

The above logic can generate ladder-like displays in the console:

```
-------Variables before execution:-------
onr = true
tof_acc = 142
tof_dn = true
tof_pre = 1000
tof_tt = true
--------------
| ===  ONR[onr] --- TOF(226/1000) --- |
| ===  XIC[tof_dn] === |
-------Variables after execution:-------
onr = true
tof_acc = 226
tof_dn = true
tof_pre = 1000
tof_tt = true
Scan time: 79 us
--------------
```

The output generally follows:
```
Key
--- (branch is false)
=== (branch is true)


For each line, start with
| ===

End each line with 
| << endl;

Each token outputs itself and the next line state, i.e.
XIC[var1]=== (var1 outputs true)
XIO[var1]--- (var1 outputs false)

Branch starts output <<
Next Branch is ^^
Branch End is >> 
```


### Example: Simple SPS Simulation

A simple simulation of an SPS with a single pump and a level transmitter.

```
001 ADD(level,inflow,level)
002 GTR(level,high) OTE(start_pump)
003 LSS(level,low) OTE(stop_pump)
004 XIC(run_pump) SUB(level,outflow,level)
005 BST XIC(start_pump) NXB XIC(run_pump) BND XIO(stop_pump) OTE(run_pump)
```

The same can be achieved with OTL's and OTUs

```
001 ADD(level,inflow,level)
002 GTR(level,high) OTL(run_pump)
003 LSS(level,low) OTU(run_pump)
004 XIC(run_pump) SUB(level,outflow,level)
```

## Currently Implemented Instructions

- `XIC`
- `XIO`
- `OTE`
- `ADD`
- `SUB`
- `LSS`
- `GTR`
- `BST` (Branch Start)
- `NXB` (Next Branch)
- `BND` (Branch End)
- `TON`
- `TOF`
- `ONR` One Shot Rising
- `ONF` One Shot Falling
- `AFI` Always False Instruction
- `CTU`
- `CTD`
- `EQU`

## Planned Instructions

- `TOG` Toggle
- `NEQ`
- `SYS` (system variables like S:FS, Scan Time, etc.)
- `DIV`
- `MUL`
- `PID` (yes, PID!)

Note: There will be no AND, OR blocks, as they should be implemented as rungs.

## Future Development

- More instructions
- IO mapping
- Scan time optimisation
- Program structuring
- Validation
- Visualisation
- Function parameters and output to use rungs as AOIs
- Run Mode, Pause Mode

## Philosophy and General Rules

- Avoid blocks for variable storage; this is cumbersome and should be limited to SLC5s.
- PLC code must be uploadable and downloadable without needing a flash or restart.
- Variables are always globally scoped.
- The source code should be customisable, allowing reasonably skilled users to create drivers and additional blocks for any ESP/Arduino.
- Use consistent naming conventions, e.g., less = LSS and greater = GTR, following programming conventions across the platform.

I'm aiming to lightweight, simple, and flexible.