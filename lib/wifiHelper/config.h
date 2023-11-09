//config file, using seeed studio xiao esp32c3 
#define DEVICE_NAME "jarlight"

#define SSID "Verizon_6NSP4Q"
#define PSWD "splash9-fax-con"
#define USE_SOFT_AP true //use soft AP in case of no WiFi
#define SOFTAP_SSID "Jar Light"
#define SOFTAP_PSWD "deeznutz"

#define PORT 80

#define NUMLEDS 68
#define DATAPIN D10
#define MPUINT (1ULL << 5)
#define BATTPIN D0