/*
 * SP/01 - Random Char Waveform Observe
 * TX: D8 (PORTB0)
 * Baud: 9600
 * Scope CH1 -> D8
 */

#define BAUD 9600
#define BIT_US (1000000UL / BAUD)

#define TX_BIT 0
#define TX_MASK (1 << TX_BIT)
#define TX_HIGH() (PORTB |= TX_MASK)
#define TX_LOW()  (PORTB &= ~TX_MASK)

void sendByte(char c) {
  // idle high
  TX_HIGH();
  delayMicroseconds(BIT_US);

  // start bit (LOW)
  TX_LOW();
  delayMicroseconds(BIT_US);

  // 8 data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    if ((c >> i) & 1) TX_HIGH();
    else              TX_LOW();
    delayMicroseconds(BIT_US);
  }

  // stop bit (HIGH)
  TX_HIGH();
  delayMicroseconds(BIT_US);
}

void setup() {
  DDRB |= TX_MASK;  // D8 output
  TX_HIGH();
}

void loop() {
  sendByte('a');    // 관찰 문자
  delay(500);       // 화면에서 “덩어리”로 보이게
}
