#include "fuzzy_logic.h"


float tempSejuk(int temp){
    float sejuk;
    if (temp == 22) {sejuk = 1;}
    else if (temp >= 20 && temp < 22) {sejuk = (temp - 20) / 2.0;}
    else if (temp > 22 && temp <= 24) {sejuk = (24 - temp) / 2.0;}
    else if (temp > 24 || temp <20) {sejuk = 0;}
    return sejuk;
}

float tempNyaman(int temp){
    float nyaman;
    if (temp >= 22 && temp <25) {nyaman = (temp - 22) / 3.0;}
    else if (temp == 25) {nyaman = 1;}
    else if (temp > 25 && temp <= 28) {nyaman = (28 - temp) / 3.0;}
    else if (temp < 22 || temp > 28) {nyaman = 0;}
    return nyaman;
}

float tempHangat(int temp){
    float hangat;
    if (temp >= 26 && temp < 27) {hangat = (temp - 26) / 1.0;}
    else if (temp > 27 && temp <= 31) {hangat = (31 - temp) / 4.0;}
    else if (temp == 27 ) {hangat = 1;}
    else if (temp >31 || temp < 26 ) {hangat = 0;}
    return hangat;
}


float occupancySedikit(int occupancy){
    float sedikit;
    if (occupancy <= 10) {sedikit = 1;}
    else if (occupancy >= 10 && occupancy <= 15) {sedikit = (15 - occupancy) / 5.0;}
    else if (occupancy >= 15) {sedikit = 0;}
    return sedikit;
}

float occupancySedang(int occupancy){
    float sedang;
    if (occupancy <= 10) {sedang = 0;}
    else if (occupancy >= 10 && occupancy <= 15) {sedang = (occupancy - 10) / 5.0;}
    else if (occupancy >= 15 && occupancy <= 20) {sedang = 1;}
    else if (occupancy >= 20 && occupancy <= 30) {sedang = (30 - occupancy) / 10.0;}
    else if (occupancy >= 30) {sedang = 0;}
    return sedang;
}

float occupancyBanyak(int occupancy){
    float banyak;
    if (occupancy <= 20) {banyak = 0;}
    else if (occupancy >= 20 && occupancy <= 30) {banyak = (occupancy - 20) / 10.0;}
    else if (occupancy >= 30) {banyak = 1;}
    return banyak;
}

float classifyDingin(float alphaPredicate){
    float outDingin = 0;
    if (alphaPredicate == 1) {outDingin = 16;}
    else if (alphaPredicate < 1 && alphaPredicate > 0) {outDingin = 30.0 - (14 * alphaPredicate);}
    else if (alphaPredicate == 0) {outDingin = 30;}
    return outDingin;
}

float classifySejuk(float alphaPredicate){
    float outSejuk = 0;
    if (alphaPredicate == 1) {outSejuk = 30;}
    else if (alphaPredicate < 1 && alphaPredicate > 0) {outSejuk = (14 * alphaPredicate) + 16.0;}
    else if (alphaPredicate == 0) {outSejuk = 16;}
    return outSejuk;
}


//// Fuzzifikasi
//void fuzzification() {
//    tempSejuk();
//    tempNyaman();
//    tempHangat();
//    tempSangaihangat();
//    occupancySedikit();
//    occupancySedang();
//    occupancyBanyak();
//}

// Rule
float fuzzy_rule(float sejuk, float nyaman, float hangat,  float sedikit, float sedang, float banyak){
    const int rows = 4;
    const int cols = 3;
    #define SEDIKIT 0
    #define SEDANG 1
    #define BANYAK 2
    #define SEJUK 0
    #define NYAMAN 1
    #define HANGAT 2
    #define NORMAL 7
    #define SEJUK 8
    #define DINGIN 9

//    float outTemp;
    // rule base dari tabel 8.3.1 yang di C251
    // langsung dihitung nilai alpha predicatenya
    // float ruleBase[rows][cols] = {
    //     //                    sedikit                         sedang                      banyak
    //     {   min(sedikit, sejuk),    min(sedang, sejuk),   min(banyak, sejuk)  },     // sangat nyaman
    //     {         min(sedikit, nyaman),          min(sedang, nyaman),         min(banyak, nyaman)  },     // nyaman
    //     {         min(sedikit, hangat),          min(sedang, hangat),         min(banyak, hangat)  },     // hangat
    //     {   min(sedikit, sangathangat),    min(sedang, sangathangat),   min(banyak, sangathangat)  }      // sangat hangat
    // };

    float ruleBase[rows][cols] = {
        //                    sedikit                         sedang                      banyak
        {           min(sedikit, sejuk),            min(sedang, sejuk),          min(banyak, sejuk)  },     // sejuk
        {         min(sedikit, nyaman),          min(sedang, nyaman),         min(banyak, nyaman)  },     // nyaman
        {         min(sedikit, hangat),          min(sedang, hangat),         min(banyak, hangat)  }     // hangat

    };

    // outrule itu nilai Z nya dari alpha predicate yang sudah dihitung
    float outRule[rows][cols] = {
        { classifySejuk(ruleBase[0][0]), classifySejuk(ruleBase[0][1]), classifySejuk(ruleBase[0][2]) }, // sejuk
        { classifySejuk(ruleBase[1][0]), classifySejuk(ruleBase[1][1]), classifyDingin(ruleBase[1][2]) }, // nyaman
        { classifyDingin(ruleBase[2][0]), classifyDingin(ruleBase[2][1]), classifyDingin(ruleBase[2][2]) } // hangat
    };

    // rerata terbobot: (sum(Z * alpha) / sum(alpha))
    float sumZAlpha = 0;
    float sumAlpha = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sumZAlpha += outRule[i][j] * ruleBase[i][j];
            sumAlpha += ruleBase[i][j];
        }
    }
    int rerataTerbobot = floor(sumZAlpha / sumAlpha);
//    outTemp = rerataTerbobot;
    return rerataTerbobot;;
}


int getTemperature(int currentTemp, int currentOccupancy){
    float outTemp_fuzzy;
    float sejuk = tempSejuk(currentTemp);
    float nyaman = tempNyaman(currentTemp);
    float hangat = tempHangat(currentTemp);
    float sedikit = occupancySedikit(currentOccupancy);
    float sedang = occupancySedang(currentOccupancy);
    float banyak = occupancyBanyak(currentOccupancy);
    outTemp_fuzzy = fuzzy_rule(sejuk, nyaman, hangat, sedikit, sedang, banyak);
    int outTemp = ceil(outTemp_fuzzy);
    return outTemp;
}