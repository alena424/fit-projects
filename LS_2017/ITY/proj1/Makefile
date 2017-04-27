   PROJ = proj1

.PHONY : all
.PHONY : distro

all : $(PROJ).pdf
$(PROJ).pdf : $(PROJ).ps
	 ps2pdf $^ $@
$(PROJ).ps : $(PROJ).dvi
	dvips -t a4 $^ -o $@
$(PROJ).dvi : $(PROJ).tex
	 latex $^ --jobname=$@

distro :
	 rm -f xtesar36-fit.zip	
	 zip xtesar36-fit.zip *.tex *.bib Makefile

