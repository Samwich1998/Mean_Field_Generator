#include <typeinfo>
#include "spec_info.h"
#include "echelon.cpp"
#include "TSpline.h"
#include "TH2D.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

// k = momentum; es = energy (for the corresponding spectral value you want to generate)

spec_info::spec_info(){}
spec_info::~spec_info(){} 
double spec_info::spec_find(double k_test, double es_test, int code)
{
  
// If you want to give your own random vairables (depends on time of run, not random in loop)
/** 
// Ask user if they want to find a random value spectral value or pick one
  std::string want_rand;
  std::cout << "Type 'Y' if you want to output a random spectral function value; else type anything: \n";
  std::getline (std::cin,want_rand);
  
// Grab user inputs (either random or given) ] functions return scaled value as it rounds to make it an integer
  k_test = k_input(k_test, want_rand)/(long double)10000;
  es_test = es_input(es_test, want_rand)/(long double)100;
//**/


// If inputs are out of bounds, spec_fun = 0. Energy and momentum are in units inverse fermi
   if (es_test > 998.5 or es_test < 1.5)
    {
      //std::cout << "Out of Bounds Momentum, will return spec = 0"  << "\n";
      return 0;
    }
   if (k_test > 9.975 or k_test < 0.075)
    {
      //      std::cout << "Out of Bounds Energy, will return spec = 0" << "\n";
      return 0;
    }

// Round the inputs to match the given data points significant digits
  double k_test_r = ((int)(k_test * 1000 + .5) / 1000.0);  // stop at third decimal, later edited to exactly match a momentum value
  double es_test_r = ((int)(es_test * 10 + .5) / 10.0);    // stop at first decimal, later edited to exactly match an energy value

// Round the inputs to match a given data point exactly (scale because function returns integer)
// NOTE: for findClosest to work, values must be in increasing order.
    int arr_k[] = { 25,75,125,175,225,275,325,375,425,475,525,575,625,675,725,775,825,875,925,975,1025,1075,1125,1175,1225,1275,1325,1375,1425,1475,1525,1575,1625,1675,1725,1775,1825,1875,1925,1975,2025,2075,2125,2175,2225,2275,2325,2375,2425,2475,2525,2575,2625,2675,2725,2775,2825,2875,2925,2975,3025,3075,3125,3175,3225,3275,3325,3375,3425,3475,3525,3575,3625,3675,3725,3775,3825,3875,3925,3975,4025,4075,4125,4175,4225,4275,4325,4375,4425,4475,4525,4575,4625,4675,4725,4775,4825,4875,4925,4975,5025,5075,5125,5175,5225,5275,5325,5375,5425,5475,5525,5575,5625,5675,5725,5775,5825,5875,5925,5975,6025,6075,6125,6175,6225,6275,6325,6375,6425,6475,6525,6575,6625,6675,6725,6775,6825,6875,6925,6975,7025,7075,7125,7175,7225,7275,7325,7375,7425,7475,7525,7575,7625,7675,7725,7775,7825,7875,7925,7975,8025,8075,8125,8175,8225,8275,8325,8375,8425,8475,8525,8575,8625,8675,8725,8775,8825,8875,8925,8975,9025,9075,9125,9175,9225,9275,9325,9375,9425,9475,9525,9575,9625,9675,9725,9775,9825,9875,9925,9975 }; 
    int n_k = sizeof(arr_k) / sizeof(arr_k[0]); 
    int target_k = k_test_r*1000; 
    k_test_r = (findClosest(arr_k, n_k, target_k)/(long double)1000);

    int arr_es[] = { 5,15,25,35,45,55,65,75,85,95,105,115,125,135,145,155,165,175,185,195,205,215,225,235,245,255,265,275,285,295,305,315,325,335,345,355,365,375,385,395,405,415,425,435,445,455,465,475,485,495,505,515,525,535,545,555,565,575,585,595,605,615,625,635,645,655,665,675,685,695,705,715,725,735,745,755,765,775,785,795,805,815,825,835,845,855,865,875,885,895,905,915,925,935,945,955,965,975,985,995,1005,1015,1025,1035,1045,1055,1065,1075,1085,1095,1105,1115,1125,1135,1145,1155,1165,1175,1185,1195,1205,1215,1225,1235,1245,1255,1265,1275,1285,1295,1305,1315,1325,1335,1345,1355,1365,1375,1385,1395,1405,1415,1425,1435,1445,1455,1465,1475,1485,1495,1505,1515,1525,1535,1545,1555,1565,1575,1585,1595,1605,1615,1625,1635,1645,1655,1665,1675,1685,1695,1705,1715,1725,1735,1745,1755,1765,1775,1785,1795,1805,1815,1825,1835,1845,1855,1865,1875,1885,1895,1905,1915,1925,1935,1945,1955,1965,1975,1985,1995,2005,2015,2025,2035,2045,2055,2065,2075,2085,2095,2105,2115,2125,2135,2145,2155,2165,2175,2185,2195,2205,2215,2225,2235,2245,2255,2265,2275,2285,2295,2305,2315,2325,2335,2345,2355,2365,2375,2385,2395,2405,2415,2425,2435,2445,2455,2465,2475,2485,2495,2505,2515,2525,2535,2545,2555,2565,2575,2585,2595,2605,2615,2625,2635,2645,2655,2665,2675,2685,2695,2705,2715,2725,2735,2745,2755,2765,2775,2785,2795,2805,2815,2825,2835,2845,2855,2865,2875,2885,2895,2905,2915,2925,2935,2945,2955,2965,2975,2985,2995,3005,3015,3025,3035,3045,3055,3065,3075,3085,3095,3105,3115,3125,3135,3145,3155,3165,3175,3185,3195,3205,3215,3225,3235,3245,3255,3265,3275,3285,3295,3305,3315,3325,3335,3345,3355,3365,3375,3385,3395,3405,3415,3425,3435,3445,3455,3465,3475,3485,3495,3505,3515,3525,3535,3545,3555,3565,3575,3585,3595,3605,3615,3625,3635,3645,3655,3665,3675,3685,3695,3705,3715,3725,3735,3745,3755,3765,3775,3785,3795,3805,3815,3825,3835,3845,3855,3865,3875,3885,3895,3905,3915,3925,3935,3945,3955,3965,3975,3985,3995,4005,4015,4025,4035,4045,4055,4065,4075,4085,4095,4105,4115,4125,4135,4145,4155,4165,4175,4185,4195,4205,4215,4225,4235,4245,4255,4265,4275,4285,4295,4305,4315,4325,4335,4345,4355,4365,4375,4385,4395,4405,4415,4425,4435,4445,4455,4465,4475,4485,4495,4505,4515,4525,4535,4545,4555,4565,4575,4585,4595,4605,4615,4625,4635,4645,4655,4665,4675,4685,4695,4705,4715,4725,4735,4745,4755,4765,4775,4785,4795,4805,4815,4825,4835,4845,4855,4865,4875,4885,4895,4905,4915,4925,4935,4945,4955,4965,4975,4985,4995,5005,5015,5025,5035,5045,5055,5065,5075,5085,5095,5105,5115,5125,5135,5145,5155,5165,5175,5185,5195,5205,5215,5225,5235,5245,5255,5265,5275,5285,5295,5305,5315,5325,5335,5345,5355,5365,5375,5385,5395,5405,5415,5425,5435,5445,5455,5465,5475,5485,5495,5505,5515,5525,5535,5545,5555,5565,5575,5585,5595,5605,5615,5625,5635,5645,5655,5665,5675,5685,5695,5705,5715,5725,5735,5745,5755,5765,5775,5785,5795,5805,5815,5825,5835,5845,5855,5865,5875,5885,5895,5905,5915,5925,5935,5945,5955,5965,5975,5985,5995,6005,6015,6025,6035,6045,6055,6065,6075,6085,6095,6105,6115,6125,6135,6145,6155,6165,6175,6185,6195,6205,6215,6225,6235,6245,6255,6265,6275,6285,6295,6305,6315,6325,6335,6345,6355,6365,6375,6385,6395,6405,6415,6425,6435,6445,6455,6465,6475,6485,6495,6505,6515,6525,6535,6545,6555,6565,6575,6585,6595,6605,6615,6625,6635,6645,6655,6665,6675,6685,6695,6705,6715,6725,6735,6745,6755,6765,6775,6785,6795,6805,6815,6825,6835,6845,6855,6865,6875,6885,6895,6905,6915,6925,6935,6945,6955,6965,6975,6985,6995,7005,7015,7025,7035,7045,7055,7065,7075,7085,7095,7105,7115,7125,7135,7145,7155,7165,7175,7185,7195,7205,7215,7225,7235,7245,7255,7265,7275,7285,7295,7305,7315,7325,7335,7345,7355,7365,7375,7385,7395,7405,7415,7425,7435,7445,7455,7465,7475,7485,7495,7505,7515,7525,7535,7545,7555,7565,7575,7585,7595,7605,7615,7625,7635,7645,7655,7665,7675,7685,7695,7705,7715,7725,7735,7745,7755,7765,7775,7785,7795,7805,7815,7825,7835,7845,7855,7865,7875,7885,7895,7905,7915,7925,7935,7945,7955,7965,7975,7985,7995,8005,8015,8025,8035,8045,8055,8065,8075,8085,8095,8105,8115,8125,8135,8145,8155,8165,8175,8185,8195,8205,8215,8225,8235,8245,8255,8265,8275,8285,8295,8305,8315,8325,8335,8345,8355,8365,8375,8385,8395,8405,8415,8425,8435,8445,8455,8465,8475,8485,8495,8505,8515,8525,8535,8545,8555,8565,8575,8585,8595,8605,8615,8625,8635,8645,8655,8665,8675,8685,8695,8705,8715,8725,8735,8745,8755,8765,8775,8785,8795,8805,8815,8825,8835,8845,8855,8865,8875,8885,8895,8905,8915,8925,8935,8945,8955,8965,8975,8985,8995,9005,9015,9025,9035,9045,9055,9065,9075,9085,9095,9105,9115,9125,9135,9145,9155,9165,9175,9185,9195,9205,9215,9225,9235,9245,9255,9265,9275,9285,9295,9305,9315,9325,9335,9345,9355,9365,9375,9385,9395,9405,9415,9425,9435,9445,9455,9465,9475,9485,9495,9505,9515,9525,9535,9545,9555,9565,9575,9585,9595,9605,9615,9625,9635,9645,9655,9665,9675,9685,9695,9705,9715,9725,9735,9745,9755,9765,9775,9785,9795,9805,9815,9825,9835,9845,9855,9865,9875,9885,9895,9905,9915,9925,9935,9945,9955,9965,9975,9985,9995 }; 
    int n_es = sizeof(arr_es) / sizeof(arr_es[0]); 
    int target_es = es_test_r*10; 
    es_test_r = (findClosest(arr_es, n_es, target_es)/(long double)10);

    
// Parse the CSV of the experimental proton or neutron spectral function data
    std::ifstream  data(CSV_file);
// check if file opens
          if ( ! data.is_open() ) {                 
      cout <<" Failed to open" << endl;
      }      
    std::string line;
    std::vector<std::vector<std::string> > parsedCsv;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(cell);
        }
        parsedCsv.push_back(parsedRow);
	}

    spec_info Spec_CSV;
  
// Find CSV index for k_test and es_test value. (first possible value is [0][0] for [k][es]
// Value corresponds to point 6 (by the math I was using)
    int k_test_index = ((k_test_r/0.05)); // really should be ((k_test_r/0.05) - 0.5) but C++ always rounds down so int(1.5)=1
    int es_test_index = (es_test_r-0.5);   
    
// Initializing the spectral function values from CSV; Convert str char type from CSV to long double type
    long double z2 = Spec_CSV.CSV_array(k_test_index-1, es_test_index, CSV_file);
    long double z3 = Spec_CSV.CSV_array(k_test_index-1, es_test_index+1, CSV_file);   
    long double z5 = Spec_CSV.CSV_array(k_test_index, es_test_index-1, CSV_file);   
    long double z6 = Spec_CSV.CSV_array(k_test_index, es_test_index, CSV_file);   
    long double z7 = Spec_CSV.CSV_array(k_test_index, es_test_index+1, CSV_file);
    long double z8 = Spec_CSV.CSV_array(k_test_index, es_test_index+2, CSV_file); 
    long double z9 = Spec_CSV.CSV_array(k_test_index+1, es_test_index-1, CSV_file);
    long double z10 = Spec_CSV.CSV_array(k_test_index+1, es_test_index, CSV_file);
    long double z11 = Spec_CSV.CSV_array(k_test_index+1, es_test_index+1, CSV_file);
    long double z12 = Spec_CSV.CSV_array(k_test_index+1, es_test_index+2, CSV_file); 
    long double z14 = Spec_CSV.CSV_array(k_test_index+2, es_test_index, CSV_file);
    long double z15 = Spec_CSV.CSV_array(k_test_index+2, es_test_index+1, CSV_file);
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(cell);
        }
        parsedCsv.push_back(parsedRow);
	}**/

    spec_info Spec_CSV;
  
// Find CSV index for k_test and es_test value. (first possible value is [0][0] for [k][es]
// Value corresponds to point 6 (by the math I was using)
    int k_test_index = ((k_test_r/0.05)); // really should be ((k_test_r/0.05) - 0.5) but C++ always rounds down so int(1.5)=1
    int es_test_index = (es_test_r-0.5);   
    
// Initializing the spectral function values from CSV; Convert str char type from CSV to long double type
    long double z2 = Spec_CSV.CSV_array(k_test_index-1, es_test_index, CSV_file);
    long double z3 = Spec_CSV.CSV_array(k_test_index-1, es_test_index+1, CSV_file);   
    long double z5 = Spec_CSV.CSV_array(k_test_index, es_test_index-1, CSV_file);   
    long double z6 = Spec_CSV.CSV_array(k_test_index, es_test_index, CSV_file);   
    long double z7 = Spec_CSV.CSV_array(k_test_index, es_test_index+1, CSV_file);
    long double z8 = Spec_CSV.CSV_array(k_test_index, es_test_index+2, CSV_file); 
    long double z9 = Spec_CSV.CSV_array(k_test_index+1, es_test_index-1, CSV_file);
    long double z10 = Spec_CSV.CSV_array(k_test_index+1, es_test_index, CSV_file);
    long double z11 = Spec_CSV.CSV_array(k_test_index+1, es_test_index+1, CSV_file);
    long double z12 = Spec_CSV.CSV_array(k_test_index+1, es_test_index+2, CSV_file); 
    long double z14 = Spec_CSV.CSV_array(k_test_index+2, es_test_index, CSV_file);
    long double z15 = Spec_CSV.CSV_array(k_test_index+2, es_test_index+1, CSV_file);

    long double z3 = atof(parsedCsv[k_test_index-1][es_test_index+1].c_str());   
    long double z5 = atof(parsedCsv[k_test_index][es_test_index-1].c_str());   
    long double z6 = atof(parsedCsv[k_test_index][es_test_index].c_str());   
    long double z7 = atof(parsedCsv[k_test_index][es_test_index+1].c_str());   
    long double z8 = atof(parsedCsv[k_test_index][es_test_index+2].c_str());   
    long double z9 = atof(parsedCsv[k_test_index+1][es_test_index-1].c_str());   
    long double z10 = atof(parsedCsv[k_test_index+1][es_test_index].c_str());   
    long double z11 = atof(parsedCsv[k_test_index+1][es_test_index+1].c_str());   
    long double z12 = atof(parsedCsv[k_test_index+1][es_test_index+2].c_str());   
    long double z14 = atof(parsedCsv[k_test_index+2][es_test_index].c_str());   
    long double z15 = atof(parsedCsv[k_test_index+2][es_test_index+1].c_str());**/

// Set up Matrix to solve with given variables ] values for k seperated by 0.05 and es by 1
    long double x2 = k_test_r-0.05;
    long double x3 = k_test_r-0.05;
    long double x6 = k_test_r;
    long double x7 = k_test_r;
    long double x10 = k_test_r+0.05;
    long double x11 = k_test_r+0.05;
    long double x14 = k_test_r+0.1;
    long double x15 = k_test_r+0.1;

    long double y5 = es_test_r-1;
    long double y6 = es_test_r;
    long double y7 = es_test_r+1;
    long double y8 = es_test_r+2;
    long double y9 = es_test_r-1;
    long double y10 = es_test_r;
    long double y11 = es_test_r+1;
    long double y12 = es_test_r+2;

    
// Solve the Matrix for power series' variables using row echelon form
// all equations solved on paper. In the form of Ax = d (Last column is d, 0-39 is A)
long double M[40][41] = {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z6},
                         {1,0,(y5-y6),0,0,pow(y5-y6,2),0,0,0,pow(y5-y6,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z5},
                         {1,0,(y7-y6),0,0,pow(y7-y6,2),0,0,0,pow(y7-y6,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z7},
                         {1,(x10-x6),0,pow(x10-x6,2),0,0,pow(x10-x6,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z10},
                         {1,(x2-x6),0,pow(x2-x6,2),0,0,pow(x2-x6,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z2},
  
                         {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z7},
                         {0,0,0,0,0,0,0,0,0,0,1,0,(y8-y7),0,0,pow(y8-y7,2),0,0,0,pow(y8-y7,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z8},
                         {0,0,0,0,0,0,0,0,0,0,1,0,(y6-y7),0,0,pow(y6-y7,2),0,0,0,pow(y6-y7,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z6},
                         {0,0,0,0,0,0,0,0,0,0,1,(x3-x7),0,pow(x3-x7,2),0,0,pow(x3-x7,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z3},
                         {0,0,0,0,0,0,0,0,0,0,1,(x11-x7),0,pow(x11-x7,2),0,0,pow(x11-x7,3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z11},

                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,z10},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,(x14-x10),0,pow(x14-x10,2),0,0,pow(x14-x10,3),0,0,0,0,0,0,0,0,0,0,0,0,0,z14},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,(x6-x10),0,pow(x6-x10,2),0,0,pow(x6-x10,3),0,0,0,0,0,0,0,0,0,0,0,0,0,z6},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,(y11-y10),0,0,pow(y11-y10,2),0,0,0,pow(y11-y10,3),0,0,0,0,0,0,0,0,0,0,z11},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,(y9-y10),0,0,pow(y9-y10,2),0,0,0,pow(y9-y10,3),0,0,0,0,0,0,0,0,0,0,z9},
                         
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,z11},

                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,(x15-x11),0,pow(x15-x11,2),0,0,pow(x15-x11,3),0,0,0,z15},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,(x7-x11),0,pow(x7-x11,2),0,0,pow(x7-x11,3),0,0,0,z7},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,(y12-y11),0,0,pow(y12-y11,2),0,0,0,pow(y12-y11,2),z12},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,(y10-y11),0,0,pow(y10-y11,2),0,0,0,pow(y10-y11,3),z10},
                        
                         {0,-1,0,0,0,0,0,0,0,0,0,1,0,0,(y6-y7),0,0,0,pow(y6-y7,2),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,1,0,0,(y7-y6),0,0,0,pow(y7-y6,2),0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,-1,0,0,0,0,0,0,0,0,0,1,0,0,0,(y6-y7),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,1,0,0,0,(y7-y6),0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        
                         {0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,(x6-x10),0,0,pow(x6-x10,2),0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,1,0,(x10-x6),0,0,pow(x10-x6,2),0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,(x6-x10),0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,1,0,0,(x10-x6),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,1,0,0,(y10-y11),0,0,0,pow(y10-y11,2),0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,(y11-y10),0,0,0,pow(y11-y10,2),0,0,-1,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,1,0,0,0,(y10-y11),0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,(y11-y10),0,0,0,0,0,-1,0,0,0,0,0,0,0},
                        
                         {0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,(x7-x11),0,0,pow(x7-x11,2),0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,1,0,(x11-x7),0,0,pow(x11-x7,2),0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,(x7-x11),0,0},
                         {0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,3*(x6-x10),0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,1,0,0,0,3*(y10-y11),0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,3*(y11-y10),0,0,0,0,0,-1,0,0,0,0,0},
                         {0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,1,0,0,0,3*(y6-y7),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,1,0,0,3*(x10-x6),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };

   to_reduced_row_echelon_form(M);


// EDGE CASE: IF ALL 4 VALUES AROUND IT ARE 0 (should not interpolate: its zero)
// Interperlator will try and give it a super small number.
   if (z6 == 0 and z7 == 0  and z10 == 0 and z11 == 0){
     return 0;}
   
// Find which point the function is closest to: 6, 7, 10, or 11
   double dist_6 = sqrt(pow(x6 - k_test,2) + pow(y6 - es_test,2));
   double dist_7 = sqrt(pow(x7 - k_test,2) + pow(y7 - es_test,2));
   double dist_10 = sqrt(pow(x10 - k_test,2) + pow(y10 - es_test,2));
   double dist_11 = sqrt(pow(x11 - k_test,2) + pow(y11 - es_test,2));



   long double spec_fun_6 = (M[0][40]+M[1][40]*(k_test-x6)+M[2][40]*(es_test-y6)+M[3][40]*pow(k_test-x6,2)+M[4][40]*(k_test-x6)*(es_test-y6)+M[5][40]*pow(es_test-y6,2)+M[6][40]*pow(k_test-x6,3)+M[7][40]*pow(k_test-x6,2)*(es_test-y6)+M[8][40]*(k_test-x6)*pow(es_test-y6,2)+M[9][40]*pow(es_test-y6,3));
   long double spec_fun_7 = (M[10][40]+M[11][40]*(k_test-x7)+M[12][40]*(es_test-y7)+M[13][40]*pow(k_test-x7,2)+M[14][40]*(k_test-x7)*(es_test-y7)+M[15][40]*pow(es_test-y7,2)+M[16][40]*pow(k_test-x7,3)+M[17][40]*pow(k_test-x7,2)*(es_test-y7)+M[18][40]*(k_test-x7)*pow(es_test-y7,2)+M[19][40]*pow(es_test-y7,3));
   long double spec_fun_10 = (M[20][40]+M[21][40]*(k_test-x10)+M[22][40]*(es_test-y10)+M[23][40]*pow(k_test-x10,2)+M[24][40]*(k_test-x10)*(es_test-y10)+M[25][40]*pow(es_test-y10,2)+M[26][40]*pow(k_test-x10,3)+M[27][40]*pow(k_test-x10,2)*(es_test-y10)+M[28][40]*(k_test-x10)*pow(es_test-y10,2)+M[29][40]*pow(es_test-y10,3));
  long double spec_fun_11 = (M[30][40]+M[31][40]*(k_test-x11)+M[32][40]*(es_test-y11)+M[33][40]*pow(k_test-x11,2)+M[34][40]*(k_test-x11)*(es_test-y11)+M[35][40]*pow(es_test-y11,2)+M[36][40]*pow(k_test-x11,3)+M[37][40]*pow(k_test-x11,2)*(es_test-y11)+M[38][40]*(k_test-x11)*pow(es_test-y11,2)+M[39][40]*pow(es_test-y11,3));

// Find the interpolated Spectral Function Value for the given k and es
// If spline gives values out of range, just return the weighted average: happens for power series
  if (dist_6 < dist_7 and dist_6 < dist_10 and dist_6 < dist_11){
    if (spec_fun_6 < z6/10 or spec_fun_6 > z6*10){
      return (z6*dist_6+z7*dist_7+z10*dist_10+z11*dist_11)/(dist_6+dist_7+dist_10+dist_11);}
    return (double)spec_fun_6;}
   else if (dist_7 < dist_10 and dist_7 < dist_11 and dist_7 < dist_6){
    if (spec_fun_7 < z7/10 or spec_fun_7 > z7*10){
      return (z6*dist_6+z7*dist_7+z10*dist_10+z11*dist_11)/(dist_6+dist_7+dist_10+dist_11);}
    return (double)spec_fun_7;}
   else if (dist_10 < dist_11 and dist_10 < dist_6 and dist_10 < dist_7){
    if (spec_fun_10 < z10/10 or spec_fun_10 > z10*10){
      return (z6*dist_6+z7*dist_7+z10*dist_10+z11*dist_11)/(dist_6+dist_7+dist_10+dist_11);}
    return (double)spec_fun_10;}
   else{
    if (spec_fun_11 < z11/10 or spec_fun_11 > z11*10){
      return (z6*dist_6+z7*dist_7+z10*dist_10+z11*dist_11)/(dist_6+dist_7+dist_10+dist_11);}
    return (double)spec_fun_11;}
}



  // USEFUL CODE TO CHECK ANSWER  

  
// Print out the answers
//  std::cout << "The Spectral Function for Momentum " << k_test << " and Energy " << es_test << " is: " << spec_fun_6 << "\n";
//  std::cout << "The local Spectral Function for Momentum near " << k_test << " and Energy near " << es_test << " is: " <<  M[0][40] << " + " << M[1][40] << "*(x-" << x6 << ")" << " + " << M[2][40] << "*(y-" << y6 << ")" << " + " << M[3][40] << "*(x-" << x6 << ")^2" << " + " << M[4][40] << "*(x-" << x6 << ")*(y-" << y6 << ")" << " + " << M[5][40] << "*(y-" << y6 << ")^2" << " + " << M[6][40] << "*(x-" << x6 << ")^3" << " + " << M[7][40] << "*(x-" << x6 << ")^2*(y-" << y6 << ")" << " + " << M[8][40] << "*(x-" << x6 << ")*(y-" << y6 << ")^2" << " + " << M[9][40] << "*(y-" << y6 << ")^3" << "\n";

//  std::cout << spec_fun_6 << "\n";
//  std::cout << spec_fun_7 << "\n";
//  std::cout << spec_fun_10 << "\n";
//  std::cout << spec_fun_11 << "\n";
  
  /** if you want to print the matrix in reduced row echelon form 
  for (int i = 0; i < 40; ++i)
  {
    for (int j = 0; j < 41; ++j)
      std::cout << M[i][j] << '\t';
      std::cout << "\n";
      } **/
  
  /** if you want to print out the power series coefficient: zx6(0-9) , z7(10-19) , z10(21-29) , z11(30-39)  
  for (int i = 0; i <40; ++i)
    {
      std::cout << M[i][40];
      std::cout << "\n";
      } **/

      //TEST ROUNDING
  //   std::cout << "index: " << es_test_index << "\n" << "round: " << es_test_r << "\n" << "initial: " << es_test << "\n";
  //std::cout << "k_index: " << k_test_index << "\n" << "k_round: " << k_test_r<< "\n" << "k_initial: " <<  k_test << "\n";

  //  if (spec_fun_6<0){


