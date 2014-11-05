source("../../basic_functions.R")

tc_config <- readConfig("../test_case.config", c("TC_ID","PATH","LATEX-NAME","URL"))

raw <- data_frame_from_key_value_pairs("../results/all.txt")

sink("fig-space.tex")

cat("\\begin{tabular}{l|*{", nrow(raw), "}{r}}\n", sep="")
cat("\\hline\n")

header = c("")
table1 = c(
    "$\\sigma$",
    "$n/2^{20}$",
    "$|T|/2^{20}$",
    "$\\delta$",
    "$|T|/|S|$")
table2 = c(
    "FCST (MB)",
    "CST (MB)",
    "CSA (MB)")

for(i in 1:nrow(raw)) {
	data<-raw[i,]

    header = paste(header, c(
		tc_config[data[["TC_ID"]], "LATEX-NAME"]
	), sep=" & ")

	table1 = paste(table1, c(
		data[["ALPHABET_SIZE"]],
		sprintf("%.1f", data[["DATA_SIZE"]] / (1024 * 1024)),
		sprintf("%.1f", data[["TREE_SIZE"]] / (1024 * 1024)),
		data[["SAMPLING_FACTOR"]],
		format(round(data[["TREE_SIZE"]] / data[["SAMPLED_TREE_SIZE"]], 0), big.mark=",")
	), sep=" & ")

	table2 = paste(table2, c(
        sprintf("%.2f", data[["FCST_SIZE"]] / (1024 * 1024)),
        sprintf("%.2f", data[["CST_SIZE"]] / (1024 * 1024)),
        sprintf("%.2f", data[["CSA_SIZE"]] / (1024 * 1024))
	), sep=" & ")
}

cat(header, "\\hline", sep="\\\\\n")
cat(table1, "\\hline", sep="\\\\\n")
cat(table2, "\\hline", sep="\\\\\n")
cat("\\end{tabular}\n")

sink(NULL)
