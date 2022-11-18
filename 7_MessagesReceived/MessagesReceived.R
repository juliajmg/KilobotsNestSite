setwd("~/MEGA/KilobotsNestSelection/7_MessagesReceived/files/")


files <- list.files(pattern = "*.csv")


df.list <- list()

for(i in 1:length(files)){
  df <- read.csv(files[i])
  df$neighbors <- str_extract(files[i], "5|10")
  df.list[[i]] <- df
  
}

m.df <- as.data.frame(do.call("rbind", df.list))


m.time.plot <- m.df %>% ggplot() + 
  geom_line(aes(x = Counter, y = MessagesReceived, color = as.factor(ID)), size = 1) + 
  theme_few() + 
  geom_vline(xintercept = 800, linetype = "dashed", color = "darkgrey") + 
  scale_y_continuous(breaks = seq(0, 1000, 25))+
  labs(x = "t (loops)", y = "Mean messages received", color = "Number of \n neighbors")+
  scale_color_d3() +
  facet_wrap(.~neighbors)

mmean.df <- m.df %>% group_by(ID) %>% filter(Counter >= 1000 & Counter <= 3000) %>% group_by(Counter, neighbors) %>% 
  summarise(MeanMessages = mean(MessagesReceived)) %>% group_by(neighbors) %>% 
  mutate(Counter = Counter - Counter[1], MeanMessages = MeanMessages - MeanMessages[1]) %>%
  group_by(Counter, neighbors) %>% mutate(MeanMessagesPerNeighbor = MeanMessages/as.numeric(neighbors))

m.time.plot <- mmean.df %>% ggplot() + 
  geom_line(aes(x = Counter, y = MeanMessages, color = neighbors), size = 1) + 
  theme_few() + 
  geom_vline(xintercept = 800, linetype = "dashed", color = "darkgrey") + 
  scale_y_continuous(breaks = seq(0, 1000, 50))+
  labs(x = "t (loops)", y = "Mean messages received", color = "Number of \n neighbors")+
  scale_color_d3()+
  geom_hline(yintercept = mmean.df$MeanMessages[mmean.df$Counter == 800], linetype = "dashed", color = "darkgrey")
  

ggsave("MeanMessagesReceived_vs_iterations.png", width = 12, height = 10, units = "cm")


mn.time.plot <- mmean.df %>% ggplot() + 
  geom_line(aes(x = Counter, y = MeanMessagesPerNeighbor, color = neighbors), size = 1) + 
  theme_few() + 
  geom_vline(xintercept = 800, linetype = "dashed", color = "darkgrey") + 
  scale_y_continuous(breaks = seq(0, 500, 10))+
  labs(x = "t (loops)", y = "Mean messages received from one neighbor", color = "Number of \n neighbors")+
  scale_color_d3()+
  geom_hline(yintercept = c(mmean.df$MeanMessagesPerNeighbor[mmean.df$Counter == 800]), linetype = "dashed", color = "darkgrey")



ggsave("MeanMessagesReceivedPerNeighbor_vs_iterations.png", width = 12, height = 10, units = "cm")
