/*
 * SP/06 - Normal Baud Sweep
 * 1200 -> 2400 -> 4800 -> 9600 -> 19200 -> 38400 -> 57600 -> 115200
 */

#define TX_BIT 0
#define TX_MASK (1 << TX_BIT)
#define TX_HIGH() (PORTB |= TX_MASK)
#define TX_LOW()  (PORTB &= ~TX_MASK)

const uint32_t bauds[] = { 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };

void sendByte(char c, uint32_t baud) {
  uint16_t bitUs = (uint16_t)(1000000UL / baud);
  TX_LOW();  delayMicroseconds(bitUs);
  for (int i = 0; i < 8; i++) {
    if ((c >> i) & 1) TX_HIGH(); else TX_LOW();
    delayMicroseconds(bitUs);
  }
  TX_HIGH(); delayMicroseconds(bitUs);
}

bool receiveBytes(char* out, int n, uint16_t timeoutMs) {
  uint32_t t0 = millis();
  int got = 0;
  while (got < n && (millis() - t0) < timeoutMs) {
    if (Serial.available()) out[got++] = (char)Serial.read();
  }
  return got == n;
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
    delay(100);
    while (Serial.available()) Serial.read();

    const char tx[] = { 'a', 'A', 'Z', '\n' };
    for (int i = 0; i < (int)sizeof(tx); i++) sendByte(tx[i], baud);

    char rx[4];
    bool ok = receiveBytes(rx, 4, 200);

    (void)ok;

    delay(1500);
  }
}
