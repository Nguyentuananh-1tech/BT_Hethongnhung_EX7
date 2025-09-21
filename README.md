
## Yêu cầu:
- Cấu hình STM32 là Master SPI.
- Kết nối với một module SPI đơn giản (Cảm biến, Màn hình, SD-Card).
- Gửi một byte dữ liệu và nhận phản hồi, hiển thị dữ liệu nhận được lên terminal.
## Kết nối phần cứng :
STM32F103 (SPI1)           ST7735 TFT
+----------------+        +-------------+
| PA5 (SCK)  ----+------->| SCL         |
| PA7 (MOSI) ----+------->| SDA         |
| PA4 (CS)  -----+------->| CS          |
| PB0 (DC)  -----+------->| DC (A0)     |
| PB1 (RST) -----+------->| RESET       |
| 3.3V      -----+------->| VCC         |
| GND       -----+------->| GND         |
+----------------+        +-------------+
