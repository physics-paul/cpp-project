#include <string>
#include <vector>

using namespace std;

int dateDifference(std::string, std::string);
double avg(std::vector<double>);
double N(double);

class Option
{

    double assetPrice;
    double maturityTime;
    double riskRate;
    double strikePrice;
    double sigma;
    // callPutFlag = 0 long call
    // callPutFlag = 1 short call
    // callPutFlag = 2 long put
    // callPutFlag = 3 short put
    int callPutFlag;

    public:
        Option();
        Option(
            double new_assetPrice, 
            double new_maturityTime, 
            double new_riskRate, 
            double new_strikePrice, 
            double new_sigma,
            int new_callPutFlag);
        ~Option();

        double getDelta();
        double getImpliedVolatility(double);
        double getOptionPrice();
        void changeAssetPrice(double);
        void changeMaturityTime(double);
        void changeRiskRate(double);
        void changeStrikePrice(double);
        void changeSigma(double);
        void changeCallPutFlag(int);

};

