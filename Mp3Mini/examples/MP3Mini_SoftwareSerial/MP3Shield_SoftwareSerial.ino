#include <SoftwareSerial.h>
#include <MP3.h>

/** define mp3 class */
MP3 mp3;

void setup()
{
  /** begin function */
  mp3.begin(MP3_HARDWARE_SERIAL);    // select software serial
//  mp3.begin();                       // select hardware serial(or mp3.begin(MP3_HARDWARE_SERIAL);)
  
  /** set volum to the MAX */
  mp3.play("music");
  
  
}

void loop()
{
	/** function code here */
}
