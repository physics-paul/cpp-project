This project seeks to analyze the a dynamic delta hedging strategy where a call option is sold and then delta hedged from a zero-cost hedged portfolio at the starting time. The call option is then dynamically hedged each day to remain net-delta neutral. The hedging error is then calculated and compared against the actual call option value for sample stock price paths using the Black-Scholes equation. Then, the hedging error is compared to the GOOG call option and stock price values.

This project is divided into 3 main parts:
- 1.1. Testing the delta hedging strategy using the Black-Scholes model
- 2.1. Testing the delta hedging using GOOG call option data
- 3.1. Compilation instructions

### 1.1.  Testing the delta hedging strategy using the Black-Scholes model

In order to test the delta hedging implementation, a random sample of stock price paths was generated, assuming the stock price follows a log-normal distribution. The time was split into N equally-spaced time steps and 1000 sample paths were created.

Then, the call option values were generated for each stock sample path.

Finally, the correct number of stocks were bought at each time step to remain delta-neutral for the next time step. This was financed by borrowing cash, and the amount of stocks purchased and cash borrowed changed with each time step.

Since the delta-hedging is only valid for small stock price movements, there will be some error in hedging each period.

The traded cash-flows and value of the portfolio at each time step looks like:

| Cash Flows | t=0 | t=1 | --- | t=n |
| Beginning of Day | | | | |
| Sell Call option | --- | -C0 | --- | -C(n-1) |
| Buy Stock        | --- | delta0 S0 | --- | delta(n) S(n-1) |
| Borrow Cash      | --- | (C0 - delta0 S0) | --- | B(n-1) |
| End of Day       | | | | |
| Sell Call option | +C0 | -C1 | --- | -Cn |
| Buy Stock        | -delta0 S0 | delta0 S1 | --- | delta(n) Sn |
| Borrow Cash      | (delta0 S0 - C0) | B0 exp(r0 dt) | --- | B(n-1) exp(r(n-1) dt) |
| Adjustments      | | | | | 
| Buy Stock        | --- | (delta1 - delta0) S1 | --- | (delta(n) - delta(n-1)) S2 |
| Borrow Cash      | --- | -(delta1 - delta0) S1 | --- | -(delta(n) - delta(n-1)) S2 |
| Portfolio Value  | | | | |
| Short Call option | -C0 | -C1 | --- | Cn |
| Stock            | delta0 S0 | delta1 S1 | --- | delta(n) Sn |
| Borrowed Cash    | B0 = (C0 - delta0 S0) | B1 = B0 exp(r0 dt) - (delta1 - delta0) S1 | --- | Bn = B(n-1) exp(r(n-1) dt) - (delta(n) - delta(n-1)) Sn |
| Total            | 0 | delta1 S1 - C1 + B1 | --- | delta(n) S(n) - Cn + Bn |

If we run this simulation for 1000 different runs, then we can get a cumulative hedging error at the end of each time period. Using the parameters S0 = 100, T = 0.4, mu = 0.05, sigma = 0.24, r = 0.025, and N = 100 for a European call option with K = 105 and T = 0.4, then we arrive at a distribution of hedged portfolio values at the end of time T given by:

<p align="center">
  <img width="400" height="300" src="https://github.com/physics-paul/cpp-project/blob/main/image.png">
</p>

### 2.1.  Testing the delta hedging using GOOG call option data

For the second part, the hedging error is calculated using listed option prices on the GOOG stock.

The exact procedure outlined in the previous section is exactly the same. However, there is one wrinkle. In order to calculate the delta of the call option, the volatility of the underlyting must be given. Two methods are proposed to obtain the volatility:
- use historical volatility from past data
- use the implied volatility given by call option prices

The second option is in general more accurate and was chosen in order to calculate the volatility. This was calculated at the end of each day to get the updated delta value to be used in calculating the amount of GOOG stock shares to hedge.

The output of the C++ code is a table with the hedged portfolio value in between the user-specified dates.

### 3.1.  Compilation instructions

In order to compile both C++ scripts, please ensure the latest Boost package is installed. You can find more information about this [here](https://www.boost.org/users/download/).

To complete Section 1.1, please execute the following commands:

g++ mainPT1.cpp Option.cpp -l boost_date_time -I/usr/local/include/python3.7 -o mainPT1

./mainPT1

The output file is found in the 'hedgingErrors.csv' file.

To complete Section 2.1, please execute the following command:

g++ mainPT1.cpp Option.cpp -l boost_date_time -I/usr/local/include/python3.7 -o mainPT1

./mainPT2

The output file is found in the 'hedgingResults.csv' file.
