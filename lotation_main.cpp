#include "mbed.h"
#include "rbms.h"
#include "rote_robo_ms.h"

#define BUFFER_MAX 4
#define TOTAL_MOTOR 1
#define GIAR_RATIO 36 // m2006の内部にあるギアの比率
#define MOTOR1_MAX 18000 // 軸がこの角度回ったらモーターの回転を停止させる(しきい値)

UnbufferedSerial pc(USBTX, USBRX, 9600);
CANMessage msg, buffer_motor[BUFFER_MAX];
CAN can(PD_0, PD_1);
rbms m2006(can, 0, TOTAL_MOTOR); // canの実体, モータータイプ(m2006...0, m3508...1), モーターの数
rote_robo_ms rote_robo_ms_motor1(can, m2006, 0,16); // canの実体, rbmsの実体, 制御するモーターのid, モーターにつけた回転体の半径(この引数はなくても動作する)
Thread thread1, thread_can;

void motor1_func();
void can_receive();

int motor[TOTAL_MOTOR];
long motor1_angle;//long:intと同じく整数が入るけど、intより大きい数も入る

int main(){
    motor[0] = 3000;
    thread1.start(motor1_func);
    thread_can.start(can_receive);
    while(true){
        m2006.rbms_send(motor);
        ThisThread::sleep_for(10ms);
    }
}

void motor1_func(){
    while (true){
        rote_robo_ms_motor1.rote_robo_ms_update(buffer_motor, BUFFER_MAX);
        motor1_angle = rote_robo_ms_motor1.get_rote_sum() / GIAR_RATIO; // モーター軸の回転数を計算
        printf("\r\nmotor1::angle:%ld",motor1_angle);
        if(motor1_angle >= MOTOR1_MAX){
            motor[0] = 0;
            printf("motor1::Brake!!!");
        }
        ThisThread::sleep_for(2ms);
    }
}

void can_receive(){ // rote_robo_msで角度を取得するときはこのような書き方じゃないといけないらしい
    while(true){
        if(can.read(msg)){
            for(int i = 1; i < BUFFER_MAX; i++){
                buffer_motor[i - 1] = buffer_motor[i];
            }
            buffer_motor[BUFFER_MAX - 1] = msg;
        }
    }
}
