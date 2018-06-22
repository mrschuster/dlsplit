# dlsplit
Compute Damerau Levenshtein Distance for worst fitting pair.

#### Installation

Easy installation via devtools. If you don't have them yet, install them via
```r
install.packages("devtools")
```
in the `R` console. Then
```r
devtools::install_github("mrschuster/dlsplit")
```

#### Usage
```r
library("dlsplit")
dl_split("Hans Meier", "Maier Hans")
```
