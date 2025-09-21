
## Yêu cầu:
- Cấu hình STM32 là Master SPI.
- Kết nối với một module SPI đơn giản (Cảm biến, Màn hình, SD-Card).
- Gửi một byte dữ liệu và nhận phản hồi, hiển thị dữ liệu nhận được lên terminal.

## 🔌 Kết nối phần cứng (Wiring)

| STM32F103 (SPI1) | ST7735 TFT | Chức năng |
|------------------|-----------|-----------|
| **PA5**          | SCL       | SPI Clock (SCK) |
| **PA7**          | SDA       | SPI Data (MOSI) |
| **PA6** *(tùy chọn)* | *(Không dùng)* | SPI MISO *(Không cần cho LCD)* |
| **PA4**          | CS        | Chip Select |
| **PB0**          | DC (A0)   | Data/Command (0 = lệnh, 1 = dữ liệu) |
| **PB1**          | RESET (RST)| Reset phần cứng màn hình |
| **3.3V**         | VCC       | Nguồn cấp 3.3V |
| **GND**          | GND       | Mass chung |
