


N <- function(x) { 
  m <- c(0.5,0.5)
  s <- c(sqrt(3),sqrt(3))
  return ((x-m)*s)
}

K <- function(x,y) {
  gamma <- 0.15625
  return ( exp(-gamma *norm(x-y,"2")^2))
}

alphas <- c(-7.14094, 7.14094, -7.14094, 7.14094)
vectors <- list(c(0,0),c(1,0),c(1,1),c(0,1))
NV <- lapply(vectors, N)
R <- function (x,y) sum(unlist(lapply(1:length(alphas), function(i) alphas[i] * K(N(c(x,y)), unlist(NV[i])))))




library(rgl)

x <- y <- (0:100)/100
M <- matrix(0, nrow=101, ncol=101)

for (i in 1:101) {
  for (j in 1:101) {
    M[i,j] <- R(x[i],y[j])
  }
}

# Starting the plot!

# a) plotting the axes
axes3d()  
title3d('SVM with RBF', 'sub', 'X', 'Y', 'Xor(X,Y)')

# b) plotting the surfaces
surface3d(x,y,M,col = "#E69F00")
# c) plotting the spheres
spheres3d(c(0,0,1,1),c(0,1,0,1),c(0,1,1,0),radius=c(0.1,0.1,0.1,0.1),color="red")
