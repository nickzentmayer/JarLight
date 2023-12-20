//config file, using seeed studio xiao esp32c3 
#define DEVICE_NAME "Christmas Lights" //Caps and spaces allowed, dont uses symbols

//enable serial monitor (disable for production, espeacially esp's with USB (S3, C3, S2, etc.))
#define DEBUG

#define SSID "Barnegat"
#define PSWD "Mustang1967"
#define USE_SOFT_AP true //use soft AP in case of no WiFi
//#define SOFTAP_SSID "Jar Light" //overwrite SSID, will default to device name if not defined
#define SOFTAP_PSWD "deeznutz"

//uncomment to use OTA
#define USEOTA
//local webserver port
#define PORT 80
//probably unessicary and pretty insecure but its here :))))
#define USEUPNP
#define UPNP_PORT 6996 //port for upnp to forward and websever to be on, will NOT overwrite local port
#define LEASE_DURATION 604800
//setting for dynamic dns (for use with upnp)
#ifdef USEUPNP
#define USE_DDNS
#define DDNS_TOKEN "c2bd249d-7ca4-4107-a4b3-925f705c12ac"
#define DDNS_SERVICE "duckdns"
#endif
//led config
#define NUMLEDS 150
#define DATAPIN 13
#define PIXELTYPE NeoRgbFeature
#define PIXELSPEED Neo800KbpsMethod
//mpu config
//#define USEMPU //comment out if not connected to mpu6500
#define MPUINT D3
#define MPUADDR 0x68
#define MPUTHRESHOLD 125
//battery config, comment out to not use battery
//#define BATTPIN D0