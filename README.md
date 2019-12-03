# cooleytukey fourier transform

The number of threads created is set in the transform function as the number of times the work is divided.
results for ryzen 2700 processor with 16384 (2^14) data points were roughly:

| t | threads  | time  |
| :-----: | :-: | :-: |
| 0 | 1 | 24ms |
| 1 | 2 | 11ms |
| 2 | 4 | 8ms |
| 3 | 8 | 6ms |
| 4 | 16 | 9ms |
| 5 | 32 | 13ms |
| 6 | 64 | 21ms |
