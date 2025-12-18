/*
 * SP/04 - Loopback Verify (D8->D0) and print to Terminal
 * Baud: 9600
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
  delay(200);
  Serial.println("Loopback: connect D8 <-> D0");
}

void loop() {
  sendByte('a');             // D8로 송신
  delay(2);

  if (Serial.available()) {  // D0로 수신된 바이트
    char r = (char)Serial.read();
    Serial.print("RX: ");
    Serial.println(r);       // USB TX로 출력
  }
  delay(500);
}
