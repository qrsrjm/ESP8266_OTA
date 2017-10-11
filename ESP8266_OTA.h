/****************************************************************
* ESP8266 OTA UPDATE LIBRARY
*
* REQUIRES rBoot
*
* OCTOBER 11 2017
*
* ANKIT BHATNAGAR
* ANKIT.BHATNAGARINDIA@GMAIL.COM
*
* FOR A SUCCESSFULL COMPILATION, NEED TO ADD THE FOLLOWING MODULES TO THE MAKEFILE
*   MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot
*   MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot/appcode
*   MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot/build
*   MODULES		:= $(MODULES) user/libs/ESP8266_OTA/rboot-sample
*
* REFERENCES
* ------------
*		(1) rBoot
*				https://github.com/raburton/rboot
*				https://richard.burtons.org/2015/05/18/rboot-a-new-boot-loader-for-esp8266/
*				https://richard.burtons.org/2015/05/23/rboot-tutorial-for-esp8266-ota-updates/
****************************************************************/

#ifndef _ESP8266_OTA_H_
#define _ESP8266_OTA_H_

#include "osapi.h"
#include "mem.h"
#include "ets_sys.h"
#include "ip_addr.h"
#include "espconn.h"
#include "os_type.h"
#include "rboot-api.h"
#include "rboot-ota.h"


#define ESP8266_OTA_HTTP_HEADER     "Connection: keep-alive\r\n"\
                                    "Cache-Control: no-cache\r\n"\
                                    "User-Agent: rBoot-Sample/1.0\r\n"\
                                    "Accept: */*\r\n\r\n"
#define ESP8266_OTA_HTTP_STRING     "GET %s%s HTTP/1.1\r\nHost: \"%s\"\r\n"

//CUSTOM VARIABLE STRUCTURES/////////////////////////////
//END CUSTOM VARIABLE STRUCTURES/////////////////////////

//FUNCTION PROTOTYPES/////////////////////////////////////
//CONFIGURATION FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_OTA_SetDebug(uint8_t debug_on);
void ICACHE_FLASH_ATTR ESP8266_OTA_Initialize(uint8_t* server_ip,
											    uint16_t server_port,
												char* server_path);
//CONTROL FUNCTIONS
void ICACHE_FLASH_ATTR ESP8266_OTA_Start(void);

//INTERNAL CALLBACK FUNCTIONS
void ICACHE_FLASH_ATTR _esp8266_ota_done_cb(bool result, uint8_t rom_slot);
//END FUNCTION PROTOTYPES/////////////////////////////////
#endif
