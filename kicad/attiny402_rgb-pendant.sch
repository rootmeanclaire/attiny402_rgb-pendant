EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "ATTiny402 RGB Pendant"
Date "2021-10-31"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Evan Shimoniak"
$EndDescr
$Comp
L MCU_Microchip_ATtiny:ATtiny402-SS U1
U 1 1 617F0AE6
P 4650 4050
F 0 "U1" H 4121 4096 50  0000 R CNN
F 1 "ATtiny402-SS" H 4121 4005 50  0000 R CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 4650 4050 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny202-402-AVR-MCU-with-Core-Independent-Peripherals_and-picoPower-40001969A.pdf" H 4650 4050 50  0001 C CNN
	1    4650 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 3350 4650 3450
Wire Wire Line
	4650 4650 4650 5200
Text Label 6150 4050 0    50   ~ 0
RED
Wire Wire Line
	4650 5200 6150 5200
Connection ~ 4650 5200
Wire Wire Line
	6350 5200 6150 5200
Connection ~ 6150 5200
Wire Wire Line
	6550 5200 6350 5200
Connection ~ 6350 5200
Text Label 6550 4150 0    50   ~ 0
BLUE
Wire Wire Line
	5250 4150 6550 4150
Wire Wire Line
	6550 4150 6550 4650
Wire Wire Line
	6150 4050 6150 4250
Wire Wire Line
	6350 3850 6350 4650
Wire Wire Line
	5250 3850 6350 3850
Text Label 6350 3850 0    50   ~ 0
GREEN
Wire Wire Line
	5250 4050 6150 4050
$Comp
L Device:R_US R1
U 1 1 618132F8
P 6150 4400
F 0 "R1" H 6082 4354 50  0000 R CNN
F 1 "40" H 6082 4445 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 6190 4390 50  0001 C CNN
F 3 "~" H 6150 4400 50  0001 C CNN
	1    6150 4400
	1    0    0    1   
$EndComp
Wire Wire Line
	6150 4550 6150 4650
$Comp
L Connector:Conn_01x03_Male J1
U 1 1 6181AF97
P 7150 3450
F 0 "J1" V 7212 3262 50  0000 R CNN
F 1 "Conn_01x03_Male" V 7303 3262 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 7150 3450 50  0001 C CNN
F 3 "~" H 7150 3450 50  0001 C CNN
	1    7150 3450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4650 3350 6950 3350
Connection ~ 4650 3350
Wire Wire Line
	6550 5200 6850 5200
Wire Wire Line
	6850 5200 6850 3550
Wire Wire Line
	6850 3550 6950 3550
Connection ~ 6550 5200
Wire Wire Line
	5250 3750 6050 3750
Wire Wire Line
	6050 3450 6950 3450
Wire Wire Line
	6050 3750 6050 3450
Text Label 6050 3650 2    50   ~ 0
UPDI
$Comp
L power:+3V0 #PWR0101
U 1 1 618316BA
P 4650 3350
F 0 "#PWR0101" H 4650 3200 50  0001 C CNN
F 1 "+3V0" H 4665 3523 50  0000 C CNN
F 2 "" H 4650 3350 50  0001 C CNN
F 3 "" H 4650 3350 50  0001 C CNN
	1    4650 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 61832473
P 4650 5200
F 0 "#PWR0102" H 4650 4950 50  0001 C CNN
F 1 "GND" H 4655 5027 50  0000 C CNN
F 2 "" H 4650 5200 50  0001 C CNN
F 3 "" H 4650 5200 50  0001 C CNN
	1    4650 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 5200 4650 5200
Wire Wire Line
	3400 3350 4650 3350
Wire Wire Line
	3400 5200 3400 4400
Wire Wire Line
	3400 4100 3400 3350
$Comp
L Device:Battery_Cell BT1
U 1 1 617F2AD4
P 3400 4300
F 0 "BT1" H 3282 4396 50  0000 R CNN
F 1 "3V" H 3282 4305 50  0000 R CNN
F 2 "Connector_Wire:SolderWire-0.1sqmm_1x02_P3.6mm_D0.4mm_OD1mm" V 3400 4360 50  0001 C CNN
F 3 "~" V 3400 4360 50  0001 C CNN
	1    3400 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 5050 6550 5200
Wire Wire Line
	6350 5050 6350 5200
Wire Wire Line
	6150 5200 6150 5050
$Comp
L Project:LED_RGB D1
U 1 1 61864BD2
P 6350 4850
F 0 "D1" V 6396 5210 50  0000 L CNN
F 1 "LED_RGB" V 6305 5210 50  0000 L CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 6350 4800 50  0001 C CNN
F 3 "https://www.amazon.com/gp/product/B01CUGABPU/" H 6350 4800 50  0001 C CNN
	1    6350 4850
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
