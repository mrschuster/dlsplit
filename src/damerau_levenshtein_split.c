
#include <R.h>
#include <Rdefines.h>
#include "utils.h"


unsigned int damerau_levenshtein(const unsigned int *word1, int len1, const unsigned int *word2, int len2) {
//     Rprintf("\ndam %d %d\n", len1, len2);
    unsigned int *prevprev = calloc(len2 + 1, sizeof(unsigned int));
    unsigned int *prev = calloc(len2 + 1, sizeof(unsigned int));
    unsigned int *cur = calloc(len2 + 1, sizeof(unsigned int));
    unsigned int i, j;

    /* strip common prefixes */
    while (len1 > 0 && len2 > 0 && (word1[0] == word2[0]))
        word1++, word2++, len1--, len2--;

    /* handle degenerate cases */
    if (!len1) return len2;
    if (!len2) return len1;
    
    for (j = 0; j < len2 + 1; j++)
        prev[j] = j;

    for (i = 1; i <= len1; i++) {
        cur[0] = i;
        for (j = 1; j <= len2; j++) {
            cur[j] = MIN(prev[j]+1, cur[j-1]+1); // ins and del
            cur[j] = MIN(cur[j], prev[j-1] + (word1[i-1] == word2[j-1] ? 0 : 1) ); // sub resp. equal
            
            // swap
            if (i>1 && j>1 && (word1[i-2] == word2[j-1]) && (word1[i-1] == word2[j-2])) {
              cur[j] = MIN(cur[j], prevprev[j-2]+1);
            }
        }
        unsigned int *temp = prevprev;
        prevprev = prev;
        prev = cur;
        cur = temp;
    }
    int res = prev[len2];
    free(prevprev);
    free(prev);
    free(cur);
    return res;
}

int next_token_length(const unsigned int *arr, int len, const unsigned int e) {
  int pos=0;
  while (pos<len && arr[pos] != e)
    pos++;
  return pos;
}

int compare_function(const void *a, const void *b) {
  double *x = (double *) a;
  double *y = (double *) b;
  if (*x < *y) return -1;
  else if (*x > *y) return 1; return 0;
}

double damerau_levenshtein_split(const unsigned int *word1, int len1, const unsigned int *word2, int len2, double penalty) {
  const unsigned int delim = ' ';

  int parts1 = 1;
  for (int i=0; i<len1; i++) {
    if (word1[i]==delim)
      parts1++;
  }
    
  int parts2 = 1;
  for (int i=0; i<len2; i++) {
    if (word2[i]==delim)
      parts2++;
  }

  int len1cp = len1;
  const unsigned int *word1cp = word1;

  int pairs=0;
  double *pair_distances = (double *) malloc(parts1 * parts2 * sizeof(double));
  while (len1cp > 0) {
    if (word1cp[0]==delim) {
      word1cp++; len1cp--;
      continue;
    }
    int partlen1 = next_token_length(word1cp, len1cp, delim);
    
    int len2cp = len2;
    const unsigned int *word2cp = word2;
    while (len2cp > 0) {
      if (word2cp[0]==delim) {
        word2cp++; len2cp--;
        continue;
      }
      int partlen2 = next_token_length(word2cp, len2cp, delim);
      pair_distances[pairs++] = (double) damerau_levenshtein(word1cp, partlen1, word2cp, partlen2) / MAX(partlen1, partlen2);

      len2cp -= (partlen2+1);
      word2cp += (partlen2+1);
    }
    
    len1cp -= (partlen1+1);
    word1cp += (partlen1+1);
  }
  
  qsort(pair_distances, pairs, sizeof(double), compare_function);
  double mindist = pair_distances[ MIN(parts1,parts2)-1 ];
//   for (int i=0;i<pairs;i++) {
//     printf("Q: %lf\n", pair_distances[i]);
//   }
  //printf("M: %lf\n", mindist);
  if (pairs>1) {
    double dist = (double) damerau_levenshtein(word1, len1, word2, len2) / MAX(len1, len2);
    mindist = MIN(mindist + penalty, dist);
  }

  free(pair_distances);
  
  return mindist;
}

SEXP R_damerau_levenshtein_split(SEXP a, SEXP b, SEXP penalty){
  if ( length(a) != length(b) ){
    error("Vectors must be of equal length.\n");
  }
//   Rprintf("penalty %lf\n",asReal(penalty));
  
  int n = length(a);
  
  // output vector
  SEXP yy;
  PROTECT(yy = allocVector(REALSXP, n));
  double *y = REAL(yy);

  for (int i=0; i<n; i++) {
    const char *str1 = CHAR(STRING_ELT(a,i));
    const char *str2 = CHAR(STRING_ELT(b,i));
    unsigned int *int1 = (unsigned int *) malloc(( 1L + strlen(str1)) * sizeof(int));
    unsigned int *int2 = (unsigned int *) malloc(( 1L + strlen(str2)) * sizeof(int));
    int len1 = utf8_to_int( str1, int1 );
    int len2 = utf8_to_int( str2, int2 );
    if ( len1 < 0 || len2 < 0 ){
      error("Encountered byte sequence not representing an utf-8 character.\n");
    }
    y[i] = damerau_levenshtein_split(int1, len1, int2, len2, asReal(penalty));
    free(int1);
    free(int2);
  }
  
  UNPROTECT(1);
  if (n < 0 ) error("Unable to allocate enough memory");
  return(yy);
}
