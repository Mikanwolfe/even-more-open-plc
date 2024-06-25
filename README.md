# Even More Open PLC

The goal of this small mini project is:

1. Write ESP32 firmware such that it can interpret Ladder Logic
2. Provide Debugging support for on-board PLC tags
3. Provide functionality to upload/download PLC code.

The "PLC Code" will be Allen-Bradley/Rockwell styled Ladder Logic.

It won't be actual AB code but inspired by it.

To build: `make`. To run, `./ladder_logic`.


## Syntax

All code lines must start with a number. This does not have to be sequential, but it is preferred.
All non-number lines will be skipped/treated as comments. The following is valid:

```
This represents:
|     var1    var2    var3    |
|-----[ ]-----[/]-----( )-----|
001 XIC(var1) XIO(var2) OTE(var3)
002 END
```

Typically all instructions are 3 letters long. This may change later.
Instructions can be any number of params
Instructions must be separated by a space ` `

All variables must be declared before scanning
Adding may only occur to variables of the same type

### Example scan

logic.txt

```

This represents:
|     var1    var2    var3
|-----[ ]-----[/]-----( )-----|

001 XIC(var1) XIO(var2) OTE(var3)
002 ADD(var11,var12,var13)
003 SUB(var21,var22,var23)
004 LSS(i1,i2) OTE(r1)
004 GTR(i1,i2) OTE(r2)
006 END
```

Gives the following output

```
-------Variables before execution:-------
i1 = 5
i2 = 15
r1 = false
r2 = false
var1 = false
var11 = 2
var12 = 3
var13 = 5
var2 = false
var21 = 5
var22 = 3
var23 = 2
var3 = false
--------------
XIC(var1) = 0, Line = 0
XIO(var2) = 1, Line = 0
OTE(var3) = 0
ADD(var11,var12,var13) = 5
SUB(var21,var22,var23) = 2
LSS(i1,i2) = LSS(5,15) = true
OTE(r1) = 1
GTR(i1,i2) = GTR(5,15) = false
OTE(r2) = 0
End found, stopping further instructions.
-------Variables after execution:-------
i1 = 5
i2 = 15
r1 = true
r2 = false
var1 = false
var11 = 2
var12 = 3
var13 = 5
var2 = false
var21 = 5
var22 = 3
var23 = 2
var3 = false
--------------
```


### Currently implemented instructions

XIC

- Excite on Close

XIO

- Excite on Open

OTE

- Output Terminal

ADD

- Adds INTs or REALs

SUB

- Subtracts INTs or REALs

LSS

- Less than

GTR

- Greater than


## Next up
A simple SPS simulation

## Philosophy/General rules

I'll adhere to the following general rules as a rule of thumb or preference.

- No such things as blocks for variable storage. That's kind of annoying and silly and should be relegated to SLC5s
- "PLC Code" must be uploadable and downloadble, without flash or restart
- Variables are always globally scoped
- Source code should be customisable such that any reasonably skilled user can make a driver and additional blocks for any ESP/arduino
- LESS and GREATER should follow CMD syntax, i.e., less = LSS and greater = GTR. This applies across the platform: follow programming convention.