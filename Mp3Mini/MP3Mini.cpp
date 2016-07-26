/*****************************************************************************/
/**
	@file    	MP3Mini.cpp
	@author  	ELECHOUSE
	@version 	V1.0
	@brief      head file for Mp3mini module
	
	@section  HISTORY
	V1.0	2016-08-02	Initial version.
	
	Copyright (c) 2016 www.elechouse.com  All right reserved.
*/
/*****************************************************************************/
#include "MP3Mini.h"

SoftwareSerial COM_SOFT(7, 8); //change pins for softserial here

MP3::MP3()
{
	serial_type_t st=MP3_HARDWARE_SERIAL;
}

void MP3::begin(serial_type_t st)
{
	delay(3000);
	serial_type = st;
	if(serial_type == MP3_HARDWARE_SERIAL){
		COM.begin(9600);
	}else{
		COM_SOFT.begin(9600);
	}
}

void MP3::send_cmd(u8 *cmd)
{
    u8 i;
    u8 length;
    length = cmd[1]+2;
    if(length>CMD_BUF_LEN){
        return;
    }
    cmd[0] = 0x7E;
    cmd[length-1] = 0xEF;
	cmd[length-2] = 0x00;
	for(i=1; i<length-2; i++){
		cmd[length-2]= (cmd[length-2]+cmd[i])&0xFF;
    }
	
	//clear receiving buffer
	if(serial_type == MP3_HARDWARE_SERIAL){
			COM.flush();
		}else{
			while(COM_SOFT.available())
				COM_SOFT.read();
	}
	
    for(i=0; i<length; i++){
		if(serial_type == MP3_HARDWARE_SERIAL){
			COM.write(cmd[i]);
		}else{
			COM_SOFT.write(cmd[i]);
		}
    }
	delay(100);
}

/** MP3 receive function */
u8 MP3::receive_cmd()
{	
	serial_type_t st = serial_type;
	switch (st){
		case MP3_HARDWARE_SERIAL:
			if (COM.available())
			return COM.read();
			break;
			
		case MP3_SOFTWARE_SERIAL:
			if (COM_SOFT.available())
			return COM_SOFT.read();
			break;
    }
}

void MP3::next()
{
    cmd_buf[1]=0x03;
    cmd_buf[2]=0xAC;
    send_cmd(cmd_buf);
}

void MP3::previous() {
    cmd_buf[1]=0x03;
    cmd_buf[2]=0xAD;
    send_cmd(cmd_buf);
}


void MP3::pause()
{
    cmd_buf[1]=0x03;
    cmd_buf[2]=0xAA;
    send_cmd(cmd_buf);
}

void MP3::stop()
{
    cmd_buf[1]=0x03;
    cmd_buf[2]=0xAB;
    send_cmd(cmd_buf);
}


void MP3::volume(u8 vol)
{
    cmd_buf[1]=0x04;
    cmd_buf[2]=0xAE;
    cmd_buf[3]=vol&0x01F;
    send_cmd(cmd_buf);
}

void MP3::set_mode(u8 mode)
{
    cmd_buf[1]=0x04;
    cmd_buf[2]=0xAF;
    cmd_buf[3]=mode;
    send_cmd(cmd_buf);
}



/****************************************
forward_play(void) is to play forward backward
To send again make it back to playing normally
*****************************************/
void MP3::forward_play(void)
{
    cmd_buf[1]=0x03;
    cmd_buf[2]=0xD0;
    cmd_buf[3]=0xD3;
    send_cmd(cmd_buf);
}


/****************************************
backward_play(void) is to play fast backward
To send again make it back to playing normally
*****************************************/
void MP3::backward_play(void)
{
    cmd_buf[1]=0x03;
    cmd_buf[2]=0xD1;
    cmd_buf[3]=0xD4;
    send_cmd(cmd_buf);
}

/****************************************
choose_media(u8 media) is to choose the media
0x00: SD
0x01: U Disk
If you want to play U disk, you need to run this 
command first.
*****************************************/

void MP3::choose_media(u8 media)
{
    cmd_buf[1]=0x04;
    cmd_buf[2]=0xD2;
    cmd_buf[3]=media;
    send_cmd(cmd_buf);
}


/**************************************************************
file name should be less than 8 bytes.
folder name is 5 bytes.
to play "yellow.mp3", play("yellow").
to play "yellow.mp3" under foler "MUSIC", play("MUSIC/yellow").
***************************************************************/
u8 MP3::play(const char* fileName)
{		
		char file[strlen(fileName)+1];
		strcpy(file, fileName);
		u8 file_len=strlen(fileName);
		
		for(int i=0; i<sizeof(file);i++){
		if (file[i]>0x60&&file[i]<0x7A)
		file[i]=file[i]-0x20;
		}
		
	if((file_len>6)&&(file[5]=='/')){
		cmd_buf[1]=0x02 + file_len;
		cmd_buf[2]=0xA5;
		cmd_buf[3]=file[0];
		cmd_buf[4]=file[1];
		cmd_buf[5]=file[2];
		cmd_buf[6]=file[3];
		cmd_buf[7]=file[4];
		for(int i=6;i<file_len;i++){
			cmd_buf[i+2]=file[i];
		}
	}
	else{
		cmd_buf[1]=0x03 + file_len;
		cmd_buf[2]=0xA3;
		for(int i=0;i<(file_len);i++){
			cmd_buf[i+3]=file[i];
		}
	}
	
	send_cmd(cmd_buf);
	return receive_cmd();
}

/**************************************************************
set recording
mode:
00: MIC recording with gain 10DB (default)
01: External LINE-IN signal input with gain 3DB
02: External Dual-channel AUX (AUX_L and AUX_R) signal input with 
gain 3DB (stereo input)
***************************************************************/
void MP3::recording_scr(u8 mode){
	cmd_buf[1]=0x04;
    cmd_buf[2]=0xD3;
    cmd_buf[3]=mode;
    send_cmd(cmd_buf);
}

/**************************************************************
set recording
rate:
00:  128KPBS£¨default£©
01:  96KPBS
02:  64KPBS
03:  32KPBS
***************************************************************/
void MP3::recording_rate(u8 rate){
	cmd_buf[1]=0x04;
    cmd_buf[2]=0xD4;
    cmd_buf[3]=rate;
    send_cmd(cmd_buf);
}


/**************************************************************
file name should be no more than 6 bytes.
folder name is 5 bytes.
to record "yellow.mp3", record("yellow").
to record "yellow.mp3" under foler "MUSIC", record("MUSIC/yellow").
***************************************************************/
void MP3::record(const char* fileName)
{		
		char file[strlen(fileName)+1];
		strcpy(file, fileName);
		u8 file_len=strlen(fileName);
		
		for(int i=0; i<sizeof(file);i++){
		if (file[i]>0x60&&file[i]<0x7A)
		file[i]=file[i]-0x20;
		}
		//COM.println(file);
		/*
		COM.println(file_len);
		*/
		cmd_buf[2]=0xD7;
	if((file_len>6)&&(file[5]=='/')){
		cmd_buf[1]=0x02 + file_len;
		cmd_buf[3]=file[0];
		cmd_buf[4]=file[1];
		cmd_buf[5]=file[2];
		cmd_buf[6]=file[3];
		cmd_buf[7]=file[4];
		for(int i=6;i<file_len;i++){
			cmd_buf[i+2]=file[i];
		}
	}
	else{
		cmd_buf[1]=0x03 + file_len;
		for(int i=0;i<(file_len);i++){
			cmd_buf[i+3]=file[i];
		}
	}
	
	send_cmd(cmd_buf);
}

/**************************************************************
Stop recording
***************************************************************/
void MP3::record_stop(){
	cmd_buf[1]=0x03;
    cmd_buf[2]=0xD9;
    send_cmd(cmd_buf);
}

/**************************************************************
file name should be no more than 8 bytes.
folder name is 5 bytes.
to delete "yellow.mp3", delete_file("yellow").
to delete "yellow.mp3" under foler "MUSIC", delete_file("MUSIC/yellow").
to delete all the audio files in the storage, delete_file("~all~");
***************************************************************/
void MP3::delete_file(const char* fileName)
{	
	if (strcmp(fileName, "~all~")){
	cmd_buf[1]=0x03;
    cmd_buf[2]=0xDE;
	}
	else{
		char file[strlen(fileName)+1];
		strcpy(file, fileName);
		u8 file_len=strlen(fileName);
		
		for(int i=0; i<sizeof(file);i++){
		if (file[i]>0x60&&file[i]<0x7A)
		file[i]=file[i]-0x20;
		}
		
		cmd_buf[2]=0xD7;
		if((file_len>6)&&(file[5]=='/')){
		cmd_buf[1]=0x02 + file_len;
		cmd_buf[3]=file[0];
		cmd_buf[4]=file[1];
		cmd_buf[5]=file[2];
		cmd_buf[6]=file[3];
		cmd_buf[7]=file[4];
		for(int i=6;i<file_len;i++){
			cmd_buf[i+2]=file[i];
		}
	}
	else{
		cmd_buf[1]=0x03 + file_len;
		for(int i=0;i<(file_len);i++){
			cmd_buf[i+3]=file[i];
		}
	}
}	
	send_cmd(cmd_buf);
}

/**************************************************************
Check current volume
***************************************************************/
u8 MP3::check_volume()
{	cmd_buf[1]=0x03;
    cmd_buf[2]=0xC1;
    send_cmd(cmd_buf);
	
	if (receive_cmd()==0xc1)
	return receive_cmd();
	else
	return 0xFF;
}

/**************************************************************
Check working state
***************************************************************/
u8 MP3::check_working()
{	cmd_buf[1]=0x03;
    cmd_buf[2]=0xC2;
    send_cmd(cmd_buf);
	
	if (receive_cmd()==0xc2)
	return receive_cmd();
	else
	return 0xFF;
}  


/**************************************************************
Check file quantity in storage
To check all the files in the media, check_files("~all~");
To check the files in certain folder such as "Music", check_files("MUSIC");

***************************************************************/
u16 MP3::file_qty(const char* fileName)
{
	u16 result;
	if (strcmp(fileName, "~all~")){
	cmd_buf[1]=0x03;
    cmd_buf[2]=0xC5;
	}
	else{
		cmd_buf[1]=8;
		cmd_buf[2]=0xC6;
		
		char file[strlen(fileName)+1];
		strcpy(file, fileName);
		for(int i=0; i<strlen(fileName);i++){
		if (file[i]>0x60&&file[i]<0x7A)
		file[i]=file[i]-0x20;
		}
		
		for(int i=0;i<5;i++){
			cmd_buf[i+3]=file[i];
		}
	}
	send_cmd(cmd_buf);
	if (receive_cmd()==0xc5)
	{ 
		result = receive_cmd();
		result = (result<<8)+receive_cmd();
		return result;
	}
	else
		return 0xFF;
}


/**************************************************************
Check file currently playing, return file index number
***************************************************************/
u8 MP3::check_playingMusic()
{
	cmd_buf[1]=0x03;
    cmd_buf[2]=0xC9;
    send_cmd(cmd_buf);
	if (receive_cmd()==0xc9)
	return receive_cmd();
	else
	return 0xFF;
} 

/**************************************************************
Check media State
Return:
00: SD card and U disk in 
01: SD card in, U disk out
02: SD card out, U disk in 
03: SD card or U disk out
***************************************************************/
u8 MP3::check_media()
{
	cmd_buf[1]=0x03;
    cmd_buf[2]=0xCA;
    send_cmd(cmd_buf);
	
	if (receive_cmd()==0xCA)
	return receive_cmd();
	else
	return 0xFF;
}

/**************************************************************
Check if file exist
To check "yellow.mp3" in root dictionary, check_exist("yellow");
To check "yellow.mp3" in certain folder such as "Music", check_exist("Music/yellow");

***************************************************************/
u8 MP3::check_files(const char* fileName)
{
	char file[strlen(fileName)+1];
	strcpy(file, fileName);
	u8 file_len=strlen(fileName);
	
	for(int i=0; i<sizeof(file);i++){
		if (file[i]>0x60&&file[i]<0x7A)
		file[i]=file[i]-0x20;
		}
		
	//Serial.println(file);
		
	if((file_len>6)&&(file[5]=='/')){
		cmd_buf[1]=0x02 + file_len;
		cmd_buf[2]=0xCC;
		cmd_buf[3]=file[0];
		cmd_buf[4]=file[1];
		cmd_buf[5]=file[2];
		cmd_buf[6]=file[3];
		cmd_buf[7]=file[4];
		for(int i=6;i<file_len;i++){
			cmd_buf[i+2]=file[i];
		}

	}
	else{
		cmd_buf[1]=0x03 + file_len;
		cmd_buf[2]=0xCB;
		for(int i=0;i<(file_len);i++){
			cmd_buf[i+3]=file[i];
			}
		
		}
	send_cmd(cmd_buf);
	u8 cmd = receive_cmd();
	//Serial.print("check file 1st byte:");
	//Serial.print(cmd,HEX);
	return cmd;
	
}

/**************************************************************
Check free space
Return in Mbyte
***************************************************************/
u16 MP3::check_freeSpace()
{
	u16 result;
	
	cmd_buf[1]=0x03;
    cmd_buf[2]=0xCE;

	send_cmd(cmd_buf);
	if (receive_cmd()==0xCE)
	{ 
		result = receive_cmd();
		result = (result<<8)+receive_cmd();
		return result;
	}
	else
		return 0xFF;
}
