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

Nested branches are supported.

```

Branch testing

|     in1                 |
|-----[ ]----+----( )-----|
|     in2    |
|-----[ ]----|
|     in3    |
|-----[ ]----|

001 BST XIC(in1) NXB XIC(in2) NXB XIC(in3) BND OTE(out)

```


### Currently implemented instructions

- `XIC`
- `XIO`
- `OTE`
- `ADD`
- `SUB`
- `LSS`
- `GTR`
- `BST` Branch Start
- `NXB` Next Branch
- `BND` Branch End


### Instructions I want to add

- `OTL`
- `OTU`
- `PTS` - custom positive transition - (ONS)
- `NTS` - custom negative transition - 
- `TON`
- `TOF`
- `CTU`
- `CTD`
- `EQU`
- `NEQ`
- `SYS`tem vars like S:FS, Scan Time, etc. GSV?
- `DIV`
- `MUL`
- `PID` (yes, PID!)

I know some languages have AND, OR, as blocks. I am not making that.

## Next up

- More instructions
- Mapping IO
- Scan Time
- Programs
- Validation
- Some form of visualiastion


## Philosophy/General rules

I'll adhere to the following general rules as a rule of thumb or preference.

- No such things as blocks for variable storage. That's kind of annoying and silly and should be relegated to SLC5s
- "PLC Code" must be uploadable and downloadble, without flash or restart
- Variables are always globally scoped
- Source code should be customisable such that any reasonably skilled user can make a driver and additional blocks for any ESP/arduino
- LESS and GREATER should follow CMD syntax, i.e., less = LSS and greater = GTR. This applies across the platform: follow programming convention.