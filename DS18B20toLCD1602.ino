#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <NonBlockingDallas.h> // https://github.com/Gbertaz/NonBlockingDallas
#include <OneWire.h>

// LCD pin assignment for the Arduino R3
const int pin_RS        = 8;
const int pin_EN        = 9;
const int pin_d4        = 4;
const int pin_d5        = 5;
const int pin_d6        = 6;
const int pin_d7        = 7;
const int ONE_WIRE_BUS  = 3; // Use GPIO 3 for the Maxim DS18B20 temperature sensor signal.
const int TIME_INTERVAL = 1500;
const long pollInterval = 1000;
unsigned long lasPoll   = 0;
float tempF0            = 21.12;
float tempF1            = 21.12;

// Custom LCD characters:
byte degree[] = {
      B01110,
      B01010,
      B01110,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
};
byte Heart[] = {
      B00000,
      B01010,
      B11111,
      B11111,
      B01110,
      B00100,
      B00000,
      B00000 };
byte Bell[] = {
      B00100,
      B01110,
      B01110,
      B01110,
      B11111,
      B00000,
      B00100,
      B00000 };

LiquidCrystal lcd( pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7 );
OneWire oneWire( ONE_WIRE_BUS );
DallasTemperature dallasTemp( &oneWire );
NonBlockingDallas sensorDs18b20( &dallasTemp );


// Invoked at every sensor reading (TIME_INTERVAL milliseconds)
void handleIntervalElapsed( float temperature, bool valid, int deviceIndex )
{
	//	Serial.printf( "Sensor %d changed temperature: %f °F\n", deviceIndex, temperature );
	Serial.print( "Sensor : " );
	Serial.print( deviceIndex );
	Serial.print( " timer temperature " );
	Serial.println( temperature );
	if( deviceIndex == 0 )
		tempF0 = temperature;
	else if( deviceIndex == 1 )
		tempF1 = temperature;
	else
	{
		Serial.print( "Unexpected device index: " );
		Serial.println( deviceIndex );
	}
} // End of the handleIntervalElapsed() function.

//Invoked ONLY when the temperature changes between two sensor readings
void handleTemperatureChange( float temperature, bool valid, int deviceIndex )
{
	//	Serial.printf( "Sensor %d changed temperature: %f °F\n", deviceIndex, temperature );
	Serial.print( "Sensor : " );
	Serial.print( deviceIndex );
	Serial.print( " changed temperature " );
	Serial.println( temperature );
	if( deviceIndex == 0 )
		tempF0 = temperature;
	else if( deviceIndex == 1 )
		tempF1 = temperature;
	else
	{
		Serial.print( "Unexpected device index: " );
		Serial.println( deviceIndex );
	}
} // End of the handleTemperatureChange() function.

void setup()
{
	delay( 1000 );
	Serial.begin( 115200 );
	if( !Serial )
		delay( 1000 );
	Serial.println( "Setup has begun." );

	// Set the number of columns and rows.
	lcd.begin( 16, 2 );
	lcd.clear();

	//	lcd.createChar( 0, customChar );
	lcd.createChar( 0, degree );
	lcd.createChar( 1, Heart );
	lcd.createChar( 2, Bell );
	lcd.setCursor( 15, 1 );
	lcd.write( byte( 0 ) );

	// Move the cursor to the first column of the first row.
	lcd.setCursor( 0, 0 );
	lcd.print( "Temp 1:" );
	// Move the cursor to the first column of the second row.
	lcd.setCursor( 0, 1 );
	lcd.print( "Temp 2:" );

	// Initialize the sensor passing the resolution, unit of measure and reading interval [milliseconds]
	sensorDs18b20.begin( NonBlockingDallas::resolution_12, NonBlockingDallas::unit_F, TIME_INTERVAL );

	// Callbacks
	sensorDs18b20.onIntervalElapsed( handleIntervalElapsed );
	sensorDs18b20.onTemperatureChange( handleTemperatureChange );

	// Call the following function to request a new temperature reading without waiting for TIME_INTERVAL to elapse.
	sensorDs18b20.requestTemperature();
	Serial.println( "Setup has finished." );
} // End of the setup() function.

void loop()
{
	sensorDs18b20.update();
	int analogKeypadReading = analogRead( 0 );

	if( analogKeypadReading < 60 )
	{
		tempF0++;
		//		lcd.print( "Right " );
		Serial.println( "Right" );
	}
	else if( analogKeypadReading < 200 )
	{
		tempF1++;
		//		lcd.print( "Up    " );
		Serial.println( "Up" );
	}
	else if( analogKeypadReading < 400 )
	{
		tempF1--;
		//		lcd.print( "Down  " );
		Serial.println( "Down" );
	}
	else if( analogKeypadReading < 600 )
	{
		tempF0--;
		//		lcd.print( "Left  " );
		Serial.println( "Left" );
	}
	else if( analogKeypadReading < 800 )
	{
		//		lcd.print( "Select" );
		Serial.println( "Select" );
	}

	unsigned long now = millis();
	if( now - lasPoll >= pollInterval )
	{
		// Move the cursor to the 10th column of the first row.
		lcd.setCursor( 9, 0 );
		lcd.print( tempF0 );
		// Move the cursor to the 10th column of the second row.
		lcd.setCursor( 9, 1 );
		lcd.print( tempF1 );
		// Move the cursor to the 15th column of the first row.
		lcd.setCursor( 14, 0 );
		lcd.write( byte( 0 ) );
		// Move the cursor to the 15th column of the second row.
		lcd.setCursor( 14, 1 );
		lcd.write( byte( 0 ) );
		// Move the cursor to the 16th column of the first row.
		lcd.setCursor( 15, 0 );
		lcd.write( "F" );
		// Move the cursor to the 16th column of the second row.
		lcd.setCursor( 15, 1 );
		lcd.write( "F" );
		lasPoll = now;
	}
} // End of the loop() function.
