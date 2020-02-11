library(sqldf)

## reading the file form the 
t <- read.csv("data/cr_data_1535999786739.csv", sep="^")

# 
players1 <- sqldf(paste(c("select distinct ", paste(names(t)[4:105], collapse=", "), " from t"), collapse=" "))
players2 <- sqldf(paste(c("select distinct ", paste(names(t)[106:207], collapse=", "), " from t"), collapse=" "))
players <- sqldf("select * from players1 union select * from players2")

## human_deck_P1 --> carte in possesso di ogni signolo giocatore
## Association rules --> determinare quali sono le configurazioni più probabili di carte 