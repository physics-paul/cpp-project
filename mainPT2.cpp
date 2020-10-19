/*###############################################
#                                               #
# System Processes in Finance                   #
#                                               #
# PAUL SANDERS                                  #
#                                               #
# MIDTERM PROJECT PART #2                       #
#                                               #
# DATE: 10/13/2020                              #
#                                               #
###############################################*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <numeric>
#include "Option.h"

using namespace std;

int main()
{

  /*###############################################
  # INITIALIZING VARIABLES                        #
  ###############################################*/

  int i; int j;
  double St;
  double T;
  double K;
  double sigma;
  double r;
  double dt=1/252.0;

  string line;
  string item;

  vector<double> rf;

  vector<string> date;
  vector<double> stock;

  vector<string> dateOption;
  vector<string> exDateOption;
  vector<string> cpFlag;
  vector<double> strike;
  vector<double> bid;
  vector<double> ask;

  string t0;
  string tN;
  string Texp;
  int start;
  int stop;
  int optExpire;
  int nSteps;

  string currentDate;
  double price;
  double correction;
  double hedgeError;
  double pnl;
  vector<double> borrowed(nSteps);
  vector<double> amountStocks(nSteps);
  vector<double> hedgedPortfolio(nSteps);
  vector<double> delta(nSteps);
  vector<double> optionPrice(nSteps);
  vector<double> impVolatility(nSteps);
  
  Option option;
  Option iterOption;

  /*###############################################
  # IMPORT RISK-FREE RATE                         #
  ###############################################*/

  ifstream rate("interest.csv");

  getline(rate,line);

  while(getline(rate,line)) {
    istringstream iss(line);
    getline(iss, item, ',');
    getline(iss, item, ',');
    rf.push_back(stod(item)/100.0);
  }

  /*###############################################
  # IMPORT STOCK PRICES                           #
  ###############################################*/

  ifstream stockPrice("sec_GOOG.csv");

  getline(stockPrice,line);

  while(getline(stockPrice,line)) {
    istringstream iss(line);
    getline(iss, item, ',');
    date.push_back(item);
    getline(iss, item, ',');
    stock.push_back(stod(item));
  }

  if ((date.size()!=rf.size()) || (stock.size()!=rf.size())) {
    std::cout << "The dates for the interest rate and stock price must match exactly. Try again \n \n";
    return 0;
  }

  /*###############################################
  # IMPORT OPTION INFORMATION                     #
  ###############################################*/

  ifstream optionFile("op_GOOG.csv");

  getline(optionFile,line);

  while(getline(optionFile,line)) {
    istringstream iss(line);
    getline(iss, item, ',');
    dateOption.push_back(item);
    getline(iss, item, ',');
    exDateOption.push_back(item);
    getline(iss, item, ',');
    cpFlag.push_back(item);
    getline(iss, item, ',');
    strike.push_back(stod(item));
    getline(iss, item, ',');
    bid.push_back(stod(item));
    getline(iss, item, ',');
    ask.push_back(stod(item));
  }

  /*###############################################
  # INPUT USER PARAMETERS                         #
  ###############################################*/

  std::cout << " Please enter start date 'YYYY-MM-DD' from " << 
  date[0] << " to " << date[date.size()-1] << ": \n";

  std::cin >> t0;

  // check to see if this is valid.

  start=-1;

  for (i=0;i<date.size();i++) {
    if (date[i]==t0) {

      start=i;

    }

  }

  while (start==-1) {
    std::cout << "The starting date is not a trading day. Try again. \n";
    std::cin >> t0;

    for (i=0;i<date.size();i++) {
      if (date[i]==t0) {

        start=i;

      }

    }

  }
  
  std::cout << " Please enter end date 'YYYY-MM-DD' from " << 
  date[0] << " to " << date[date.size()-1] << ": \n";
  
  std::cin >> tN;

  // check to see if this is valid.

  stop=-1;

  for (i=0;i<date.size();i++) {
    if (date[i]==tN) {

      stop=i;

    }

  }

  while (stop==-1) {
    
    std::cout << "The end date is not a trading day. Try again. \n";
    std::cin >> tN;

    for (i=0;i<date.size();i++) {
      if (date[i]==tN) {

        stop=i+1;

      }

    }
    
  }

  // test cases where the dates are not correct.

  if (start-stop>=0) {
    std::cout << "The start date is not less than the end date." <<
      "Try again. \n";
    return 0;
  }

  std::cout << "Please enter strike price : \n" ;
  std::cin >> K;

  std::cout << " Please enter option expiry date 'YYYY-MM-DD' from " << 
  exDateOption[0] << " to " << exDateOption[exDateOption.size()-1] << ": \n";

  std::cin >> Texp;

  // check to see if this is valid.

  if (Texp<tN) {
    std::cout << "The option expiry date is less than the end date. " <<
    "Try again. \n";
    return 0;
  }

  // ensure an option expiry exists for each hedging date.

  optExpire=0;

  for (i=start;i<stop;i++) {
  
    for (j=0;j<exDateOption.size();j++) {
      if (
        (strike[j]==K)&&(
          dateOption[j]==date[i])&&(
            exDateOption[j]==Texp)&&(
              cpFlag[j]=="C")) {

        optExpire+=1;

      }

    }

  }

  nSteps=stop-start;

  if (optExpire!=nSteps) {
    std::cout << "The option expiry date is not valid, because call option prices " <<
    "are not available for some dates in the range. Try again. \n";
    return 0;
  }

  // resize the date, rf, and stock array to be in line with the user dates.

  for (i=start;i<stop;i++) {
    
    date[i-start]=date[i];
    rf[i-start]=rf[i];
    stock[i-start]=stock[i];
  
  }

  date.resize(nSteps);
  rf.resize(nSteps);
  stock.resize(nSteps);  

  /*###############################################
  # CALCULATE HEDGED PORTFOLIO IN TIME            #
  ###############################################*/

  // the portfolio is assumed to be changed at the end of each trading
  // day, where buy and sell orders are placed at the closing price.
  // for the starting time, to, the portfolio is assumed to be created
  // at the end of the day, and not change in the profit and loss has taken
  // place during the day.

  for (i=0;i<nSteps;i++) {

    // calcualte the end of day parameters.

    currentDate=date[i];
    r=rf[i];
    St=stock[i];
    T=dateDifference(currentDate,Texp)/252.0;

    // determine implied volatility.

    sigma=0.0;

    // initialize iterative option used to find implied volatility.

    iterOption=Option(St,T,r,K,sigma,0); 

    for (j=0;j<dateOption.size();j++) {

      // search the option database for the iterative option corresponding 
      // to the specific option parameters.

      if (
        (dateOption[j]==currentDate)&&(
          strike[j]==K)&&(
            cpFlag[j]=="C")&&(
              exDateOption[j]==Texp)) {
        
        // get the price and implied volatility of the particular
        // iterative option in question.

        price=0.5*(bid[j]+ask[j]); 

        sigma=iterOption.getImpliedVolatility(price);
      
      }
    
    }

    impVolatility[i]=sigma;
    optionPrice[i]=price;

    // initialize call option.

    option=Option(St,T,r,K,sigma,0);
    
    // calculate the call option delta.

    delta[i]=option.getDelta();

    // initialize starting portfolio at to.

    if (i==0) {

      // the amount to buy of stocks is the delta*stock price.

      amountStocks[i]=delta[i]*St;

      // the amount to borrow is the difference in option price and
      // amount bought in stocks.

      borrowed[i]=optionPrice[i]-amountStocks[i];

      // the hedged portfolio is the short option, the long
      // stocks, and the borrowed cash, which at to is zero.

      hedgedPortfolio[i]=-optionPrice[i]+amountStocks[i]+borrowed[i];

    }

    // calculate the change in portfolio over the course of the day.

    if (i==0) {

      ; // the portfolio value is unchanged on the first day.

    } else {
      
      // the amount of stocks is the delta from the previous day
      // multiplied by the stock price.

      amountStocks[i]=delta[i-1]*St;
    
      // the amount borrowed simply grows at the day's interest rate.

      borrowed[i]=borrowed[i-1]*exp(r*dt);

    }

    // perform adjustments to delta hedge for the next day.
    // the correction term is the amount of stocks which must be bought
    // or sold to remain delta hedged for the next day.

    correction=delta[i]*St-amountStocks[i];

    // the amount of stocks bought is adjusted by this correction.

    amountStocks[i]+=correction;

    // this does not affect the portfolio value, but is a redistribution
    // from the amount of stocks bought to the borrowed cash.

    borrowed[i]-=correction;

    // get the new portfolio value at the end of the day.

    hedgedPortfolio[i]=-optionPrice[i]+amountStocks[i]+borrowed[i];

  }

  /*###############################################
  # OUTPUT RESULTS TO CSV                         #
  ###############################################*/

  ofstream out("hedgingResults.csv");

  // initialize the first header row.

  out << "date" << "," << "S" << "," <<
    "V" << "," << "implied volatility" << "," << 
    "delta" << "," << "hedging error" << "," << 
    "PNL" << "," << "PNL (with hedge)" << "\n";

  // iterate through the time steps.

  for (i=0;i<nSteps;i++) {

    if (i==0) {

      // at the first day, there is no hedging error since the portfolio
      // has only been created on this day.

      hedgeError=0;
      pnl=0;

      out << date[0] << "," << stock[0] << "," << 
      optionPrice[0] << "," << impVolatility[0] << "," <<
      delta[0] << "," << hedgeError << "," <<
      pnl << "," << hedgedPortfolio[0] << "\n";

    } else {

      // for each subsequent day, the hedging error is the deviation from
      // the delta hedge. if the stock price moves significantly away from 
      // the previous day's price, then the hedge error most likely
      // will be greater.

      hedgeError=delta[i-1]*(stock[i]-stock[i-1])
        -(optionPrice[i]-optionPrice[i-1]);
      pnl=optionPrice[0]-optionPrice[i];

      out << date[i] << "," << stock[i] << "," << 
      optionPrice[i] << "," << impVolatility[i] << "," <<
      delta[i] << "," << hedgeError << "," <<
      pnl << "," << hedgedPortfolio[i] << "\n";

    }
    
  }

  return 0;

}
