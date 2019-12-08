require(ggplot2)
t <- read.csv("result.csv", header=F)
X11()
p = ggplot() + 
  geom_line(aes(x = 1:30, y = (as.numeric(t[1,]))), color = "orange") +
  geom_line(aes(x = 1:30, y = (as.numeric(t[2,]))), color = "green") +
  geom_line(aes(x = 1:30, y = (as.numeric(t[3,]))), color = "red") +
  xlab('input value') +
  ylab('clock ticks/avg(binet clock ticks)') 
print(p)
message("Press Return To Continue")
invisible(readLines("stdin", n=1))
