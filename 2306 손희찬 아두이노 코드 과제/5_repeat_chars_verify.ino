/*
 * SP/05 - Repeat Characters Verify
 * Sends: 'a','A','Z'
 * Loopback: D8 <-> D0
 */

#define BAUD 9600
#define BIT_US (1000000UL / BAUD)

#define TX_BIT 0
#define TX_MASK (1 << TX_BIT)
#define TX_HIGH() (PORTB |= TX_MASK)
#define TX_LOW()  (PORTB &= ~TX_MASK)

void sendByte(char c) {
  TX_LOW();  delayMicroseconds(BIT_US);
  for (int i = 0; i < 8; i++) {
    if ((c >> i) & 1) TX_HIGH(); else TX_LOW();
    delayMicroseconds(BIT_US);
  }
  TX_HIGH(); delayMicroseconds(BIT_US);
}

void setup() {
  DDRB |= TX_MASK;
  TX_HIGH();
  Serial.begin(BAUD);
  Serial.println("Repeat test: D8 <-> D0");
}

void loop() {
  const char seq[] = { 'a', 'A', 'Z', '\n' };
  for (int i = 0; i < (int)sizeof(seq); i++) {
    sendByte(seq[i]);
    delay(2);
    if (Serial.available()) Serial.write(Serial.read()); // 받은 걸 그대로 출력
  }
  Serial.println("---");
  delay(800);
}
