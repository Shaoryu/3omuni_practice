#include "mbed.h"
#include "rbms.h"
#include "im920_can_lib.h"

enum {
    m2006,
    m3508
};//m2006=0,m3508=1が代入される
enum {
    x,
    y,
    angle
};//同じようにx=0,y=1,Z=2

CAN can(PA_11,PA_12,1000000);
CAN canmotor(PB_12,PB_13,1000000);//テスト用基板
//CAN canmotor(PB_5,PB_6,1000000);//河上先輩用基板
can920 ps5(can,1);

rbms motor(canmotor, m2006, 3);//関数名(canの,0 or 1,モーターの数)
UnbufferedSerial pc(USBTX, USBRX, 9600);

Thread thread_motor, thread_can;

void speed_control();
void can_receive();



int torque[3] = {0};
int set_speed[3] = {0};
int speed[3]={0};

int main(){

    ps5.setup(10);
    int val;

    int data[PS5::ALL_BUTTON];
    bool Stop_Signal;

    thread_motor.start(speed_control);
    thread_can.start(can_receive);
    while (true){
        

        val=ps5.get_data(data,&Stop_Signal);
        if(val==1){
            if(data[PS5::PSBUTTON])NVIC_SystemReset();//おまじない？
            speed[x]=data[PS5::RSTICKX]*38;
            speed[y]=data[PS5::RSTICKY]*38;

            speed[angle]=data[PS5::LSTICKX]*38;
        }else if(val==-1){
            speed[x]=0;
            speed[y]=0;
            speed[angle]=0;
        }
        
        set_speed[0]= -speed[x]                                 +speed[angle];
        set_speed[1]=2*speed[x]-(int)(1.7320508*(float)speed[y])+speed[angle];//1.73...は√3
        set_speed[2]=2*speed[x]+(int)(1.7320508*(float)speed[y])+speed[angle];

        motor.rbms_send(torque);

        printf("torque 1: %5d,2:  %5d,3:  %5d\r\n", torque[0], torque[1], torque[2]);
        ThisThread::sleep_for(10ms);
    }
}

void speed_control(){
    motor.spd_control(set_speed, torque); 
}

void can_receive(){
    motor.can_read();
}

