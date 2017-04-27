PROJ = proj4

.PHONY : all
.PHONY : distro
.PHONY : clean

all : $(PROJ).pdf

$(PROJ).pdf : $(PROJ).tex $(PROJ).bib
	latex $(PROJ).tex 
	bibtex $(PROJ).aux
	latex $(PROJ).tex 
	latex $(PROJ).tex
	dvips -t a4 $(PROJ).dvi
	ps2pdf $(PROJ).ps
clean:
	rm -f *.pdf *.dvi *.ps *.log *.aux *.blg *.bbl
distro :
	 rm -f xtesar36-fit.zip	
	 zip xtesar36-fit.zip *.tex *.bib Makefile czplain.bst

