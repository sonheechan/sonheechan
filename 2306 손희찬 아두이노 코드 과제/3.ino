/*
 * SP/03 - 2CH Compare (Better Sync)
 * CH1 -> D8 (Software TX, Bit-bang)
 * CH2 -> D1 (Hardware TX, Serial)
 * (Optional) Sync pulse on D7 for trigger/reference
 *
 * ⚠️ Serial Monitor 열지 말 것(파형 오염 가능)
 */

#define BAUD_RATE 9600
#define BIT_DELAY_US (1000000UL / BAUD_RATE)

#define TX_BIT 0                 // D8 = PB0
#define TX_MASK (1 << TX_BIT)
#define TX_HIGH() (PORTB |= TX_MASK)
#define TX_LOW()  (PORTB &= ~TX_MASK)

#define SYNC_PIN 7               // 기준점 펄스(옵션)

void sendByteBitBang(char c) {
  // Start bit
  TX_LOW();
  delayMicroseconds(BIT_DELAY_US);

  // Data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    if ((c >> i) & 1) TX_HIGH();
    else              TX_LOW();
    delayMicroseconds(BIT_DELAY_US);
  }

  // Stop bit
  TX_HIGH();
  delayMicroseconds(BIT_DELAY_US);
}

void setup() {
  // D8 output
  DDRB |= TX_MASK;
  TX_HIGH(); // idle high

  pinMode(SYNC_PIN, OUTPUT);
  digitalWrite(SYNC_PIN, LOW);

  // Hardware UART (D1 = TX)
  Serial.begin(BAUD_RATE);

  delay(2000);
}

void loop() {
  // 0) 기준점 펄스 (스코프 트리거/동기화용)
  digitalWrite(SYNC_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SYNC_PIN, LOW);

  // 1) 하드웨어 UART 전송 "준비" (버퍼에 먼저 넣기)
  //    실제 TX 시작은 UART 하드웨어가 진행
  Serial.write('a');

  // 2) 바로 이어서 소프트웨어 UART 시작
  //    (둘의 시작이 최대한 근접하게 됨)
  sendByteBitBang('a');

  // Single 모드 캡처용 대기
  delay(2000);
}
