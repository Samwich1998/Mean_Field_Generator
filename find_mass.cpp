#include <iostream>
#include <cmath>

#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TLorentzVector.h"


int main(int argc, char** argv){
  if (argc != 3){
    std::cout << "wrong number of arguments\n";
    std::cout << "Try find_mass /input/file /output/file\n";
    return -1;
  }

  
//Get D and A trees. Open output file.
  TFile * input_file = new TFile(argv[1]);
  TFile * output_file = new TFile(argv[2],"RECREATE");


  double total_bins = 300.;  
//Make trees and histograms for the nuclei
  TTree * generator_tree = (TTree*)input_file->Get("genT");
  TH2D * his_P1_Mtf = new TH2D("P1_VS_Mtf","P1_VS_Mtf;P1;Mtf;counts", total_bins, 0, 400, total_bins, 700, 1100); // bin #, min1, min2, max
  TH2D * his_P1_Mtf_mean = new TH2D("P1_VS_Mtf_mean","P1_VS_Mtf_mean;P1;Mtf;counts", 1000, 0, 1500, 1000, 0, 1500); // bin #, min1, min2, max
  TH2D * his_theta_P1prime_q = new TH2D("theta_VS_P1prime_q","theta_VS_P1prime_q;P1prime/q;theta;counts", 20, 0, 1.3, 30, 0, 1.5); // bin #, min1, min2, max

  
// Define Variables
  double X_b, Q_2, q_vec[3], P1_prime_vec[3], P1_prime_mag, q_mag, weight, P1_mag, w;  // Define variables from generator.cpp
  double P1_vec[3];  // Define variables from generator.cpp
  double M_n = 0.93827231;	                                                       // Mass of exiting proton (close to neutron); Gev
  
// Get branch (values) from generator.cpp   
  generator_tree->SetBranchAddress("X_b",&X_b);
  generator_tree->SetBranchAddress("Q_2",&Q_2);
  generator_tree->SetBranchAddress("P1_prime_mag",&P1_prime_mag);
  generator_tree->SetBranchAddress("q_mag",&q_mag);
  generator_tree->SetBranchAddress("weight",&weight);
  generator_tree->SetBranchAddress("P1_mag",&P1_mag);
  generator_tree->SetBranchAddress("w",&w);
  generator_tree->SetBranchAddress("P1_prime_vec",P1_prime_vec);
  generator_tree->SetBranchAddress("q_vec",q_vec);
  generator_tree->SetBranchAddress("P1_vec",P1_vec);

  double x_max = 0;
// Loop over all entries
  for (int i = 0; i < generator_tree->GetEntries(); i++){   
    generator_tree->GetEvent(i);
    if (X_b < 1.) continue;

    if(Q_2 > 2. and Q_2 < 2.5) continue;
    
    if ((P1_prime_mag/q_mag) > 0.96) continue;
    if ((P1_prime_mag/q_mag) < 0.62) continue;
    TVector3 P1_prime_TVec(P1_prime_vec[0], P1_prime_vec[1], P1_prime_vec[2]); //turn to TVector
    TVector3 q_TVec(q_vec[0], q_vec[1], q_vec[2]);                             //turn to TVector
    TVector3 P1_TVec(P1_vec[0], P1_vec[1], P1_vec[2]);                         //turn to TVector


    TLorentzVector test_P1_prime;
    test_P1_prime.SetPxPyPzE(P1_prime_vec[0],P1_prime_vec[1],P1_prime_vec[2],sqrt(P1_prime_TVec.Mag2() + M_n*M_n));

    TLorentzVector test_q;
    test_q.SetPxPyPzE(q_vec[0], q_vec[1], q_vec[2], w);

    TLorentzVector test_pair;
    test_pair.SetPxPyPzE(0, 0, 0, 2*M_n);

    double Mtf = sqrt((test_q+test_pair-test_P1_prime).Mag2());


    
    Double_t theta_P1_prime_q = P1_prime_TVec.Angle(q_TVec);                   //give angle between vectors, Radians
    double_t q_dot_pprime = P1_prime_TVec.Dot(q_TVec);
    double_t q_dot_P1 = P1_TVec.Dot(q_TVec);
    double_t P1_dot_pprime = P1_prime_TVec.Dot(P1_TVec);   
    // std::cout << theta_P1_prime_q*360/(2*M_PI) << "   real    " << "\n";   
    if (theta_P1_prime_q > 25.*(2.*M_PI/360.)) continue;
    double E1_prime = sqrt(P1_prime_mag*P1_prime_mag + M_n*M_n);  // final Energy of ejected nucleon; Gev
    double Mtf_1 = sqrt(abs((-Q_2 + 4*M_n*M_n + E1_prime*E1_prime - P1_prime_mag*P1_prime_mag + 4*M_n*(w - E1_prime) - 2*E1_prime*w + 2*q_dot_pprime)));
    double Mtf_2 = sqrt(abs((-Q_2 + 4*M_n*M_n + E1_prime*E1_prime - P1_prime_mag*P1_prime_mag + 4*M_n*(w - E1_prime) - 2*E1_prime*w + 2*q_dot_pprime - P1_mag*P1_mag - 2*q_dot_P1 + 2*P1_dot_pprime)));
    double Mtf_3 = sqrt(abs((-Q_2 + M_n*M_n + E1_prime*E1_prime - P1_prime_mag*P1_prime_mag + 2*M_n*(w - E1_prime) - 2*E1_prime*w + 2*q_dot_pprime - P1_mag*P1_mag - 2*q_dot_P1 + 2*P1_dot_pprime)));

    //  std::cout << " mine  "<< Mtf_1 << "  new "  << Mtf << "\n";
    if (Mtf < 0.) continue;
    his_P1_Mtf->Fill(P1_mag*1000,Mtf*1000,weight);
    his_theta_P1prime_q->Fill((P1_prime_mag/q_mag),theta_P1_prime_q,weight);
  }

  //his_P1_Mtf->Draw("colz");

  
// Sum of squares (error)
  his_P1_Mtf->Sumw2();
  his_theta_P1prime_q->Sumw2();

// Find mean for every x point and replot
  double section_width = 30.;
  double total_sections = total_bins/section_width;
  double x[(int) total_sections];
  double y[(int) total_sections];
  double ex[(int) total_sections];
  double ey[(int) total_sections];
  

for (int round = 0.; round < total_sections; round++){ 
  TH1D * proj_Mtf = his_P1_Mtf->ProjectionY(":)", (total_bins/total_sections)*round, (total_bins/total_sections)*(round+1.));
  double Mtf_bin_mean = proj_Mtf->GetMean(); // find mean given in bin graph
  double Mtf_bin_std = proj_Mtf->GetStdDev();
  x[round] = (round+0.5)*section_width;  
  y[round] = Mtf_bin_mean;
  ex[round] = section_width/2.;
  ey[round] = Mtf_bin_std;
  his_P1_Mtf_mean->Fill((round+0.5)*section_width,Mtf_bin_mean);
  std::cout << "std:  " <<  Mtf_bin_std << "        mean:  " << Mtf_bin_mean << "\n";
  proj_Mtf->Reset("ICESM");
 }


 TCanvas *c2 = new TCanvas("c2","A Simple Graph Example",900.,900.);

  // TGraphErrors * error = new TGraphErrors(ADC_3_x.size(),&ADC_3_x[0],&ADC_3_y[0]);

  
  TGraphErrors* Mtf_error = new TGraphErrors(total_sections, x, y, ex, ey);
   Mtf_error->SetTitle("");      // graph title
   Mtf_error->SetLineColor(kBlue);                                
   Mtf_error->SetLineWidth(1);                                   
   Mtf_error->SetMarkerStyle(48);                                // rad style for point
   Mtf_error->SetMarkerSize(1);                                // point size
   Mtf_error->SetMarkerColor(46);                                // cool color points
   Mtf_error->GetYaxis()->SetTitle("M_tf");  // y-axis title
   Mtf_error->GetYaxis()->SetTitleOffset(1.3);                   // y-axis title offset from axis
   Mtf_error->GetYaxis()->CenterTitle(true);                     // center on axis
   Mtf_error->GetXaxis()->SetTitle("P1");                     // X-axis title
   Mtf_error->GetXaxis()->CenterTitle(true);                     // Center on axis
   Mtf_error->SetMinimum(0);                                     // y-axis minimum
   Mtf_error->SetMaximum(3500);                                  // y-axis maximum
   Mtf_error->Draw("AP");                                        // Draw on canvas
   


  
    input_file->Close();
    his_P1_Mtf->Write();
    his_P1_Mtf_mean->Write();
    his_theta_P1prime_q->Write();
    Mtf_error->Write();
    //proj_Mtf->Write();
    output_file->Close();
  
  return 0;
}
