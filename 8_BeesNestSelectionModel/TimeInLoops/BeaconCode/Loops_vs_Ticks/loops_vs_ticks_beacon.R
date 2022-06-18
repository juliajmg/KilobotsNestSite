library(dplyr)
library(tidyr)
library(ggplot2)
library(ggthemes)
setwd("~/MEGA/KilobotsNestSelection/8_BeesNestSelectionModel/TimeInLoops/BeaconCode/Loops_vs_Ticks/")


df <- read.csv("loops_vs_ticks.csv", sep = ":")


colnames(df) <- c("parameter", "value")

df <- df %>% filter(parameter=="Contador" | parameter == "Ticks") %>% group_by(parameter) %>% mutate(ID = row_number())


df <- df %>% spread(parameter, value)


df %>% ggplot() + 
  geom_line(aes(x = Contador, y = Ticks), color = "darkblue")  + 
  geom_vline(xintercept = c(400, 800, 1600, 3200), linetype = "dashed", color = "darkgrey")+
  scale_x_continuous(breaks = seq(200, 4000, 200))+
  scale_y_continuous(breaks = seq(100, 1300, 100)) +
  theme_few()

lm(Contador~Ticks, df)



#iteración = 0.2878*ticks
# 1 tick = 3.474635*iteración


#for 400 iterations i have 115 ticks 