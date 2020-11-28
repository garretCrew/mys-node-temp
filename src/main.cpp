//#define MY_DEBUG

#define MY_RADIO_RFM69
//#define MY_RFM69_NEW_DRIVER
#define MY_RFM69_FREQUENCY RFM69_433MHZ
#define MY_RFM69_TX_POWER_DBM 10
//#define MY_RFM69_ENABLE_ENCRYPTION
//#define MY_ENCRYPTION_SIMPLE_PASSWD "xxxxx"
#define MY_RFM69_NETWORKID 100
#define MY_PARENT_NODE_ID 254

#define MY_NODE_ID 11
#define TEMP_SENSOR 0

#define ONE_WIRE_BUS 4 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 1

#include <MySensors.h>
#include <DallasTemperature.h>
#include <OneWire.h>

unsigned long SLEEP_TIME = 60000; // Sleep time between reads (in milliseconds)
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 
float lastTemperature;
int numSensors=0;
bool receivedConfig = false;
bool metric = true;
// Initialize temperature message
MyMessage msg(TEMP_SENSOR,V_TEMP);

void before()
{
  // Startup up the OneWire library
  sensors.begin();
}

void setup()  
{ 
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Temperature Sensor", "0.1");

  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();

  // Present all sensors to controller
    present(TEMP_SENSOR, S_TEMP);
}

void loop() {
  sensors.requestTemperatures();
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  sleep(conversionTime);

  float temperature = static_cast<float>(static_cast<int>((getControllerConfig().isMetric?sensors.getTempCByIndex(0):sensors.getTempFByIndex(0)) * 10.)) / 10.;

  if (lastTemperature != temperature && temperature != -127.00 && temperature != 85.00) {
  //if (temperature != -127.00 && temperature != 85.00) {
    send(msg.setSensor(0).set(temperature,1));
    lastTemperature=temperature;
  }
  sleep(SLEEP_TIME);
}
