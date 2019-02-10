#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

.SUFFIXES: .tex .dvi .aux .bib .obj .gif .eps


LATEX = platex -kanji=euc-jp
BIBTEX = pbibtex -kanji=euc-jp
XDVI = pxdvi-xaw
SRCS = $(wildcard *.tex)
DVIPS = dvips
TGIF = tgif
GV = gv
RM = rm -f
MV = mv
DVIPDF = dvipdfmx
SRC = mcube.tex

DVI = $(SRC:.tex=.dvi)
AUX = $(SRC:.tex=.aux) $(SRCS:.tex=.aux)
LOG = *.log
LOF = $(SRC:.tex=.lof)
LOT = $(SRC:.tex=.lot)
TOC = $(SRC:.tex=.toc)
BIB = refer.bib
BBL = $(SRC:.tex=.bbl)
BLG = $(SRC:.tex=.blg)
BAK = *~ *.bak
PS  = $(SRC:.tex=.ps)
PDF = $(SRC:.tex=.pdf)

#PSFLAG = -O -12mm,-10mm
PSFLAG = 

.PHONY: all build preview ps gv tar clean
all: $(DVI)

build: clean all

$(DVI): $(SRC) $(BIB) $(EPS) $(SRCS)
	$(LATEX) $(SRC)
	$(BIBTEX) $(SRC:.tex=)
	$(LATEX) $(SRC) 
	$(LATEX) $(SRC)

view : $(DVI)
	$(XDVI) $(DVI) &

ps $(PS): $(DVI)
	$(DVIPS) $(PSFLAG) -f $(DVI) > $(PS)

pdf: $(DVI)
	$(DVIPDF) $(DVI)

gv: $(PS)
	$(GV) $(PS) &

%.obj: %.eps
	$(TGIF) -print -eps $<

tar:
	make clean
	cd ..;\
	tar jcvpf b_thesis.tar.bz2 b_thesis

clean:
	@$(RM) $(DVI) $(AUX) $(LOG) $(LOF) $(LOT) $(TOC) $(BBL) $(BLG) $(BAK) $(PS) $(PDF) $(SRC:.tex=.out) config.*
