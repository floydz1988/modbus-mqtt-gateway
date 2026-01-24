#include <ModbusMaster.h>

// ---------- Pin definitions ----------
#define MAX485_DE_RE 4        // DE and RE tied together
#define MODBUS_RX    16       // UART2 RX
#define MODBUS_TX    17       // UART2 TX

// ---------- Modbus ----------
ModbusMaster node;

// ---------- RS-485 direction control ----------
void preTransmission()
{
  digitalWrite(MAX485_DE_RE, HIGH);  // Transmit mode
}

void postTransmission()
{
  
  digitalWrite(MAX485_DE_RE, LOW);   // Receive mode
  delayMicroseconds(200);
  
}

void setup()
{
  // USB serial for logs
  Serial.begin(115200);
  delay(1000);

  Serial.println("SDM120M Modbus RTU test starting...");

  // Direction control pin
  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, LOW);   // Start in receive mode

  // UART2 for Modbus
  Serial2.begin(
    2400,                // baud rate
    SERIAL_8N1,
    MODBUS_RX,
    MODBUS_TX
  );

  // Modbus slave ID = 1
  node.begin(1, Serial2);
  //node.setTimeout(500);  // milliseconds
  delay(1500);
  digitalWrite(MAX485_DE_RE, LOW);   // Start in receive mode


  // Attach direction control callbacks
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Serial.println("Setup complete.");
}

void loop()
{
  uint8_t result;
  float voltage;

  // SDM120M voltage:
  // Input Register 0x0000
  // 2 registers (32-bit float)
  result = node.readInputRegisters(0x0000, 2);

  if (result == node.ku8MBSuccess)
  {
    // SDM120M uses big-endian word order
    uint32_t raw =
      ((uint32_t)node.getResponseBuffer(0) << 16) |
       (uint32_t)node.getResponseBuffer(1);

    memcpy(&voltage, &raw, sizeof(voltage));

    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.println(" V");
  }
  else
  {
    Serial.print("Modbus error: ");
    Serial.println(result);
  }

  delay(1000);
}
