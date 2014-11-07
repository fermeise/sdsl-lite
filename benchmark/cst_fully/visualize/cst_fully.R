source("../../basic_functions.R")

tc_config <- readConfig("../test_case.config", c("TC_ID","PATH","LATEX-NAME","URL"))

operations = data.frame(id=c("SLINK", "CHILD", "DEPTH", "PARENT"),
						caption=c("SLink", "Child", "SDepth", "Parent"))
rownames(operations)<-operations[["OP_ID"]]
colnames(operations)<-c("OP_ID", "LATEX-NAME")

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
table3 = c(
    "$CSA/FCST$",
    "$CSA/CST$")

for(i in 1:nrow(raw)) {
	data<-raw[i,]

    header = paste(header, c(
		tc_config[as.character(data[["TC_ID"]]), "LATEX-NAME"]
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

	table3 = paste(table3, c(
        sprintf("%.3f", data[["CSA_SIZE"]] / data[["FCST_SIZE"]]),
        sprintf("%.3f", data[["CSA_SIZE"]] / data[["CST_SIZE"]])
	), sep=" & ")
}

cat(header, "\\hline", sep="\\\\\n")
cat(table1, "\\hline", sep="\\\\\n")
cat(table2, "\\hline", sep="\\\\\n")
cat(table3, "\\hline", sep="\\\\\n")
cat("\\end{tabular}\n")

sink("fig-time.tex")

cat("\\begin{tabular}{llr|*{", nrow(raw), "}{r}}\n", sep="")
cat("\\hline\n")

header = c("Operation & &")
for(i in 1:nrow(raw)) {
	data<-raw[i,]
    header = paste(header, c(
		tc_config[as.character(data[["TC_ID"]]), "LATEX-NAME"]
	), sep=" & ")
}
cat(header, "\\hline", sep="\\\\\n")

format_string = "%.1e"

for(op_idx in 1:nrow(operations)) {
	op_name = as.character(operations[op_idx, "OP_ID"])
	table = c(
		paste(as.character(operations[op_idx, "LATEX-NAME"]), " & F & u", sep="", collapse=""),
		" & F & su",
		" & F & pu",
		" & C & u",
		" & C & su",
		" & C & pu")

	for(i in 1:nrow(raw)) {
		data<-raw[i,]

		table = paste(table, c(
			sprintf(format_string, data[[paste("FCST_", op_name, "_U_TIME", sep="", collapse="")]] / 1000000000),
			sprintf(format_string, data[[paste("FCST_", op_name, "_SU_TIME", sep="", collapse="")]] / 1000000000),
			sprintf(format_string, data[[paste("FCST_", op_name, "_PU_TIME", sep="", collapse="")]] / 1000000000),
			sprintf(format_string, data[[paste("CST_", op_name, "_U_TIME", sep="", collapse="")]] / 1000000000),
			sprintf(format_string, data[[paste("CST_", op_name, "_SU_TIME", sep="", collapse="")]] / 1000000000),
			sprintf(format_string, data[[paste("CST_", op_name, "_PU_TIME", sep="", collapse="")]] / 1000000000)
		), sep=" & ")
	}

	cat(table, "\\hline", sep="\\\\\n")
}

cat("\\end{tabular}\n")

sink(NULL)
