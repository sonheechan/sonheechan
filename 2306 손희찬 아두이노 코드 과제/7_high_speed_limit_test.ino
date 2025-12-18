/*
 * SP/07 - High Speed Limit Test
 * 230400 -> 460800 -> 921600 -> 1000000 -> 2000000
 */

#define TX_BIT 0
#define TX_MASK (1 << TX_BIT)
#define TX_HIGH() (PORTB |= TX_MASK)
#define TX_LOW()  (PORTB &= ~TX_MASK)

const uint32_t bauds[] = { 230400, 460800, 921600, 1000000, 2000000 };

void sendByte(char c, uint32_t baud) {
  uint16_t bitUs = (uint16_t)(1000000UL / baud); // 고속에선 0~몇 us로 떨어질 수 있어 한계가 드러남
  TX_LOW();  delayMicroseconds(bitUs);
  for (int i = 0; i < 8; i++) {
    if ((c >> i) & 1) TX_HIGH(); else TX_LOW();
    delayMicroseconds(bitUs);
  }
  TX_HIGH(); delayMicroseconds(bitUs);
}

void setup() {
  DDRB |= TX_MASK;
  TX_HIGH();
}

void loop() {
  for (int b = 0; b < (int)(sizeof(bauds)/sizeof(bauds[0])); b++) {
    uint32_t baud = bauds[b];
    Serial.end();
    Serial.begin(baud);
    delay(120);
    while (Serial.available()) Serial.read();

    // 파형 관찰용 패턴
    const char tx[] = { 'U', 'a', 'Z', '\n' }; // 'U'(0x55)는 10101010이라 파형 확인에 좋음
    for (int i = 0; i < (int)sizeof(tx); i++) sendByte(tx[i], baud);

    // 2초 쉬어서 Single capture
    delay(2000);
  }
}
