#include <boost/date_time.hpp>
#include <iostream>

using namespace std;
using namespace boost::gregorian; // compile with linker option as g++ computeBusinessDays.cpp -l boost_date_time

// this function computes the number of business days between start and end dates
long dateDifference( string start_date, string end_date )
{
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

int main(){

    string start_date, end_date;
    long numOfDays;
    cout << "Enter the date in yyyy-mm-dd format" << endl;

    cout << "Enter start date" << endl;
    cin >> start_date;
    cout << "Enter end date" << endl;
    cin >> end_date;

    numOfDays = dateDifference(start_date,end_date);

    cout << "Number of business days are " << numOfDays << endl;
    return 0;

}
