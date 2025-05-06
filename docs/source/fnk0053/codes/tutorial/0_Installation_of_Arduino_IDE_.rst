##############################################################################
Chapter 0 Installation of Arduino IDE 
##############################################################################

CH340 (Importance)
***************************

ESP32 uses CH340 to download codes. So before using it, we need to install CH340 driver in our computers.

Windows
===========================

Check whether CH340 has been installed
-------------------------------------------

1.	Connect your computer and ESP32 with a USB cable.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_00.png
    :align: center

2.	Turn to the main interface of your computer, select “This PC” and right-click to select “Manage”.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_01.png
    :align: center

3.	Click “Device Manager”. If your computer has installed CH340, you can see“USB-SERIAL CH340 (COMx)”. And you can click here to move to the next step.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_02.png
    :align: center

Installing CH340
---------------------------------------------

1.	First, download CH340 driver, click http://www.wch-ic.com/search?q=CH340&t=downloads to download the appropriate one based on your operating system.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_03.png
    :align: center

If you would not like to download the installation package, you can open **“Freenove_Ultimate_Starter_Kit_for_ESP32/CH340”**, we have prepared the installation package.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_04.png
    :align: center

2.	Open the folder “Freenove_Ultimate_Starter_Kit_for_ESP32/CH340/Windows/”

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_05.png
    :align: center

3.	Double click “CH341SER.EXE”.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_06.png
    :align: center

4.	Click “INSTALL” and wait for the installation to complete.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_07.png
    :align: center

5.	Install successfully. Close all interfaces.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_08.png
    :align: center

6.	When ESP32 is connected to computer, select “This PC”, right-click to select “Manage” and click “Device Manager” in the newly pop-up dialog box, and you can see the following interface.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_09.png
    :align: center

7.	So far, CH340 has been installed successfully. Close all dialog boxes. 

MAC
=========================================

First, download CH340 driver, click http://www.wch-ic.com/search?q=CH340&t=downloads to download the appropriate one based on your operating system.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_10.png
    :align: center

If you would not like to download the installation package, you can open **“Freenove_Ultimate_Starter_Kit_for_ESP32/CH340”** , we have prepared the installation package.

Second, open the folder **“Freenove_Ultimate_Starter_Kit_for_ESP32/CH340/MAC/”**

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_11.png
    :align: center

Third, click Continue.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_12.png
    :align: center

Fourth, click Install.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_13.png
    :align: center

Then, waiting Finsh.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_14.png
    :align: center

Finally, restart your PC.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_15.png
    :align: center

If you still haven't installed the CH340 by following the steps above, you can view readme.pdf to install it.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_16.png
    :align: center

Arduino Software
******************************************

Arduino Software (IDE) is used to write and upload the code for Arduino Board.

First, install Arduino Software (IDE): visit https://www.arduino.cc, click "Download" to enter the download page.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_17.png
    :align: center

Select and download corresponding installer according to your operating system. If you are a windows user, please select the "Windows Installer" to download to install the driver correctly.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_18.png
    :align: center

After the download completes, run the installer. For Windows users, there may pop up an installation dialog box of driver during the installation process. When it popes up, please allow the installation.

After installation is completed, an Arduino Software shortcut will be generated in the desktop. Run the Arduino Software.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_19.png
    :align: center

The interface of Arduino Software is as follows:

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_20.png
    :align: center

Programs written with Arduino Software (IDE) are called sketches. These sketches are written in the text editor and saved with the file extension.ino. The editor has features for cutting/pasting and searching/replacing text. The message area gives feedback while saving and exporting and also displays errors. The console displays text output by the Arduino Software (IDE), including complete error messages and other information. The bottom right-hand corner of the window displays the configured board and serial port. The toolbar buttons allow you to verify and upload programs, create, open, and save sketches, and open the serial monitor.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_21.png
    :align: center

Additional commands are found within the five menus: File, Edit, Sketch, Tools, Help. The menus are context sensitive, which means only those items relevant to the work currently being carried out are available.

Environment Configuration
***************************************

First, open the software platform arduino, and then click File in Menus and select Preferences.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_22.png
    :align: center

Second, click on the symbol behind "Additional Boards Manager URLs" 

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_23.png
    :align: center

Third, fill in https://espressif.github.io/arduino-esp32/package_esp32_index.json in the new window, click OK, and click OK on the Preferences window again.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_24.png
    :align: center

Fourth, click "BOARDS MANAGER" on the left and type "ESP32" in the search box.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_25.png
    :align: center

Fifth, select Espressif Systems' ESP32 and select version 3.0.x. Click "INSTALL" to install esp32.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_26.png
    :align: center

.. note::
    
    **it takes a while to install the ESP32, make sure your network is stable.**

When finishing installation, click Tools in the Menus again and select Board: "Arduino Uno", and then you can see information of ESP32 Wrover Module. Click " ESP32 Wrover Module" so that the ESP32 programming development environment is configured.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_27.png
    :align: center

Uploading the First Code
*************************************

Here we use “00.0_Servo_90” in “Freenove_4WD_Car_Kit_for_ESP32\\Sketches” as an example.

The servo on the car is controlled by PCA9685. Therefore, it is necessary to add the related libararies to Arduino IDE.

How to Add libraries 
=====================================

We prefer to use method 2 to install libs.

Method 1
----------------------------------------

Open Arduino IDE, click Sketch on Menu bar, move your mouse to Include Library and then click Manage Libraries. 

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_28.png
    :align: center

There is an input field on the right top of the pop-up window. Enter PCA9685 there and click to install the library boxed in the following picture.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_29.png
    :align: center

Wait for the installation to finish.

Method 2
----------------------------------------

Open Arduino IDE, click Sketch on Menu bar, move your mouse to Include Library and then click Add .ZIP library. 

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_30.png
    :align: center

On the pop-up window, select PCA9685.zip of Libraries folder in **“Freenove_4WD_Car_Kit_for_ESP32\\Libraries”**, and then click Open.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_31.png
    :align: center

How to compile and upload code
=================================

Step 1. Connect your computer and ESP32 with a USB cable.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_32.png
    :align: center

Step 2. Open “00.0_Servo_90” folder in **“Freenove_4WD_Car_Kit_for_ESP32\\Sketches”**, double-click “00.0_Servo_90.ino”. The code is to rotate the two servo motors to 90°.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_33.png
    :align: center

Step 3. Select development board.

Click Tolls on Menu bar, move your mouse to Board: “Arduino Uno”, select ESP32 Arduino and then select ESP32 Wrover Module.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_34.png
    :align: center

Step 4. Select serial port.

Cilick Tools on Menu bar, move your mouse to Port and select COMx on your computer. The value of COMx varies in different computers, but it won’t affect the download function of ESP32, as long as you select the correct one.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_35.png
    :align: center

Click “Upload” and the program will be downloaded to ESP32.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_36.png
    :align: center

When you see the following content, it indicates that the program has been uploaded to ESP32.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_37.png
    :align: center

Note: For macOS users, if the uploading fails, please set the baud rate to 115200 before clicking “Upload Using Programmer”.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_38.png
    :align: center

The car needs to be installed with batteries. When installing them, please following the silk print on the board.

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_39.png
    :align: center

Plug the ESP32 to the car shield. Pay attention to the orientation of ESP32. 

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_40.png
    :align: center

Make sure ESP32 is plugged into the shield correctly. Take out two servo motors and plug them into the car shield. Please note the color of the wires Do NOT connect them wrongly. 

.. image:: ../_static/imgs/0_Installation_of_Arduino_IDE_/Chapter00_41.png
    :align: center

Turn ON the switch and the two servos will keep at 90°.