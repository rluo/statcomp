#!/usr/local/bin/Rscript

# docker run -ti --rm -v "$PWD":/root -w /root -u root rossiluo/autograder Rscript --vanilla debugger.R prob_debugger_results.RData  Your_R_Function_file.R
library(peakRAM)
args <- commandArgs(trailingOnly = TRUE)

if (length(args) < 2) {
    stop("Please specify the *_debug_results.R and submission files.")
}

db_results_file <- args[1]
sub_file_name <- args[2]

# db_results_file <- "len_one_debugger_results.RData"
# sub_file_name <- "hw2_func.R"

# not real solutions, use submission instead
sol_env <- new.env()
load(db_results_file, sol_env)

# prob func names
prob_name <- get("prob_name", sol_env)

## load submission file
# sub_file_name <- "test_sub.R"
sub_env <- new.env()
loading_time_sec <- system.time(sys.source(sub_file_name, sub_env))[3]

author <- get("author", sub_env)


sub_fun <- sub_env[[prob_name]] # get(p_name, sub_env)
# cannot run
# input_case_lt <- sol_env[[paste0(p_name, "_input_lt")]] ## get( paste0(p_name, "_input_lt" ) , sol_env)
input_case_lt <- get("input_case_lt", sol_env)
output_case_lt <- get("output_case_lt", sol_env)

test_pass <- rep(FALSE, length(input_case_lt))
total_run_time_sec <- 0
ram_used <- 0
for (i in 1:length(input_case_lt)) {
    case <- input_case_lt[[i]]
    case_ram <- 1e8 # large number
    total_run_time_sec <- total_run_time_sec + system.time({
        setTimeLimit(60) # 10 seconds running time limit
        case_ram <- peakRAM(re <- sub_fun(case))[4]
    })[3]
    ram_used <- ram_used + case_ram
    val_comp <- all.equal(output_case_lt[[i]], re)
    val_comp <- ifelse(is.logical(val_comp), val_comp, FALSE)
    test_pass[i] <- val_comp
}

gc_re <- gc(verbose = FALSE, reset = FALSE)
prob_ram <- gc_re[1, 6] + gc_re[2, 6]

re_df <- data.frame(author = author, prob_name = prob_name, pass_rate = mean(test_pass), time = as.numeric(total_run_time_sec + loading_time_sec), ram = as.numeric(ram_used + prob_ram))
print(re_df)
print(get("metric_lt", sol_env))
