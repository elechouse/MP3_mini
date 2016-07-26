#include <SoftwareSerial.h>
#include <MP3Mini.h>

/** define mp3 class */
MP3 mp3;

void setup()
{
/** begin function */
mp3.begin(MP3_SOFTWARE_SERIAL);    // select software serial, change the pins in .cpp file
//  mp3.begin();                       // select hardware serial(or mp3.begin(MP3_HARDWARE_SERIAL);)
Serial.begin(9600);

/**check storage on MP3**/
u8 media = mp3.check_media();
delay(100);  
switch(media) {
	case 00:
		Serial.println("TF card and U disk in");
		mp3.choose_media(0x00); // to choose USB pen, mp3.choose_media(0x01);
		Serial.println("operate on TF card");
		break;
	case 01:
		Serial.println("TF card is found, no USB disk");
		break;
	case 02:
		Serial.println("USB disk is found, no TF card");
		break;
	case 03:
		Serial.println("Found nothing, please insert TF card/USB disk");
		break;
	default:
		Serial.println("unkown error");
   Serial.println(media,HEX);
		break;

}

if( mp3.check_files("yellow")==0x00){
  if(mp3.play("yellow")==0)
  Serial.println("playing yellow");
}
else
Serial.println("not found yellow");

}

void loop()
{
	/** function code here */
}