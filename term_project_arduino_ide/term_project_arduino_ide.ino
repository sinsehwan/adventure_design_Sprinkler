#include <Servo.h>

const int relayPin = 40; // 릴레이 핀
const int servoPinNp = 9; // 노즐 각도 조절 서보 핀
const int servoPinNy = 10; // y축 서보 핀

Servo servoNp; // 노즐 서보
Servo servoNy; // y축 서보

//상태 변수

float yaw_deg = 0; // yaw 각도
float pitch_deg = 27.5; // pitch 각도 초기값
float temparature1 = 0; // 외부 온도
float temparature2 = 0; // 화재 온도 

int np_deg = 167; // 노즐 각도 초기화
bool flag_high_temp = false; // 고온 감지 플래그
bool flag_fire = false; // 화재 감지 플래그
bool flag_time_inc = true; // 시간 증가 플래그
bool flag_inc = true; // 각도 증가/감소 방향 플래그



//초기화
void setup(){
    pinMode(relayPin, OUTPUT);
    servoNp.attach(servoPinNp);
    servoNy.attach(servoPinNy);

    digitalWrite(relayPin, LOW); //릴레이 초기화

    servoNp.write(np_deg); // 초기 노즐 위치 설정
    Serial.begin(9600);
}

void loop(){
    //get_data(); // 센서 데이터 받아오기 (thread처럼 동작)

    //고온 감지
    if(flag_high_temp){
        move_center();

        if(flag_fire){
            extinguish(2000); // 화재 발생시 물 뿌려주기
            reset_state();
        }
        else{
            judge_fire(); // 화재 판단
        }
    }
    else{
        inc_deg(); // 각도 증가 로직
    }

    delay(1000); // 1초 간격으로 동작
}

// 각도 증가
void inc_deg(){
    if(flag_time_inc){ // 시간 진행 플래그 활성일 때만 각도 수정
        if(flag_inc) { // 각도 증가 플래그 설정 상태
            yaw_deg += 6;

            if(yaw_deg >= 360){
                flag_inc = false;
            }
        } 
        else{
            yaw_deg -= 6;
            if(yaw_deg <= 0){
                flag_inc = true;
            }
        }
        servoNy.write(yaw_deg / 2); // 서보 동작
        servoNp.write(pitch_deg); // pitch 서보 위치
    }
}

// 고온 위치로 이동
void move_center(){
    // 고온 위치 중심으로 이동
    while(temparature2 >= 50){
        yaw_deg = (yaw_deg + 10) / 360;
        pitch_deg = min(pitch_deg + 5, 90);
        servoNp.write(pitch_deg);
        delay(2000);
    }
}

//화재 판단
void judge_fire() {
    if (temparature2 >= 70){
        flag_fire = true; // 화재로 판단.
    }
}

void extinguish(int ext_time) {
    digitalWrite(relayPin, HIGH); // 릴레이 켜기
    spray(ext_time); // 물 뿌리기
    digitalWrite(relayPin, LOW);
}

void spray(int ext_time) {
    for (int i = -10; i <= 10; i++){
        for (int j = -10; j <= 10; j++)
        {
            servoNp.write(np_deg + i);
            servoNy.write(90 + j);
            delay(ext_time / 800);
        }
    }
}

void reset_state() {
    np_deg = 167;
    servoNp.write(np_deg);
    delay(2000);

    flag_time_inc = true;
    flag_high_temp = false;
    flag_fire = false;
    pitch_deg = 27.5;
}