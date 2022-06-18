setwd("~/MEGA/KilobotsNestSelection/10_KilomboResults/Model4_RealKilobots/")

library(jsonlite)
library(tidyr)
library(dplyr)
library(ggplot2)
library(stringr)


files <- list.files(pattern = "*_N=037-deltat=*")

  files

coord_list <- list()

for(i in 1:length(files)){

  coord_df <- fromJSON(files[i], flatten = T)$bot_state

  ticks_df <- fromJSON(files[i], flatten = T)$ticks

  
  coord_df <- as.data.frame(do.call("rbind", coord_df))
  coord_df <- coord_df %>% group_by(ID) %>% mutate(Ticks = ticks_df)
  
  deltat <- str_extract(files[i], "deltat=.....")
  
  deltat <- str_remove(deltat, "deltat=")
  


  # Number of bots
  nbots <- str_extract(files[i], "N=...")
  q_one <- str_extract(files[i], "q1=......")
  q_one <- as.numeric(str_remove(q_one, "q1="))/as.numeric(deltat)
  q_two <- str_extract(files[i], "q2=......")
  q_two <- as.numeric(str_remove(q_two, "q2="))/as.numeric(deltat)
  p_one <- str_extract(files[i], "p1=..")
  p_one <- str_remove(p_one, "p1=")
  p_two <- str_extract(files[i], "p2=..")
  p_two <- str_remove(p_two, "p2=")
  lamb <- str_extract(files[i], "lambda=..")
  lamb <- str_remove(lamb, "lambda=")
  rep <- str_extract(files[i], "rep=...")
  rep <- as.numeric(str_remove(rep, "rep="))


  fname <- paste("BeesModel_",nbots,"_deltat=",deltat,"_p1=",p_one,"_p2=", p_two,"_q1=0",q_one,"_q2=0",q_two,"_lambda=",lamb,"_rep=0",rep,"_kilombo.csv", sep = "")
  
  write.csv(coord_df, fname, row.names = FALSE)         


}
  
