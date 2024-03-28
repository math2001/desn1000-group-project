Install the arduino IDE, and upload `code.c` to the arduino board (open the
file and click on the arrow).

To control the robot, you need to send instructions via the serial (so the
cable needs to be connected).

It starts with an initial speed of 50%.

- `f`: move **forward** at current speed
- `b`: move **backwards** at current speed
- `l`: **turn left** at current speed
- `r`: **turn right** at current speed
- `L`: **slide left** at current speed
- `R`: **slide right** at current speed

- `0`: **STOP** (set current speed to **0%**)
- `1`: set current speed to **10%**
- `2`: set current speed to **20%**
- `3`: set current speed to **30%**
- `4`: set current speed to **40%**
- `5`: set current speed to **50%**

## Example

- type `f` and press enter (makes the robot move at 50% forward)
- type `l` and press enter (makes the robot turn left at 50%)