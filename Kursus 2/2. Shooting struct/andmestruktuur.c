//tunninäide

typedef struct date{
    int year;
    int month;
    int day;
}date_t

typedef struct performance{
    int core_clock;
    int mem_clock;
    int pwr_limit;
    int fan_spd;
    float voltage_lock; // võib tühi olla v 0
    float total_revenue; // arvutatav / läbi API kättesaadav
    float operating_cost; // arvutatav
    //pidevalt muutuvad andmed, võivad ka keskmised olla üle 24h
    float hashrate;
    float power;
    float efficiency;
    float core_temp;
    float mem_temp; // juhul kui on sensor
}cardPerf_t

typedef struct cryptominer{
    char make[10]; // AMD / Nvidia, Antminer
    char AIB[15]; // ASUS, MSI, FE jne. võib tühi olla kui ASIC nt
    date_t release_date;
    date_t warranty_til;
    date_t ROI; // spekuleeritav / arvutatav
    date_t last_maintenace;
    char product_line[7]; // nt GTX, RTX, Radeon, Vega jne
    char model_nr[7]; // 1060, 3090, 6900xt, 3080ti
    int LHR; // 0/1 n/y
    int vram;
    int pwr_plug; // 6, 8, 12 pin
    int pwr_plug_count;
    float original price;
    char coin[10];
    cardPerf_t performance;
    int rig_location; // mis masinas asub
    int slotnr;
}rigEntry_t

/* Rakenduse eesmärk võiks olla info võrdlemine ja hoidmine enamjaolt. Säästlikumate või lihtsalt parimate kaartide leidmine/sorteerimine, ROI arvutamine, saab ka leida asukoha. Võib ka seada hoiatusi ebasoodsate temperatuuride korral.*/