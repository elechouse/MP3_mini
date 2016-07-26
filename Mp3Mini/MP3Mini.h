/*****************************************************************************/
/**
	@file    	MP3Mini.h
	@author  	ELECHOUSE
	@version 	V1.0
	@brief      head file for MP3 Mini Module
	
	@section  HISTORY
	
	V1.0	2016-07-23	Initial version.
	
	Copyright (c) 2012 www.elechouse.com  All right reserved.
*/
/*****************************************************************************/

#ifndef __MP3Mini_H
#define __MP3Mini_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "wiring_private.h"
#include "SoftwareSerial.h"
#ifndef __TYPE_REDEFINE
#define __TYPE_REDEFINE
typedef uint8_t u8;
typedef int8_t  s8;
/****************************************************
 * for older IDE before 1.6, please enable line 34
 * *************************************************/
//typedef uint16_t u16;
typedef int16_t  s16;
typedef uint32_t u32;
typedef int32_t  s32;
#endif

/** Serial port definition */
#define COM         Serial

typedef enum{
	MP3_HARDWARE_SERIAL,
	MP3_SOFTWARE_SERIAL
}serial_type_t;

class MP3{
public:
    /** constructor */
    MP3();
	
    /** begin function, Arduino style */
    void begin(serial_type_t st=MP3_HARDWARE_SERIAL );
	
    /** MP3 shield basic send command function */
    void send_cmd(u8 *cmd);
	
	/** MP3 receive function */
    u8 receive_cmd();
    
	/** play next music */
    void next();
	
    /** play previous music */
    void previous();
	
    /** pause when play, play when pause */
    void pause();
	
    /** stop playing */
    void stop();
	
    /** set volume */
    void volume(u8 vol);
	
    /** set mode, Use SINGLE, REPEAT, CYCLE or RANDOM as parameter */
    void set_mode(u8 mode);
    
/****************************************
forward_play(void) is to play forward backward
To send again make it back to playing normally
*****************************************/
void forward_play();
	
/****************************************
backward_play(void) is to play fast backward
To send again make it back to playing normally
*****************************************/
void backward_play();	

/****************************************
choose_media(u8 media) is to choose the media
0x00: SD
0x01: U Disk
If you want to play U disk, you need to run this 
command first.
*****************************************/
void choose_media(u8 media);

/**************************************************************
file name should be less than 8 bytes.
folder name is 5 bytes.
to play "yellow.mp3", play("yellow").
to play "yellow.mp3" under foler "MUSIC", play("MUSIC/yellow").
Return
00: playing
0E: no file found
***************************************************************/
u8 play(const char* fileName);

/**************************************************************
set recording
mode:
00: MIC recording with gain 10DB (default)
01: External LINE-IN signal input with gain 3DB
02: External Dual-channel AUX (AUX_L and AUX_R) signal input with 
gain 3DB (stereo input)
***************************************************************/
void recording_scr(u8 mode);

/**************************************************************
set recording
rate:
00:  128KPBS£¨default£©
01:  96KPBS
02:  64KPBS
03:  32KPBS
***************************************************************/
void recording_rate(u8 rate);

/**************************************************************
file name should be less than 8 bytes.
folder name is 5 bytes.
to record "yellow.mp3", record("yellow").
to record "yellow.mp3" under foler "MUSIC", record("MUSIC/yellow").
***************************************************************/
void record(const char* fileName);

/**************************************************************
Stop recording
***************************************************************/
void record_stop();

/**************************************************************
Check current volume
***************************************************************/
u8 check_volume();

/**************************************************************
Check working state
***************************************************************/
u8 check_working();    
    
/**************************************************************
Check if file exist
To check "yellow.mp3" in root dictionary, check_exist("yellow");
To check "yellow.mp3" in certain folder such as "Music", check_exist("Music/yellow");
return:
00: yes
01: no

***************************************************************/
u8 check_files(const char* fileName);    


/**************************************************************
Check file quantity in storage
To check all the files in the media, check_files("~all~");
To check the files in certain folder such as "Music", check_files("MUSIC");

***************************************************************/
u16 file_qty(const char* fileName);    

/**************************************************************
Check file currently playing, return file index number
***************************************************************/
u8 check_playingMusic();  

/**************************************************************
Check free space
Return in Mbyte
***************************************************************/
u16 check_freeSpace();  


/**************************************************************
file name should be no more than 8 bytes.
folder name is 5 bytes.
to delete "yellow.mp3", delete_file("yellow").
to delete "yellow.mp3" under foler "MUSIC", delete_file("MUSIC/yellow").
to delete all the audio files in the storage, delete_file("~all~");
***************************************************************/
void delete_file(const char* fileName);


/**************************************************************
Check media State
Return:
00: SD card and U disk in 
01: SD card in, U disk out
02: SD card out, U disk in 
03: SD card or U disk out
***************************************************************/
u8 check_media();
	

	
	
	
private:
    /** command buffer */
    enum {CMD_BUF_LEN = 25};
    u8 cmd_buf[CMD_BUF_LEN];
	serial_type_t serial_type;
};

#endif
