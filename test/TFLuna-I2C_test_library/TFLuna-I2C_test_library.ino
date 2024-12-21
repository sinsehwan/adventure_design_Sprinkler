#include <Arduino.h>
#include <Wire.h>
#include <TFLI2C.h>

TFLI2C tflI2C;

int16_t tfAddr = TFL_DEF_ADR;
uint16_t tfFrame = TFL_DEF_FPS;

int16_t tfDist = 0;
int16_t tfFlux = 0;
int16_t tfTemp = 0;

uint16_t tfTime = 0;
uint8_t tfVer[3]; //device version
uint8_t tfCode[14]; // device serial number

uint8_t tfCount  =0;

void sampleCommands(int16_t adr){
    Serial.print("Device Address: ");
    Serial.println(adr);

    Serial.print("System Reset: ");
    if(tflI2C.Soft_Reset(adr)){
        Serial.println("Passed");
    }
    else{
        tflI2C.printStatus();
        Serial.println("reset?");
    }
    delay(500);

    if(tflI2C.Get_Firmware_Version(tfVer, adr)){

    }
    else{
        tflI2C.printStatus();
        Serial.println("firm?");
    }
    delay(500);
}

void setup(){
    Serial.begin(9600);
    Wire.begin();

    sampleCommands(tfAddr);
}

void loop(){
    if(tflI2C.getData(tfDist, tfFlux, tfTemp, tfAddr)){
        Serial.print(tfDist);
        Serial.print(" | Flux : ");
        Serial.print(tfFlux);

        tfTemp = int16_t(tfTemp / 100);

        Serial.print(" | Temp: ");
        Serial.println(tfTemp);
    }
    else{
        tflI2C.printStatus();
        Serial.println();
    }

    delay(506);
}