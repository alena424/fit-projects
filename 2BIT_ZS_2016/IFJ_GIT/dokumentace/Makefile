#####################################
# Makefile for IFJ project documentation 
#  @date 3.12.2017
#  
#  @author Alena Tesarova (xtesar36)
#  @author Jan Sorm (xsormj00)
#  @author Daniel Uhricek (xuhric00)
#  @author Peter Uhrin (xuhrin02)
#
#  ###################################

DOK=dokumentace

.PHONY : all
.PHONY : clean

all: $(DOK).pdf

$(DOK).pdf : $(DOK).ps
	ps2pdf $^ $@

$(DOK).ps : $(DOK).dvi
	dvips -t a4 $^ -o $@

$(DOK).dvi : $(DOK).tex png/FSM_diagram.png
	pdflatex $^ --jobname=$@
	pdflatex $(DOK).tex

clean:
	rm -rf *.out *.pdf *.dvi *.log *.aux *.cpt 
