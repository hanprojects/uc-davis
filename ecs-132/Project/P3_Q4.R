## Read data from .csv file
data = read.csv("Traffic_data_orig.csv", header=TRUE)

## Get message information
secret_message = "this is a secret message"
message_length = nchar(secret_message)
message_bits = as.integer(rawToBits((charToRaw(secret_message))))

## Get lists for delay times, inter packet delays (in total) and covert packet stream
delay_times = numeric(message_length * 8)
inter_packet_delays = numeric(length(data$Time) - 1)
covert_packet_stream = numeric(length(data$Time) - 1)

## Get inter packet delay times
for (i in (1:(length(data$Time) - 1)))
{
  inter_packet_delays[i] = data$Time[i+1] - data$Time[i]
}
# print(inter_packet_delays)

## Get median, min, max of the inter packet delays
min = min(inter_packet_delays)
m = median(inter_packet_delays)
max = max(inter_packet_delays)

## Get delay times by message bits
curr_index = 1

for (i in 0:(message_length-1))
{
  for (j in 1:8) 
  {
    if (message_bits[i*8+j] == 0)
    {
      delay_times[curr_index] = runif(1, min, m)
    }
    else
    {
      delay_times[curr_index] = runif(1, m, max)
    }
    
    curr_index = curr_index + 1
    j = j - 1
  }
}
# print(delay_times)

## Get covert packet stream
updated_covert_packet_stream = inter_packet_delays

for (i in 1:(message_length*8))
{
  updated_covert_packet_stream[i] = delay_times[i]
}
# print(updated_covert_packet_stream)

par(mfrow=c(2,1))
hist(inter_packet_delays, main="Inter Packet Delays", xlab="Delay")
hist(updated_covert_packet_stream, main="Updated Covert Packet Stream", xlab="Delay")