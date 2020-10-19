/*###############################################
#                                               #
# System Processes in Finance                   #
#                                               #
# PAUL SANDERS                                  #
#                                               #
# MIDTERM PROJECT PART #1                       #
#                                               #
# DATE: 10/13/2020                              #
#                                               #
###############################################*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Option.h"
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

using namespace std;

int main()
{

  /*###############################################
  # INITIALIZING VARIABLES                        #
  ###############################################*/

  int i; int j;
  double So    = 600;
  double K     = 500.0;
  double T     = 1.0;
  double mu    = 0.05;
  double sigma = 0.24;
  double r     = 0.025;
  int npath    = 1000;
  int nsamp    = 100;
  double dt    = 1/252.0;
  
  /*###############################################
  # GENERATE RANDOM BROWNIAN MOTION               #
  ###############################################*/

  boost::mt19937 rng;
  boost::normal_distribution<> norm(0,1);
  boost::variate_generator<boost::mt19937&,
                           boost::normal_distribution<> > n(rng, norm);

  vector<vector<double>> dw(nsamp,vector<double>(npath));
  vector<vector<double>> temp(nsamp+1,vector<double>(npath));
  vector<vector<double>> S(nsamp+1,vector<double>(npath));
  double dLogS;

  // initialize first row as initial log stock price

  for (j=0; j<npath; j++) {
    temp[0][j] = log(So);
    S[0][j] = So;
  }

  // calculate random stock price movements

  for (i=1; i<nsamp+1; i++) {
    for (j=0; j<npath; j++) {
      //initialize dw
      dw[i-1][j]=n();     
      // initialize dLogS
      dLogS = (r-0.5*pow(sigma,2.0))*dt+sigma*sqrt(dt)*dw[i-1][j];
      temp[i][j] = dLogS;
      // cumsum over temp to get log S
      temp[i][j] += temp[i-1][j];
      // raise to the exponent to get S
      S[i][j] = exp(temp[i][j]);
    }
  }

  /*###############################################
  # GENENERATE CALL OPTION PRICES                 #
  ###############################################*/

  vector<vector<double>> callValue(nsamp+1,vector<double>(npath));

  // instantiate the option class

  Option option = Option(So,T,r,K,sigma,0);

  // initialize first row option price

  for (j=0; j<npath; j++) {
    callValue[0][j] = option.getOptionPrice();
  }

  // calculate call option price paths

  for (i=1; i<nsamp+1; i++) {
    for (j=0; j<npath; j++) {
      option.changeAssetPrice(S[i][j]);
      option.changeMaturityTime(T-i*dt);
      callValue[i][j] = option.getOptionPrice();
    }
  }

  /*###############################################
  # GENENERATE DELTA-HEDGED PORTFOLIO             #
  ###############################################*/

  vector<vector<double>> borrowed(nsamp+1,vector<double>(npath));
  vector<vector<double>> amountStocks(nsamp+1,vector<double>(npath));
  vector<vector<double>> hedgedPortfolio(nsamp+1,vector<double>(npath));
  double correction;

  // reset the option class

  option = Option(So,T,r,K,sigma,0);

  // initialize starting portfolio

  for (j=0; j<npath; j++) {
    amountStocks[0][j]=option.getDelta()*S[0][j];
    borrowed[0][j] = callValue[0][j]-amountStocks[0][j];
    hedgedPortfolio[0][j]=-callValue[0][j]+amountStocks[0][j]+borrowed[0][j];
  }

  for (i=1; i<nsamp+1; i++) {
    for (j=0; j<npath; j++) {
      
      // calculate the change in portfolio over the course of a day

      option.changeAssetPrice(S[i][j]);
      option.changeMaturityTime(T-i*dt);

      amountStocks[i][j]=amountStocks[i-1][j]/S[i-1][j]*S[i][j];
      borrowed[i][j]=borrowed[i-1][j]*exp(r*dt);

      // perform adjustments to delta hedge for the next day

      correction=option.getDelta()*S[i][j]-amountStocks[i][j]; 
      amountStocks[i][j]+=correction;
      borrowed[i][j]-=correction;

      // get the new portfolio value at the end of the day

      hedgedPortfolio[i][j]=-callValue[i][j]+amountStocks[i][j]+borrowed[i][j];
    }
  }

  // output results

  ofstream out("hedgingErrors.csv");
  
  for (j=0;j<npath;j++) {
    out << hedgedPortfolio[nsamp][j] << "\n";
  } 

  return 0;
  
}
