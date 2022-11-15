#include "fuzzy_logic.h"

float tempSangatRendah(int temp){
    float sangatRendah;
    if (temp <= 10) {sangatRendah = 1;}
    else if (temp >= 10 && temp <= 15) {sangatRendah = (15 - temp) / 15;}
    else if (temp >= 15) {sangatRendah = 0;}
    return sangatRendah;
}

float tempRendah(int temp){
    float rendah;
    if (temp >= 10 && temp <= 15) {rendah = (temp - 10) / 5;}
    else if (temp >= 15 && temp <= 25) {rendah = 1;}
    else if (temp >= 25 && temp <= 30) {rendah = (30 - temp) / 5;}
    else if (temp >= 30) {rendah = 0;}
    else if (temp <= 10) {rendah = 0;}
    return rendah;
}

float tempTinggi(int temp){
    float tinggi;
    if (temp >= 25 && temp <= 30) {tinggi = (temp - 25) / 5;}
    else if (temp >=30 && temp <= 35) {tinggi = (35 - temp) / 5;}
    else if (temp >= 35) {tinggi = 0;}
    else if (temp <= 25) {tinggi = 0;}
    return tinggi;
}

float tempSangatTinggi(int temp){
    float sangatTinggi;
    if (temp >= 30 && temp <= 35) {sangatTinggi = (temp - 30) / 5;}
    else if (temp >= 35) {sangatTinggi = 1;}
    else if (temp <= 30) {sangatTinggi = 0;}
    return sangatTinggi;
}

float occupancySedikit(int occupancy){
    float sedikit;
    if (occupancy <= 10) {sedikit = 1;}
    else if (occupancy >= 10 && occupancy <= 15) {sedikit = (15 - occupancy) / 5;}
    else if (occupancy >= 15) {sedikit = 0;}
    return sedikit;
}

float occupancySedang(int occupancy){
    float sedang;
    if (occupancy <= 10) {sedang = 0;}
    else if (occupancy >= 10 && occupancy <= 15) {sedang = (occupancy - 10) / 5;}
    else if (occupancy >= 15 && occupancy <= 20) {sedang = 1;}
    else if (occupancy >= 20 && occupancy <= 30) {sedang = (30 - occupancy) / 10;}
    else if (occupancy >= 30) {sedang = 0;}
    return sedang;
}

float occupancyBanyak(int occupancy){
    float banyak;
    if (occupancy <= 20) {banyak = 0;}
    else if (occupancy >= 20 && occupancy <= 30) {banyak = (occupancy - 20) / 10;}
    else if (occupancy >= 30) {banyak = 1;}
    return banyak;
}

float classifyDingin(float alphaPredicate){
    float outDingin = 0;
    if (alphaPredicate == 1) {outDingin = 16;}
    else if (alphaPredicate < 1 && alphaPredicate > 0) {outDingin = 27 - (7 * alphaPredicate);}
    else if (alphaPredicate == 0) {outDingin = 27;}
    return outDingin;
}

float classifySejuk(float alphaPredicate){
    float outSejuk = 0;
    if (alphaPredicate == 1) {outSejuk = 27;}
    else if (alphaPredicate < 1 && alphaPredicate > 0) {outSejuk = (7 * alphaPredicate) + 16;}
    else if (alphaPredicate == 0) {outSejuk = 16;}
    return outSejuk;
}


//// Fuzzifikasi
//void fuzzification() {
//    tempSangatRendah();
//    tempRendah();
//    tempTinggi();
//    tempSangatTinggi();
//    occupancySedikit();
//    occupancySedang();
//    occupancyBanyak();
//}

// Rule
float fuzzy_rule(float sangatRendah, float rendah, float tinggi, float sangatTinggi, float sedikit, float sedang, float banyak){
    const int rows = 4;
    const int cols = 3;
    #define SEDIKIT 0
    #define SEDANG 1
    #define BANYAK 2
    #define SANGAT_RENDAH 0
    #define RENDAH 1
    #define TINGGI 2
    #define SANGAT_TINGGI 3
    #define NORMAL 7
    #define SEJUK 8
    #define DINGIN 9

//    float outTemp;
    // rule base dari tabel 8.3.1 yang di C251
    // langsung dihitung nilai alpha predicatenya
    // float ruleBase[rows][cols] = {
    //     //                    sedikit                         sedang                      banyak
    //     {   min(sedikit, sangatRendah),    min(sedang, sangatRendah),   min(banyak, sangatRendah)  },     // sangat rendah
    //     {         min(sedikit, rendah),          min(sedang, rendah),         min(banyak, rendah)  },     // rendah
    //     {         min(sedikit, tinggi),          min(sedang, tinggi),         min(banyak, tinggi)  },     // tinggi
    //     {   min(sedikit, sangatTinggi),    min(sedang, sangatTinggi),   min(banyak, sangatTinggi)  }      // sangat tinggi
    // };

    float ruleBase[rows][cols] = {
        //                    sedikit                         sedang                      banyak
        {   min(sedikit, sangatRendah),    min(sedang, sangatRendah),   min(banyak, sangatRendah)  },     // sangat rendah
        {         min(sedikit, rendah),          min(sedang, rendah),         min(banyak, rendah)  },     // rendah
        {         min(sedikit, tinggi),          min(sedang, tinggi),         min(banyak, tinggi)  },     // tinggi
        {   min(sedikit, sangatTinggi),    min(sedang, sangatTinggi),   min(banyak, sangatTinggi)  }      // sangat tinggi
    };

    // outrule itu nilai Z nya dari alpha predicate yang sudah dihitung
    float outRule[rows][cols] = {
        { classifySejuk(ruleBase[0][0]), classifySejuk(ruleBase[0][1]), classifySejuk(ruleBase[0][2]) }, // sangat rendah
        { classifySejuk(ruleBase[1][0]), classifySejuk(ruleBase[1][1]), classifyDingin(ruleBase[1][2]) }, // rendah
        { classifySejuk(ruleBase[2][0]), classifyDingin(ruleBase[2][1]), classifyDingin(ruleBase[2][2]) }, // tinggi
        { classifyDingin(ruleBase[3][0]), classifyDingin(ruleBase[3][1]), classifyDingin(ruleBase[3][2]) }  // sangat tinggi
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


float getTemperature(float currentTemp, float currentOccupancy){
    float outTemp;
    float sangatRendah = tempSangatRendah(currentTemp);
    float rendah = tempRendah(currentTemp);
    float tinggi = tempTinggi(currentTemp);
    float sangatTinggi = tempSangatTinggi(currentTemp);
    float sedikit = occupancySedikit(currentOccupancy);
    float sedang = occupancySedang(currentOccupancy);
    float banyak = occupancyBanyak(currentOccupancy);
    outTemp = fuzzy_rule(sangatRendah, rendah, tinggi, sangatTinggi, sedikit, sedang, banyak);
    return outTemp;
}
