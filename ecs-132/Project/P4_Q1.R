i = c(2, 6, 10, 14, 18)
m = c(16, 32)
B = 20
sample_num = 500
lambda = 1

## Get random bits size of m for the secret message
get_random_secret_message_bits = function(m)
{
  message_bits = numeric(m)
  
  for (i in 1:m)
  {
    message_bits[i] = sample(c(0,1), 1)
  }
  
  return(message_bits)
}

## Get time for each packet source generation using IPD distribution
get_packet_times = function(idp, expo)
{
  packet_times = numeric(expo)
  
  for (i in 1:expo)
  {
    if (i == 1)
    {
      packet_times[i] = 0
    }
    
    else
    {
      packet_times[i] = packet_times[i-1] + idp[i-1]
    }
  }
  
  return(packet_times)
}

## Get overflow and underflow of the packet stream under IPD distribution
get_overflow_underflow = function(m, i)
{
  curr_buffer = i
  message_bits = get_random_secret_message_bits(m)
  
  worst_case = 49
  
  idp_expo = rexp(worst_case, lambda)
  packet_times = get_packet_times(idp_expo, worst_case+1)
  next_sent_time = packet_times[i]
  
  sender_delay = rexp(worst_case, lambda)
  med = median(sender_delay)
  min = min(sender_delay)
  max = max(sender_delay)
  message_bit_delay = rexp(worst_case, lambda)
  
  underflow = 0
  overflow = 0
  
  next_buffer = i+1
  
  for (j in 1:m)
  {
    if (message_bits[j] == 0)
    {
      delay = runif(1, min, med)
      message_bit_delay[j] = delay
    }
    
    else if (message_bits[j] == 1)
    {
      delay = runif(1, med, max)
      message_bit_delay[j] = delay
    }
    
    next_sent_time = next_sent_time + delay
    
    # Update buffer
    if (next_sent_time <= packet_times[next_buffer])
    {
      curr_buffer = curr_buffer - 1
      
      if (curr_buffer < 1)
      {
        underflow = 1
        break
      }
    }
    
    else
    {
      curr_buffer = curr_buffer + 1
      
      if (curr_buffer > B)
      {
        overflow = 1
        break
      }
    }
    
    next_buffer = next_buffer + 1
  }
  
  return(c(overflow, underflow))
}

## Get results of overflow-underflow probabilities
get_prob_results = function(m, i)
{
  overflow_probs = numeric(sample_num)
  underflow_probs = numeric(sample_num)
  
  sample_count = 1
  
  for (j in 1:sample_num)
  {
    overflow = 0
    underflow = 0
    over_under = get_overflow_underflow(m, i)
    
    # Update overflow and underflow
    overflow = overflow + over_under[1]
    underflow = underflow + over_under[2]
    
    overflow_probs[j] = overflow/sample_count
    underflow_probs[j] = underflow/sample_count
    
    sample_count = sample_count + 1
  }
  
  return(c(mean(overflow_probs), mean(underflow_probs)))
}

## GET PROBABILITIES OF OVERFLOW-UNDERFLOW of EACH BIT AND BUFFER
prob_16_2 = get_prob_results(16,2)
prob_16_6 = get_prob_results(16,6)
prob_16_10 = get_prob_results(16,10)
prob_16_14 = get_prob_results(16,14)
prob_16_18 = get_prob_results(16,18)
prob_32_2 = get_prob_results(32,2)
prob_32_6 = get_prob_results(32,6)
prob_32_10 = get_prob_results(32,10)
prob_32_14 = get_prob_results(32,14)
prob_32_18 = get_prob_results(32,18)

## GET TABLE OF OVERFLOW PROBABILITIES
table_overflow = matrix(list(), nrow = 2, ncol = 5)
table_overflow[1, 1] = prob_16_2[1]
table_overflow[1, 2] = prob_16_6[1]
table_overflow[1, 3] = prob_16_10[1]
table_overflow[1, 4] = prob_16_14[1]
table_overflow[1, 5] = prob_16_18[1]
table_overflow[2, 1] = prob_32_2[1]
table_overflow[2, 2] = prob_32_6[1]
table_overflow[2, 3] = prob_32_10[1]
table_overflow[2, 4] = prob_32_14[1]
table_overflow[2, 5] = prob_32_18[1]
print(table_overflow)

## GET TABLE OF UNDERFLOW PROBABILITIES
table_underflow = matrix(list(), nrow = 2, ncol = 5)
table_underflow[1, 1] = prob_16_2[2]
table_underflow[1, 2] = prob_16_6[2]
table_underflow[1, 3] = prob_16_10[2]
table_underflow[1, 4] = prob_16_14[2]
table_underflow[1, 5] = prob_16_18[2]
table_underflow[2, 1] = prob_32_2[2]
table_underflow[2, 2] = prob_32_6[2]
table_underflow[2, 3] = prob_32_10[2]
table_underflow[2, 4] = prob_32_14[2]
table_underflow[2, 5] = prob_32_18[2]
print(table_underflow)