//macro for detection af falling edge
#define FE(signal, state) ((state<<1)|((signal&1)&3))==2
#define SOIL_MOIST_MIN_DEFAULT 700;
