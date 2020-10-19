This project seeks to analyze the a dynamic delta hedging strategy where a call option is sold and then delta hedged from a zero-cost hedged portfolio at the starting time. The call option is then dynamically hedged each day to remain net-delta neutral. The hedging error is then calculated and compared against the actual call option value for sample stock price paths using the Black-Scholes equation. Then, the hedging error is compared to the GOOG call option and stock price values.

This project is divided into 5 main parts:

### 1.1.  Testing the delta hedging using the Black-Scholes model

In order to test the delta hedging implementation, a random sample of stock price paths was generated, assuming the stock price follows a log-normal distribution. The time was split into N equally-spaced time steps and 1000 sample paths were created.

Then, the call option values were generated for each stock sample path.

Finally, the correct number of stocks were bought at each time step to remain delta-neutral for the next time step. This was financed by borrowing cash, and the amount of stocks purchased and cash borrowed changed with each time step.

Since the delta-hedging is only valid for small stock price movements, there will be some error in hedging each period.

| --- | CAR(1)  | CAR(3) | CAR(5) | CAV(1) | CAV(3) | CAV (5) |
| Mean | -0.001508 |	-0.002782	| -0.003579 |	0.526526	| 1.206844	| 1.863234 | 
|Standard Deviation	|0.061269 |	0.096360 |	0.124626 |	1.513595 |	3.473083 |	6.329492 |
| Minimum |	-0.732448 |	-0.807893	| -0.934338 |	-8.344340	| -19.990874	| -27.465825 |
|25%	|-0.018018 |	-0.029812	| -0.043501	| -0.423825	| -1.007499 | -2.104361 |
|50%	|-0.000766 |	-0.001695	| -0.002122	| 0.387644	| 0.844627 |	1.365758 |
|75%	|0.014817	| 0.024814 |	0.035890 |	1.236411	| 2.974241	| 5.078070 |
| Maximum |	1.105913 | 1.936429 |	1.787982 |	12.774111 |	23.541118	| 42.558456|
