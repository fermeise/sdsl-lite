source("../../basic_functions.R")

tc_config <- readConfig("../test_case.config", c("TC_ID","PATH","LATEX-NAME","URL"))

operations = data.frame(id=c("LCA", "LETTER", "SLINK", "CHILD", "DEPTH", "PARENT"),
						caption=c("LCA", "Letter", "SLink", "Child", "SDepth", "Parent"))
rownames(operations)<-operations[["OP_ID"]]
colnames(operations)<-c("OP_ID", "LATEX-NAME")

raw <- data_frame_from_key_value_pairs("../results/all.txt")

for(op_idx in 1:nrow(operations)) {
	op_name = as.character(operations[op_idx, "OP_ID"])
	
	sink(paste("output/", op_name, ".txt", sep="", collapse=""))
	
	for(i in 1:nrow(raw)) {
		data<-raw[i,]
	
		cat(as.character(data[["IDX_ID"]]), "")
		cat(as.character(data[["TC_ID"]]), "")
		cat(sprintf("%d", data[["SAMPLING_FACTOR"]]), "")
		cat(sprintf("%d", data[["FCST_SIZE"]]), "")
		cat(sprintf("%d", data[[paste("FCST_", op_name, "_U_TIME", sep="", collapse="")]]))
		cat("\n")
	}
}

sink(NULL)

