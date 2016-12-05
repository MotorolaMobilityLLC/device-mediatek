#include <stdio.h>
#include <cutils/properties.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "cutils/log.h"
#include "CFG_file_lid.h"
#include "CFG_Wifi_File.h"
#include "libnvram.h"
#include "Custom_NvRam_LID.h"

#define DEBUG_FUNC
#undef LOG_TAG
#define LOG_TAG "OVERRIDE_NVRAM"

#define rTxPwr_Length 40
#define aucEFUSE_Length 144
#define bandedge_Length 4
#define countryCode_Length 2

static char FCC_rTxPwr[rTxPwr_Length] = {
    /*{*/ 0x24, 0x24, 0x00, 0x00, /*cTxPwr2G4Cck*/ /*cTxPwr2G4Dsss*/
    0x20, 0x20, 0x20, 0x00, 0x20, 0x20, /*cTxPwr2G4OFDM*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /*cTxPwr2G4HT20*/
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, /*cTxPwr2G4HT40*/
    0x1A, 0x1A, 0x1A, 0x00, 0x1A, 0x1A, /*cTxPwr5GOFDM*/
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, /*cTxPwr5GHT20*/
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C /*}*/, /*cTxPwr5GHT40*/ /* TX_PWR_PARAM_T */
};

static char FCC_aucEFUSE[aucEFUSE_Length] = {
    /*{*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x18, 0x00, 0x00, 0x00,// 5G band edge limit
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x1C,// 11AC txpower
    0x1C, 0x1C, 0x1C, 0x00, 0xFe, 0xFc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 11AC txpower
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*}*/, /* aucEFUSE */
};

static char FCC_24g_bandedge[bandedge_Length] = {
    0x01, 0x22, 0x1C, 0x1C,
};

static char *FCC_Country[] = {
    "TW", "CL"
};

static int override_nvram(char *buffer, int write_Length, int offset)
{
    int file_lid = AP_CFG_RDEB_FILE_WIFI_LID;
    WIFI_CFG_PARAM_STRUCT wifi_param;
    int rec_size;
    int rec_num;
    F_ID fd_r;
    F_ID fd_w;
    int i;

    fd_r = NVM_GetFileDesc(file_lid, &rec_size, &rec_num, ISREAD);
    if (fd_r.iFileDesc < 0) {
        ALOGE("nvram open for read = %d", fd_r.iFileDesc);
        return -1;
    } else {
        memset(&wifi_param, 0, rec_num*rec_size);
        read(fd_r.iFileDesc, &wifi_param, rec_num*rec_size);
#ifdef DEBUG_FUNC
        ALOGD("rec_size=%d, rec_num=%d", rec_size, rec_num);
        for (i = offset; i < offset + write_Length; i++)
            ALOGD("[%02x]", *((char *)&wifi_param + i));
#endif
        NVM_CloseFileDesc(fd_r);
        fd_w = NVM_GetFileDesc(file_lid, &rec_size, &rec_num, ISWRITE);
        if (fd_w.iFileDesc < 0) {
            ALOGE("nvram open for write = %d", fd_w.iFileDesc);
            return -1;
        } else {
            // modify
            memcpy((char *)&wifi_param + offset, buffer, write_Length);
#ifdef DEBUG_FUNC
            ALOGD("check nvram after modify");
            for (i = offset; i < offset + write_Length; i++)
                ALOGD("[%02x]", *((char *)&wifi_param + i));
#endif
            write(fd_w.iFileDesc, &wifi_param, rec_num*rec_size);
            NVM_CloseFileDesc(fd_w);
        }
        return 0;
    }
}

static int override_countryCode(char *countryCode)
{
	// write country code area
	override_nvram(countryCode, countryCode_Length, 10);
	return 0;
}

static int override_rTxPwr(char *rTxPwr)
{
	// write rTxPwr area
	override_nvram(rTxPwr, rTxPwr_Length, 12);
	return 0;
}

static int override_aucEFUSE(char *aucEFUSE)
{
	// write aucEFUSE area
	override_nvram(aucEFUSE, aucEFUSE_Length, 52);
	return 0;
}

static int override_24gBandedge(char *bandedge_value)
{
	// write 2.4g band edge area
	override_nvram(bandedge_value, bandedge_Length, 198);
	return 0;
}

static int Is_FCC(char *country)
{
    unsigned int i;
    for (i = 0; i < (sizeof(FCC_Country)/sizeof(char *)); i++) {
        if (strncasecmp(country, FCC_Country[i], 2) == 0)
            return 1;
    }
    return 0;
}

int main(void)
{
#if 0
    char country[PROPERTY_VALUE_MAX] = {0};
    property_get("ro.lenovo.easyimage.code", country, NULL);
    usleep(1000000);
    if (strcmp(country, "") != 0) {
        country[0] = toupper(country[0]);
        country[1] = toupper(country[1]);
        override_countryCode(country);
    }
    if (Is_FCC(country)) {
        ALOGE("Override wifi dac value for FCC after 300ms");
        if (override_rTxPwr(FCC_rTxPwr) < 0) {
            ALOGE("Override wifi dac value rTxPwr area failed");
            return -1;
        }
        if (override_aucEFUSE(FCC_aucEFUSE) < 0) {
            ALOGE("Override wifi dac value aucEFUSE area failed");
            return -1;
        }
        if (override_24gBandedge(FCC_24g_bandedge) < 0) {
            ALOGE("Override wifi dac value 2.4g bandedge area failed");
            return -1;
        }
    }
#endif
    return 0;
}


