PROJ = dokumentace

.PHONY : all
.PHONY : clean

all : $(PROJ).pdf

$(PROJ).pdf : $(PROJ).tex  
	pdflatex $^ --jobname=$@
	pdflatex $^ 
clean:
	rm -f *.pdf *.dvi *.ps *.log *.aux *.blg *.bbl *.toc

