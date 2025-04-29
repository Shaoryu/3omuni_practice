#include "mbed.h"
#include "rbms.h"

enum {
    m2006,
    m3508
};//m2006=0,m3508=1が代入される


//コンストラクタ
CAN canmotor(PB_5,PB_6,1000000);

rbms motor(canmotor, m2006, 3);//関数名(canの,0 or 1,モーターの数)
UnbufferedSerial pc(USBTX, USBRX, 9600);

int torque[3] = {0};

int main(){
    int motor_torque=1000;//モーターのパワーを決める変数
    while (true){
        torque[0]=motor_torque;
        torque[1]=motor_torque;
        torque[2]=motor_torque;

        motor.rbms_send(torque);

        printf("torque 1: %5d,2:  %5d,3:  %5d\r\n", torque[0], torque[1], torque[2]);
        ThisThread::sleep_for(10ms);
    }
}
