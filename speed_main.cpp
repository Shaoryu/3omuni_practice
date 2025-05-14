//速度制御のサンプルプログラム。
//もうすぐライブラリが変わるらしいので長くは使えない、かも？
#include "mbed.h"
#include "rbms.h"

enum {
    m2006,
    m3508
};//だいたいint m2006=0;int m3508=1と同じ

CAN can(PA_11,PA_12,1000000);//CAN 名前(rx,tx,お呪い)
CAN canmotor(PB_5,PB_6,1000000);
//CAN canmotor(PB_12,PB_13,1000000);//rxとtxのピンは使う基板によって変わります

CANMessage msg;
rbms motor(canmotor, m2006, 3);//関数名(使うcanの,モーターの種類(0 or 1,モーターの数)
UnbufferedSerial pc(USBTX, USBRX, 9600);

Thread thread1, thread_can;




int torque[3] = {0};
int set_speed[3] = {0};

void can_receive(){
    motor.can_read();
} 

void speed_control(){
    motor.spd_control(set_speed, torque); 
}


int main(){
    thread1.start(speed_control);
    thread_can.start(can_receive);
    while (true){
        set_speed[0]=50;
        set_speed[1]=50;
        set_speed[2]=50;
        motor.rbms_send(torque);

        printf("torque 1: %5d,2:  %5d,3:  %5d\r\n", torque[0], torque[1], torque[2]);//デバッグ文。あると原因究明が楽。
        ThisThread::sleep_for(10ms);//次の送信まで10ms待つ。
    }
}
