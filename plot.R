require(ggplot2)
t <- read.csv("result.csv", header=F)
X11()

dt <- data.frame(algorithm=factor(rep(c("Memoization (clear)", "Memoization (no clear)", "Recursive"), each = 30)),
           clocks=c(log(as.numeric(t[1,])), log(as.numeric(t[2,])), log(as.numeric(t[3,]))),
           time=c(1:30,1:30,1:30))

p = ggplot(dt, aes(x=time, colour=algorithm, y=clocks)) +
  xlab('input value, n') +
  ylab('log(clock ticks/avg(binet clock ticks)) lof(T(n))') +
  geom_line()
print(p)
message("Press Return To Continue")
invisible(readLines("stdin", n=1))
