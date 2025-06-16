#include "mbed.h"
#include "ps5_can_lib.h"
//2行目は指示によって変わる

CAN can(PA_11,PA_12,1000000);
CAN canmotor(PB_12,PB_13,1000000);//テスト用基板
//CAN canmotor(PB_5,PB_6,1000000);//テスト用基板

can920 ps5(can,1);//canの,ノード番号(1固定なはず)


UnbufferedSerial pc(USBTX, USBRX, 9600);


int main(){
    //ここから
    ps5.setup(10);
    int val;
    int data[PS5::ALL_BUTTON];
    bool Stop_Signal;
    //ここまで定型文として扱ってもいい


    while (true){
        val=ps5.get_data(data,&Stop_Signal);
        if(val==1){//何かコントローラのデータが変化したとき
            if(data[PS5::PSBUTTON])NVIC_SystemReset();//システムリセット
            printf("%d,%d,%d,%d",data[PS5::RSTICKX],data[PS5::RSTICKY],data[PS5::LSTICKX],data[PS5::LSTICKY]);

            if(data[PS5::CIRCLE])printf("○");
            if(data[PS5::CROSS])printf("×");
            if(data[PS5::TRIANGLE])printf("△");
            if(data[PS5::SQUARE])printf("□");
            printf("\r\n");
        }else if(val==-1){//確かデータに変化なし
            //基本的に速度、トルクを0にする処理
        }


        ThisThread::sleep_for(10ms);
    }
}
