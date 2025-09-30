#!/usr/bin/python
# -*- coding: utf-8 -*-
import numpy as np
import cv2
import socket
import os
import io
import sys
import time
from threading import Timer, Thread
from PIL import Image
import string

from Command import COMMAND as cmd
from Thread import *
from Client_Ui import Ui_Client
from Video import *

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

def resource_path(relative_path):
    try:
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

class mywindow(QWidget, Ui_Client):
    def __init__(self):
        super(mywindow, self).__init__()
        self.setupUi(self)
        
        self.endChar='\n'
        self.intervalChar='#'
        self.trackFlag=0; self.lightFlag=0; self.commandFlag=1
        self.LED_Flag=0; self.Matrix_Flag=0
        self.ws2812_number=4095
        self.camera_angle=0
        self.W_flag=0

        self.car_image_path = resource_path('image/ESP32_4WD_Car.png')
        self.original_car_pixmap = QPixmap(self.car_image_path)
        self.current_video_source_pixmap = self.original_car_pixmap
        
        self.label_Video.mouseDoubleClickEvent = self.toggle_video_fullscreen
        self.video_is_fullscreen = False

        self.initial_capture_done = False
        self.aspect_ratio = 0.0
        self.widget_info = {}
        
        self.setAutoFillBackground(True)
        palette = self.palette()
        palette.setColor(QPalette.Window, QColor('black'))
        self.setPalette(palette)
        
        self.IP.setText("")
        self.IP.setPlaceholderText("IP Address")
        
        icon_path = resource_path('image/logo_Mini.png')
        self.setWindowIcon(QIcon(icon_path))
        self.update_video_pixmap()

        ipValidator = QRegExpValidator(QRegExp('^((2[0-4]\d|25[0-5]|\d?\d|1\d{2})\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?)$'))
        self.IP.setValidator(ipValidator)
        
        self.TCP=VideoStreaming()
        self.servo1=90; self.servo2=90
        
        self.setFocusPolicy(Qt.StrongFocus)
        self.progress_Power.setMinimum(0); self.progress_Power.setMaximum(100)
        self.progress_Power.setValue(100)
        self.progress_Power.setTextVisible(True)
        self.update_battery_style(100)
        
        self.HSlider_Servo1.setMinimum(0); self.HSlider_Servo1.setMaximum(180)
        self.HSlider_Servo1.setValue(self.servo1)
        self.VSlider_Servo2.setMinimum(80); self.VSlider_Servo2.setMaximum(180)
        self.VSlider_Servo2.setValue(self.servo2)
        self.HSlider_FineServo1.setMinimum(-10); self.HSlider_FineServo1.setMaximum(10); self.HSlider_FineServo1.setValue(0)
        self.HSlider_FineServo2.setMinimum(-10); self.HSlider_FineServo2.setMaximum(10); self.HSlider_FineServo2.setValue(0)

        self.connect_signals()
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.time)
        
    def update_battery_style(self, value):
        if value > 60: color = "#4CAF50"
        elif value > 30: color = "#FFC107"
        else: color = "#F44336"
        self.progress_Power.setStyleSheet(f"""
            QProgressBar {{ border: 1px solid #777; border-radius: 4px; text-align: center; background-color: #DDD; color: black; }}
            QProgressBar::chunk {{ background-color: {color}; border-radius: 3px; }}
        """)
        
    def showEvent(self, event):
        if not self.initial_capture_done:
            self.original_size = self.size()
            self.aspect_ratio = self.original_size.width() / self.original_size.height()
            self.widget_info = {}
            for widget in self.findChildren(QtWidgets.QWidget):
                if widget is self: continue
                font = widget.font()
                font_size = font.pointSize() if font.pointSize() > 0 else font.pixelSize()
                if font_size <= 0: font_size = 9
                self.widget_info[widget] = { 'geometry': widget.geometry(), 'font_size': font_size }
            self.initial_capture_done = True
        super(mywindow, self).showEvent(event)

    def toggle_video_fullscreen(self, event=None):
        self.video_is_fullscreen = not self.video_is_fullscreen
        
        for widget in self.findChildren(QtWidgets.QWidget):
            if widget.parent() is self and widget is not self.label_Video:
                widget.setVisible(not self.video_is_fullscreen)
        
        self.resizeEvent(QResizeEvent(self.size(), self.size()))

    def resizeEvent(self, event):
        super(mywindow, self).resizeEvent(event)
        if not self.initial_capture_done:
            return

        if self.video_is_fullscreen:
            self.label_Video.setGeometry(self.rect())
            self.update_video_pixmap()
            return
        
        window_size = self.size()
        w, h = window_size.width(), window_size.height()
        target_h = w / self.aspect_ratio
        
        if target_h <= h:
            content_w, content_h = w, int(target_h)
            content_x, content_y = 0, (h - content_h) // 2
        else:
            target_w = h * self.aspect_ratio
            content_w, content_h = int(target_w), h
            content_x, content_y = (w - content_w) // 2, 0
            
        content_rect = QRect(content_x, content_y, content_w, content_h)
        ratio = content_rect.width() / self.original_size.width()
        
        for widget, info in self.widget_info.items():
            original_geom = info['geometry']
            
            if widget.parentWidget() is self:
                new_x = content_rect.x() + int(original_geom.x() * ratio)
                new_y = content_rect.y() + int(original_geom.y() * ratio)
            else:
                new_x = int(original_geom.x() * ratio)
                new_y = int(original_geom.y() * ratio)

            new_w, new_h = int(original_geom.width() * ratio), int(original_geom.height() * ratio)
            widget.setGeometry(new_x, new_y, new_w, new_h)
            
            try:
                original_font_size = info['font_size']
                font = widget.font()
                new_font_size = int(original_font_size * ratio)
                if new_font_size > 1: font.setPointSize(new_font_size); widget.setFont(font)
            except: pass

            if isinstance(widget, QCheckBox):
                indicator_size = max(10, int(13 * ratio))
                widget.setStyleSheet(f"QCheckBox::indicator {{ width: {indicator_size}px; height: {indicator_size}px; }}")

        self.update_video_pixmap()

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape and self.video_is_fullscreen:
            self.toggle_video_fullscreen()
            return

        if event.key() == Qt.Key_Escape and self.isFullScreen():
            self.showNormal()
            return
            
        if(event.key() == Qt.Key_Up): 
            self.on_btn_Up()
        elif(event.key() == Qt.Key_Left): 
            self.on_btn_Left()
        elif(event.key() == Qt.Key_Down): 
            self.on_btn_Down()
        elif(event.key() == Qt.Key_Right): 
            self.on_btn_Right()
        elif(event.key() == Qt.Key_Home): 
            self.on_btn_Home()
        elif(event.key() == Qt.Key_R): 
            self.on_btn_Light()
        elif(event.key() == Qt.Key_T): 
            self.on_btn_Track()
        elif(event.key() == Qt.Key_C): 
            self.on_btn_Connect()
        elif(event.key() == Qt.Key_V): 
            self.on_btn_video()
        if event.isAutoRepeat(): return
        if event.key() == Qt.Key_W: 
            self.on_btn_ForWard(); 
            self.Key_W=True
        elif event.key() == Qt.Key_S: 
            self.on_btn_BackWard(); 
            self.Key_S=True
        elif event.key() == Qt.Key_A: 
            self.on_btn_Turn_Left(); 
            self.Key_A=True
        elif event.key() == Qt.Key_D: 
            self.on_btn_Turn_Right(); 
            self.Key_D=True  
        elif event.key() == Qt.Key_Space: 
            self.on_btn_Buzzer(); 
            self.Key_Space=True
    
    def connect_signals(self):
        self.HSlider_Servo1.valueChanged.connect(self.Change_Left_Right)
        self.VSlider_Servo2.valueChanged.connect(self.Change_Up_Down)
        self.HSlider_FineServo1.valueChanged.connect(self.Fine_Tune_Left_Right)
        self.HSlider_FineServo2.valueChanged.connect(self.Fine_Tune_Up_Down)
        self.Led_Module.clicked.connect(lambda: self.LedChange(self.Led_Module))
        self.RGB.clicked.connect(lambda: self.LedChange(self.RGB))
        self.checkBox_Led_Mode1.stateChanged.connect(lambda: self.LedChange(self.checkBox_Led_Mode1))
        self.checkBox_Led_Mode2.stateChanged.connect(lambda: self.LedChange(self.checkBox_Led_Mode2))
        self.checkBox_Led_Mode3.stateChanged.connect(lambda: self.LedChange(self.checkBox_Led_Mode3))
        self.checkBox_Led_Mode4.stateChanged.connect(lambda: self.LedChange(self.checkBox_Led_Mode4))
        self.checkBox_Matrix_Mode1.stateChanged.connect(lambda: self.MatrixChange(self.checkBox_Matrix_Mode1))
        self.checkBox_Matrix_Mode2.stateChanged.connect(lambda: self.MatrixChange(self.checkBox_Matrix_Mode2))
        self.checkBox_Matrix_Mode3.stateChanged.connect(lambda: self.MatrixChange(self.checkBox_Matrix_Mode3))
        self.checkBox_Matrix_Mode4.stateChanged.connect(lambda: self.MatrixChange(self.checkBox_Matrix_Mode4))
        self.Track.clicked.connect(self.on_btn_Track)
        self.Light.clicked.connect(self.on_btn_Light)
        self.Btn_ForWard.pressed.connect(self.on_btn_ForWard)
        self.Btn_ForWard.released.connect(self.on_btn_Stop)
        self.Btn_Turn_Left.pressed.connect(self.on_btn_Turn_Left)
        self.Btn_Turn_Left.released.connect(self.on_btn_Stop)
        self.Btn_BackWard.pressed.connect(self.on_btn_BackWard)
        self.Btn_BackWard.released.connect(self.on_btn_Stop)
        self.Btn_Turn_Right.pressed.connect(self.on_btn_Turn_Right)
        self.Btn_Turn_Right.released.connect(self.on_btn_Stop)
        self.Btn_Video.clicked.connect(self.on_btn_video)
        self.Btn_Up.clicked.connect(self.on_btn_Up)
        self.Btn_Left.clicked.connect(self.on_btn_Left)
        self.Btn_Down.clicked.connect(self.on_btn_Down)
        self.Btn_Home.clicked.connect(self.on_btn_Home)
        self.Btn_Right.clicked.connect(self.on_btn_Right)
        self.Btn_Buzzer.pressed.connect(self.on_btn_Buzzer)
        self.Btn_Buzzer.released.connect(self.on_btn_Buzzer)
        self.Btn_Connect.clicked.connect(self.on_btn_Connect)
        self.Btn_Cam_Left.clicked.connect(self.on_btn_Cam_Left)
        self.Btn_Cam_Right.clicked.connect(self.on_btn_Cam_Right)
        self.Btn_Cam_Origin.clicked.connect(self.on_btn_Cam_Origin)
        self.Color_W.textChanged.connect(self.WS2812_Text_Change)
        for i in range(1,13): getattr(self,f"L{i}").clicked.connect(self.WS2812_Calculate)
        self.W.clicked.connect(self.ALL_Click)
    
    def update_video_pixmap(self):
        pixmap = self.current_video_source_pixmap
        if pixmap and not pixmap.isNull():
            self.label_Video.setPixmap(pixmap.scaled(self.label_Video.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
    
    def keyReleaseEvent(self, event):
        key_map = {Qt.Key_W: 'Key_W', Qt.Key_S: 'Key_S', Qt.Key_A: 'Key_A', Qt.Key_D: 'Key_D'}
        if event.key() in key_map and not event.isAutoRepeat() and getattr(self, key_map[event.key()], False):
            self.on_btn_Stop(); setattr(self, key_map[event.key()], False)
        if event.key() == Qt.Key_Space and not event.isAutoRepeat() and getattr(self, 'Key_Space', False):
            self.on_btn_Buzzer(); self.Key_Space = False
    
    def on_btn_Connect(self):
        if self.Btn_Connect.text() == "Connect":
            self.h = self.IP.text()
            if self.h:
                self.TCP.StartTcpClient(self.h)
                try:
                    self.streaming = Thread(target=self.TCP.streaming, args=(self.h,))
                    self.streaming.start()
                    self.recv=Thread(target=self.recvmassage)
                    self.recv.start()
                    self.Btn_Connect.setText( "Disconnect")
                    print ('Server address:'+str(self.h)+'\n')
                except Exception as e: print(f"Connection Error: {e}")
        else:
            self.Btn_Connect.setText("Connect")
            self.Btn_Video.setText('Open Video')
            self.TCP.sendData(cmd.CMD_VIDEO + self.intervalChar + '0' + self.endChar)
            self.current_video_source_pixmap = self.original_car_pixmap
            self.update_video_pixmap()
            try:
                if hasattr(self, 'streaming'): stop_thread(self.streaming)
                if hasattr(self, 'timer'): self.timer.stop()
                if hasattr(self, 'power'): stop_thread(self.power)
                if hasattr(self, 'recv'): stop_thread(self.recv)
            except: pass
            self.TCP.StopTcpcClient()
    
    def on_btn_video(self):
        if self.Btn_Video.text()=='Open Video':
            self.timer.start(10)
            self.Btn_Video.setText('Close Video')
            if self.Btn_Connect.text()=="Disconnect": self.TCP.sendData(cmd.CMD_VIDEO+self.intervalChar+'1'+self.endChar)
        else:
            self.timer.stop()
            self.Btn_Video.setText('Open Video')
            if self.Btn_Connect.text() == "Disconnect": self.TCP.sendData(cmd.CMD_VIDEO + self.intervalChar + '0' + self.endChar)
            self.current_video_source_pixmap = self.original_car_pixmap
            self.update_video_pixmap()
    
    def on_btn_ForWard(self):
        if self.commandFlag: self.TCP.sendData(f"{cmd.CMD_MOTOR}#2500#2500#2500#2500\n")
    
    def on_btn_Turn_Left(self):
        if self.commandFlag: self.TCP.sendData(f"{cmd.CMD_MOTOR}#-2500#-2500#2500#2500\n")
    
    def on_btn_BackWard(self):
        if self.commandFlag: self.TCP.sendData(f"{cmd.CMD_MOTOR}#-2500#-2500#-2500#-2500\n")
    
    def on_btn_Turn_Right(self):
        if self.commandFlag: self.TCP.sendData(f"{cmd.CMD_MOTOR}#2500#2500#-2500#-2500\n")
    
    def on_btn_Stop(self):
        if self.commandFlag: self.TCP.sendData(f"{cmd.CMD_MOTOR}#0#0#0#0\n")
    
    def on_btn_Buzzer(self):
        if self.commandFlag:
            if self.Btn_Buzzer.text()=='Buzzer': self.TCP.sendData(f"{cmd.CMD_BUZZER}#1#2000\n"); self.Btn_Buzzer.setText('Noise')
            else: self.TCP.sendData(f"{cmd.CMD_BUZZER}#0\n"); self.Btn_Buzzer.setText('Buzzer')
    
    def on_btn_Up(self): 
        self.servo2 = min(180, self.servo2 + 10); 
        self.VSlider_Servo2.setValue(self.servo2)
    
    def on_btn_Left(self): 
        self.servo1 = max(0, self.servo1 - 10); 
        self.HSlider_Servo1.setValue(self.servo1)
    
    def on_btn_Down(self): 
        self.servo2 = max(80, self.servo2 - 10); 
        self.VSlider_Servo2.setValue(self.servo2)
    
    def on_btn_Right(self): 
        self.servo1 = min(180, self.servo1 + 10); 
        self.HSlider_Servo1.setValue(self.servo1)
    
    def on_btn_Home(self): 
        self.servo1, self.servo2 = 90, 90; 
        self.HSlider_Servo1.setValue(self.servo1); 
        self.VSlider_Servo2.setValue(self.servo2)
    
    def on_btn_Cam_Left(self): 
        self.camera_angle = (self.camera_angle - 90) % 360
    
    def on_btn_Cam_Right(self): 
        self.camera_angle = (self.camera_angle + 90) % 360
    
    def on_btn_Cam_Origin(self): 
        self.camera_angle = 0
    
    def Change_Left_Right(self): 
        self.servo1=self.HSlider_Servo1.value(); 
        self.label_Servo1.setText(f"{self.servo1}"); 
        self.send_servo_command()
    
    def Change_Up_Down(self): 
        self.servo2=self.VSlider_Servo2.value();
        self.label_Servo2.setText(f"{self.servo2}");
        self.send_servo_command()
    
    def Fine_Tune_Left_Right(self): 
        self.label_FineServo1.setText(str(self.HSlider_FineServo1.value())); 
        self.send_servo_command()
    
    def Fine_Tune_Up_Down(self): 
        self.label_FineServo2.setText(str(self.HSlider_FineServo2.value())); 
        self.send_servo_command()
    
    def send_servo_command(self):
        if self.commandFlag:
            s1_val = self.servo1 + self.HSlider_FineServo1.value()
            s2_val = self.servo2 + self.HSlider_FineServo2.value()
            self.TCP.sendData(f"{cmd.CMD_SERVO}#0#{180-s1_val}\n")
            self.TCP.sendData(f"{cmd.CMD_SERVO}#1#{s2_val}\n")
    
    def on_btn_Track(self):
        if self.commandFlag:
            self.trackFlag = 1 - self.trackFlag; 
            self.lightFlag = 0; 
            self.Light.setText("Light off"); 
            self.Track.setText("Track on" if self.trackFlag else "Track off"); 
            self.TCP.sendData(f"{cmd.CMD_TRACK}#{self.trackFlag}\n")
    
    def on_btn_Light(self):
        if self.commandFlag:
            self.lightFlag = 1 - self.lightFlag; 
            self.trackFlag = 0; 
            self.Track.setText("Track off");
            self.Light.setText("Light on" if self.lightFlag else "Light off"); 
            self.TCP.sendData(f"{cmd.CMD_LIGHT}#{self.lightFlag}\n")
    
    def WS2812_Text_Change(self):
        try: w = int(self.Color_W.text()) if self.Color_W.text() else 0
        except ValueError: w = 0
        w = min(max(w, 0), 4095)
        for i in range(1, 13): getattr(self, f"L{i}").setChecked(bool(w&(1<<(i-1))))
    
    def WS2812_Calculate(self):
        num = sum(1 << (i-1) for i in range(1, 13) if getattr(self, f"L{i}").isChecked())
        self.ws2812_number = num
        self.Color_W.setText(str(num))
    
    def ALL_Click(self):
        self.W_flag = not self.W_flag
        is_checked = self.W_flag == 0
        for i in range(1, 13): getattr(self, f"L{i}").setChecked(is_checked)
        self.WS2812_Calculate()
    
    def LedChange(self, b):
        if b is self.RGB:
            color = QColorDialog.getColor();
            if color.isValid(): self.Color_R.setText(str(color.red())); self.Color_G.setText(str(color.green())); self.Color_B.setText(str(color.blue()))
        elif b is self.Led_Module and self.commandFlag:
            self.TCP.sendData(f"{cmd.CMD_LED}#{self.Color_W.text()}#{self.Color_R.text()}#{self.Color_G.text()}#{self.Color_B.text()}\n")
        elif b.isCheckable() and self.commandFlag:
            checks, mode_map = ({self.checkBox_Led_Mode1, self.checkBox_Led_Mode2, self.checkBox_Led_Mode3, self.checkBox_Led_Mode4},
                              {self.checkBox_Led_Mode1: "2", self.checkBox_Led_Mode2: "3", self.checkBox_Led_Mode3: "4", self.checkBox_Led_Mode4: "5"})
            if b in checks:
                if b.isChecked(): [chk.setChecked(False) for chk in checks if chk is not b]; self.TCP.sendData(f"{cmd.CMD_LED_MOD}#{mode_map[b]}\n")
                else: self.TCP.sendData(f"{cmd.CMD_LED_MOD}#0\n")
    
    def MatrixChange(self, b):
        if self.commandFlag:
            checks, mode_map = ({self.checkBox_Matrix_Mode1, self.checkBox_Matrix_Mode2, self.checkBox_Matrix_Mode3, self.checkBox_Matrix_Mode4},
                              {self.checkBox_Matrix_Mode1: "1", self.checkBox_Matrix_Mode2: "2", self.checkBox_Matrix_Mode3: "3", self.checkBox_Matrix_Mode4: "6"})
            if b in checks:
                if b.isChecked(): [chk.setChecked(False) for chk in checks if chk is not b]; self.TCP.sendData(f"{cmd.CMD_MATRIX_MOD}#{mode_map[b]}\n")
                else: self.TCP.sendData(f"{cmd.CMD_MATRIX_MOD}#0\n")
    
    def time(self):
        try:
            if not self.TCP.video_Flag:
                q_img = QImage(self.TCP.image.data, self.TCP.image.shape[1], self.TCP.image.shape[0], self.TCP.image.strides[0], QImage.Format_BGR888)
                transform = QTransform().rotate(self.camera_angle)
                rotated_img = q_img.transformed(transform, Qt.SmoothTransformation)
                self.current_video_source_pixmap = QPixmap.fromImage(rotated_img)
                self.update_video_pixmap()
                self.TCP.video_Flag = True
        except Exception as e: 
            self.TCP.video_Flag = True
    
    def closeEvent(self, event):
        self.timer.stop(); self.TCP.StopTcpcClient()
        try:
            if hasattr(self, 'streaming'): stop_thread(self.streaming)
            if hasattr(self, 'power'): stop_thread(self.power)
            if hasattr(self, 'recv'): stop_thread(self.recv)
        except: pass
        event.accept(); os._exit(0)
    
    def Power(self):
        while True:
            try:
                if self.Btn_Connect.text()=="Disconnect": self.TCP.sendData(f"{cmd.CMD_POWER}{self.endChar}")
                time.sleep(3)
            except: break
    
    def recvmassage(self):
            self.TCP.socket1_connect(self.h)
            if not self.TCP.connect_Flag: return
            self.power=Thread(target=self.Power); self.power.start()
            restCmd=""
            while self.TCP.connect_Flag:
                try:
                    Alldata=restCmd+str(self.TCP.recvData())
                    if Alldata=="": break
                    cmdArray=Alldata.split("\n")
                    restCmd = cmdArray.pop() if cmdArray[-1] != "" else ""
                    for oneCmd in cmdArray:
                        Massage=oneCmd.split("#")
                        if cmd.CMD_POWER in Massage and len(Massage) > 1:
                            try:
                                val = float(Massage[1])
                                p = int((val - 7.0) / 1.4 * 100) if val >= 7 else 0
                                power_value = min(p, 100)
                                self.progress_Power.setValue(power_value)
                                self.update_battery_style(power_value)
                            except: pass
                except: break

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = mywindow()
    window.show()
    sys.exit(app.exec_())