#include <iostream>
#include <cmath>
#include "Option.h"
#include <boost/date_time.hpp>

using namespace std;
using namespace boost::gregorian;

Option::Option()
{

  // initialize option values.

  strikePrice = 100;
  assetPrice = 100;
  riskRate = 0.05;
  maturityTime = 2;
  sigma = 0.10;
  callPutFlag = 1;

}

Option::Option(
  double new_assetPrice, 
  double new_maturityTime,
  double new_riskRate, 
  double new_strikePrice, 
  double new_sigma,
  int new_callPutFlag)
{

  // create option values from user inputs.

  assetPrice = new_assetPrice;
  maturityTime = new_maturityTime;
  riskRate = new_riskRate;
  strikePrice = new_strikePrice;
  sigma = new_sigma;
  callPutFlag = new_callPutFlag;

}

Option::~Option() {
  sigma=0;
}

double N(double x)
{

  // cumulative distribution function for normal distribution.

  return (1 + erf(x / sqrt(2.0))) / 2.0;

}

double Option::getImpliedVolatility(double optionValue)
{

  double mid;
  double left;
  double right;
  double tol;
  double optionLeft;
  double optionRight;
  
  left = 0.01;
  right = 2.0;
  tol = 0.0001;
  
  changeSigma(left);
  optionLeft = getOptionPrice() - optionValue;
  changeSigma(right);
  optionRight = getOptionPrice() - optionValue;

  if (optionLeft*optionRight >= 0)
  {
    return -1.0;
  }

  // run bisection method.

  while(right-left > tol)
  {

    mid = (left+right) / 2.0;

    changeSigma(left);
    optionLeft = getOptionPrice() - optionValue;
    changeSigma(mid);
    optionRight = getOptionPrice() - optionValue;
    
    if (optionLeft*optionRight < 0)
    {
      right = mid;
    }
    else  
    {
      left = mid;
    }

  }

  return left;

}

double Option::getDelta()
{

  double d1;

  d1 = (
    log(assetPrice/strikePrice)
    +(riskRate+0.5*pow(sigma,2.0))*maturityTime);
  d1 = d1/(sigma*sqrt(maturityTime));

  switch (callPutFlag) {
    case 0:
      return N(d1);
    case 1:
      return -N(d1);
    case 2:
      return N(d1)-1.0;
    case 3:
      return 1.0-N(d1);
  }
  
}

double Option::getOptionPrice()
{

  // black-scholes price for a call is c = So N(d1) - K e^(-rT) N(d2).
  // black-scholes price for a put is p = K e^(-rT) N(-d1) - So N(-d1).

  double d1;
  double d2;

  d1 = log(assetPrice/strikePrice)
    +(riskRate+0.5*pow(sigma,2.0))*maturityTime;
  d1 = d1/(sigma*sqrt(maturityTime));

  d2 = d1 - sigma*sqrt(maturityTime);

  switch (callPutFlag) {
    case 0:
      return assetPrice*N(d1)-strikePrice*exp(-riskRate*maturityTime)*N(d2);
    case 1:
      return -assetPrice*N(d1)+strikePrice*exp(-riskRate*maturityTime)*N(d2);
    case 2:
      return -assetPrice*N(-d1)+strikePrice*exp(-riskRate*maturityTime)*N(-d2);
    case 3:
      return assetPrice*N(-d1)-strikePrice*exp(-riskRate*maturityTime)*N(-d2);
  }

}

void Option::changeAssetPrice(double new_assetPrice) {
  assetPrice = new_assetPrice; 
}

void Option::changeMaturityTime(double new_maturityTime) {
  maturityTime = new_maturityTime;
}

void Option::changeRiskRate(double new_riskRate) {
  riskRate = new_riskRate;
}

void Option::changeStrikePrice(double new_strikePrice) {
  strikePrice = new_strikePrice;
}

void Option::changeSigma(double new_sigma) {
  sigma = new_sigma;
}

void Option::changeCallPutFlag(int new_callPutFlag) {
  callPutFlag = new_callPutFlag;
}

int dateDifference(string start_date, string end_date) {
    // this function computes the number of business days between start and end dates

    date _start_date(from_simple_string(start_date));
    date _end_date(from_simple_string(end_date));

    // counter for weekdays
    int count=0;
    for(day_iterator iter = _start_date; iter!=_end_date; ++iter)
    {
        // increment counter if it's no saturday and no sunday
        if(    iter->day_of_week() !=  boost::date_time::Saturday
            && iter->day_of_week() !=  boost::date_time::Sunday)
            ++count;
    }
    return count;
}

double avg(std::vector<double> v) {
    double returnValue=0.0;
    int n=v.size();

    for (int i=0; i<n; i++) {
      returnValue += v[i];
    }

    return (returnValue/n);

}