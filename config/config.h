//config file, using seeed studio xiao esp32c3 
#define DEVICE_NAME "Jar Light" //Caps and spaces allowed, dont uses symbols

#define SSID "Verizon_6NSP4Q"
#define PSWD "splash9-fax-con"
#define USE_SOFT_AP true //use soft AP in case of no WiFi
//#define SOFTAP_SSID "Jar Light" //overwrite SSID, will default to device name if not defined
#define SOFTAP_PSWD "deeznutz"

//uncomment to use OTA
#define USEOTA
//webserver port
#define PORT 80
//led config
#define NUMLEDS 68
#define DATAPIN D10
//mpu config
#define MPUINT D3
#define MPUADDR 0x68
//battery config, comment out to not use battery
#define BATTPIN D0