#' Worst-Pair-Fitting Damerau Levenshtein Distance
#' 
#' Split strings at spaces and find the distance of the worst fitting pair.
#'
#' @useDynLib dlsplit
#'
#' @param a \code{[character]} vector.
#' @param b \code{[character]} vector.
#' @param penalty \code{[numeric]} scalar. Penalty for using the worst-fit-pair instead of whole string.
#'
#' @section Details:
#' 
#' 
#'
#' @examples
#' 
#' dl_split(a,b)
#'
#' @export
dl_split <- function(a, b, penalty=0.1) {
  a <- enc2utf8( as.character(a) )
  b <- enc2utf8( as.character(b) )
  penalty <- as.numeric(penalty)
  result <- .Call("R_damerau_levenshtein_split", a, b, penalty)
  return(result)
}

#' Similarity version of dl_split
#' @export
dl_split_sim <- function(a, b, penalty=0.1) {
  return(1-dl_split(a,b,penalty))
}
