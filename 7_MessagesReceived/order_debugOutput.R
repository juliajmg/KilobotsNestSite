library(dplyr)
library(tidyr)
library(ggplot2)
library(stringr)

setwd("~/MEGA/KilobotsNestSelection/7_MessagesReceived/")

df <- read.csv("BeaconInfoID=17.csv", sep = ",", header = FALSE)

df <- as.data.frame(str_split_fixed(df$V1, ", ", 2))

df$V1[df$V1 == "Co"] <- "Contador"
df$V1[df$V1=="Neighb"] <- "Neighbor ID"


#df$V1[df$V1 == "Messages Receives Received"] <- "Messages Received"

unique(df$V1)

ids <- unique(filter(df, V1 == "ID"))

new.list <- list()
MessagesReceived <- vector()
ID <- vector()
NeighborID <- vector()
Counter <- vector()
Ticks <- vector()
for(i in 1:(length(df$V1)-10)){
  print(i)
  if(df$V1[i] == "Messages Received" & df$V1[i+1] == "ID" & df$V1[i+2] == "Neighbor ID" & df$V1[i+3]== "Contador" & df$V1[i+4]== "Ticks"){
    MessagesReceived[i] <- df$V2[i]
    ID[i] <- df$V2[i+1]
    NeighborID[i] <- df$V2[i+2]
    Counter[i] <- df$V2[i + 3]
    Ticks[i] <- df$V2[i + 4]
  } else {
    MessagesReceived[i] <- NA
    ID[i] <- NA
    NeighborID[i] <- NA
    Counter[i] <- NA
    Ticks[i] <- NA
  }
}



for(i in 1:(length(df$V1)-10)){
  print(i)
  if(df$V1[i] == "ID" & df$V1[i+1] == "Neighbor ID" & df$V1[i+2]== "Contador" & df$V1[i+3]== "Ticks"){
    #MessagesReceived[i] <- df$V2[i]
    ID[i] <- df$V2[i]
    NeighborID[i] <- df$V2[i+1]
    Counter[i] <- df$V2[i + 2]
    Ticks[i] <- df$V2[i + 3]
  } else {
    #MessagesReceived[i] <- NA
    ID[i] <- NA
    NeighborID[i] <- NA
    Counter[i] <- NA
    Ticks[i] <- NA
  }
}

new.df <- data.frame(MessagesReceived= MessagesReceived, ID= ID, NeighborID= NeighborID,  Counter= Counter, Ticks = Ticks)


new.df <- data.frame(ID= ID, NeighborID= NeighborID,  Counter= Counter, Ticks = Ticks)

new.df <- drop_na(new.df) 

new.df$Counter <- as.numeric(new.df$Counter)
new.df$Ticks <- as.numeric(new.df$Ticks)
new.df$MessagesReceived <- as.numeric(new.df$MessagesReceived)
#new.df <- new.df[4:length(new.df$MessagesReceived), ]


write.csv(new.df, "DebugOutput_BeaconID=17.csv", row.names = FALSE)


mperticks <- new.df %>% group_by(ID, NeighborID) %>% summarise(TicksPassed = max(Ticks)-min(Ticks),IterationsPassed= max(Counter)-min(Counter)) %>%
  filter(IterationsPassed>0, TicksPassed >0)

hist.df <- mperticks %>% ggplot() + 
  geom_histogram(aes(x =  TicksPassed, y = ..density..), bins = 100, color = "white")


hist.df <- ggplot_build(hist.df)

hist.df <- as.data.frame(hist.df$data)

pdf <- hist.df %>% filter(density > 0) %>% ggplot() + geom_point(aes(x = x, y = density)) +
  scale_y_log10() + 
  scale_x_continuous(breaks= seq(0,35, 5))




new.sum <- new.df %>% group_by(Counter) %>% 
  summarise(MeanTicks = mean(Ticks), sdTicks = sd(Ticks), MeanMessages = mean(MessagesReceived), sdMessages = sd(MessagesReceived))



new.sum %>% filter(Counter < 250) %>% ggplot() + geom_point(aes(x = Counter, y = MeanTicks))


plot <- new.sum %>% filter(Counter <= 300) %>%
  ggplot() + 
  geom_point(aes(x= Counter, y = MeanTicks), color = "blue")+
  geom_errorbar(aes(x = Counter, ymin = MeanTicks - sdTicks,ymax = MeanTicks + sdTicks ), alpha = 0.3, size = 0.4, width = 1, color = "blue")


filter(new.sum, MeanTicks == 1000)

plot <- new.sum %>% filter(Counter <= 4000) %>%
  ggplot() + 
  geom_point(aes(x= Counter, y = MeanMessages), color = "blue")+
  geom_errorbar(aes(x = Counter, ymin = MeanMessages - sdMessages,ymax = MeanMessages + sdMessages), alpha = 0.3, size = 0.4, width = 1, color = "blue")




ticks_iterations <- new.df %>% 