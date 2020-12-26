#ifndef _FREENOVE_4WD_CAR_EMOTION_H
#define _FREENOVE_4WD_CAR_EMOTION_H

#define EMOTION_ADDRESS 0x71
#define EMOTION_SDA     13
#define EMOTION_SCL     14

void clearEmtions(void);           //clear all
void eyesRotate(int delay_ms);     //rotate eyes
void eyesBlink(int delay_ms);      //blink eyes
void eyesSmile(int delay_ms);      //smile
void eyesCry(int delay_ms);        //cry
void wheel(int mode, int delay_ms);//wheel

extern int emotion_task_mode;

void Emotion_Setup();              //Initializes the Led Matrix
void Emotion_Show(int mode);       //Display:0-Display off,1-Turn the eyes,2-blink eyes,3-smile,4-cry,5-left-wheel,6-right-wheel
void Emotion_SetMode(int mode);    //set the emotion show mode
void staticEmtions(int emotion);   //show static emotion


#endif













//
