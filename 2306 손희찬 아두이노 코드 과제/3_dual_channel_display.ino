/*
 * SP/03 - 2CH Display: TX(D8) vs RX(D0)
 * CH1 -> D8, CH2 -> D0
 * Jumper: D8 <-> D0
 * Baud: 9600
 */

#define BAUD 9600
#define BIT_US (1000000UL / BAUD)

#define TX_BIT 0
#define TX_MASK (1 << TX_BIT)
#define TX_HIGH() (PORTB |= TX_MASK)
#define TX_LOW()  (PORTB &= ~TX_MASK)

void sendByte(char c) {
  TX_LOW();  delayMicroseconds(BIT_US);  // Start
  for (int i = 0; i < 8; i++) {
    if ((c >> i) & 1) TX_HIGH(); else TX_LOW();
    delayMicroseconds(BIT_US);
  }
  TX_HIGH(); delayMicroseconds(BIT_US);  // Stop
}

void setup() {
  DDRB |= TX_MASK;
  TX_HIGH();
  Serial.begin(BAUD);   // D0가 RX로 동작(루프백 들어옴)
}

void loop() {
  // 파형 관찰용: 'a'를 주기적으로 보냄
  sendByte('a');
  delay(2000); // Single 트리거 잡기 좋게
}
