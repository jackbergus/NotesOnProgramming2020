library(PerformanceAnalytics)
library(ggcorrplot)
data <- read.csv("starcraft.csv")

## Plotting the heatmap of the correlations
pdf("starcraft_plot_all_corr.pdf") 
ggcorrplot(cor(data))
dev.off()

pdf("starcraft_plot_relevant_corr.pdf")
# projecting the values to the most relevant ones: getting more information
proj <- data[,c(2,6,7,8,9,10,11,12,13,14)]
chart.Correlation(proj)
dev.off()

library(caret)
library(rpart)
library(rpart.plot)
tree <- rpart(data$LeagueIndex ~ ., data=data)

colMax()

prob <- function(filter, class) sum(data[filter, c(2)] == class)/length(data[filter, c(2)])
