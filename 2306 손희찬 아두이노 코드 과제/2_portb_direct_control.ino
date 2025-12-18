/*
 * SP/02 - PORTB Direct Control (Bit-bang UART)
 * 해식: TX_HIGH/TX_LOW 는 "레지스터의 특정 비트"를 set/clear 해석
 *       D8 핌 전압을 즉시 바꿔줍니다 (digitalWrite보다 오버헤드가 적음)
 */

#define BAUD 9600
#define BIT_US (1000000UL / BAUD)

#define TX_BIT 0
#define TX_MASK (1 << TX_BIT)

// PORTB 레지스터의 bit0을 1로 만들면 => PB0(D8) HIGH
#define TX_HIGH() (PORTB |= TX_MASK)
// PORTB 레지스터의 bit0을 0으로 만들면 => PB0(D8) LOW
#define TX_LOW()  (PORTB &= ~TX_MASK)

void sendByte(char c) {
  TX_LOW();  delayMicroseconds(BIT_US);     // Start bit
  for (int i = 0; i < 8; i++) {             // Data bits
    if ((c >> i) & 1) TX_HIGH(); else TX_LOW();
    delayMicroseconds(BIT_US);
  }
  TX_HIGH(); delayMicroseconds(BIT_US);     // Stop bit
}

void setup() {
  DDRB |= TX_MASK;   // PB0를 출력 모드로
  TX_HIGH();         // idle = HIGH
}

void loop() {
  sendByte('a');
  delay(500);
}
