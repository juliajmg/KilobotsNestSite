library(dplyr)
library(tidyr)
library(ggplot2)
library(stringr)

setwd("~/MEGA/KilobotsNestSelection/8_BeesNestSelectionModel/TimeInLoops/BeaconCode/")

df <- read.csv("beacon_debug.csv", sep = ",", header = FALSE)

df <- as.data.frame(str_split_fixed(df$V1, ", ", 2))

df$V1[df$V1 == "Co"] <- "Contador"
df$V1[df$V1=="Neighb"] <- "Neighbor ID"


#df$V1[df$V1 == "Messages Receives Received"] <- "Messages Received"

unique(df$V1)

#ids <- unique(filter(df, V1 == "ID"))

new.list <- list()
MessagesReceived <- vector()
ID <- vector()
NeighborID <- vector()
Counter <- vector()
Ticks <- vector()
Cycles <- vector()




for(i in 1:(length(df$V1)-10)){
  print(i)
  if(df$V1[i]== "Ticks" &  df$V1[i+1] == "Ciclos" & df$V1[i+2]== "Contador"){
    #MessagesReceived[i] <- df$V2[i]
    Ticks[i] <- df$V2[i]
    Cycles[i] <- df$V2[i + 1]
    Counter[i] <- df$V2[i + 2]
  } else {
    #MessagesReceived[i] <- NA
    Ticks[i] <- NA
    Cycles[i] <- NA
    Counter[i] <- NA
  }
}


#new.df <- data.frame(MessagesReceived= MessagesReceived, ID= ID, NeighborID= NeighborID,  Counter= Counter, Ticks = Ticks)


new.df <- data.frame(Ticks = Ticks, Cycles = Cycles, Counter= Counter)

new.df <- drop_na(new.df) 

new.df <- new.df %>% filter(Counter > 1000)

new.df$Counter <- as.numeric(new.df$Counter)
new.df$Ticks <- as.numeric(new.df$Ticks)
new.df$Cycles <- as.numeric(new.df$Cycles)
#new.df$MessagesReceived <- as.numeric(new.df$MessagesReceived)
#new.df <- new.df[4:length(new.df$MessagesReceived), ]


write.csv(new.df, "DebugOutput_BeaconID=17.csv", row.names = FALSE)



cycles.plot <- new.df %>% filter(Counter >= 1000 & Counter <= 1500) %>% 
  ggplot() + geom_line(aes(x = Ticks, y = Cycles))


