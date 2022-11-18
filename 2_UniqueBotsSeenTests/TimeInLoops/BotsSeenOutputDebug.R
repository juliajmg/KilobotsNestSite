library(stringr)
library(dplyr)
library(tidyr)
library(ggplot2)
library(ggpubr)
library(ggthemes)
setwd("~/MEGA/KilobotsNestSelection/2_UniqueBotsSeenTests/TimeInLoops/files/")


files <- list.files(pattern = "*time_*")


df.list <- list()

for(i in 1:length(files)){
  df <- read.csv(files[i], header = FALSE, sep = ":")
  df$dt <- str_extract(files[i], "400|800|1600")
  
  df <- df %>% filter(V1 != "bot_id")

  df.list[[i]] <- df
  
}

m.df <- as.data.frame(do.call("rbind", df.list))



unique(m.df$V1)


new.list <- list()
ID <- vector()
t <- vector()
cycle <- vector()
bots_seen <- vector()
BotDistance <- vector()
signals_total <- vector()

#while((k %% 5  == 1) & (i < length(m.df$V1)-5)){
for(i in 1:(length(m.df$V1)-6)){
  print(i)
  if(m.df$V1[i] == "ID" & m.df$V1[i+1] == "t" & m.df$V1[i+2] == "cycle" & m.df$V1[i+3] == "bots_seen" & m.df$V1[i+4]== "BotDistance" &  m.df$V1[i+5]== "signals_total"){
    ID[i] <- m.df$V2[i]
    t[i] <- m.df$V2[i+1]
    cycle[i] <- m.df$V2[i+2]
    bots_seen[i] <- m.df$V2[i+3]
    BotDistance[i] <- m.df$V2[i + 4]
    signals_total[i] <- m.df$V2[i + 5]
    
  } else {
    ID[i] <- NA
    t[i] <- NA
    cycle[i] <- NA
    bots_seen[i] <- NA
    BotDistance[i] <- NA
    signals_total[i] <- NA
    
  }
}


new.df <- data.frame(ID= as.numeric(ID), t = as.numeric(t), cycle= as.numeric(cycle),  bots_seen = as.numeric(bots_seen), BotDistance = as.numeric(BotDistance),  signals_total = as.numeric(signals_total))

new.df <- drop_na(new.df) 

new.df <- new.df %>% group_by(ID, cycle) %>% mutate(time = row_number())


new.df <- new.df %>% select(!BotDistance) 
new.df <- filter(new.df, t > 0)

BSmeantotal.df <- new.df %>% ungroup()%>%  filter(cycle > 2) %>% summarise(BS_IDmean = mean(bots_seen), BS_IDsd = sd(bots_seen))
# Mean bots seen per cycle 
BSmean.df <- new.df %>% group_by(ID) %>% filter(cycle > 2) %>% summarise(BS_IDmean = mean(bots_seen), BS_IDsd = sd(bots_seen))

BSmean.plot <- BSmean.df %>% ggplot(aes(x= as.factor(ID)))+ 
  geom_bar(aes(y = BS_IDmean),position=position_dodge(), stat="identity", width = 0.5, fill = "turquoise4", alpha = 0.8) + 
  geom_errorbar(aes(ymin=BS_IDmean - BS_IDsd, ymax = BS_IDmean + BS_IDsd), width = 0.2, color = "orange")+ 
  theme_few() + 
  theme(text = element_text(family = "Times New Roman"), plot.subtitle = element_text(size = 16, face = "bold"))+
  labs(subtitle = "A", x = "Center bot ID", y = expression("Mean Bots Seen per "*Delta*"t cycle"))

ggsave("MeanBotsSeenPerIDInCycle.png", width = 4, height = 3, units = "in")


# If we fit a linear model to log-log plot. 
newdflm.df <- new.df %>% filter(signals_total > 0, signals_total < 15)
BSloglog_lm <- lm(log10(newdflm.df$signals_total)~ log10(newdflm.df$time))


# If we fit a linear model to semilog plot 

newdflm.df <- new.df %>% filter(signals_total > 0, signals_total < 15)
BSloglog_lm <- lm(log10(newdflm.df$signals_total)~ log10(newdflm.df$time))

# Bots seen in time 

BStime.df <- new.df %>% filter(cycle > 2, time <= 400)  %>% group_by(ID, time) %>%  summarise(BS_CycleMean = mean(signals_total), BS_CycleSD = sd(signals_total)) %>%
  ggplot(aes(x = time, y =BS_CycleMean, color = as.factor(ID))) + geom_line() + 
  #geom_ribbon(aes(ymin = BS_CycleMean - BS_CycleSD, ymax = BS_CycleMean + BS_CycleSD, fill = as.factor(ID)), alpha = 0.3, color= "transparent")+
  theme_few()+
  geom_abline(slope = BSloglog_lm$coefficients[[2]], intercept =BSloglog_lm$coefficients[[1]],color = "orange", linetype = "dashed")+
  scale_x_continuous(trans = "log10") + 
  scale_y_continuous(trans = "log10") + 
  #scale_y_continuous(breaks = seq(0, 30, 5)) + 
  labs(x = "t (loops)", y = "Mean Bots Seen", color = "Center bot ID",  fill = "Center bot ID")

ggsave("MeanBotsSeenPerIDinTime_loglog.png", width = 6, height = 4, units = "in")

# If we fit a linear model to semilog plot 

newdflm.df <- new.df %>% filter(time > 10, time <= 80)
BSsemilog_lm <- lm(newdflm.df$signals_total~ log10(newdflm.df$time))

BStime.df <- new.df %>% filter(cycle > 2, time <= 400)  %>% group_by(ID, time) %>%  summarise(BS_CycleMean = mean(signals_total), BS_CycleSD = sd(signals_total)) %>%
  ggplot(aes(x = time, y =BS_CycleMean, color = as.factor(ID))) + geom_line() + 
  geom_ribbon(aes(ymin = BS_CycleMean - BS_CycleSD, ymax = BS_CycleMean + BS_CycleSD, fill = as.factor(ID)), alpha = 0.3, color= "transparent")+
  theme_few()+
  theme(text = element_text(family = "Times New Roman"), plot.subtitle = element_text(size = 16, face="bold"))+
  #geom_abline(slope = BSsemilog_lm$coefficients[[2]], intercept =BSsemilog_lm$coefficients[[1]],color = "orange", linetype = "dashed")+
  scale_x_continuous(trans = "log10") + 
  scale_y_continuous(breaks = seq(0, 30, 5)) + 
  labs(subtitle = "B", x = "time (iterations)", y = "Mean Bots Seen", color = "Center bot ID",  fill = "Center bot ID")


ggsave("MeanBotsSeenPerIDinTime_semilog.png", width = 5, height = 3, units = "in")



# ---- Load data without time column ----


files <- list.files(pattern = "*progressive_*")


df.list <- list()

for(i in 1:length(files)){
  df <- read.csv(files[i], header = FALSE, sep = ":")
  df$dt <- str_extract(files[i], "400|800|1600")
  
  #df <- df %>% filter(V1 != "bot_id" & V1 != "signals_total")
  
  df.list[[i]] <- df
  
}
bst.df <- as.data.frame(do.call("rbind", df.list))



unique(bst.df$V1)


new.list <- list()
ID <- vector()
cycle <- vector()
bots_seen <- vector()
BotDistance <- vector()

for(i in 1:(length(bst.df$V1)-5)){
  print(i)
  if(bst.df$V1[i] == "ID" & bst.df$V1[i+1] == "cycle" & bst.df$V1[i+2] == "bots_seen" & bst.df$V1[i+3]== "BotDistance"){
    ID[i] <- bst.df$V2[i]
    cycle[i] <- bst.df$V2[i+1]
    bots_seen[i] <- bst.df$V2[i+2]
    BotDistance[i] <- bst.df$V2[i + 3]
  } else {
    ID[i] <- NA
    cycle[i] <- NA
    bots_seen[i] <- NA
    BotDistance[i] <- NA
  }
}

progress.df <- data.frame(ID= as.numeric(ID),  cycle= as.numeric(cycle),  bots_seen = as.numeric(bots_seen), BotDistance = as.numeric(BotDistance))

progress.df <- drop_na(progress.df) 

progress.df <- progress.df %>% group_by(ID, cycle) %>% mutate(time = row_number())

BStime.plot <- progress.df %>% ggplot(aes(x = time,y = si))
