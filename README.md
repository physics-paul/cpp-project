This project seeks to analyze the a dynamic delta hedging strategy where a call option is sold and then delta hedged from a zero-cost hedged portfolio at the starting time. The call option is then dynamically hedged each day to remain net-delta neutral. The hedging error is then calculated and compared against the actual call option value for sample stock price paths using the Black-Scholes equation. Then, the hedging error is compared to the GOOG call option and stock price values.

This project is divided into 5 main parts:

### 1.1.  Testing the delta hedging using the Black-Scholes model

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
  <img width="400" height="300" src="https://raw.githubusercontent.com/physics-paul/cpp-project/master/image.pdf">
</p>
