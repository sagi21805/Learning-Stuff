# What are Transistors
Transistors are mechanical switches, Each transistor has 3 legs
- <u>Collector</u>  -> Electricity flow
- <u>Emitter</u> -> The output
- <u>Base</u> -> Input signal
![[1. Transistor|center]]

When the electricity flow in the collector, it doesn't flow to the Emitter unless a voltage is also applied to the base which creates an electrical switch 
# Logic Gates
The combination of transistors can create something that is called logic gate (Even one transistor can be a logic gate)
Each logic gate can be described both by a sequence of transistors but also with a <u>truth table</u> which is a table that 
## NOT Gate
![[2. NOT Gate|center]]
This is an inverter gate (A One is a Zero and a Zero is a One) and can be described with the following table

| X   | NOT X |     |
| --- | ----- | --- |
| 0   | 1     |     |
| 1   | 0     |     |

## AND Gate
![[3. AND Gate|center]]
This gate can be described by the following table

| X   | Y   | X & Y |
| --- | --- | ----- |
| 0   | 0   | 0     |
| 0   | 1   | 0     |
| 1   | 0   | 0     |
| 1   | 1   | 1     |
As we can see, electricity will flow only if X <u>AND</u> Y are on. 
## OR Gate 
![[4. OR Gate|center]]This gate can be described with the following table

| X   | Y   | X \| Y |
| --- | --- | ------ |
| 0   | 0   | 0      |
| 0   | 1   | 1      |
| 1   | 0   | 1      |
| 1   | 1   | 1      |
As we cab see, electricity will flow if X <u>OR</u> Y or Both will be on.
## XOR
This one is more complex, and created with some of the other base gates, but still considered one of the basics
![[5. XOR Gate|center]]
This gate can be described by the following table

| X   | Y   | X ^ Y |
| --- | --- | ----- |
| 0   | 0   | 0     |
| 0   | 1   | 1     |
| 1   | 0   | 1     |
| 1   | 1   | 0     |
As we can see only if X OR Y are one the output will be one, but not if both

# Logical Arithmetic Unit (ALU)
_From this point the gates are pictured from the videos of [@CoreDumpped](https://www.youtube.com/@CoreDumpped)_
## Binary Addition
