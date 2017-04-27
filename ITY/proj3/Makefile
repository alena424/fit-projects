PROJ = proj3

.PHONY : all
.PHONY : distro
.PHONY : clean

all : $(PROJ).pdf
$(PROJ).pdf : $(PROJ).ps
	 ps2pdf $^ $@
$(PROJ).ps : $(PROJ).dvi
	dvips -t a4 $^ -o $@

$(PROJ).dvi : $(PROJ).tex oniisan.eps oniisan2.eps etiopan.eps
	 latex $^ --jobname=$@ 
	latex $(PROJ).tex 
clean:
	rm -f *.pdf *.dvi *.ps *.log *.aux
distro :
	 rm -f xtesar36-fit.zip	
	 zip xtesar36-fit.zip *.tex *.bib Makefile oniisan.eps oniisan2.eps etiopan.eps

