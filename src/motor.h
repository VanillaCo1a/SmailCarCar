#ifndef NOT_BLACK
#define NOT_BLACK 0
#endif
#ifndef BLACK
#define BLACK 1
#endif
#ifndef OBSTACLE
#define OBSTACLE 0
#endif
#ifndef NON_OBSTACLE
#define NON_OBSTACLE 1
#endif
#ifndef FORWARD
#define FORWARD 1
#endif
#ifndef BACK
#define BACK -1
#endif
#ifndef STOP
#define STOP 0
#endif

void Car_Forward(void);
void Car_Back(void);
void Car_Stop(void);
void Car_Left(void);
void Car_Right(void);
void Car_Forward_Faster(void);
void Car_Forward_Lower(void);
void Wheel_Left(char);
void Wheel_Right(char);