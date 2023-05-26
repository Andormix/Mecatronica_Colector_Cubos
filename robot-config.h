using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature Vision2__CUB_TARONJA;
extern signature Vision2__SIG_2;
extern signature Vision2__SIG_3;
extern signature Vision2__SIG_4;
extern signature Vision2__SIG_5;
extern signature Vision2__SIG_6;
extern signature Vision2__SIG_7;
extern vision Vision2;
extern distance Distance3;
extern optical Optical4;
extern motor Motor19;
extern motor Motor20;
extern motor Motor11;
extern inertial Inertial1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );