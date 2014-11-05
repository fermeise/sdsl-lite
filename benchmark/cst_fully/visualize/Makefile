include ../../../Make.helper

CONFIG_FILES=../test_case.config

all: cst_fully.pdf

cst_fully.pdf: cst_fully.tex
	@echo "Use pdflatex to generate cst_fully.pdf"
	@pdflatex cst_fully.tex >> LaTeX.log 2>&1

cst_fully.tex: ../results/all.txt ../../basic_functions.R cst_fully.R $(CONFIG_FILES)
	@echo "Use R to generate tex files."
	@R --vanilla < cst_fully.R > R.log 2>&1

clean:
	rm -f cst_fully.pdf cst_fully.aux cst_fully.log fig-space.tex R.log LaTeX.log
