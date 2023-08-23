#ifdef PJ_ADC
#include <Arduino.h>

int a=F_CPU;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400,SERIAL_8N1|SERIAL_TX_ONLY);//1M
  pinModeFast(1,INPUT);
  pinModeFast(2,INPUT);
  pinModeFast(3,INPUT);
  pinModeFast(4,INPUT);
  analogReference(INTERNAL4V34);
}
 struct {
  union{
  uint16_t adc[4];
  uint8_t buf[8];
  };
 }data;
void loop() {
  // put your main code here, to run repeatedly:
  for(uint8_t ii=0;ii<12;ii++){
  for(uint8_t i=0;i<4;i++){
    data.adc[i]=(analogReadEnh(i+1,12)+data.adc[i]*7)>>3;
    delay(1);
  }
  }
  // data.adc[0]=100;
  // data.adc[1]=200;
  // data.adc[2]=300;
  // data.adc[3]=400;
  Serial.write(data.buf,8);

  delay(2);

}

#endif