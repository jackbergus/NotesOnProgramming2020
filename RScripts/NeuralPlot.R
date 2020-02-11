


library(rgl)

x <- y <- (0:100)/100

# First perceptron
p1 <- function(x,y) 1/(1+exp(-(x * 7.10607 + y * -6.50904 + 3.41603)))
# Second perceptron
p2 <- function(x,y) 1/(1+exp(-(x * -8.05875 + y * 8.086 + 4.31492)))
# Output perceptron
p3 <- function(x,y) 1/(1+exp(-(x * -8.65042 + y *-8.48615 + 12.8072)))

# Starting the plot!

# a) plotting the axes
axes3d()  
title3d('Perceptron plotting', 'sub', 'X', 'Y', 'Xor(X,Y)')

# b) plotting the surfaces
surface3d(x,y,outer(x,y,p1),col = "#E69F00")
surface3d(x,y,outer(x,y,p2),col = "#56B4E9")
# c) plotting the spheres
spheres3d(c(0,0,1,1),c(0,1,0,1),c(0,1,1,0),radius=c(0.1,0.1,0.1,0.1),color="red")

surface3d(x,y,outer(x,y,p3),col = "#009E73")


