This project seeks to analyze the a dynamic delta hedging strategy where a call option is sold and then delta hedged from a zero-cost hedged portfolio at the starting time. The call option is then dynamically hedged each day to remain net-delta neutral. The hedging error is then calculated and compared against the actual call option value for sample stock price paths using the Black-Scholes equation. Then, the hedging error is compared to the GOOG call option and stock price values.

This project is divided into 5 main parts:

### 1.1.  Testing the delta hedging using the Black-Scholes model

In order to test the delta hedging implementation, a random sample of stock price paths was generated, assuming the stock price follows a log-normal distribution. The time was split into N equally-spaced time steps and 1000 sample paths were created.

Then, the call option values were generated for each stock sample path.

Finally, the correct number of stocks were bought at each time step to remain delta-neutral for the next time step. This was financed by borrowing cash, and the amount of stocks purchased and cash borrowed changed with each time step.

Since the delta-hedging is only valid for small stock price movements, there will be some error in hedging each period.

The traded cash-flows look like:

| Cash Flows |      t=0 |      t=1 |      t=2 |      --- |      t=T |
| Beginning of Day |            |          |          |          |          |          |
| Sell Call option |   ---       | -C0         |          |      --- |          |
| Buy Stock        |   ---       | delta0*S0         |          |      --- |          |
| Borrow Cash      |   ---     | (C0 - delta0*S0)         |          |      --- |          |
| End of Day       |            |          |          |          |          |          |
| Sell Call option |   +C0       |  -C1        |          |      --- |          |
| Buy Stock        |   -delta0*S0 | delta0*S1          |          |      --- |          |
| Borrow Cash      |   (delta0*S0 - C0) | (C0 - delta0*S0)*exp(r dt)          |          |      --- |          |
| Adjustments      |            |          |          |          |          |          |
| Buy Stock        | ---        | (delta1 - delta0)*S1         |          |      --- |          |
| Borrow Cash      | ---        | -(delta1 - delta0)*S1         |          |      --- |         |
| Portfolio Value  |            |          |          |          |          |          |
| Short Call option |  -C0       |          |          |      --- |          |
| Stock            | delta0*S0         |          |          |      --- |          |
| Borrowed Cash    | (C0 - delta0*S0)         |          |          |      --- |          |
| Total            | 0         |          |          |          |           |
