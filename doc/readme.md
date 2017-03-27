I'm taking these values from the analysis done in the report.

I attach the experiments with the appropriate constants that I found best for the experiment.

These values assume a Jacobi-like master, but may not be guaranteed to be stable because of some nuances that you may have in the algorithm (also, the finite representation was not taken into account).

Stable:
m1 = m2 = c1 = c2 = d1 = cc = dc = 1
d2 = 2
fixed cosim step_size = 0.001

Unstable:
m1 = m2 = c1 = c2 = d1 = cc = 1
d2 = 2
dc = from 100 onwards (6E6 will make it unstable for sure!)
fixed cosim step_size = 0.001
