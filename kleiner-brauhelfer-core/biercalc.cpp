#include "biercalc.h"
#include <math.h>
#include <limits>

const double BierCalc::dichteAlkohol = 0.7894;
double BierCalc::faktorPlatoToBrix = 1.03;
double BierCalc::KleierGaerungskorrektur = 0.44552;
const double BierCalc::MalzVerdraengung = 0.7;
const double BierCalc::Balling = 2.0665;

double BierCalc::brixToPlato(double brix)
{
    return brix / faktorPlatoToBrix;
}

double BierCalc::platoToBrix(double plato)
{
    return plato * faktorPlatoToBrix;
}

double BierCalc::brixToDichte(double sw, double brix, FormulaBrixToPlato formel)
{
    double b = brixToPlato(brix);
    double tre;
    switch (formel)
    {
    case Terrill:
        // http://seanterrill.com/2011/04/07/refractometer-fg-results/
        return 1 - 0.0044993*sw + 0.0117741*b + 0.000275806*sw*sw - 0.00127169*b*b - 0.00000727999*sw*sw*sw + 0.0000632929*b*b*b;
    case TerrillLinear:
        // http://seanterrill.com/2011/04/07/refractometer-fg-results/
        return 1.0000 - 0.00085683*sw + 0.0034941*b;
    case Standard:
        return 1.001843-0.002318474*sw - 0.000007775*sw*sw - 0.000000034*sw*sw*sw + 0.00574*brix + 0.00003344*brix*brix + 0.000000086*brix*brix*brix;
    case Kleier:
        // http://hobbybrauer.de/modules.php?name=eBoard&file=viewthread&tid=11943&page=2#pid129201
        tre = (Balling * brix - KleierGaerungskorrektur * sw)/(Balling * faktorPlatoToBrix - KleierGaerungskorrektur);
        return platoToDichte(toSRE(sw, tre));
    default:
        return 0.0;
    }
}

double BierCalc::dichteToPlato(double sg)
{
  #if 0
    // deClerk: http://www.realbeer.com/spencer/attenuation.html
    return 668.72 * sg - 463.37 - 205.347 * sg * sg;
  #else
    // http://www.brewersfriend.com/plato-to-sg-conversion-chart/
    return (-1 * 616.868) + (1111.14 * sg) - (630.272 * sg * sg) + (135.997 * sg * sg * sg);
  #endif
}

double BierCalc::platoToDichte(double plato)
{
  #if 0 // Umkehrformel zu dichteToPlato()
    // deClerk: http://www.realbeer.com/spencer/attenuation.html
    double a = 205.347;
    double b = 668.72;
    double c = 463.37 + plato;
    double d = 4 * a * c;
    return (-b + sqrt(b * b - d)) / (2 * a) * -1;
  #elif 0 // Formel vom Internet
    return 261.11 / (261.53 - plato);
  #else
    // http://www.brewersfriend.com/plato-to-sg-conversion-chart/
    return 1 + (plato / (258.6 - ((plato / 258.2) * 227.1)));
  #endif
}

static double interpolate3(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x)
{
    // Lagrange Interpolation k=3 (y=ax^3+bx^2+cx+d)
    double y =
        y1 * (((x-x2)*(x-x3)*(x-x4)) / ((x1-x2)*(x1-x3)*(x1-x4))) +
        y2 * (((x-x1)*(x-x3)*(x-x4)) / ((x2-x1)*(x2-x3)*(x2-x4))) +
        y3 * (((x-x1)*(x-x2)*(x-x4)) / ((x3-x1)*(x3-x2)*(x3-x4))) +
        y4 * (((x-x1)*(x-x2)*(x-x3)) / ((x4-x1)*(x4-x2)*(x4-x3)));
    return y;
}

// interpolates the density values for a given temperature
static void interpolateTemperature(double* result, double temperature)
{
    // Zuckertechniker-Taschenbuch, Albert Bartens Verlage, Berlin, 1966, 7. Auflage
    const double density_tbl[11][9] = {
        /* 0      5     10     15     20     25     30     35     40     g/100g */
        {1.0002,1.0204,1.0418,1.0632,1.0851,1.1088,1.1323,1.1574,1.1840}, /*  0°C, extrapolated, does not make much sense below 4°C */
        {0.9997,1.0196,1.0402,1.0614,1.0835,1.1064,1.1301,1.1547,1.1802}, /* 10°C */
        {0.9982,1.0178,1.0381,1.0591,1.0810,1.1035,1.1270,1.1513,1.1764}, /* 20°C */
        {0.9957,1.0151,1.0353,1.0561,1.0777,1.1000,1.1232,1.1473,1.1723}, /* 30°C */
        {0.9922,1.0116,1.0316,1.0522,1.0737,1.0958,1.1189,1.1428,1.1676}, /* 40°C */
        {0.9881,1.0072,1.0271,1.0477,1.0690,1.0910,1.1140,1.1377,1.1624}, /* 50°C */
        {0.9832,1.0023,1.0221,1.0424,1.0636,1.0856,1.1085,1.1321,1.1568}, /* 60°C */
        {0.9778,0.9968,1.0165,1.0368,1.0579,1.0798,1.1026,1.1262,1.1507}, /* 70°C */
        {0.9718,0.9908,1.0104,1.0306,1.0517,1.0735,1.0963,1.1198,1.1443}, /* 80°C */
        {0.9653,0.9842,1.0038,1.0240,1.0450,1.0669,1.0896,1.1130,1.1375}, /* 90°C */
        /*{0.9591,0.9780,0.9975,1.0176,1.0386,1.0606,1.0832,1.1065,1.1309}     99°C, original data, extrapolated 100°C is used instead */
        {0.9584,0.9773,0.9968,1.0169,1.0379,1.0599,1.0825,1.1058,1.1301}  /*100°C, extrapolated */
    };

    int lowest = (int)floor(temperature / 10) - 1;
    if (lowest < 0)
        lowest = 0;
    if (lowest > 7)
        lowest = 7;
    int temp1 = lowest;
    int temp2 = lowest + 1;
    int temp3 = lowest + 2;
    int temp4 = lowest + 3;
    for (int i=0; i < 9; i++)
        result[i] = interpolate3(temp1*10, density_tbl[temp1][i], temp2*10, density_tbl[temp2][i], temp3*10, density_tbl[temp3][i], temp4*10, density_tbl[temp4][i], temperature);
}

// reads the density values for a given temperature from the data table
// values for non existing temperatures are interpolated
static double densityAtBaseTemp(double base, double plato)
{
    double density_base[9];
    interpolateTemperature(density_base, base);
    int lowest = (int)floor(plato / 5) - 1;
    if (lowest < 0)
        lowest = 0;
    if (lowest > 5)
        lowest = 5;
    int plato1 = lowest;
    int plato2 = lowest + 1;
    int plato3 = lowest + 2;
    int plato4 = lowest + 3;
    return interpolate3(plato1*5, density_base[plato1], plato2*5, density_base[plato2], plato3*5, density_base[plato3], plato4*5, density_base[plato4], plato);
}

double BierCalc::dichteAtTemp(double plato, double T, double Tcalib)
{
    double dAt20 = densityAtBaseTemp(Tcalib, plato);
    double tmp[9];
    interpolateTemperature(tmp, T);

    // find density interval
    int minimalIndex = 0;
    for (int i = 0; i < 9; i++) {
        if (dAt20 > tmp[i]) {
            minimalIndex = i;
        }
    }
    int lowest = minimalIndex - 1;
    if (0 > lowest) {
        lowest = 0;
    }
    //  if (lowest > tmp.length - 4) {
    //    lowest = tmp.length - 4;
    //  }

    int plato1 = lowest;
    int plato2 = lowest + 1;
    int plato3 = lowest + 2;
    int plato4 = lowest + 3;

    return interpolate3(tmp[plato1], plato1*5, tmp[plato2], plato2*5, tmp[plato3], plato3*5, tmp[plato4], plato4*5, dAt20);
}

double BierCalc::toTRE(double sw, double sre)
{
#if 1
    // Balling: http://realbeer.com/spencer/attenuation.html
    return 0.1808 * sw + 0.8192 * sre;
#else
    // http://hobbybrauer.de/modules.php?name=eBoard&file=viewthread&fid=1&tid=11943
    double q = 0.22 + 0.001 * sw;
    return (q * sw + sre)/(1 + q);
#endif
}

double BierCalc::toSRE(double sw, double tre)
{
#if 1
    // Balling: http://realbeer.com/spencer/attenuation.html
    return (tre - 0.1808 * sw) / 0.8192;
#else
    // http://hobbybrauer.de/modules.php?name=eBoard&file=viewthread&fid=1&tid=11943
    double q = 0.22 + 0.001 * sw;
    return tre * (1 + q) - q * sw;
#endif
}

double BierCalc::vergaerungsgrad(double sw, double re)
{
    if (sw <= 0.0)
        return 0.0;
    double res = (1 - re / sw) * 100;
    if (res < 0.0)
        res = 0.0;
    return res;
}

double BierCalc::sreAusVergaerungsgrad(double sw, double vg)
{
    return sw * (100 - vg) / 100;
}

double BierCalc::alkohol(double sw, double sre)
{
    if (sw <= 0.0)
        return 0.0;

    double tre = toTRE(sw, sre);
    double dichte = platoToDichte(tre);

    // Alkohol Gewichtsprozent
    // http://www.realbeer.com/spencer/attenuation.html
    // Balling: 2.0665g Extrakt ergibt 1g Alkohol, 0.9565g CO2, 0.11g Hefe
    double alcGewicht = (sw - tre) / (Balling - 0.010665 * sw);

    // Alkohol Volumenprozent
    double alc = alcGewicht * dichte / dichteAlkohol;
    if (alc < 0.0)
        alc = 0.0;
    return alc;
}

double BierCalc::co2(double p, double T)
{
    return (1.013 + p) * exp(-10.73797+(2617.25/(T+273.15))) * 10;
}

double BierCalc::p(double co2, double T)
{
    return co2 / (exp(-10.73797+(2617.25/(T+273.15))) * 10) - 1.013;
}

double BierCalc::gruenschlauchzeitpunkt(double co2Soll, double sw, double sreSchnellgaerprobe, double T)
{
    double tre = toTRE(sw, sreSchnellgaerprobe);
    double co2Noetig = co2Soll - co2(0.0, T);
    double extraktCO2 = co2Noetig / (zuckerCO2Potential() * 10);
    double res = toSRE(sw, tre + extraktCO2);
    if (res < 0.0)
        res = 0.0;
    return res;
}

double BierCalc::spundungsdruck(double co2Soll, double T)
{
    double res = p(co2Soll, T);
    if (res < 0.0)
        res = 0.0;
    return res;
}

double BierCalc::co2Noetig(double co2Soll, double sw, double sreSchnellgaerprobe, double sreJungbier, double T)
{
    double treSchnellgaerprobe = toTRE(sw, sreSchnellgaerprobe);
    double treJungbier = toTRE(sw, sreJungbier);
    double co2PotJungbier = wuerzeCO2Potential(treJungbier, treSchnellgaerprobe);
    return co2Soll - co2(0.0, T) - co2PotJungbier;
}

double BierCalc::wuerzeCO2Potential(double sw, double sre)
{
    double tre = toTRE(sw, sre);
    return zuckerCO2Potential() * 10 * (sw - tre);
}

double BierCalc::zuckerCO2Potential()
{
    //https://www.maischemalzundmehr.de/index.php?inhaltmitte=toolsspeiserechner
    return 0.468;
}

double BierCalc::speise(double co2Soll, double sw, double sreSchnellgaerprobe, double sreJungbier, double T)
{
    double co2Diff = co2Noetig(co2Soll, sw, sreSchnellgaerprobe, sreJungbier, T);
    double pot = wuerzeCO2Potential(sw, sreSchnellgaerprobe);
    if (pot <= 0)
        return std::numeric_limits<double>::infinity();
    return co2Diff / pot;
}

double BierCalc::zucker(double co2Soll, double sw, double sreSchnellgaerprobe, double sreJungbier, double T)
{
    double co2Diff = co2Noetig(co2Soll, sw, sreSchnellgaerprobe, sreJungbier, T);
    double pot = zuckerCO2Potential();
    if (pot <= 0)
        return std::numeric_limits<double>::infinity();
    return co2Diff / pot;
}

double BierCalc::dichteWasser(double T)
{
    const double a0 = 999.83952;
    const double a1 = 16.952577;
    const double a2 = -0.0079905127;
    const double a3 = -0.000046241757;
    const double a4 = 0.00000010584601;
    const double a5 = -0.00000000028103006;
    const double b = 0.016887236;
    return (a0 + T * a1 + pow(T,2) * a2 + pow(T,3) * a3 + pow(T,4) * a4 + pow(T,5) * a5) / (1 + T * b);
}

double BierCalc::volumenWasser(double T1, double T2, double V1)
{
    double rho1 = dichteWasser(T1);
    double rho2 = dichteWasser(T2);
    return (rho1 * V1) / rho2;
}

double BierCalc::verdampfungsziffer(double V1, double V2, double t)
{
    if (t == 0.0 || V2 == 0.0 || V1 < V2)
        return 0.0;
    return ((V1 - V2) * 100 * 60) / (V2 * t);
}

double BierCalc::verdampfung(double V1, double V2)
{
    if (V1 == 0.0 || V1 < V2)
        return 0.0;
    return ((V1 - V2) * 100) / V1;
}

double BierCalc::sudhausausbeute(double sw, double sw_dichte, double V, double schuettung)
{
    if (schuettung <= 0.0)
        return 0.0;
    return sw * platoToDichte(sw_dichte) * V / schuettung;
}

double BierCalc::schuettung(double sw, double sw_dichte, double V, double sudhausausbeute)
{
    if (sudhausausbeute <= 0.0)
        return 0.0;
    return sw * platoToDichte(sw_dichte) * V / sudhausausbeute;
}

double BierCalc::verschneidung(double swIst, double swSoll, double menge)
{
    if (swIst < swSoll || swSoll == 0.0)
        return 0.0;
    return menge * (swIst / swSoll - 1);
}

double BierCalc::tinseth(double t, double sw)
{
    // https://realbeer.com/hops/
    double bigness  = 1.65 * pow(0.000125, 0.004 * sw);
    double boiltime = (1 - exp(-0.04 * t)) / 4.15;
    return bigness * boiltime;
}

static unsigned int toRgb(unsigned char r, unsigned char g, unsigned char b)
{
    return (0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu);
}

unsigned int BierCalc::ebcToColor(double ebc)
{
    const unsigned char aFarbe[300][3] = {
        {250,250,210},
        {250,250,204},
        {250,250,199},
        {250,250,193},
        {250,250,188},
        {250,250,182},
        {250,250,177},
        {250,250,171},
        {250,250,166},
        {250,250,160},
        {250,250,155},
        {250,250,149},
        {250,250,144},
        {250,250,138},
        {250,250,133},
        {250,250,127},
        {250,250,122},
        {250,250,116},
        {250,250,111},
        {250,250,105},
        {250,250,100},
        {250,250,94},
        {250,250,89},
        {250,250,83},
        {250,250,78},
        {249,250,72},
        {248,249,67},
        {247,248,61},
        {246,247,56},
        {245,246,50},
        {244,245,45},
        {243,244,45},
        {242,242,45},
        {241,240,46},
        {240,238,46},
        {239,236,46},
        {238,234,46},
        {237,232,47},
        {236,230,47},
        {235,228,47},
        {234,226,47},
        {233,224,48},
        {232,222,48},
        {231,220,48},
        {230,218,48},
        {229,216,49},
        {228,214,49},
        {227,212,49},
        {226,210,49},
        {225,208,50},
        {224,206,50},
        {223,204,50},
        {222,202,50},
        {221,200,51},
        {220,198,51},
        {219,196,51},
        {218,194,51},
        {217,192,52},
        {216,190,52},
        {215,188,52},
        {214,186,52},
        {213,184,53},
        {212,182,53},
        {211,180,53},
        {210,178,53},
        {209,176,54},
        {208,174,54},
        {207,172,54},
        {206,170,54},
        {205,168,55},
        {204,166,55},
        {203,164,55},
        {202,162,55},
        {201,160,56},
        {200,158,56},
        {200,156,56},
        {199,154,56},
        {199,152,56},
        {198,150,56},
        {198,148,56},
        {197,146,56},
        {197,144,56},
        {196,142,56},
        {196,141,56},
        {195,140,56},
        {195,139,56},
        {194,139,56},
        {194,138,56},
        {193,137,56},
        {193,136,56},
        {192,136,56},
        {192,135,56},
        {192,134,56},
        {192,133,56},
        {192,133,56},
        {192,132,56},
        {192,131,56},
        {192,130,56},
        {192,130,56},
        {192,129,56},
        {192,128,56},
        {192,127,56},
        {192,127,56},
        {192,126,56},
        {192,125,56},
        {192,124,56},
        {192,124,56},
        {192,123,56},
        {192,122,56},
        {192,121,56},
        {192,121,56},
        {192,120,56},
        {192,119,56},
        {192,118,56},
        {192,118,56},
        {192,117,56},
        {192,116,56},
        {192,115,56},
        {192,115,56},
        {192,114,56},
        {192,113,56},
        {192,112,56},
        {192,112,56},
        {192,111,56},
        {192,110,56},
        {192,109,56},
        {192,109,56},
        {192,108,56},
        {191,107,56},
        {190,106,56},
        {189,106,56},
        {188,105,56},
        {187,104,56},
        {186,103,56},
        {185,103,56},
        {184,102,56},
        {183,101,56},
        {182,100,56},
        {181,100,56},
        {180,99,56},
        {179,98,56},
        {178,97,56},
        {177,97,56},
        {175,96,55},
        {174,95,55},
        {172,94,55},
        {171,94,55},
        {169,93,54},
        {168,92,54},
        {167,91,54},
        {165,91,54},
        {164,90,53},
        {162,89,53},
        {161,88,53},
        {159,88,53},
        {158,87,52},
        {157,86,52},
        {155,85,52},
        {154,85,52},
        {152,84,51},
        {151,83,51},
        {149,82,51},
        {148,82,51},
        {147,81,50},
        {145,80,50},
        {144,79,50},
        {142,78,50},
        {141,77,49},
        {139,76,49},
        {138,75,48},
        {137,75,47},
        {135,74,47},
        {134,73,46},
        {132,72,45},
        {131,72,45},
        {129,71,44},
        {128,70,43},
        {127,69,43},
        {125,69,42},
        {124,68,41},
        {122,67,41},
        {121,66,40},
        {119,66,39},
        {118,65,39},
        {117,64,38},
        {115,63,37},
        {114,63,37},
        {112,62,36},
        {111,61,35},
        {109,60,34},
        {108,60,33},
        {107,59,32},
        {105,58,31},
        {104,57,29},
        {102,57,28},
        {101,56,27},
        {99,55,26},
        {98,54,25},
        {97,54,24},
        {95,53,23},
        {94,52,21},
        {92,51,20},
        {91,51,19},
        {89,50,18},
        {88,49,17},
        {87,48,16},
        {85,48,15},
        {84,47,13},
        {82,46,12},
        {81,45,11},
        {79,45,10},
        {78,44,9},
        {77,43,8},
        {75,42,9},
        {74,42,9},
        {72,41,10},
        {71,40,10},
        {69,39,11},
        {68,39,11},
        {67,38,12},
        {65,37,12},
        {64,36,13},
        {62,36,13},
        {61,35,14},
        {59,34,14},
        {58,33,15},
        {57,33,15},
        {55,32,16},
        {54,31,16},
        {52,30,17},
        {51,30,17},
        {49,29,18},
        {48,28,18},
        {47,27,19},
        {45,27,19},
        {44,26,20},
        {42,25,20},
        {41,24,21},
        {39,24,21},
        {38,23,22},
        {37,22,21},
        {37,22,21},
        {36,22,21},
        {36,21,20},
        {35,21,20},
        {35,21,20},
        {34,20,19},
        {34,20,19},
        {33,20,19},
        {33,19,18},
        {32,19,18},
        {32,19,18},
        {31,18,17},
        {31,18,17},
        {30,18,17},
        {30,17,16},
        {29,17,16},
        {29,17,16},
        {28,16,15},
        {28,16,15},
        {27,16,15},
        {27,15,14},
        {26,15,14},
        {26,15,14},
        {25,14,13},
        {25,14,13},
        {24,14,13},
        {24,13,12},
        {23,13,12},
        {23,13,12},
        {22,12,11},
        {22,12,11},
        {21,12,11},
        {21,11,10},
        {20,11,10},
        {20,11,10},
        {19,10,9},
        {19,10,9},
        {18,10,9},
        {18,9,8},
        {17,9,8},
        {17,9,8},
        {16,8,7},
        {16,8,7},
        {15,8,7},
        {15,7,6},
        {14,7,6},
        {14,7,6},
        {13,6,5},
        {13,6,5},
        {12,6,5},
        {12,5,4},
        {11,5,4},
        {11,5,4},
        {10,4,3},
        {10,4,3},
        {9,4,3},
        {9,3,2},
        {8,3,2},
        {8,3,2}
    };
    if (ebc < 0 || ebc > 2000)
        return 0;
    double srm = ebc / 1.97;
    int index = (int)round(srm * 10);
    if (index > 300)
        index = 300;
    --index;
    if (index >= 0)
        return toRgb(aFarbe[index][0], aFarbe[index][1], aFarbe[index][2]);
    else
        return 0;
}
